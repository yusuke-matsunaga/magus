#ifndef LIBYM_CELL_DOTLIB_LIBCOMPLEXHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBCOMPLEXHANDLER_H

/// @file libym_cell/dotlib/LibComplexHandler.h
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
/// @class CapacitiveLoadUnitHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};


// default_part

// define


//////////////////////////////////////////////////////////////////////
/// @class DefineCellAreaHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};


// define_group


//////////////////////////////////////////////////////////////////////
/// @class LibraryFeaturesHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};


//////////////////////////////////////////////////////////////////////
/// @class PieceDefineHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};


//////////////////////////////////////////////////////////////////////
/// @class RoutingLayersHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};


//////////////////////////////////////////////////////////////////////
/// @class TechnologyHandler LibComplexHandler.h "LibComplexHandler.h"
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
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBCOMPLEXHANDLER_H
