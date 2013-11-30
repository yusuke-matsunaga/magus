
/// @file GdsRecored.cc
/// @brief GdsRecord の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsRecord.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// Record
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GdsRecord::GdsRecord()
{
}

// @brief デストラクタ
GdsRecord::~GdsRecord()
{
}

// レコード型を表すトークンを返す．
int
GdsRecord::rtype_token() const
{
  return GdsRecTable::obj().rtype_token(mRtype);
}

// レコード型を表す文字列を返す．
const char*
GdsRecord::rtype_string() const
{
  return GdsRecTable::obj().rtype_string(mRtype);
}

// データ型を表す文字列を返す．
const char*
GdsRecord::dtype_string() const
{
  return GdsRecTable::obj().dtype_string(mDtype);
}

// pos 番目の 2バイトのデータを符号つき数(2の補数表現)に変換する．
// kGds2Int 用の変換関数
ymint16
GdsRecord::conv_2byte_int(ymuint32 pos) const
{
  ymuint32 offset = pos * 2;
  ymuint16 ans = (conv_1byte(offset) << 8) + conv_1byte(offset + 1);
  return static_cast<ymint16>(ans);
}

// pos 番目の 4バイトのデータを符号つき数(2の補数表現)に変換する．
// kGds4Int 用の変換関数
ymint32
GdsRecord::conv_4byte_int(ymuint32 pos) const
{
  ymuint32 offset = pos * 4;
  ymuint32 ans = (conv_1byte(offset + 0) << 24);
  ans += (conv_1byte(offset + 1) << 16);
  ans += (conv_1byte(offset + 2) << 8);
  ans += conv_1byte(offset + 3);
  return static_cast<ymint32>(ans);
}

// pos 番目の 4バイトのデータを浮動小数点数に変換する．
// kGds4Real 用の変換関数
double
GdsRecord::conv_4byte_real(ymuint32 pos) const
{
  ymuint32 offset = pos * 4;
  bool zero = true;
  ymuint v[4];
  for (ymuint i = 0; i < 4; ++ i) {
    v[i] = conv_1byte(i + offset);
    if ( v[i] ) {
      zero = false;
    }
  }
  if ( zero ) {
    // すべてのビットが0なら0
    return 0.0;
  }
  ymuint sign = (v[0] >> 7) & 1;
  ymuint exp = (v[0] & 127); // +64 のゲタをはいている．
  ymuint mag = (v[1] << 16) + (v[2] << 8) + v[0];
  double ans = 0.0;
  double w = 0.5;
  if ( exp >= 64 ) {
    ymuint sn = exp - 64;
    for (ymuint i = 0; i < sn; ++ i) {
      w *= 16.0;
    }
  }
  else {
    ymuint sn = 64 - exp;
    for (ymuint i = 0; i < sn; ++ i) {
      w /= 16.0;
    }
  }
  ymuint mask = (1 << 23);
  for (ymuint i = 0; i < 24; ++ i) {
    if ( mag & mask ) {
      ans += w;
    }
    mask >>= 1;
    w /= 2.0;
  }
  if ( sign ) {
    ans = -ans;
  }
  return ans;
}

// pos 番目の 8バイトのデータを浮動小数点数に変換する．
// kGds8Real 用の変換関数
double
GdsRecord::conv_8byte_real(ymuint pos) const
{
  ymuint offset = pos * 8;
  bool zero = true;
  ymuint v[8];
  for (ymuint i = 0; i < 8; ++ i) {
    v[i] = conv_1byte(i + offset);
    if ( v[i] ) {
      zero = false;
    }
  }
  if ( zero ) {
    // すべてのビットが0なら0
    return 0.0;
  }
  ymuint sign = (v[0] >> 7) & 1;
  ymuint exp = (v[0] & 127); // +64 のゲタをはいている．
  double ans = 0.0;
  double w = 0.5;
  if ( exp >= 64 ) {
    ymuint sn = exp - 64;
    for (ymuint i = 0; i < sn; ++ i) {
      w *= 16.0;
    }
  }
  else {
    ymuint sn = 64 - exp;
    for (ymuint i = 0; i < sn; ++ i) {
      w /= 16.0;
    }
  }
  ymuint block = 1;
  ymuint mask = (1 << 7);
  for (ymuint i = 0; i < 56; ++ i) {
    if ( v[block] & mask ) {
      ans += w;
    }
    mask >>= 1;
    if ( mask == 0 ) {
      ++ block;
      mask = (1 << 7);
    }
    w /= 2.0;
  }
  if ( sign ) {
    ans = -ans;
  }
  return ans;
}

// ptr からはじまる最大 n バイトのデータを文字列に変換する．
// 末尾に '\0'(EndOfString) があるとは限らない．
// kGdsString 用の変換関数
string
GdsRecord::conv_string() const
{
  ymuint n = dsize();
  ymuint len = n;
  for (ymuint i = 0; i < n; ++ i) {
    if ( mData[i] == '\0' ) {
      len = i;
      break;
    }
  }
  string buf;
  for (ymuint i = 0; i < len; ++ i) {
    buf.push_back(mData[i]);
  }

  return buf;
}

END_NAMESPACE_YM_GDS
