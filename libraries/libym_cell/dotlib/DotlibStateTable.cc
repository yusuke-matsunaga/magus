
/// @file libym_dotlib/DotlibFF.cc
/// @brief DotlibFF の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibFF.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibFF
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibFF::DotlibFF()
{
  init();
}

// @brief デストラクタ
DotlibFF::~DotlibFF()
{
}

// @brief 内容を初期化する．
void
DotlibFF::init()
{
  DotlibFL::init();

  mNextState = NULL;
  mClockedOn = NULL;
  mClockedOnAlso = NULL;
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

END_NAMESPACE_YM_DOTLIB

