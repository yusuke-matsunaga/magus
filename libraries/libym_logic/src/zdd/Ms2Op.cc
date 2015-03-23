
/// @file Ms2Op.cc
/// @brief Ms2Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Ms2Op.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
// クラス Ms2Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
Ms2Op::Ms2Op(ZddMgrImpl* mgr) :
  ZddOp(mgr)
{
}

// @brief デストラクタ
Ms2Op::~Ms2Op()
{
}

// @brief minimum set 演算を行う関数
ZddEdge
Ms2Op::apply(ZddEdge left)
{
  // エラー状態のチェック
  if ( left.is_invalid() ) {
    return left;
  }
  ymuint nelem;
  return apply_step(left, nelem);
}

// @brief 次回のGCで回収されるノードに関連した情報を削除する．
void
Ms2Op::sweep()
{
  mHash.clear();
}

// @brief apply() の下請け関数
// @param[in] e 対象の枝
// @param[out] nelem 要素数
// @return minimum set の枝を返す．
ZddEdge
Ms2Op::apply_step(ZddEdge e,
		  ymuint& nelem)
{
  if ( e.zattr() ) {
    nelem = 0;
    return ZddEdge::make_one();
  }

  ASSERT_COND( !e.is_zero() );

  // この時点で e は終端ではない．
  pair<ZddEdge, ymuint> ans;
  if ( mHash.find(e, ans) ) {
    nelem = ans.second;
    return ans.first;
  }

  ZddNode* e_node = e.get_node();
  ymuint level = e_node->level();
  ZddEdge e_0 = e_node->edge0();
  nelem = 0;
  ZddEdge r;
  if ( !e_0.is_zero() ) {
    r = apply_step(e_0, nelem);
  }
  ZddEdge e_1 = e_node->edge1();
  if ( !e_1.is_zero() ) {
    ymuint nelem1;
    ZddEdge r_1 = apply_step(e_1, nelem1);
    if ( nelem == 0 || nelem > nelem1 + 1 ) {
      nelem = nelem1 + 1;
      r = new_node(level, ZddEdge::make_zero(), r_1);
    }
  }
  ASSERT_COND( nelem > 0 );

  mHash.add(e, make_pair(r, nelem));

  return r;
}

END_NAMESPACE_YM_ZDD
