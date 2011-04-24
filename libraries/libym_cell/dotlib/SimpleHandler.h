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
  /// @param[in] parser パーサー
  /// @param[in] parent 親のハンドラ
  SimpleHandler(DotlibParser& parser,
		GroupHandler* parent);

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

  /// @brief 整数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     int value,
	     const FileRegion& value_loc) = 0;

  /// @brief 実数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     double value,
	     const FileRegion& value_loc) = 0;

  /// @brief 文字列を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const string& value,
	     const FileRegion& value_loc) = 0;

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
  IntSimpleHandler(DotlibParser& parser,
		   GroupHandler* parent);

  /// @brief デストラクタ
  ~IntSimpleHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     int value,
	     const FileRegion& value_loc);

  /// @brief 実数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     double value,
	     const FileRegion& value_loc);

  /// @brief 文字列を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const string& value,
	     const FileRegion& value_loc);

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
  BoolSimpleHandler(DotlibParser& parser,
		    GroupHandler* parent);

  /// @brief デストラクタ
  ~BoolSimpleHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     int value,
	     const FileRegion& value_loc);

  /// @brief 実数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     double value,
	     const FileRegion& value_loc);

  /// @brief 文字列を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const string& value,
	     const FileRegion& value_loc);

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
  FloatSimpleHandler(DotlibParser& parser,
		     GroupHandler* parent);

  /// @brief デストラクタ
  ~FloatSimpleHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     int value,
	     const FileRegion& value_loc);

  /// @brief 実数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     double value,
	     const FileRegion& value_loc);

  /// @brief 文字列を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const string& value,
	     const FileRegion& value_loc);

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
  StrSimpleHandler(DotlibParser& parser,
		   GroupHandler* parent);

  /// @brief デストラクタ
  ~StrSimpleHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 整数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     int value,
	     const FileRegion& value_loc);

  /// @brief 実数値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     double value,
	     const FileRegion& value_loc);

  /// @brief 文字列を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  /// @param[in] value_loc 値のファイル上の位置
  virtual
  bool
  read_value(const string& attr_name,
	     const FileRegion& attr_loc,
	     const string& value,
	     const FileRegion& value_loc);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
