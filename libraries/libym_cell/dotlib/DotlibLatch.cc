
/// @file libym_dotlib/DotlibLatch.cc
/// @brief DotlibLatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibLatch.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibLatch::DotlibLatch()
{
  init();
}

// @brief デストラクタ
DotlibLatch::~DotlibLatch()
{
}

// @brief 内容を初期化する．
void
DotlibLatch::init()
{
  DotlibFL::init();

  mDataIn = NULL;
  mEnable = NULL;
  mEnableAlso = NULL;
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
