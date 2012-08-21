
/// @file xform_comp_gen.cc
/// @brief NpnXform の comp_table を作るためのプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// 順列を表す配列
ymuint8 perm_table[24][4] = {
#include "perm_table"
};

void
comp_table_gen()
{
  for (ymuint i = 0; i < 24; ++ i) {
    ymuint p0 = perm_table[i][0];
    ymuint p1 = perm_table[i][1];
    ymuint p2 = perm_table[i][2];
    ymuint p3 = perm_table[i][3];
    cout << "  ";
    const char* comma = "";
    for (ymuint j = 0; j < 24; ++ j) {
      ymuint q0 = perm_table[j][p0];
      ymuint q1 = perm_table[j][p1];
      ymuint q2 = perm_table[j][p2];
      ymuint q3 = perm_table[j][p3];
      // { q0, q1, q2, q3 } の順列の番号を探す．
      for (ymuint k = 0; k < 24; ++ k) {
	if ( perm_table[k][0] == q0 &&
	     perm_table[k][1] == q1 &&
	     perm_table[k][2] == q2 &&
	     perm_table[k][3] == q3 ) {
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
    ymuint p0 = perm_table[i][0];
    ymuint p1 = perm_table[i][1];
    ymuint p2 = perm_table[i][2];
    ymuint p3 = perm_table[i][3];
    // 逆変換の番号を探す．
    for (ymuint k = 0; k < 24; ++ k) {
      if ( perm_table[k][p0] == 0 &&
	   perm_table[k][p1] == 1 &&
	   perm_table[k][p2] == 2 &&
	   perm_table[k][p3] == 3 ) {
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
	new_nn |= (inv[j] << (perm_table[i][j] + 1));
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
	new_nn |= (inv[perm_table[i][j]] << (j + 1));
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

bool
is_equal(ymuint8* perm0,
	 ymuint8* perm1,
	 ymuint8 sup)
{
  for (ymuint i = 0; i < 4; ++ i) {
    if ( sup & (1U << i) ) {
      if ( perm0[i] != perm1[i] ) {
	return false;
      }
    }
  }
  return true;
}

void
rep_perm_table_gen()
{
  for (ymuint i = 0; i < 24; ++ i) {
    ymuint8* perm0 = perm_table[i];
    cout << "  {";
    const char* comma = "";
    for (ymuint sup = 0U; sup < 16U; ++ sup) {
      for (ymuint j = 0; j < 24; ++ j) {
	ymuint8* perm1 = perm_table[j];
	if ( is_equal(perm0, perm1, sup) ) {
	  cout << comma << j;
	  comma = ", ";
	  break;
	}
      }
    }
    cout << "}";
    if ( i < 23 ) {
      cout << ",";
    }
    cout << endl;
  }
}

void
xchg2_table_gen()
{
  const char* comma = "";
  for (ymuint i = 0; i < 24; ++ i) {
    ymuint8* perm0 = perm_table[i];
    ymuint i2 = i;
    if ( perm0[0] > perm0[1] ) {
      for (ymuint j = 0; j < 24; ++ j) {
	ymuint8* perm1 = perm_table[j];
	if ( perm1[0] == perm0[1] &&
	     perm1[1] == perm0[0] &&
	     perm1[2] == perm0[2] &&
	     perm1[3] == perm0[3] ) {
	  i2 = j;
	  break;
	}
      }
    }
    if ( 0 ) {
      cout << i << ": "
	   << static_cast<int>(perm0[0]) << " "
	   << static_cast<int>(perm0[1]) << " "
	   << static_cast<int>(perm0[2]) << " "
	   << static_cast<int>(perm0[3]) << endl;
      cout << "  ==> " << i2 << endl;
    }
    cout << comma << i2;
    comma = ", ";
  }
  cout << endl;
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
  else if ( strcmp(tblname, "rep_perm") == 0 ) {
    nsYm::rep_perm_table_gen();
  }
  else if ( strcmp(tblname, "xchg2") == 0 ) {
    nsYm::xchg2_table_gen();
  }
  else {
    nsYm::usage(argv[0]);
    return 2;
  }

  return 0;
}
