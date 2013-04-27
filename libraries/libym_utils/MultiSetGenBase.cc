
/// @file MultiSetGenBase.cc
/// @brief MultiSetGenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MsGenIterator
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親の MultiSetGenBase オブジェクト
MsGenIterator::MsGenIterator(const MultiSetGenBase* parent)
{
  assert_cond( parent != NULL, __FILE__, __LINE__);

  mParent = parent;
  ymuint k = parent->k();
  mElem.resize(k);
  ymuint pos = 0;
  ymuint count = 0;
  for (ymuint i = 0; i < k; ++ i) {
    if ( count >= parent->n(pos) ) {
      ++ pos;
      count = 0;
    }
    assert_cond( count < parent->n(pos), __FILE__, __LINE__);
    mElem[i] = pos;
    ++ count;
  }
}

END_NAMESPACE_YM
