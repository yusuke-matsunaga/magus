
/// @file libym_dotlib/DotlibFF.cc
/// @brief DotlibFF の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibFF.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibFF
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibFF::DotlibFF()
{
  clear();
}

// @brief デストラクタ
DotlibFF::~DotlibFF()
{
}

// @brief 内容を初期化する．
void
DotlibFF::clear()
{
  DotlibAttrMap::clear();

  mNextState = NULL;
  mClockedOn = NULL;
  mClockedOnAlso = NULL;
  mClear = NULL;
  mPreset = NULL;
}

// @brief var1 の名前を返す．
ShString
DotlibFF::var1_name() const
{
  return mVar1;
}

// @brief var2 の名前を返す．
ShString
DotlibFF::var2_name() const
{
  return mVar2;
}

// @brief "next_state" を返す．
const DotlibNode*
DotlibFF::next_state() const
{
  return mNextState;
}

// @brief "clocked_on" を返す．
const DotlibNode*
DotlibFF::clocked_on() const
{
  return mClockedOn;
}

// @brief "clocked_on_also" を返す．
const DotlibNode*
DotlibFF::clocked_on_also() const
{
  return mClockedOnAlso;
}

// @brief "clear" を返す．
const DotlibNode*
DotlibFF::clear_pin() const
{
  return mClear;
}

// @brief "preset" を返す．
const DotlibNode*
DotlibFF::preset() const
{
  return mPreset;
}

// @brief "clear_preset_var1" を返す．
ShString
DotlibFF::clear_preset_var1() const
{
  return mClearPresetVar1;
}

// @brief "clear_preset_var2" を返す．
ShString
DotlibFF::clear_preset_var2() const
{
  return mClearPresetVar2;
}

END_NAMESPACE_YM_DOTLIB

