
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
  mLink(NULL)
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
		   const unordered_map<ymuint, Expr>& logmap)
{
  Expr ans;
  if ( node == NULL ) {
    ans =  Expr::make_zero();
  }
  else {
    unordered_map<ymuint, Expr>::const_iterator p = logmap.find(node->id());
    if ( p != logmap.end() ) {
      ans = p->second;
    }
    else {
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
  if ( root() == NULL ) {
    return Expr::make_zero();
  }

  unordered_map<ymuint, Expr> logmap;
  for (ymuint i = 0; i < input_num(); i ++) {
    const BdnNode* node = mInputs[i];
    ymuint id = node->id();
    logmap.insert(make_pair(id, Expr::make_posiliteral(VarId(i))));
  }
  return calc_expr_for_node(root(), logmap);
}

// デバッグ用の表示関数
void
Cut::dump(ostream& s) const
{
  if ( root() == NULL ) {
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
