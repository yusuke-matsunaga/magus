#ifndef LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H

/// @file libym_cell/dotlib/SimpleHandler.h
/// @brief SimpleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class SimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief simple attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class SimpleHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  SimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~SimpleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const ShString& attr_name,
	    const FileRegion& attr_loc);


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む処理
  /// @return 値を表す PtNode を返す．
  /// @note エラーが起きたら NULL を返す．
  /// @note デフォルトの実装では普通に DotlibParser::read_token() を呼ぶ．
  virtual
  PtNode*
  read_value();

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @note デフォルトの実装ではなにもしないで true を返す．
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    PtNode* value);

};


//////////////////////////////////////////////////////////////////////
/// @class SymSimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief 数字で始まっていても SYMBOL とみなす SimpleHandler
//////////////////////////////////////////////////////////////////////
class SymSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  SymSimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~SymSimpleHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む処理
  /// @return 値を表す PtNode を返す．
  /// @note エラーが起きたら NULL を返す．
  /// @note ここではシンボルモードで DotlibParser::read_token() を呼ぶ．
  virtual
  PtNode*
  read_value();

};


//////////////////////////////////////////////////////////////////////
/// @class IntSimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief 整数値を取る属性用のハンドラ
//////////////////////////////////////////////////////////////////////
class IntSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  IntSimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~IntSimpleHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  virtual
  PtNode*
  read_value();

};


//////////////////////////////////////////////////////////////////////
/// @class BoolSimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief ブール値を取る属性用のハンドラ
//////////////////////////////////////////////////////////////////////
class BoolSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  BoolSimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~BoolSimpleHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  virtual
  PtNode*
  read_value();

};


//////////////////////////////////////////////////////////////////////
/// @class FloatSimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief 実数値を取る属性用のハンドラ
//////////////////////////////////////////////////////////////////////
class FloatSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  FloatSimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~FloatSimpleHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  virtual
  PtNode*
  read_value();

};


//////////////////////////////////////////////////////////////////////
/// @class StrSimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief 文字列を取る属性用のハンドラ
//////////////////////////////////////////////////////////////////////
class StrSimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  StrSimpleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  ~StrSimpleHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む．
  virtual
  PtNode*
  read_value();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
