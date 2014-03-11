
/// @file mincov_test.cc
/// @brief mincov_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/MincovMatrix.h"
#include "utils/MincovCost.h"
#include "utils/MincovSolver.h"


BEGIN_NAMESPACE_YM

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
	   int& num2)
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

  return 1;
}

int
mincov_test(int argc,
	    char** argv)
{
  char buff[1024];
  bool go_on = true;
  int max_r = 0;
  int max_c = 0;
  vector<pair<int, int> > pair_list;
  while ( go_on && cin.getline(buff, 1024, '\n') ) {
    int num1;
    int num2;
    int stat = parse_line(buff, num1, num2);
    if ( stat == -1 ) {
      cerr << "Error: " << buff << endl;
      return 1;
    }
    else if ( stat == 0 ) {
      // コメント行
    }
    else if ( stat == 1 ) {
      if ( num1 == -1 && num2 == -1 ) {
	go_on = false;
      }
      else {
	if ( max_r < num2 ) {
	  max_r = num2;
	}
	if ( max_c < num1 ) {
	  max_c = num1;
	}
	pair_list.push_back(make_pair(num1, num2));
      }
    }
  }

  MincovMatrix matrix(max_r + 1, max_c + 1);
  for (vector<pair<int, int> >::iterator p = pair_list.begin();
       p != pair_list.end(); ++ p) {
    matrix.insert_elem(p->second, p->first);
  }

  MincovSolver solver;

  vector<ymuint32> solution;
  MincovCost cost = solver(matrix, solution);

  for (vector<ymuint32>::iterator p = solution.begin();
       p != solution.end(); ++ p) {
    cout << " " << *p;
  }
  cout << endl;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  return nsYm::mincov_test(argc, argv);
}
