#ifndef LIBYM_DOTLIB_COMPLEXHANDLER_H
#define LIBYM_DOTLIB_COMPLEXHANDLER_H

/// @file libym_dotlib/ComplexHandler.h
/// @brief ComplexHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief complex attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class ComplexHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  ComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~ComplexHandler();


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
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

  /// @brief vector_mode で読み込むとき true を返す．
  virtual
  bool
  vector_mode() const;

};


//////////////////////////////////////////////////////////////////////
/// @class Str1ComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief 1つの文字列型をとる complex attribute ハンドラ
//////////////////////////////////////////////////////////////////////
class Str1ComplexHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  Str1ComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~Str1ComplexHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

};


//////////////////////////////////////////////////////////////////////
/// @class VectorComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief 1つのベクタ型をとる complex attribute ハンドラ
//////////////////////////////////////////////////////////////////////
class VectorComplexHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  VectorComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~VectorComplexHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

  /// @brief vector_mode で読み込むとき true を返す．
  virtual
  bool
  vector_mode() const;

};


//////////////////////////////////////////////////////////////////////
/// @class VectorListComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief ベクタのリストをとる complex attribute ハンドラ
//////////////////////////////////////////////////////////////////////
class VectorListComplexHandler :
  public VectorComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  VectorListComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~VectorListComplexHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

};


//////////////////////////////////////////////////////////////////////
/// @class UnitComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief 単位型のcomplex attribute ハンドラ
//////////////////////////////////////////////////////////////////////
class UnitComplexHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  UnitComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~UnitComplexHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

};


//////////////////////////////////////////////////////////////////////
/// @class PwComplexHandler ComplexHandler.h "ComplexHandler.h"
/// @brief 折れ線近似モデル用ののcomplex attribute ハンドラ
//////////////////////////////////////////////////////////////////////
class PwComplexHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  PwComplexHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~PwComplexHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値のリスト
  /// @param[in] end_loc 右括弧の位置
  virtual
  bool
  set_value(const ShString& attr_name,
	    const FileRegion& attr_loc,
	    DotlibNodeImpl* value,
	    const FileRegion& end_loc);

};


END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_COMPLEXHANDLER_H
