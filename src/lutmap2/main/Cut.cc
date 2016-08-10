
/// @file Cut.cc
/// @brief Cut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "Cut.h"
#include "SbjNode.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_LUTMAP2

BEGIN_NONAMESPACE

// valmap に終端のノード番号をキーとしてビットベクタ値を登録する．
// その時の node の値を計算する．
ymuint64
eval_node(const SbjNode* node,
	  HashMap<ymuint, ymuint64>& valmap)
{
  if ( node == nullptr ) {
    return 0ULL;
  }

  // まずすでに評価済みかどうか調べる．
  // 葉のノードの場合もここに登録されている．
  ymuint64 ans;
  if ( !valmap.find(node->id(), ans) ) {
    // 未登録の場合は必ず論理ノード
    ASSERT_COND( node->is_logic() );

    // 子供の値を評価する．
    ymuint64 val0 = eval_node(node->fanin(0), valmap);
    if ( node->fanin_inv(0) ) {
      val0 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    ymuint64 val1 = eval_node(node->fanin(1), valmap);
    if ( node->fanin_inv(0) ) {
      val1 ^= 0xFFFFFFFFFFFFFFFFULL;
    }

    // 自分の値を計算する．
    if ( node->is_xor() ) {
      ans = val0 ^ val1;
    }
    else {
      ans = val0 & val1;
    }

    // 登録しておく．
    valmap.add(node->id(), ans);
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
    const SbjNode* inode = cut->input(i);
    valmap.add(inode->id(), vals[i]);
  }
  return eval_node(cut->root(), valmap);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス Cut
//////////////////////////////////////////////////////////////////////

// @brief カットを生成する．
// @param[in] root 根のノード
// @param[in] input_list 入力ノードのリスト
Cut*
Cut::new_cut(const SbjNode* root,
	     const vector<const SbjNode*>& input_list)
{
  ymuint ni = input_list.size();
  ymuint size = sizeof(Cut) + (ni - 1) * sizeof(const SbjNode*);
  void* p = new char[size];
  return new (p) Cut(root, input_list);
}

// @brief カットの領域を開放する．
// @param[in] cut 開放するカット
void
Cut::delete_cut(Cut* cut)
{
  char* p = reinterpret_cast<char*>(cut);
  delete [] p;
}

// @brief コンストラクタ
// @param[in] root 根のノード
// @param[in] input_list 入力ノードのリスト
Cut::Cut(const SbjNode* root,
	 const vector<const SbjNode*>& input_list)
{
  mRoot = root;
  mNi = input_list.size();
  for (ymuint i = 0; i < mNi; ++ i) {
    mInputs[i] = input_list[i];
  }
}

// @brief デストラクタ
Cut::~Cut()
{
}

// @brief 論理関数を表す真理値表を得る．
// @param[in] inv 出力を反転する時 true にするフラグ
TvFunc
Cut::make_tv(bool inv) const
{
  ymuint ni = input_num();
  ymuint np = 1 << ni;

  vector<int> tv(np);

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

  return TvFunc(ni, tv);
}

END_NAMESPACE_YM_LUTMAP2
