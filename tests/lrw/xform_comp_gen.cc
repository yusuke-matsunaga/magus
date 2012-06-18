
/// @file xform_comp_gen.cc
/// @brief NpnXform の comp_table を作るためのプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// 順列を表す配列
ymuint8 perm[24][4] = {
  { 0, 1, 2, 3},
  { 0, 1, 3, 2},
  { 0, 2, 1, 3},
  { 0, 2, 3, 1},
  { 0, 3, 1, 2},
  { 0, 3, 2, 1},
  { 1, 0, 2, 3},
  { 1, 0, 3, 2},
  { 1, 2, 0, 3},
  { 1, 2, 3, 0},
  { 1, 3, 0, 2},
  { 1, 3, 2, 0},
  { 2, 0, 1, 3},
  { 2, 0, 3, 1},
  { 2, 1, 0, 3},
  { 2, 1, 3, 0},
  { 2, 3, 0, 1},
  { 2, 3, 1, 0},
  { 3, 0, 1, 2},
  { 3, 0, 2, 1},
  { 3, 1, 0, 2},
  { 3, 1, 2, 0},
  { 3, 2, 0, 1},
  { 3, 2, 1, 0}
};

void
comp_table_gen()
{
  for (ymuint i = 0; i < 24; ++ i) {
    ymuint p0 = perm[i][0];
    ymuint p1 = perm[i][1];
    ymuint p2 = perm[i][2];
    ymuint p3 = perm[i][3];
    cout << "  ";
    const char* comma = "";
    for (ymuint j = 0; j < 24; ++ j) {
      ymuint q0 = perm[j][p0];
      ymuint q1 = perm[j][p1];
      ymuint q2 = perm[j][p2];
      ymuint q3 = perm[j][p3];
      // { q0, q1, q2, q3 } の順列の番号を探す．
      for (ymuint k = 0; k < 24; ++ k) {
	if ( perm[k][0] == q0 &&
	     perm[k][1] == q1 &&
	     perm[k][2] == q2 &&
	     perm[k][3] == q3 ) {
	  cout << comma << k;
	  comma = ",";
	  break;
	}
      }
    }
    if ( i < 23 ) {
      cout << ",";
    }
    cout << endl;
  }
}

void
inv_table_gen()
{
  cout << "  ";
  const char* comma = "";
  for (ymuint i = 0; i < 24; ++ i) {
    ymuint p0 = perm[i][0];
    ymuint p1 = perm[i][1];
    ymuint p2 = perm[i][2];
    ymuint p3 = perm[i][3];
    // 逆変換の番号を探す．
    for (ymuint k = 0; k < 24; ++ k) {
      if ( perm[k][p0] == 0 &&
	   perm[k][p1] == 1 &&
	   perm[k][p2] == 2 &&
	   perm[k][p3] == 3 ) {
	cout << comma << k;
	comma = ",";
	break;
      }
    }
  }
  cout << endl;
}

void
nperm_table_gen()
{
  for (ymuint nn = 0; nn < 32; ++ nn) {
    ymuint inv[4];
    for (ymuint i = 0; i < 4; ++ i) {
      inv[i] = (nn >> (i + 1)) & 1U;
    }

    cout << "  ";
    const char* comma = "";
    for (ymuint i = 0; i < 24; ++ i) {
      ymuint new_nn = nn & 1U;
      for (ymuint j = 0; j < 4; ++ j) {
	new_nn |= (inv[j] << (perm[i][j] + 1));
      }
      cout << comma << new_nn;
      comma = ",";
    }
    if ( nn < 31 ) {
      cout << ",";
    }
    cout << endl;
  }
}

void
inv_nperm_table_gen()
{
  for (ymuint nn = 0; nn < 32; ++ nn) {
    ymuint inv[4];
    for (ymuint i = 0; i < 4; ++ i) {
      inv[i] = (nn >> (i + 1)) & 1U;
    }

    cout << "  ";
    const char* comma = "";
    for (ymuint i = 0; i < 24; ++ i) {
      ymuint new_nn = nn & 1U;
      for (ymuint j = 0; j < 4; ++ j) {
	new_nn |= (inv[perm[i][j]] << (j + 1));
      }
      cout << comma << new_nn;
      comma = ",";
    }
    if ( nn < 31 ) {
      cout << ",";
    }
    cout << endl;
  }
}

void
usage(const char* progname)
{
  cerr << "Usage: " << progname << " tblname" << endl;
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  if ( argc != 2 ) {
    nsYm::usage(argv[0]);
    return 1;
  }

  const char* tblname = argv[1];
  if ( strcmp(tblname, "comp") == 0 ) {
    nsYm::comp_table_gen();
  }
  else if ( strcmp(tblname, "inv") == 0 ) {
    nsYm::inv_table_gen();
  }
  else if ( strcmp(tblname, "nperm") == 0 ) {
    nsYm::nperm_table_gen();
  }
  else if ( strcmp(tblname, "inv_nperm") == 0 ) {
    nsYm::inv_nperm_table_gen();
  }
  else {
    nsYm::usage(argv[0]);
    return 2;
  }

  return 0;
}
