
/// @file GdsParser.cc
/// @brief GdsParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsParser.h"
#include "GdsRecTable.h"
#include "ym_gds/Msg.h"


BEGIN_NAMESPACE_YM_GDS_PARSER

#include "gds_grammer.hh"

extern
int yyparse(GdsParser&);

END_NAMESPACE_YM_GDS_PARSER


BEGIN_NAMESPACE_YM_GDS

// @brief コンストラクタ
GdsParser::GdsParser()
{
}

// @brief デストラクタ
GdsParser::~GdsParser()
{
}

// @brief ファイルを読み込む．
// @param[in] filename ファイル名
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
GdsParser::parse(const string& filename)
{
  if ( !mScanner.open_file(filename) ) {
    return false;
  }

  nsParser::yyparse(*this);

  mScanner.close_file();

  return true;
}

// @brief yylex() の実装
int
GdsParser::yylex(nsParser::YYSTYPE& lval)
{
  bool stat = mScanner.read_rec();
  if ( stat ) {
    return GdsRecTable::obj().rtype_token(mScanner.cur_rtype());
  }
  return 0;
}

// @brief yyerror() の実装
int
GdsParser::yyerror(const char* s)
{
  error_header(__FILE__, __LINE__, "GdsParser", mScanner.cur_pos(), s);
  msg_end();

  return 1;
}

END_NAMESPACE_YM_GDS
