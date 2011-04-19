
/// @file libym_cell/dotlib/LibComplexHandler.cc
/// @brief library グループの complex attribute のハンドラの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス CapacitiveLoadUnitHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
CapacitiveLoadUnitHandler::CapacitiveLoadUnitHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
CapacitiveLoadUnitHandler::~CapacitiveLoadUnitHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
CapacitiveLoadUnitHandler::read_value(const string& attr_name,
				      const vector<Token>& token_list)
{
  cout << "capacitive_load_unit (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス DefineCellAreaHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DefineCellAreaHandler::DefineCellAreaHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
DefineCellAreaHandler::~DefineCellAreaHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
DefineCellAreaHandler::read_value(const string& attr_name,
				  const vector<Token>& token_list)
{
  cout << "define_cell_area (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス LibraryFeaturesHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
LibraryFeaturesHandler::LibraryFeaturesHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
LibraryFeaturesHandler::~LibraryFeaturesHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
LibraryFeaturesHandler::read_value(const string& attr_name,
				   const vector<Token>& token_list)
{
  cout << "library_features (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス PieceDefineHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
PieceDefineHandler::PieceDefineHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
PieceDefineHandler::~PieceDefineHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
 /// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
PieceDefineHandler::read_value(const string& attr_name,
			       const vector<Token>& token_list)
{
  cout << "piece_define (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス RoutingLayersHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
RoutingLayersHandler::RoutingLayersHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
RoutingLayersHandler::~RoutingLayersHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
RoutingLayersHandler::read_value(const string& attr_name,
				 const vector<Token>& token_list)
{
  cout << "routing_layers (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス TechnologyHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
TechnologyHandler::TechnologyHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
TechnologyHandler::~TechnologyHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
TechnologyHandler::read_value(const string& attr_name,
			      const vector<Token>& token_list)
{
  // technlogy ( cmos|FPGA ) ;

  cout << "technology (";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}

END_NAMESPACE_YM_CELL_DOTLIB
