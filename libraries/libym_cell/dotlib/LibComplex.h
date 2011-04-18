#ifndef LIBYM_CELL_DOTLIB_LIBCOMPLEX_H
#define LIBYM_CELL_DOTLIB_LIBCOMPLEX_H

/// @file libym_cell/dotlib/LibComplex.h
/// @brief library グループの complex attribute のハンドラのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

// 参考文献:
//  Library Compiler Reference Manual, Version Z-2007.03
//  1-7 Example 1-1 Attributes, Groups, and Values in a Technology Library
//  /* Library Description: Complex Attributes */


#include "ComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CapacitiveLoadUnitHandler LibComplex.h "LibComplex.h"
/// @brief capacitive_load_unit 属性
//////////////////////////////////////////////////////////////////////
class CapacitiveLoadUnitHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CapacitiveLoadUnitHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~CapacitiveLoadUnitHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};


// default_part

// define


//////////////////////////////////////////////////////////////////////
/// @class DefineCellAreaHandler LibComplex.h "LibComplex.h"
/// @brief define_cell_area 属性
//////////////////////////////////////////////////////////////////////
class DefineCellAreaHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DefineCellAreaHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DefineCellAreaHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};


// define_group


//////////////////////////////////////////////////////////////////////
/// @class LibraryFeaturesHandler LibComplex.h "LibComplex.h"
/// @brief library_features 属性
//////////////////////////////////////////////////////////////////////
class LibraryFeaturesHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LibraryFeaturesHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~LibraryFeaturesHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};


//////////////////////////////////////////////////////////////////////
/// @class PieceDefineHandler LibComplex.h "LibComplex.h"
/// @brief piece_define 属性
//////////////////////////////////////////////////////////////////////
class PieceDefineHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PieceDefineHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~PieceDefineHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};


//////////////////////////////////////////////////////////////////////
/// @class RoutingLayersHandler LibComplex.h "LibComplex.h"
/// @brief routing_layers 属性
//////////////////////////////////////////////////////////////////////
class RoutingLayersHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RoutingLayersHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~RoutingLayersHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};


//////////////////////////////////////////////////////////////////////
/// @class TechnologyHandler LibComplex.h "LibComplex.h"
/// @brief technology 属性
//////////////////////////////////////////////////////////////////////
class TechnologyHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  TechnologyHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~TechnologyHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBCOMPLEX_H
