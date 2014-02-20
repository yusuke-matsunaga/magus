// $Id: npn_tablegen.cc 700 2007-05-31 00:41:30Z matsunaga $

// NPN パッケージのテスト

#include "ymtools.h"
#include "utils/PermGen.h"

//#define DEBUG

using namespace std;
using namespace nsYm;

void
naive(ymuint ni)
{
  ymuint ni_pow = 1 << ni;
  ymuint end = 1 << ni_pow;
  vector<ymuint> table(end);
  vector<ymuint> tmp_order(ni);
  list<ymuint> mo_list;
  for (ymuint fv = 0; fv < end; ++ fv) {
    // 並べ替えのために fv をベクタにばらす
    vector<int> fvect(ni_pow);
    for (ymuint i = 0; i < ni_pow; ++ i) {
      if ( fv & (1 << i) ) {
	fvect[i] = 1;
      }
      else {
	fvect[i] = 0;
      }
    }

    // 反転用のマスク
    ymuint omask = (1 << ni_pow) - 1;

    bool first = true;
    ymuint max_vect = 0UL;
    for (PermGen pg(ni, ni); !pg.is_end(); ++ pg) {
      for (ymuint i = 0; i < ni; ++ i) {
	tmp_order[i] = pg(i);
      }
      for (ymuint b = 0; b < ni_pow; ++ b) {
	// tmp_order, b にしたがって fv を変換する
	ymuint tmp = 0UL;
	for (ymuint i = 0; i < ni_pow; ++ i) {
	  if ( fvect[i ^ b] ) {
	    ymuint new_i = 0;
	    for (ymuint j = 0; j < ni; ++ j) {
	      if ( i & (1 << j) ) {
		new_i |= (1 << tmp_order[j]);
	      }
	    }
	    tmp |= 1 << new_i;
	  }
	}
	if ( first == true || max_vect < tmp ) {
	  first = false;
	  max_vect = tmp;
	  mo_list.clear();
	  ymuint v = b << 1;
	  for (ymuint i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	else if ( max_vect == tmp ) {
	  ymuint v = b << 1;
	  for (ymuint i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	tmp ^= omask;
	if ( max_vect < tmp ) {
	  max_vect = tmp;
	  mo_list.clear();
	  ymuint v = (b << 1) | 1;
	  for (ymuint i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
	else if ( max_vect == tmp ) {
	  ymuint v = (b << 1) | 1;
	  for (ymuint i = 0; i < ni; ++ i) {
	    v |= (tmp_order[i] << (i * 2 + 5));
	  }
	  mo_list.push_back(v);
	}
      }
    }
    ymuint v = max_vect | (mo_list.front() << 16);
    table[fv] = v;
#if 0
    cout << "original func: ";
    for (ymuint i = 0; i < ni_pow; ++ i) {
      cout << fvect[i];
    }
    cout << endl;
    cout << "max func:      ";
    for (ymuint i = 0; i < ni_pow; ++ i) {
      if ( max_vect & (1 << i) ) {
	cout << "1";
      }
      else {
	cout << "0";
      }
    }
    cout << endl;
    for (list<ymuint>::const_iterator p = mo_list.begin();
	 p != mo_list.end(); ++ p) {
      ymuint tmp = *p;
      ymuint b = tmp & 0xFF;
      if ( b & 1 ) {
	cout << " ~(";
      }
      else {
	cout << "  (";
      }
      b >>= 1;
      const char* comma = "";
      for (ymuint i = 0; i < ni; ++ i) {
	cout << comma;
	comma = ", ";
	if ( b & (1 << i) ) {
	  cout << "~";
	}
	else {
	  cout << " ";
	}
	ymuint v = (tmp >> (i * 2 + 4)) & 0x3;
	cout << v;
      }
      cout << ")" << endl;
    }
    cout << endl;
#endif
  }

  bool first = true;
  for (ymuint i = 0; i < end; ++ i) {
    if ( first ) {
      first = false;
    }
    else {
      cout << "," << endl;
    }
    cout << table[i];
  }
  cout << endl;
}

int
main(int argc, char** argv)
{
  try {
    if ( argc != 2 ) {
      return 2;
    }

    ymuint ni = atoi(argv[1]);
    naive(ni);
  }
  catch ( nsYm::AssertError x ) {
    cerr << x;
  }
  return 0;
}
