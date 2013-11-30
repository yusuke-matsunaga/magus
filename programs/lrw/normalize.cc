
/// @file normalize.cc
/// @brief 変数番号を詰めて正極性に整える変換テーブルを作るプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "NpnXform.h"


BEGIN_NAMESPACE_YM

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

// サポートを求める．
ymuint
support(ymuint16 func)
{
  ymuint16 supvec = 0U;
  ymuint16 negvec = 0U;

  // 0番めの変数によるコファクター
  ymuint16 c0_0 = func & 0x5555U;
  ymuint16 c0_1 = (func & 0xaaaaU) >> 1;
  if ( c0_0 != c0_1 ) {
    supvec |= 1U;
    if ( (c0_0 & c0_1) == c0_1 ) {
      negvec |= 1U;
    }
  }

  // 1番めの変数によるコファクター
  ymuint16 c1_0 = func & 0x3333U;
  ymuint16 c1_1 = (func & 0xccccU) >> 2;
  if ( c1_0 != c1_1 ) {
    supvec |= 2U;
    if ( (c1_0 & c1_1) == c1_1 ) {
      negvec |= 2U;
    }
  }

  // 2番めの変数によるコファクター
  ymuint16 c2_0 = func & 0x0f0fU;
  ymuint16 c2_1 = (func & 0xf0f0U) >> 4;
  if ( c2_0 != c2_1 ) {
    supvec |= 4U;
    if ( (c2_0 & c2_1) == c2_1 ) {
      negvec |= 4U;
    }
  }

  // 3番めの変数によるコファクター
  ymuint16 c3_0 = func & 0x00ffU;
  ymuint16 c3_1 = (func & 0xff00U) >> 8;
  if ( c3_0 != c3_1 ) {
    supvec |= 8U;
    if ( (c3_0 & c3_1) == c3_1 ) {
      negvec |= 8U;
    }
  }

  return supvec | (negvec << 8);
}

// 準正規形になっているかのチェック
bool
is_normal(ymuint16 func)
{
  // 0番めの変数によるコファクター
  ymuint16 c0_0 = func & 0x5555U;
  ymuint16 c0_1 = (func & 0xaaaaU) >> 1;
  bool sup_0 = (c0_0 != c0_1);
  bool neg_0 = ((c0_0 & c0_1) == c0_1);

  // 1番めの変数によるコファクター
  ymuint16 c1_0 = func & 0x3333U;
  ymuint16 c1_1 = (func & 0xccccU) >> 2;
  bool sup_1 = (c1_0 != c1_1);
  bool neg_1 = ((c1_0 & c1_1) == c1_1);

  // 2番めの変数によるコファクター
  ymuint16 c2_0 = func & 0x0f0fU;
  ymuint16 c2_1 = (func & 0xf0f0U) >> 4;
  bool sup_2 = (c2_0 != c2_1);
  bool neg_2 = ((c2_0 & c2_1) == c2_1);

  // 3番めの変数によるコファクター
  ymuint16 c3_0 = func & 0x00ffU;
  ymuint16 c3_1 = (func & 0xff00U) >> 8;
  bool sup_3 = (c3_0 != c3_1);
  bool neg_3 = ((c3_0 & c3_1) == c3_1);

  bool sup_ng = false;
  if ( !sup_0 && sup_1 ) {
    sup_ng = true;
  }
  if ( !sup_1 && sup_2 ) {
    sup_ng = true;
  }
  if ( !sup_2 && sup_3 ) {
    sup_ng = true;
  }
  if ( !sup_0 && !sup_ng ) {
    return true;
  }

  bool neg_ng = false;
  if ( neg_0 && sup_0 ) {
    neg_ng = true;
  }
  if ( neg_1 && sup_1 ) {
    neg_ng = true;
  }
  if ( neg_2 && sup_2 ) {
    neg_ng = true;
  }
  if ( neg_3 && sup_3 ) {
    neg_ng = true;
  }

#if 0
  if ( sup_ng ) {
    cout << "support = ";
    if ( sup_0 ) {
      cout << " 0";
    }
    if ( sup_1 ) {
      cout << " 1";
    }
    if ( sup_2 ) {
      cout << " 2";
    }
    if ( sup_3 ) {
      cout << " 3";
    }
    cout << endl;
  }
  if ( neg_0 && sup_0 ) {
    cout << "neg_0" << endl;
  }
  if ( neg_1 && sup_1 ) {
    cout << "neg_1" << endl;
  }
  if ( neg_2 && sup_2 ) {
    cout << "neg_2" << endl;
  }
  if ( neg_3 && sup_3 ) {
    cout << "neg_3" << endl;
  }
  if ( !sup_ng && !neg_ng ) {
    cout << "normal" << endl;
  }
#endif

  return !sup_ng && !neg_ng;
}

