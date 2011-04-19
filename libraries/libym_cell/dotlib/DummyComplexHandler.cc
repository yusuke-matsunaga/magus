
/// @file libym_cell/dotlib/DummyComplexHandler.cc
/// @brief DummyComplexHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DummyComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DummyComplexHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
DummyComplexHandler::DummyComplexHandler(DotLibParser& parser) :
  ComplexHandler(parser)
{
}

// @brief デストラクタ
DummyComplexHandler::~DummyComplexHandler()
{
}

// @brief 属性値の読み込み処理を行う．
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
// @return エラーが起きたら false を返す．
bool
DummyComplexHandler::read_value(const string& attr_name,
				const vector<Token>& token_list)
{
  cout << attr_name << " (";
  const char* comma = "";
  ymuint n = token_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    cout << comma << token_list[i].value();
    comma = ", ";
  }
  cout << ")" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
