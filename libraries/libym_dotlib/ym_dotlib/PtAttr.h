#ifndef YM_DOTLIB_PTATTR_H
#define YM_DOTLIB_PTATTR_H

/// @file ym_dotlib/PtAttr.h
/// @brief PtAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtAttr PtAttr.h "dotlib/PtAttr.h"
/// @brief 属性を表すクラス
//////////////////////////////////////////////////////////////////////
class PtAttr
{
  friend class PtGroup;

public:

  /// @brief コンストラクタ
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  PtAttr(const ShString& attr_name,
	 const PtNode* value);

  /// @brief デストラクタ
  ~PtAttr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性名を得る．
  ShString
  attr_name() const;

  /// @brief 値を得る．
  const PtNode*
  value() const;

  /// @brief 次の要素を得る．
  const PtAttr*
  next() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名
  ShString mAttrName;

  // 値
  const PtNode* mValue;

  // 次の要素を指すリンクポインタ
  PtAttr* mNext;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_PTATTR_H
