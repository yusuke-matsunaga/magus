
/// @file BtgMatchTest.cc
/// @brief BtgMatch のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "YmAlgo/BtgMatch.h"


BEGIN_NAMESPACE_YM
#if 0
// c が 0 から 9 までの数字なら true を返す．
bool
is_num(char c)
{
  return c >= '0' && c <= '9';
}

// c が空白なら true を返す．
bool
is_spc(char c)
{
  return c == ' ' || c == '\t';
}

// 数字を読み込む．
bool
parse_num(char* buff,
	  ymuint& rpos,
	  int& num)
{
  ymuint pos0 = rpos;

  // 最初だけ '-' がありうる
  if ( buff[rpos] == '-' ) {
    ++ rpos;
  }

  ymuint pos1 = rpos;
  for ( ; is_num(buff[rpos]); ++ rpos) {
    ;
  }
  if ( rpos == pos1 ) {
    return false;
  }

  char old_char = buff[rpos];
  buff[rpos] = '\0';
  num = atoi(&buff[pos0]);
  buff[rpos] = old_char;
  return true;
}

// 1行をパーズする．
// 形式は <num> <sp> <num> (<sp> <num>)
// <num> ::= '-'?[1-9][0-9]*
// <sp>  ::= ' '|'\t'
// ただし，'#' で始まる行はコメント行とみなす．
// 読み込んだ数字の数を返す．
// エラーの場合には -1 を返す．
// コメント行の場合には 0 を返す．
int
parse_line(char* buff,
	   int& num1,
	   int& num2,
	   int& num3)
{
  if ( buff[0] == '#' ) {
    return 0;
  }

  ymuint count = 0;
  ymuint pos = 0;

  if ( !parse_num(buff, pos, num1) ) {
    return -1;
  }

  while ( is_spc(buff[pos]) ) {
    ++ pos;
  }

  if ( !parse_num(buff, pos, num2) ) {
    return -1;
  }

  while ( is_spc(buff[pos]) ) {
    ++ pos;
  }

  if ( buff[pos] == '\0' ) {
    num3 = 0;
    return 2;
  }

  if ( !parse_num(buff, pos, num3) ) {
    return -1;
  }
  return 3;
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
    int num3;
    int stat = parse_line(buff, num1, num2, num3);
    if ( stat == -1 ) {
      cerr << "Error: " << buff << endl;
      return -1;
    }
    else if ( stat == 0 ) {
      // コメント行
      ;
    }
    else if ( stat == 2 ) {
      cerr << "Num1 = " << num1 << ", Num2 = " << num2 << endl;
      if ( num1 == -1 ) {
	go_on = false;
	break;
      }
      if ( first_line ) {
	first_line = false;
	bm.set_size(num1, num2);
      }
      else {
	bm.add_edge(num1, num2);
      }
    }
    else if ( stat == 3 ) {
      cerr << "Num1 = " << num1 << ", Num2 = " << num2 << ", Num3 = " << num3 << endl;
      if ( num1 == -1 ) {
	go_on = false;
	break;
      }
      if ( first_line ) {
	cerr << "syntax error: <#v1> <#v2> expected" << endl;
	return -1;
      }
      bm.add_edge(num1, num2, num3);
    }
    else {
      cerr << "syntax error: <num> <num> ?<num>? expected" << endl;
      return -1;
    }
  }

  vector<ymuint> edge_list;
  int w = bm.calc_match(edge_list);

  cout << "Total weight = " << w << endl;
  for (vector<ymuint>::iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    ymuint e_id = *p;
    ymuint node1;
    ymuint node2;
    ymuint weight;
    bm.edge_info(e_id, node1, node2, weight);
    cout << node1 << " - " << node2 << endl;
  }

  return 0;
}
#endif

class BtgMatchTest :
  public testing::Test
{
public:

  BtgMatchTest();

  virtual
  ~BtgMatchTest();


public:

  BtgMatch mBtgMatch;

};

BtgMatchTest::BtgMatchTest()
{
}

BtgMatchTest::~BtgMatchTest()
{
}

TEST_F( BtgMatchTest, Empty )
{
  // なにもしない．
  // コンストラクタ/デストラクタでエラーがおきないことのテスト
}

TEST_F( BtgMatchTest, SetSize )
{
  const ymuint n1 = 10;
  const ymuint n2 = 20;

  mBtgMatch.set_size(n1, n2);

  EXPECT_EQ( n1, mBtgMatch.node1_num() ) << "mBtgMatch.node1_num() != " << n1;
  EXPECT_EQ( n2, mBtgMatch.node2_num() ) << "mBtgMatch.node2_num() != " << n1;
}

END_NAMESPACE_YM
