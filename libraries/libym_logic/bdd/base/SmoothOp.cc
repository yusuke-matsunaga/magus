
/// @file SmoothOp.cc
/// @brief SmoothOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SmoothOp.h"
#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス SmoothOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] and_op AND 演算オブジェクト
SmoothOp::SmoothOp(BddMgrImpl* mgr,
		   BddBinOp* and_op) :
  BddUniOp(mgr),
  mAndOp(and_op)
{
}

// @brief デストラクタ
SmoothOp::~SmoothOp()
{
}

// @brief 演算を行う関数
// @param[in] e 根の枝
// @param[in] v_list 消去する変数のレベルのリスト
// @return 演算結果を返す．
BddEdge
SmoothOp::apply(BddEdge e,
		const vector<ymuint32>& v_list)
{
  if ( e.is_invalid() ) {
    return e;
  }

  ymuint n = mgr()->max_level();
  mEmark.clear();
  mEmark.resize(n, false);
  mMaxLevel = 0;
  for (vector<ymuint32>::const_iterator p = v_list.begin();
       p != v_list.end(); ++ p) {
    ymuint level = *p;
    mEmark[level] = true;
    if ( mMaxLevel < level ) {
      mMaxLevel = level;
    }
  }

  clear();

  return apply_step(e);
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
SmoothOp::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数
BddEdge
SmoothOp::apply_step(BddEdge e)
{
  if ( e.is_leaf() ) {
    return e;
  }

  BddNode* node = e.get_node();
  ymuint level = node->level();
  if ( level > mMaxLevel ) {
    return e;
  }

  BddEdge result = get(e);
  if ( result.is_error() ) {
    tPol pol = e.pol();
    BddEdge e0 = node->edge0(pol);
    BddEdge e1 = node->edge1(pol);
    if ( mEmark[level] ) {
      // 消去対象の変数だった．
      BddEdge tmp = ~mAndOp->apply(~e0, ~e1);
      if ( tmp.is_overflow() ) {
	return tmp;
      }
      result = apply_step(tmp);
    }
    else {
      BddEdge r_0 = apply_step(e0);
      if ( r_0.is_overflow() ) {
	return r_0;
      }
      BddEdge r_1 = apply_step(e1);
      if ( r_1.is_overflow() ) {
	return r_1;
      }
      result = new_node(level, r_0, r_1);
    }
    put(e, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
