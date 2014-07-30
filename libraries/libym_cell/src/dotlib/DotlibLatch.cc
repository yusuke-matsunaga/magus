
/// @file DotlibLatch.cc
/// @brief DotlibLatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibLatch.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibLatch::DotlibLatch()
{
}

// @brief デストラクタ
DotlibLatch::~DotlibLatch()
{
}

// @brief 内容を初期化する．
bool
DotlibLatch::set_data(const DotlibNode* latch_node)
{
  if ( !DotlibFL::set_data(latch_node) ) {
    return false;
  }

  mDataIn = NULL;
  mEnable = NULL;
  mEnableAlso = NULL;

  // data_in を取り出す．
  if ( !get_singleton_or_null("data_in", mDataIn) ) {
    return false;
  }

  // enable を取り出す．
  if ( !get_singleton_or_null("enable", mEnable) ) {
    return false;
  }

  // enable_also を取り出す．
  if ( !get_singleton_or_null("enable_also", mEnableAlso) ) {
    return false;
  }

  return true;
}

// @brief "data_in" を返す．
const DotlibNode*
DotlibLatch::data_in() const
{
  return mDataIn;
}

// @brief "enable" を返す．
const DotlibNode*
DotlibLatch::enable() const
{
  return mEnable;
}

// @brief "enable_also" を返す．
const DotlibNode*
DotlibLatch::enable_also() const
{
  return mEnableAlso;
}

END_NAMESPACE_YM_DOTLIB
