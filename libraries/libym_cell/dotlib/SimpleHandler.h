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
  read_attr(const string& attr_name,
	    const FileRegion& attr_loc);


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルモードで read_token() を呼ぶときに true を返す．
  /// @note デフォルトの実装では false を返す．
  virtual
  bool
  symbol_mode();

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @note デフォルトの実装ではなにもしないで true を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

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


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルモードで read_token() を呼ぶときに true を返す．
  virtual
  bool
  symbol_mode();

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


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

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


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

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


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

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


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
