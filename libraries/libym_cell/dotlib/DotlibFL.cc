
/// @file libym_dotlib/DotlibFL.cc
/// @brief DotlibFL の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibFL.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibFL
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibFL::DotlibFL()
{
  init();
}

// @brief デストラクタ
DotlibFL::~DotlibFL()
{
}

// @brief 内容を初期化する．
void
DotlibFL::init()
{
  DotlibAttrMap::init();

  mClear = NULL;
  mPreset = NULL;

  mClearPresetVar1 = 0;
  mClearPresetVar2 = 0;
}

// @brief var1 の名前を返す．
ShString
DotlibFL::var1_name() const
{
  return mVar1;
}

// @brief var2 の名前を返す．
ShString
DotlibFL::var2_name() const
{
  return mVar2;
}

// @brief "clear" を返す．
const DotlibNode*
DotlibFL::clear() const
{
  return mClear;
}

// @brief "preset" を返す．
const DotlibNode*
DotlibFL::preset() const
{
  return mPreset;
}

// @brief "clear_preset_var1" を返す．
ymuint
DotlibFL::clear_preset_var1() const
{
  return mClearPresetVar1;
}

// @brief "clear_preset_var2" を返す．
ymuint
DotlibFL::clear_preset_var2() const
{
  return mClearPresetVar2;
}

END_NAMESPACE_YM_DOTLIB

