
/// @file BtgMatch_test.cc
/// @brief BtgMatch のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BtgMatch.h"


BEGIN_NAMESPACE_YM

// c が 1 から 9 までの数字なら true を返す．
bool
is_num1(char c)
{
  return c >= '1' && c <= '9';
}

// c が 0 から 9 までの数字なら true を返す．
bool
is_num2(char c)
{
  return c >= '0' && c <= '9';
}

// c が空白なら true を返す．
bool
is_spc(char c)
{
  return c == ' ' || c == '\t' || c == '\0';
}

// 1行をパーズする．
// 形式は <num> <sp> <num>
// <num> ::= '-'?[1-9][0-9]*
// <sp>  ::= ' '|'\t'
// ただし，'#' で始まる行はコメント行とみなす．
// 返り値は
//  - 0: エラー
//  - 1: 成功
//  - 2: コメント行
//
// 副作用で buff の内容は書き換えられる．
int
parse_line(char* buff,
	   int& num1,
	   int& num2)
{
  if ( buff[0] == '#' ) {
    return 2;
  }

  ymuint count = 0;
  ymuint pos = 0;

 start:
  ymuint pos0 = pos;
  if ( buff[pos] == '-' ) {
    ++ pos;
  }

  if ( is_num1(buff[pos]) ) {
    ++ pos;
  }
  else {
    // エラー
    return 0;
  }
  // わざと次に続く

 number2:
  if ( is_num2(buff[pos]) ) {
    ++ pos;
    goto number2;
  }
  else if ( is_spc(buff[pos]) ) {
    if ( count == 0 ) {
      buff[pos] = '\0';
      num1 = atoi(&buff[pos0]);
      ++ count;
    }
    else if ( count == 1 ) {
      buff[pos] = '\0';
      num2 = atoi(&buff[pos0]);
      return 1;
    }
    ++ pos;
    goto space;
  }
  else {
    // エラー
    return 0;
  }

 space:
  if ( is_spc(buff[pos]) ) {
    ++ pos;
    goto space;
  }
  else {
    goto start;
  }
}

int
BtgMatch_test(int argc,
	      const char** argv)
{

  BtgMatch bm;

  char buff[1024];
  bool go_on = true;
  bool first_line = true;
  while ( go_on && cin.getline(buff, 1024, '\n') ) {
    int num1;
    int num2;
    int stat = parse_line(buff, num1, num2);
    if ( stat == 0 ) {
      cerr << "Error: " << buff << endl;
      return -1;
    }
    else if ( stat == 1 ) {
      cerr << "Num1 = " << num1 << ", Num2 = " << num2 << endl;
      if ( num1 == -1 ) {
	go_on = false;
	break;
      }
      if ( first_line ) {
	first_line = false;
	bm.resize(num1, num2);
      }
      else {
	bm.add_edge(num1, num2);
      }
    }
    else if ( stat == 2 ) {
      // コメント行
      ;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
  }

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::BtgMatch_test(argc, argv);
}
