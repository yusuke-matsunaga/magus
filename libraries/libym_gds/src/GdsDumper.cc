
/// @file GdsDumper.cc
/// @brief GdsDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsDumper.h"
#include "gds/GdsRecord.h"
#include "gds/GdsScanner.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II ファイルの中身をダンプするためのクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
GdsDumper::GdsDumper(ostream& os) :
  mOs(os)
{
}

// デストラクタ
GdsDumper::~GdsDumper()
{
}

// record の内容を出力する．
void
GdsDumper::operator()(const GdsRecord& record)
{
  dump_common(record.offset(), record.size(),
	      record.rtype(), record.dtype(),
	      record.data());
}

// @brief 直前に読み込んだ record の内容を出力する．
// @param[in] scanner 字句解析器
void
GdsDumper::operator()(const GdsScanner& scanner)
{
  dump_common(scanner.cur_offset(), scanner.cur_size(),
	      scanner.cur_rtype(), scanner.cur_dtype(),
	      scanner.cur_data());
}

// @brief record の共通部分の出力
// @param[in] data データ
// @param[in] offset オフセット
// @param[in] size サイズ
// @param[in] rtype レコードの型
// @param[in] dtype レコードのデータ型
// @param[in] data データ
void
GdsDumper::dump_common(ymuint32 offset,
		       ymuint32 size,
		       tGdsRtype rtype,
		       tGdsDtype dtype,
		       const ymuint8 data[])
{
  // オフセット : サイズ 生データ
  mOs << endl
      << hex << setw(7) << offset << dec << ": ";
  ymuint us = size >> 8;
  ymuint ls = size & 255;
  dump_byte(us);
  dump_byte(ls);
  mOs << " ";
  ymuint rt = static_cast<ymuint8>(rtype);
  ymuint dt = static_cast<ymuint8>(dtype);
  dump_byte(rt);
  dump_byte(dt);
  mOs << " ";
  ymuint dsize = size - 4;
  for (ymuint i = 0; i < dsize; ++ i) {
    dump_byte(data[i]);
    if ( (i + 4) % 24 == 23 ) {
      mOs << " " << endl << "         ";
    }
    else if ( i % 2 == 1 ) {
      mOs << " ";
    }
  }
  mOs << endl;

  const GdsRecTable& table = GdsRecTable::obj();

  // 整形された出力
  mOs << "  " << table.rtype_string(rtype);
  switch ( rtype ) {
  case kGdsHEADER:       dump_HEADER(data);          return;
  case kGdsBGNLIB:       dump_BGNLIB(data);          return;
  case kGdsUNITS:        dump_UNITS(data);           return;
  case kGdsBGNSTR:       dump_BGNSTR(data);          return;
  case kGdsXY:           dump_XY(data, dsize);       return;
  case kGdsCOLROW:       dump_COLROW(data);          return;
  case kGdsPRESENTATION: dump_PRESENTATION(data);    return;
  case kGdsSTRANS:       dump_STRANS(data);          return;
  case kGdsELFLAGS:      dump_ELFLAGS(data);         return;
  case kGdsLIBSECUR:     dump_LIBSECUR(data, dsize); return;
  case kGdsPATHTYPE:     dump_PATHTYPE(data);        return;
  default:
    break;
  }
  if ( dtype == kGdsNodata ) {
    mOs << endl;
    return;
  }

  if ( table.data_num(rtype) == 1 ) {
    switch ( dtype ) {
    case kGds2Int:   dump_2int(data);          return;
    case kGds4Int:   dump_4int(data);          return;
    case kGds8Real:  dump_8real(data, false);  return;
    case kGdsString: dump_string(data, dsize); return;
    default:
      break;
    }
  }

  mOs << "Error: no handler for this record type" << endl;
}

// HEADER の出力
void
GdsDumper::dump_HEADER(const ymuint8 data[])
{
  int version = conv_2byte_int(data, 0);
  mOs << "  Release " << version << endl;
}

// BGNLIB の出力
void
GdsDumper::dump_BGNLIB(const ymuint8 data[])
{
  ymuint16 buf[12];
  for (ymuint i = 0; i < 12; ++ i) {
    buf[i] = conv_2byte_int(data, i);
  }

  mOs << endl
      << "    Last modified ";
  dump_date(buf);
  mOs << endl
      << "    Last accessed ";
  dump_date(buf + 6);
  mOs << endl;
}

// UNITS の出力
void
GdsDumper::dump_UNITS(const ymuint8 data[])
{
  double uu = conv_8byte_real(data, 0);
  double mu = conv_8byte_real(data, 1);

  mOs << endl << scientific
      << "    1 database unit = " << uu << " user units" << endl
      << "    1 database unit = " << mu << " meters" << endl;
}

