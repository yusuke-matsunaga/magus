
/// @file mincov_test.cc
/// @brief mincov_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/McSolver.h"


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

bool
parse_data(istream& s,
	   vector<pair<int, int> >& pair_list)
{
  char buff[1024];
  bool go_on = true;
  while ( go_on && s.getline(buff, 1024, '\n') ) {
    int num1;
    int num2;
    int stat = parse_line(buff, num1, num2);
    if ( stat == -1 ) {
      cerr << "Error: " << buff << endl;
      return false;
    }
    else if ( stat == 0 ) {
      // コメント行
    }
    else if ( stat == 1 ) {
      if ( num1 == -1 && num2 == -1 ) {
	go_on = false;
      }
      else {
	pair_list.push_back(make_pair(num1, num2));
      }
    }
  }
  return true;
}

int
mincov_test(int argc,
	    char** argv)
{
  bool heuristic = false;
  ymuint base = 1;
  if ( argc > base && argv[base][0] == '-' ) {
    if ( strcmp(argv[1], "-h") == 0 ) {
      heuristic = true;
      ++ base;
    }
    else {
      cerr << argv[1] << " : invalid option" << endl;
      return 1;
    }
  }

  vector<pair<int, int> > pair_list;

  if ( argc > base ) {
    ifstream s(argv[base]);
    if ( !s ) {
      cerr << argv[base] << " : no such file" << endl;
      return 1;
    }
    parse_data(s, pair_list);
  }
  else {
    parse_data(cin, pair_list);
  }

  ymuint max_r = 0;
  ymuint max_c = 0;
  for (vector<pair<int, int> >::iterator p = pair_list.begin();
       p != pair_list.end(); ++ p) {
    ymuint r = p->second;
    ymuint c = p->first;
    if ( max_r < r ) {
      max_r = r;
    }
    if ( max_c < c ) {
      max_c = c;
    }
  }

  McSolver solver;

  solver.set_size(max_r + 1, max_c + 1);
  for (vector<pair<int, int> >::iterator p = pair_list.begin();
       p != pair_list.end(); ++ p) {
    solver.insert_elem(p->second, p->first);
  }

  vector<ymuint32> solution;
  ymuint cost = 0;
  if ( heuristic ) {
    cost = solver.heuristic(solution);
  }
  else {
    cost = solver.exact(solution);
  }

  cout << "Cost = " << cost << endl;
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
