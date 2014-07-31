
/// @file npntest.cc
/// @brief NpnXform のテストプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "NpnXform.h"
#include "YmUtils/PermGen.h"


BEGIN_NAMESPACE_YM

void
print_xf(ostream& s,
	 NpnXform xf)
{
  s << "(" << xf.input_perm(0)
    << ", " << xf.input_perm(1)
    << ", " << xf.input_perm(2)
    << ", " << xf.input_perm(3)
    << ") ";
  if ( xf.input_inv(0) ) {
    s << "N";
  }
  else {
    s << "P";
  }
  if ( xf.input_inv(1) ) {
    s << "N";
  }
  else {
    s << "P";
  }
  if ( xf.input_inv(2) ) {
    s << "N";
  }
  else {
    s << "P";
  }
  if ( xf.input_inv(3) ) {
    s << "N";
  }
  else {
    s << "P";
  }
  s << "|";
  if ( xf.output_inv() ) {
    s << "N";
  }
  else {
    s << "P";
  }
}

bool
test1()
{
  cout << "test1" << endl;
  bool error = false;
  for (PermGen pg(4, 4); !pg.is_end(); ++ pg) {
    ymuint perm[4];
    for (ymuint i = 0; i < 4; ++ i) {
      perm[i] = pg(i);
    }
    ymuint pid = NpnXform::perm_id(perm);
    for (ymuint pols = 0; pols < 32; ++ pols) {
      NpnXform xf(pid, pols);

      print_xf(cout, xf);
      cout << endl;

      bool error1 = false;
      for (ymuint i = 0; i < 4; ++ i) {
	if ( xf.input_perm(i) != perm[i] ) {
	  error1 = true;
	  break;
	}
	ymuint p_ref = (pols >> (i + 1)) & 1U;
	ymuint p_cur = xf.input_inv(i);
	if ( p_ref != p_cur ) {
	  error1 = true;
	  break;
	}
      }
      ymuint p_ref = pols & 1U;
      ymuint p_cur = xf.output_inv();
      if ( p_ref != p_cur ) {
	error1 = true;
      }
      if ( error1 ) {
	cout << "Error" << endl
	     << "  spec: "
	     << "(" << perm[0]
	     << ", " << perm[1]
	     << ", " << perm[2]
	     << ", " << perm[3]
	     << ") ";
	if ( pols & 2U ) {
	  cout << "N";
	}
	else {
	  cout << "P";
	}
	if ( pols & 4U ) {
	  cout << "N";
	}
	else {
	  cout << "P";
	}
	if ( pols & 8U ) {
	  cout << "N";
	}
	else {
	  cout << "P";
	}
	if ( pols & 16U ) {
	  cout << "N";
	}
	else {
	  cout << "P";
	}
	cout << "|";
	if ( pols & 1U ) {
	  cout << "N";
	}
	else {
	  cout << "P";
	}
	cout << endl;
	cout << "current: ";
	print_xf(cout, xf);
	cout << endl;
      }
    }
  }
  return error;
}

bool
test2()
{
  cout << "test2" << endl;
  bool error = false;
  for (PermGen pg(4, 4); !pg.is_end(); ++ pg) {
    ymuint perm[4];
    for (ymuint i = 0; i < 4; ++ i) {
      perm[i] = pg(i);
    }
    ymuint pid = NpnXform::perm_id(perm);
    for (ymuint pols = 0; pols < 32; ++ pols) {
      NpnXform xf(pid, pols);

      for (PermGen pg2(4, 4); !pg2.is_end(); ++ pg2) {
	ymuint perm2[4];
	for (ymuint i = 0; i < 4; ++ i) {
	  perm2[i] = pg2(i);
	}
	ymuint pid2 = NpnXform::perm_id(perm2);
	for (ymuint pols2 = 0; pols2 < 32; ++ pols2) {
	  NpnXform xf2(pid2, pols2);

	  NpnXform cxf = xf * xf2;

	  ymuint i0 = perm[0];
	  ymuint i1 = perm[1];
	  ymuint i2 = perm[2];
	  ymuint i3 = perm[3];

	  ymuint j0 = perm2[i0];
	  ymuint j1 = perm2[i1];
	  ymuint j2 = perm2[i2];
	  ymuint j3 = perm2[i3];

	  ymuint inv0 = (pols >> 1) & 1U;
	  inv0 ^= ((pols2 >> (i0 + 1)) & 1U);
	  ymuint inv1 = (pols >> 2) & 1U;
	  inv1 ^= ((pols2 >> (i1 + 1)) & 1U);
	  ymuint inv2 = (pols >> 3) & 1U;
	  inv2 ^= ((pols2 >> (i2 + 1)) & 1U);
	  ymuint inv3 = (pols >> 4) & 1U;
	  inv3 ^= ((pols2 >> (i3 + 1)) & 1U);
	  ymuint oinv = ((pols >> 0) & 1U) ^ ((pols2 >> 0) & 1U);

	  if ( cxf.input_perm(0) != j0 ||
	       cxf.input_perm(1) != j1 ||
	       cxf.input_perm(2) != j2 ||
	       cxf.input_perm(3) != j3 ||
	       cxf.input_inv(0) != inv0 ||
	       cxf.input_inv(1) != inv1 ||
	       cxf.input_inv(2) != inv2 ||
	       cxf.input_inv(3) != inv3 ||
	       cxf.output_inv() != oinv) {
	    error = true;
	    cout << "Error" << endl;
	    print_xf(cout, xf);
	    cout << endl;
	    print_xf(cout, xf2);
	    cout << endl;
	    print_xf(cout, cxf);
	    cout << endl
		 << endl;
	  }
	}
      }
    }
  }
  return error;
}

bool
test3()
{
  cout << "test3" << endl;
  bool error = false;
  for (PermGen pg(4, 4); !pg.is_end(); ++ pg) {
    ymuint perm[4];
    for (ymuint i = 0; i < 4; ++ i) {
      perm[i] = pg(i);
    }
    ymuint pid = NpnXform::perm_id(perm);
    for (ymuint pols = 0; pols < 32; ++ pols) {
      NpnXform xf(pid, pols);

      NpnXform ixf = inverse(xf);

      NpnXform tmp = xf * ixf;
      bool error1 = false;
      for (ymuint i = 0; i < 4; ++ i) {
	if ( tmp.input_perm(i) != i ) {
	  error1 = true;
	  break;
	}
	if ( tmp.input_inv(i) ) {
	  error1 = true;
	  break;
	}
      }
      if ( tmp.output_inv() ) {
	error1 = true;
      }
      if ( error1 ) {
	cout << "Error in inverse" << endl;
	print_xf(cout, xf);
	cout << endl;
	print_xf(cout, ixf);
	cout << endl;
	print_xf(cout, tmp);
	cout << endl
	     << endl;
      }
    }
  }
  return error;
}

void
npntest()
{
  bool error = false;
  if ( test1() ) {
    error = true;
  }
  if ( test2() ) {
    error = true;
  }
  if ( test3() ) {
    error = true;
  }

  if ( error ) {
    cout << "Test failed." << endl;
  }
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  nsYm::npntest();

  return 0;
}
