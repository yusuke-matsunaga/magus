
/// @file libym_techmap/lutmap/Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Cut.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Cut.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM_LUTMAP

// 空のコンストラクタ
Cut::Cut() :
  mLink(nullptr)
{
}

// @brief デストラクタ
Cut::~Cut()
{
}


BEGIN_NONAMESPACE

// inputs に登録されているノードを終端と見なしたときの node の表す論理式
// を返す．
Expr
calc_expr_for_node(const BdnNode* node,
		   const HashMap<ymuint, Expr>& logmap)
{
  Expr ans;
  if ( node == nullptr ) {
    ans =  Expr::make_zero();
  }
  else {
    if ( !logmap.find(node->id(), ans) ) {
      ASSERT_COND( node->is_logic() );

      Expr cexp0 = calc_expr_for_node(node->fanin0(), logmap);
      Expr cexp1 = calc_expr_for_node(node->fanin1(), logmap);

      if ( node->fanin0_inv() ) {
	cexp0 = ~cexp0;
      }
      if ( node->fanin1_inv() ) {
	cexp1 = ~cexp1;
      }
      if ( node->is_xor() ) {
	ans = cexp0 ^ cexp1;
      }
      else {
	ans = cexp0 & cexp1;
      }
    }
  }
  return ans;
}

END_NONAMESPACE

// @brief 内容を表す論理式を得る．
Expr
Cut::expr() const
{
  if ( root() == nullptr ) {
    return Expr::make_zero();
  }

  HashMap<ymuint, Expr> logmap;
  for (ymuint i = 0; i < input_num(); i ++) {
    const BdnNode* node = mInputs[i];
    ymuint id = node->id();
    logmap.add(id, Expr::make_posiliteral(VarId(i)));
  }
  return calc_expr_for_node(root(), logmap);
}

BEGIN_NONAMESPACE

// valmap に終端のノード番号をキーとしてビットベクタ値を登録する．
// その時の node の値を計算する．
ymuint64
eval_node(const BdnNode* node,
	  HashMap<ymuint, ymuint64>& valmap)
{
  ymuint64 ans;
  if ( node == nullptr ) {
    ans =  0ULL;
  }
  else {
    if ( !valmap.find(node->id(), ans) ) {
      ASSERT_COND( node->is_logic() );

      ymuint64 val0 = eval_node(node->fanin0(), valmap);
      ymuint64 val1 = eval_node(node->fanin1(), valmap);

      if ( node->fanin0_inv() ) {
	val0 ^= 0xFFFFFFFFFFFFFFFFULL;
      }
      if ( node->fanin1_inv() ) {
	val1 ^= 0xFFFFFFFFFFFFFFFFULL;
      }
      if ( node->is_xor() ) {
	ans = val0 ^ val1;
      }
      else {
	ans = val0 & val1;
      }
      valmap.add(node->id(), ans);
    }
  }
  return ans;
}

// カットの表している論理関数を評価する．
ymuint64
eval_cut(const Cut* cut,
	 const vector<ymuint64>& vals)
{
  ymuint ni = cut->input_num();
  ASSERT_COND( ni == vals.size() );
  HashMap<ymuint, ymuint64> valmap;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* inode = cut->input(i);
    valmap.add(inode->id(), vals[i]);
  }
  return eval_node(cut->root(), valmap);
}

END_NONAMESPACE

// @brief 論理関数を表す真理値表を得る．
// @param[in] inv 出力を反転する時 true にするフラグ
// @param[out] tv 結果の真理値表を格納するベクタ
//
// tv のサイズは 2^{input_num()} を仮定している．
void
Cut::make_tv(bool inv,
	     vector<int>& tv) const
{
  ymuint ni = input_num();
  ymuint np = 1 << ni;
  ASSERT_COND( tv.size() == np );

  // 1 の値と 0 の値
  // inv == true の時には逆にする．
  int v1 = inv ? 0 : 1;
  int v0 = inv ? 1 : 0;

  // 真理値表の各変数の値を表すビットベクタ
  // 6入力以上の場合には1語に収まらないので複数回にわけて処理する．
  vector<ymuint64> vals(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    vals[i] = 0ULL;
  }

  ymuint64 s = 1ULL;
  ymuint p0 = 0;
  for (ymuint p = 0; p < np; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1U << i) ) {
	vals[i] |= s;
      }
    }
    s <<= 1;
    if ( s == 0ULL ) {
      // 64 パタン目
      ymuint64 tmp = eval_cut(this, vals);
      for (ymuint p1 = p0; p1 < p; ++ p1) {
	if ( tmp & (1ULL << (p1 - p0)) ) {
	  tv[p1] = v1;
	}
	else {
	  tv[p1] = v0;
	}
      }
      s = 1ULL;
      p0 = p + 1;
      for (ymuint i = 0; i < ni; ++ i) {
	vals[i] = 0ULL;
      }
    }
  }
  if ( s != 1ULL ) {
    // 処理されていない残りがあった．
    ymuint64 tmp = eval_cut(this, vals);
    for (ymuint p1 = p0; p1 < np; ++ p1) {
      if ( tmp & (1ULL << (p1 - p0)) ) {
	tv[p1] = v1;
      }
      else {
	tv[p1] = v0;
      }
    }
  }
}

// デバッグ用の表示関数
void
Cut::dump(ostream& s) const
{
  if ( root() == nullptr ) {
    s << "null cut" << endl;
  }
  else {
    s << "Node[" << root()->id() << "] : ";
    string comma = "";
    for (ymuint i = 0; i < input_num(); i ++) {
      const BdnNode* node = mInputs[i];
      s << comma << "Node[" << node->id() << "]";
      comma = ", ";
    }
    s << " : ";
    s << expr();
    s << endl;
  }
}

END_NAMESPACE_YM_LUTMAP
