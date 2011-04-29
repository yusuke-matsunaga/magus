
/// @file libym_dotlib/PtAttr.cc
/// @brief PtAttr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/PtAttr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtAttr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] attr_name 属性名
// @param[in] value 値
PtAttr::PtAttr(const ShString& attr_name,
	       const PtNode* value) :
  mAttrName(attr_name),
  mValue(value),
  mNext(NULL)
{
}

// @brief デストラクタ
PtAttr::~PtAttr()
{
}

// @brief 属性名を得る．
ShString
PtAttr::attr_name() const
{
  return mAttrName;
}

// @brief 値を得る．
const PtNode*
PtAttr::value() const
{
  return mValue;
}

// @brief 次の要素を得る．
const PtAttr*
PtAttr::next() const
{
  return mNext;
}


END_NAMESPACE_YM_DOTLIB
