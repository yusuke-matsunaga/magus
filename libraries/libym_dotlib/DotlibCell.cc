
/// @file libym_dotlib/DotlibCell.cc
/// @brief DotlibCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibCell.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibCell::DotlibCell()
{
  clear();
}

// @brief デストラクタ
DotlibCell::~DotlibCell()
{
}

// @brief 内容を初期化する．
void
DotlibCell::clear()
{
  DotlibAttrMap::clear();

  mArea = 0.0;
  mBusNamingStyle = NULL;
  mFf = NULL;
  mLatch = NULL;
  mStatetable = NULL;

  mPinList.clear();
  mBusList.clear();
  mBundleList.clear();
}

// @brief 名前を返す．
ShString
DotlibCell::name() const
{
  return mName;
}

// @brief 面積を返す．
double
DotlibCell::area() const
{
  return mArea;
}

// @brief ピングループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::pin_list() const
{
  return mPinList;
}

// @brief バスグループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::bus_list() const
{
  return mBusList;
}

// @brief バンドルグループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::bundle_list() const
{
  return mBundleList;
}

// @brief ff グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::ff() const
{
  return mFf;
}

// @brief latch グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::latch() const
{
  return mLatch;
}

// @brief statetable グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::statetable() const
{
  return mStatetable;
}

END_NAMESPACE_YM_DOTLIB
