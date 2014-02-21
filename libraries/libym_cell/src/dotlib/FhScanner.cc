
/// @file FhScanner.cc
/// @brief FhScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FhScanner.h"
#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス FhScanner
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FhScanner::FhScanner()
{
  mSrcString = NULL;
  mCurPos = 0;
}

// @brief デストラクタ
FhScanner::~FhScanner()
{
}

// @brief 文字列をセットする．
// @param[in] str 文字列
// @param[in] str のファイル上の位置
void
FhScanner::init(const char* str,
		const FileRegion& loc)
{
  mSrcString = str;
  mSrcLoc = loc;
  mCurPos = 0;
}

// @brief トークンを読み込む．
// @param[out] loc 対応するファイル上の位置情報を格納する変数
tTokenType
FhScanner::read_token(FileRegion& loc)
{
  int c;

  mCurString.clear();

 ST_INIT: // 初期状態
  c = get();
  set_first_loc();
  if ( isalpha(c) || c == '_' ) {
    mCurString.put_char(c);
    goto ST_ID;
  }
  if ( c == '0' || c == '1' ) {
    mCurString.put_char(c);
    goto ST_NUM;
  }

  switch ( c ) {
  case '\0':
    loc = cur_loc();
    return END;

  case ' ':
  case '\t':
    goto ST_INIT; // 最初の空白は読み飛ばす．

  case '!':
    loc = cur_loc();
    return NOT;

  case '|':
  case '+':
    loc = cur_loc();
    return OR;

  case '&':
  case '*':
    loc = cur_loc();
    return AND;

  case '^':
    loc = cur_loc();
    return XOR;

  case '(':
    loc = cur_loc();
    return LP;

  case ')':
    loc = cur_loc();
    return RP;

  case '\'':
    loc = cur_loc();
    return PRIME;

  default:
    // それ以外はエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cur_loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error");
    loc = cur_loc();
    return ERROR;
  }

 ST_ID: // 文字列モード
  c = peek();
  if ( isalnum(c) || c == '_' ) {
    accept();
    mCurString.put_char(c);
    goto ST_ID;
  }
  loc = cur_loc();
  return SYMBOL;

 ST_NUM: // 数字モード
  c = peek();
  if ( c == '0' || c == '1' ) {
    accept();
    mCurString.put_char(c);
    goto ST_NUM;
  }
  loc = cur_loc();
  return INT_NUM;
}

// @brief 直前の read_token() で読んだトークン文字列を返す．
const char*
FhScanner::cur_string()
{
  return mCurString.c_str();
}

// @brief 直前の read_token() で読んだトークンの整数値を返す．
int
FhScanner::cur_int()
{
  return strtol(mCurString.c_str(), NULL, 10);
}

// @brief 一文字読み出す．
int
FhScanner::get()
{

  int c = peek();
  accept();
  return c;
}

// @brief 一文字先読みする．
int
FhScanner::peek()
{
  return mSrcString[mCurPos];
}

// @brief 先読みを確定する．
void
FhScanner::accept()
{
  ++ mCurPos;
}

// @brief トークンの先頭のファイル位置を設定する．
void
FhScanner::set_first_loc()
{
  mFirstColumn = mCurPos;
}

// @brief 現在のファイル位置を返す．
FileRegion
FhScanner::cur_loc()
{
  return FileRegion(mSrcLoc.start_file_info(),
		    mSrcLoc.start_line(), mSrcLoc.start_column() + mFirstColumn,
		    mSrcLoc.start_line(), mSrcLoc.start_column() + mCurPos);
}

END_NAMESPACE_YM_DOTLIB
