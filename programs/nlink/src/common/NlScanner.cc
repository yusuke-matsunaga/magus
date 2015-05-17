
/// @file NlScanner.cc
/// @brief NlScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlScanner.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス NlScanner
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ido 入力データ
NlScanner::NlScanner(IDO& ido) :
  Scanner(ido)
{
}

// @brief デストラクタ
NlScanner::~NlScanner()
{
}

// @brief SIZE 行を読み込む．
// @param[out] width 読み込んだ幅を入れる変数
// @param[out] height 読み込んだ高さを入れる変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::read_SIZE(ymuint& width,
		     ymuint& height)
{
  if ( !expect_STRING("SIZE") ) {
    return false;
  }

  if ( !expect_NUMBER(width) ) {
    cerr << "syntax error at expecting 'NUMBER'(width)" << endl;
    return false;
  }

  if ( !expect_STRING("X") ) {
    return false;
  }

  if ( !expect_NUMBER(height) ) {
    cerr << "syntax error at expecting 'NUMBER'(height)" << endl;
    return false;
  }

  return expect_NL();
}

// @brief LINE_NUM 行を読み込む．
// @param[out] num 読み込んだ要素数を入れる変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::read_LINE_NUM(ymuint& num)
{
  if ( !expect_STRING("LINE_NUM") ) {
    return false;
  }

  if ( !expect_NUMBER(num) ) {
    cerr << "syntax error at expecting 'NUMBER'(LINE_NUM)" << endl;
    return false;
  }

  return expect_NL();
}

// @brief LINE行を読み込む．
// @param[out] x1, y1 始点の座標を入れる変数
// @param[out] x2, y2 終点の座標を入れる変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::read_LINE(ymuint& x1,
		     ymuint& y1,
		     ymuint& x2,
		     ymuint& y2)
{
  if ( !expect_STRING("LINE#") ) {
    return false;
  }

  ymuint num;
  if ( !expect_NUMBER(num) ) {
    cerr << "syntax error at expecting 'NUMBER'(LINE#)" << endl;
    return false;
  }

  if ( !expect_LP() ) {
    cerr << "syntax error at expecting '('" << endl;
    return false;
  }

  if ( !expect_NUMBER(x1) ) {
    cerr << "syntax error at expecting 'NUMBER'(x1)" << endl;
    return false;
  }

  if ( !expect_COMMA() ) {
    cerr << "syntax error at expecting ','" << endl;
    return false;
  }

  if ( !expect_NUMBER(y1) ) {
    cerr << "syntax error at expecting 'NUMBER'(y1)" << endl;
    return false;
  }

  if ( !expect_RP() ) {
    cerr << "syntax error at expecting ')'" << endl;
    return false;
  }

  if ( !expect_DASH() ) {
    cerr << "syntax error at expecting '-'" << endl;
    return false;
  }

  if ( !expect_LP() ) {
    cerr << "syntax error at expecting '('" << endl;
    return false;
  }

  if ( !expect_NUMBER(x2) ) {
    cerr << "syntax error at expecting 'NUMBER'(x2)" << endl;
    return false;
  }

  if ( !expect_COMMA() ) {
    cerr << "syntax error at expecting ','" << endl;
    return false;
  }

  if ( !expect_NUMBER(y2) ) {
    cerr << "syntax error at expecting 'NUMBER'(y2)" << endl;
    return false;
  }

  if ( !expect_RP() ) {
    cerr << "syntax error at expecting ')'" << endl;
    return false;
  }

  return expect_NL();
}

// @brief 数字を読み込む．
// @param[out] num 読み込んだ値を入れる変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_NUMBER(ymuint& num)
{
  skip_SPACE();

  num = 0;

  bool has_num = false;
  for ( ; ; ) {
    int c = peek();
    if ( '0' <= c && c <= '9' ) {
      accept();
      num = num * 10 + static_cast<ymuint>(c - '0');
      has_num = true;
    }
    else {
      return has_num;
    }
  }
}

// @brief 指定された文字列を読み込む．
// @param[in] str 文字列
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_STRING(const char* str)
{
  skip_SPACE();
  for (const char* s = str; *s != '\0'; ++ s) {
    if ( peek() != *s ) {
      cerr << "syntax error at expecting '" << str << "'" << endl;
      return false;
    }
    accept();
  }
  return true;
}

// @brief '(' を読み込む．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_LP()
{
  skip_SPACE();
  if ( peek() != '(' ) {
    return false;
  }
  accept();
  return true;
}

// @brief ')' を読み込む．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_RP()
{
  skip_SPACE();
  if ( peek() != ')' ) {
    return false;
  }
  accept();
  return true;
}

// @brief ',' を読み込む．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_COMMA()
{
  skip_SPACE();
  if ( peek() != ',' ) {
    return false;
  }
  accept();
  return true;
}

// @brief '-' を読み込む．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_DASH()
{
  skip_SPACE();
  if ( peek() != '-' ) {
    return false;
  }
  accept();
  return true;
}

// @brief 改行を読み込む．
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
NlScanner::expect_NL()
{
  bool has_nl = false;
  for ( ; ; ) {
    skip_SPACE();
    if ( peek() != '\n' ) {
      break;
    }
    has_nl = true;
    accept();
  }
  return has_nl;
}

// @brief 空白を読み飛ばす．
void
NlScanner::skip_SPACE()
{
  for ( ; ; ) {
    int c = peek();
    if ( c != ' ' && c != '\t' ) {
      break;
    }
    accept();
  }
}

END_NAMESPACE_YM_NLINK
