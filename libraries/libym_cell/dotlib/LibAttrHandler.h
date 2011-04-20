#ifndef LIBYM_CELL_DOTLIB_LIBATTRHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBATTRHANDLER_H

/// @file libym_cell/dotlib/LibAttrHandler.h
/// @brief LibAttrHandler のヘッダファイル
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

class LibraryGroupHandler;

//////////////////////////////////////////////////////////////////////
/// @class LibAttrHandler LibAttrHandler.h "LibAttrHandler.h"
/// @brief library グループの属性用ハンドラの基底クラス
//////////////////////////////////////////////////////////////////////
class LibAttrHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] lib_handler 親のハンドラ
  LibAttrHandler(LibraryGroupHandler* lib_handler);

  /// @brief デストラクタ
  virtual
  ~LibAttrHandler();


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

#endif // LIBYM_CELL_DOTLIB_LIBATTRHANDLER_H