ymuint16
normalize4(ymuint16 func,
	   ymuint16& cperm)
{
  ymuint supvec = support(func);
  ymuint negvec = supvec >> 8;
  supvec &= 0xFF;

  ymuint ip[4] = {0, 1, 2, 3};
  switch ( supvec ) {
  case 0: // 0000
  case 1: // 0001
  case 3: // 0011
  case 7: // 0111
  case 15: // 1111
    ip[0] = 0;
    ip[1] = 1;
    ip[2] = 2;
    ip[3] = 3;
    break;

  case 2: // 0010
    ip[0] = 1;
    ip[1] = 0;
    ip[2] = 2;
    ip[3] = 3;
    break;

  case 4: // 0100
    ip[0] = 1;
    ip[1] = 2;
    ip[2] = 0;
    ip[3] = 3;
    break;

  case 5: // 0101
    ip[0] = 0;
    ip[1] = 2;
    ip[2] = 1;
    ip[3] = 3;
    break;

  case 6: // 0110
    ip[0] = 2;
    ip[1] = 0;
    ip[2] = 1;
    ip[3] = 3;
    break;

  case 8: // 1000
    ip[0] = 1;
    ip[1] = 2;
    ip[2] = 3;
    ip[3] = 0;
    break;

  case 9: // 1001
    ip[0] = 0;
    ip[1] = 2;
    ip[2] = 3;
    ip[3] = 1;
    break;

  case 10: // 1010
    ip[0] = 2;
    ip[1] = 0;
    ip[2] = 3;
    ip[3] = 1;
    break;

  case 11: // 1011
    ip[0] = 0;
    ip[1] = 1;
    ip[2] = 3;
    ip[3] = 2;
    break;

  case 12: // 1100
    ip[0] = 2;
    ip[1] = 3;
    ip[2] = 0;
    ip[3] = 1;
    break;

  case 13: // 1101
    ip[0] = 0;
    ip[1] = 3;
    ip[2] = 1;
    ip[3] = 2;
    break;

  case 14: // 1110
    ip[0] = 3;
    ip[1] = 0;
    ip[2] = 1;
    ip[3] = 2;
    break;
  }

  bool neg[4];
#if 0
  neg[0] = static_cast<bool>((negvec >> 0) & 1U);
  neg[1] = static_cast<bool>((negvec >> 1) & 1U);
  neg[2] = static_cast<bool>((negvec >> 2) & 1U);
  neg[3] = static_cast<bool>((negvec >> 3) & 1U);
#else
  neg[0] = false;
  neg[1] = false;
  neg[2] = false;
  neg[3] = false;
#endif


  // ベタなやり方
  // 全ての変換を試して変換結果が準正規形になったらその変換を返す．
  for (ymuint p = 0; p < 768; ++ p) {
    ymuint16 perm_data = npn4perm[p];
    NpnXform xf(perm_data);
    if ( xf.output_inv() ) {
      continue;
    }
    if ( xf.input_perm(0) == ip[0] &&
	 xf.input_perm(1) == ip[1] &&
	 xf.input_perm(2) == ip[2] &&
	 xf.input_perm(3) == ip[3] &&
	 xf.input_inv(0) == neg[0] &&
	 xf.input_inv(1) == neg[1] &&
	 xf.input_inv(2) == neg[2] &&
	 xf.input_inv(3) == neg[3] ) {
      cperm = perm_data;
      ymuint16 fv1 = xform_func4(func, perm_data);
      //assert_cond( is_normal(fv1), __FILE__, __LINE__ );
      return fv1;
    }
  }
  cout << "func: " << setw(4) << setfill('0')
       << hex << func << dec << endl;
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

void
normalize(bool debug)
{
  for (ymuint32 func = 0; func < 65536; ++ func) {
    ymuint16 cperm;
    ymuint16 cfunc = normalize4(func, cperm);
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

  normalize(debug);

  return 0;

}
