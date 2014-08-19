
/// @file cannonical.cc
/// @brief NPN同値類の代表関数を求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
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

ymuint16
cannonical4(ymuint16 func,
	    const unordered_set<ymuint32>& rep_hash,
	    ymuint16& cperm)
{
  // ベタなやり方
  // 全ての変換を試して変換結果が代表関数になった変換を返す．
  for (ymuint p = 0; p < 768; ++ p) {
    ymuint16 perm_data = npn4perm[p];
    ymuint16 fv1 = xform_func4(func, perm_data);
    if ( rep_hash.count(fv1) > 0 ) {
      cperm = perm_data;
      return fv1;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

void
cannonical(bool debug)
{
  // 代表関数を記録するハッシュ表
  unordered_set<ymuint32> rep_hash;
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    rep_hash.insert(npn4rep[i]);
  }

  for (ymuint32 func = 0; func < 65536; ++ func) {
    ymuint16 cperm;
    ymuint16 cfunc = cannonical4(func, rep_hash, cperm);
    if ( debug ) {
      NpnXform xf(cperm);
      cout << "original: " << setw(4) << setfill('0')
	   << hex << func << dec << endl
	   << "  cfunc: " << setw(4) << setfill('0')
	   << hex << cfunc << dec << endl
	   << "  cperm:" << xf << endl
	   << endl;
    }
    else {
      cout << "  { 0x" << setw(4) << setfill('0')
	   << hex << cfunc << dec
	   << ", 0x" << setw(4) << setfill('0')
	   << hex << cperm << dec << "}, " << endl;
    }
  }
}

void
usage(char* progname)
{
  cerr << "USAGE: " << progname << " [-debug]" << endl;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  bool debug = false;

  if ( argc == 2 ) {
    if ( strcmp(argv[1], "-debug") == 0 ) {
      debug = true;
      -- argc;
    }
  }
  if ( argc != 1 ) {
    usage(argv[0]);
    exit(1);
  }

  cannonical(debug);

  return 0;

}
