
/// @file cannonical.cc
/// @brief NPN同値類の代表関数を求めるプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

// 3入力のNPN同値類代表関数
ymuint32 npn3rep[] = {
#include "npn3rep.h"
};

// 3入力のNPN変換を表す配列
ymuint8 npn3perm[][4] = {
#include "npn3perm.h"
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 4入力のNPN変換を表す配列
ymuint8 npn4perm[][5] = {
#include "npn4perm.h"
};

// @brief 関数ベクタを変換する(3入力版)
ymuint32
xform_func3(ymuint32 fv,
	    const ymuint8 perm[])
{
  ymuint32 ans = 0U;
  ymuint8 pols = perm[3];
  for (ymuint p = 0; p < 8; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      if ( pols & 1U ) {
	v0 ^= 1U;
      }
      if ( pols & 2U ) {
	v1 ^= 1U;
      }
      if ( pols & 4U ) {
	v2 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << perm[0]);
      }
      if ( v1 ) {
	q |= (1U << perm[1]);
      }
      if ( v2 ) {
	q |= (1U << perm[2]);
      }
      ans |= (1U << q);
    }
  }
  if ( pols & 8U ) {
    ans = ~ans & 0xFF;
  }
  return ans;
}

// @brief 関数ベクタを変換する(4入力版)
ymuint32
xform_func4(ymuint32 fv,
	    const ymuint8 perm[])
{
  ymuint32 ans = 0U;
  ymuint8 pols = perm[4];
  for (ymuint p = 0; p < 16; ++ p) {
    if ( fv & (1U << p) ) {
      ymuint v0 = (p >> 0) & 1U;
      ymuint v1 = (p >> 1) & 1U;
      ymuint v2 = (p >> 2) & 1U;
      ymuint v3 = (p >> 3) & 1U;
      if ( pols & 1U ) {
	v0 ^= 1U;
      }
      if ( pols & 2U ) {
	v1 ^= 1U;
      }
      if ( pols & 4U ) {
	v2 ^= 1U;
      }
      if ( pols & 8U ) {
	v3 ^= 1U;
      }
      ymuint q = 0U;
      if ( v0 ) {
	q |= (1U << perm[0]);
      }
      if ( v1 ) {
	q |= (1U << perm[1]);
      }
      if ( v2 ) {
	q |= (1U << perm[2]);
      }
      if ( v3 ) {
	q |= (1U << perm[3]);
      }
      ans |= (1U << q);
    }
  }
  if ( pols & 16U ) {
    ans = ~ans & 0xFFFF;
  }
  return ans;
}

ymuint32
cannonical3(ymuint32 func,
	    ymuint8 cperm[])
{
  unordered_set<ymuint32> npn_hash;
  for (ymuint i = 0; npn3rep[i] != 0xFF; ++ i) {
    npn_hash.insert(npn3rep[i]);
  }

  // ベタなやり方
  for (ymuint p = 0; p < 96; ++ p) {
    ymuint8* perm = npn3perm[p];
    ymuint32 fv1 = xform_func3(func, perm);
    if ( npn_hash.count(fv1) > 0 ) {
      cperm[0] = perm[0];
      cperm[1] = perm[1];
      cperm[2] = perm[2];
      cperm[3] = perm[3];
      return fv1;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

ymuint32
cannonical4(ymuint32 func,
	    ymuint8 cperm[])
{
  unordered_set<ymuint32> npn_hash;
  for (ymuint i = 0; npn4rep[i] != 0xFFFF; ++ i) {
    npn_hash.insert(npn4rep[i]);
  }
  // ベタなやり方
  for (ymuint p = 0; p < 768; ++ p) {
    ymuint8* perm = npn4perm[p];
    ymuint32 fv1 = xform_func4(func, perm);
    if ( npn_hash.count(fv1) > 0 ) {
      cperm[0] = perm[0];
      cperm[1] = perm[1];
      cperm[2] = perm[2];
      cperm[3] = perm[3];
      cperm[4] = perm[4];
      return fv1;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

void
cannonical(ymuint ni)
{
  assert_cond( ni == 3 || ni == 4, __FILE__, __LINE__);

  if ( ni == 3 ) {
    for (ymuint32 func = 0; func < 256; ++ func) {
      ymuint8 cperm[4];
      ymuint32 cfunc = cannonical3(func, cperm);
      cout << "  { 0x" << setw(2) << setfill('0')
	   << hex << cfunc << dec
	   << ", {"
	   << static_cast<ymuint>(cperm[0])
	   << ", "
	   << static_cast<ymuint>(cperm[1])
	   << ", "
	   << static_cast<ymuint>(cperm[2])
	   << ", "
	   << static_cast<ymuint>(cperm[3])
	   << "}}, " << endl;
    }
  }
  else {
    for (ymuint32 func = 0; func < 65536; ++ func) {
      ymuint8 cperm[5];
      ymuint32 cfunc = cannonical4(func, cperm);
      cout << "  { 0x" << setw(4) << setfill('0')
	   << hex << cfunc << dec
	   << ", {"
	   << static_cast<ymuint>(cperm[0])
	   << ", "
	   << static_cast<ymuint>(cperm[1])
	   << ", "
	   << static_cast<ymuint>(cperm[2])
	   << ", "
	   << static_cast<ymuint>(cperm[3])
	   << ", "
	   << static_cast<ymuint>(cperm[4])
	   << "}}, " << endl;
    }
  }
}

void
usage(char* progname)
{
  cerr << "USAGE: " << progname << " number-of-inputs(3 or 4)" << endl;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 2 ) {
    usage(argv[0]);
    exit(1);
  }

  ymuint ni = atoi(argv[1]);

  if ( ni != 3 && ni != 4 ) {
    usage(argv[0]);
    exit(2);
  }

  cannonical(ni);

  return 0;

}
