
/// @file GdsStruct.cc
/// @brief GdsStruct の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsStruct.h"
#include "gds/GdsDate.h"
#include "gds/GdsString.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsStruct
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] date 2つの日時の配列
// @param[in] name 名前
GdsStruct::GdsStruct(GdsDate* date,
		     GdsString* name) :
  mName(name),
  mElement(NULL),
  mLink(NULL)
{
  mCreationTime = date;
  mLastModificationTime = date + 1;
}

// @brief デストラクタ
GdsStruct::~GdsStruct()
{
}

// @brief生成日時を返す．
const GdsDate&
GdsStruct::creation_time() const
{
  return *mCreationTime;
}

// @brief 最終更新日時を返す．
const GdsDate&
GdsStruct::last_modification_time() const
{
  return *mLastModificationTime;
}

// @brief名前を返す．
const char*
GdsStruct::name() const
{
  return mName->str();
}

// 先頭の要素を返す．
const GdsElement*
GdsStruct::element() const
{
  return mElement;
}

// @brief 次の要素を返す．
const GdsStruct*
GdsStruct::next() const
{
  return mLink;
}

END_NAMESPACE_YM_GDS
