
/// @file libym_cell/dotlib/StateTableHandler.cc
/// @brief StateTableHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "StateTableHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス StateTableHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parser パーサー
StateTableHandler::StateTableHandler(DotLibParser& parser) :
  GroupHandler(parser)
{
}

// @brief デストラクタ
StateTableHandler::~StateTableHandler()
{
}

// @brief グループ名を読み込んだ時の処理
// @param[in] attr_name 属性名
// @param[in] token_list トークンのリスト
bool
StateTableHandler::begin_group(const string& attr_name,
			       const vector<Token>& token_list)
{
  cout << attr_name << "( ";
  const char* comma = "";
  for (vector<Token>::const_iterator p = token_list.begin();
       p != token_list.end(); ++ p) {
    cout << comma << p->value();
    comma = ", ";
  }
  cout << " ) {" << endl;
  return true;
}

// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
bool
StateTableHandler::end_group()
{
  cout << "}" << endl;
  return true;
}


END_NAMESPACE_YM_CELL_DOTLIB
