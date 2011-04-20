#ifndef LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H

/// @file libym_cell/dotlib/LibSimpleHandler.h
/// @brief LibSimpleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

// 参考文献:
//  Library Compiler Reference Manual, Version Z-2007.03
//  1-3 Example 1-1 Attributes, Groups, and Values in a Technology Library
//  /* Library Description: Simple Attributes */


#include "SimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LibSimpleHandler LibSimpleHandler.h "LibSimpleHandler.h"
/// @brief library グループの simple attribute 用ハンドラ
//////////////////////////////////////////////////////////////////////
class LibSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] lib_handler 親のハンドラ
  LibSimpleHandler(LibraryGroupHandler* lib_handler);

  /// @brief デストラクタ
  virtual
  ~LibSimpleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     Token token);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 親のハンドラを返す．
  LibraryGroupHandler*
  lib_handler();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のハンドラ
  LibraryGroupHandler* mLibHandler;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBSIMPLEHANDLER_H