// BGNSTR の出力
void
GdsDumper::dump_BGNSTR(const ymuint8 data[])
{
  ymuint16 buf[12];
  for (ymuint i = 0; i < 12; ++ i) {
    buf[i] = conv_2byte_int(data, i);
  }
  mOs << endl
      << "    Creation time ";
  dump_date(buf);
  mOs << endl
      << "    Last modified ";
  dump_date(buf + 6);
  mOs << endl;
}

// XY の出力
void
GdsDumper::dump_XY(const ymuint8 data[],
		   ymuint dsize)
{
  mOs << endl;
  ymuint n = dsize / 8;
  for (ymuint i = 0; i < n; ++ i) {
    int x = conv_4byte_int(data, i * 2 + 0);
    int y = conv_4byte_int(data, i * 2 + 1);
    mOs << "    (" << x << " , " << y << ")"
	<< endl;
  }
}

// COLROW の出力
void
GdsDumper::dump_COLROW(const ymuint8 data[])
{
  int col = conv_2byte_int(data, 0);
  int row = conv_2byte_int(data, 1);
  mOs << " " << col << " cols, " << row << " rows"
      << endl;
}

// PRESENTATION の出力
void
GdsDumper::dump_PRESENTATION(const ymuint8 data[])
{
  // font type
  ymuint font = 0;
  switch ( conv_bitarray(data, 10, 2) ) {
  case 0: font = 0; break;
  case 1: font = 1; break;
  case 2: font = 2; break;
  case 3: font = 3; break;
  }

  // vertical justification
  const char* vj = "";
  switch ( conv_bitarray(data, 12, 2) ) {
  case 0: vj = "top"; break;
  case 1: vj = "middle"; break;
  case 2: vj = "bottom"; break;
  }

  // horizontal justification
  const char* hj = "";
  switch ( conv_bitarray(data, 14, 2) ) {
  case 0: hj = "left"; break;
  case 1: hj = "middle"; break;
  case 2: hj = "right"; break;
  }

  mOs << "  font " << font << "  vert: " << vj << "  horiz: " << hj
      << endl;
}

// STRANS の出力
void
GdsDumper::dump_STRANS(const ymuint8 data[])
{
  // reflection
  if ( conv_bitarray(data, 0, 1) ) {
    mOs << "  reflect";
  }

  mOs << " ";

  // absolute magnification
  if ( conv_bitarray(data, 13, 1) ) {
    mOs << "absolute magnificaion";
  }

  mOs << " ";

  // absolute angle
  if ( conv_bitarray(data, 14, 1) ) {
    mOs << "absolute angle";
  }

  mOs << endl;
}

// ELFLAGS の出力
void
GdsDumper::dump_ELFLAGS(const ymuint8 data[])
{
  // Template data
  if ( conv_bitarray(data, 15, 1) ) {
    mOs << " template data";
  }

  // External data
  if ( conv_bitarray(data, 14, 1) ) {
    mOs << " external data";
  }

  mOs << endl;
}

// LIBSECUR の出力
void
GdsDumper::dump_LIBSECUR(const ymuint8 data[],
			 ymuint dsize)
{
  // 実は良く分かっていない．
  mOs << endl;
  ymuint n = dsize / 6;
  for (ymuint i = 0; i < n; ++ i) {
    ymuint g = conv_2byte_int(data, i * 3 + 0);
    ymuint u = conv_2byte_int(data, i * 3 + 1);
    ymuint a = conv_2byte_int(data, i * 3 + 2);
    mOs << "    group: " << g << "  user: " << u << "  access: "
	<< oct << a << dec << endl;
  }
}

// @brief PATHTYPE の出力
// @param[in] data データ
void
GdsDumper::dump_PATHTYPE(const ymuint8 data[])
{
  int val = conv_2byte_int(data, 0);
  const char* type_str = "";
  switch ( val ) {
  case 0: type_str = "square ends"; break;
  case 1: type_str = "round ends"; break;
  case 2: type_str = "extended square ends"; break;
  case 3: type_str = "variable square ends"; break;
  }
  mOs << " " << type_str << endl;
}

// data type が 2 byte integer 一つの場合の出力
void
GdsDumper::dump_2int(const ymuint8 data[])
{
  mOs << " " << conv_2byte_int(data, 0)
      << endl;
}

// data type が 4 byte integer 一つの場合の出力
void
GdsDumper::dump_4int(const ymuint8 data[])
{
  mOs << " " << conv_4byte_int(data, 0)
      << endl;
}

