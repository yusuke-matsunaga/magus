
/// @file libym_dotlib/DotlibLatch.cc
/// @brief DotlibLatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibLatch.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibLatch::DotlibLatch()
{
  clear();
}

// @brief デストラクタ
DotlibLatch::~DotlibLatch()
{
}

// @brief 内容を初期化する．
void
DotlibLatch::clear()
{
  DotlibAttrMap::clear();

  mDataIn = NULL;
  mEnable = NULL;
  mEnableAlso = NULL;
  mClear = NULL;
  mPreset = NULL;
}

// @brief var1 の名前を返す．
ShString
DotlibLatch::var1_name() const
{
  return mVar1;
}

// @brief var2 の名前を返す．
ShString
DotlibLatch::var2_name() const
{
  return mVar2;
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

// @brief "clear" を返す．
const DotlibNode*
DotlibLatch::clear_pin() const
{
  return mClear;
}

// @brief "preset" を返す．
const DotlibNode*
DotlibLatch::preset() const
{
  return mPreset;
}

// @brief "clear_preset_var1" を返す．
ShString
DotlibLatch::clear_preset_var1() const
{
  return mClearPresetVar1;
}

// @brief "clear_preset_var2" を返す．
ShString
DotlibLatch::clear_preset_var2() const
{
  return mClearPresetVar2;
}

END_NAMESPACE_YM_DOTLIB
