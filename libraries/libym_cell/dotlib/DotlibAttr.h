#ifndef LIBYM_DOTLIB_DOTLIBATTR_H
#define LIBYM_DOTLIB_DOTLIBATTR_H

/// @file DotlibAttr.h
/// @brief DotlibAttr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibAttr DotlibAttr.h "DotlibAttr.h"
/// @brief 属性を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibAttr
{
  friend class DotlibMgrImpl;
  friend class DotlibGroup;

private:

  /// @brief コンストラクタ
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @param[in] loc ファイル上の位置
  DotlibAttr(const ShString& attr_name,
	     const DotlibNode* value,
	     const FileRegion& loc);

  /// @brief デストラクタ
  ~DotlibAttr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル上の位置を返す．
  FileRegion
  loc() const;

  /// @brief 属性名を得る．
  ShString
  attr_name() const;

  /// @brief 属性の値を得る．
  const DotlibNode*
  attr_value() const;

  /// @brief 次の属性を得る．
  const DotlibAttr*
  next() const;

  /// @brief 内容をストリーム出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] indent インデント量
  void
  dump(ostream& s,
       ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mLoc;

  // 属性名
  ShString mAttrName;

  // 値
  const DotlibNode* mValue;

  // 次の要素を指すリンクポインタ
  DotlibAttr* mNext;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_DOTLIBNODEIMPL_H