// data type が 8 byte real 一つの場合の出力
void
GdsDumper::dump_8real(const ymuint8 data[],
		      bool s_form)
{
  mOs << " ";
  if ( s_form ) {
    mOs << scientific;
  }
  else {
    mOs << fixed;
  }
  mOs << conv_8byte_real(data, 0)
      << endl;
}

// data type が ASCII String の場合の出力
void
GdsDumper::dump_string(const ymuint8 data[],
		       ymuint n)
{
  mOs << " " << conv_string(data, n)
      << endl;
}

// 時刻のデータを出力する．
void
GdsDumper::dump_date(ymuint16 buf[])
{
  dump_2digit(buf[1]);  // month;
  mOs << "/";
  dump_2digit(buf[2]);  // day;
  mOs << "/"
     << buf[0] + 1900   // year
     << " ";
  dump_2digit(buf[3]);  // hour
  mOs << ":";
  dump_2digit(buf[4]);  // minute
  mOs << ":";
  dump_2digit(buf[5]);  // second
}

// 2桁の整数を0つきで出力する．
void
GdsDumper::dump_2digit(ymuint num)
{
  ymuint u = num / 10;
  ymuint l = num - u * 10;
  mOs << u << l;
}

// 1バイトのデータを出力する．
void
GdsDumper::dump_byte(ymuint8 byte)
{
  ymuint hb[2];
  hb[0] = byte >> 4;
  hb[1] = byte & 15;
  for (ymuint i = 0; i < 2; ++ i) {
    int x = hb[i];
    if ( x >= 10 ) {
      mOs << static_cast<char>('a' + x - 10);
    }
    else {
      mOs << static_cast<char>('0' + x);
    }
  }
}

// @brief data を 2バイト整数の配列とみなして pos 番めの要素を返す．
// @param[in] data データ
// @param[in] pos 位置
ymint16
GdsDumper::conv_2byte_int(const ymuint8 data[],
			  ymuint pos)
{
  ymuint32 offset = pos * 2;
  ymuint16 ans;
  ans  = (data[offset + 0] << 8);
  ans += (data[offset + 1] << 0);
  return static_cast<ymint16>(ans);

}

// @brief pos 番目の 4バイトのデータを符号つき数(2の補数表現)に変換する．
// @param[in] data データ
// @param[in] pos 位置
ymint32
GdsDumper::conv_4byte_int(const ymuint8 data[],
			  ymuint32 pos)
{
  ymuint32 offset = pos * 4;
  ymuint32 ans;
  ans  = (data[offset + 0] << 24);
  ans += (data[offset + 1] << 16);
  ans += (data[offset + 2] <<  8);
  ans += (data[offset + 3] <<  0);
  return static_cast<ymint32>(ans);
}

// @brief pos 番目の 4バイトのデータを浮動小数点数に変換する．
// @param[in] data データ
// @param[in] pos 位置
double
GdsDumper::conv_4byte_real(const ymuint8 data[],
			   ymuint32 pos)
{
  ymuint32 offset = pos * 4;
  bool zero = true;
  ymuint v[4];
  for (ymuint i = 0; i < 4; ++ i) {
    v[i] = data[offset + i];
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

// @brief pos 番目の 8バイトのデータを浮動小数点数に変換する．
// @param[in] data データ
// @param[in] pos 位置
double
GdsDumper::conv_8byte_real(const ymuint8 data[],
			   ymuint32 pos)
{
  ymuint offset = pos * 8;
  bool zero = true;
  ymuint v[8];
  for (ymuint i = 0; i < 8; ++ i) {
    v[i] = data[offset + i];
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

// @brief データを文字列に変換する．
// @param[in] data データ
// @param[in] n データサイズ
string
GdsDumper::conv_string(const ymuint8 data[],
		       ymuint32 n)
{
  ymuint len = n;
  for (ymuint i = 0; i < n; ++ i) {
    if ( data[i] == '\0' ) {
      len = i;
      break;
    }
  }
  string buf;
  for (ymuint i = 0; i < len; ++ i) {
    buf.push_back(data[i]);
  }

  return buf;
}

// @brief データを BitArray に変換する．
// @param[in] data データ
// @param[in] base 開始位置(ビット)
// @param[in] width ビット幅
// @note 単位はバイトではなくビット
ymuint
GdsDumper::conv_bitarray(const ymuint8 data[],
			 ymuint base,
			 ymuint width)
{
  ymuint16 val = conv_2byte_int(data, 0);
  ymuint sft = 15 - base;
  ymuint ans = 0U;
  ymuint bit = 1U << sft;
  for (ymuint i = 0; i < width; ++ i) {
    ans <<= 1;
    if ( val & bit ) {
      ans |= 1U;
    }
    bit >>= 1;
  }
  return ans;
}

END_NAMESPACE_YM_GDS
