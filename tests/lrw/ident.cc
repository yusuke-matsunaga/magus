
/// @file ident.cc
/// @brief 恒等変換を求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "NpnXform.h"


BEGIN_NAMESPACE_YM

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint16 npn4perm[] = {
#include "npn4perm.h"
};

// @brief 関数ベクタを変換する(4入力版)
ymuint32
xform_func4(ymuint16 fv,
	    ymuint16 perm)
{
  NpnXform xf(perm);

  ymuint32 ans = 0U;

  for (ymuint p = 0; p < 16; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      ymuint v3 = (p >> 3) & 1U;
      if ( xf.input_inv(0) ) {
	v0 ^= 1U;
      }
      if ( xf.input_inv(1) ) {
	v1 ^= 1U;
      }
      if ( xf.input_inv(2) ) {
	v2 ^= 1U;
      }
      if ( xf.input_inv(3) ) {
	v3 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << xf.input_perm(0));
      }
      if ( v1 ) {
	q |= (1U << xf.input_perm(1));
      }
      if ( v2 ) {
	q |= (1U << xf.input_perm(2));
      }
      if ( v3 ) {
	q |= (1U << xf.input_perm(3));
      }
      ans |= (1U << q);
    }
  }
  if ( xf.output_inv() ) {
    ans ^= 0xFFFF;
  }
  return ans;
}

void
get_ident(ymuint16 func,
	  vector<ymuint16>& ident_list)
{
  for (ymuint p = 0; p < 768; ++ p) {
    ymuint16 perm_data = npn4perm[p];
    ymuint16 func1 = xform_func4(func, perm_data);
    if ( func1 == func ) {
      ident_list.push_back(perm_data);
    }
  }
}

int
ident(int argc,
      const char** argv)
{
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    if ( i > 0 ) {
      cout << "," << endl;
    }
    ymuint16 func = npn4rep[i];
    vector<ymuint16> ident_list;
    get_ident(func, ident_list);
    cout << "{";
    const char* comma = "";
    for (vector<ymuint16>::iterator p = ident_list.begin();
	 p != ident_list.end(); ++ p) {
      cout << comma << "0x" << setw(4) << setfill('0') << hex << *p << dec;
      comma = ", ";
    }
    cout << "}";
  }
  cout << endl;
  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  return nsYm::ident(argc, argv);
}
