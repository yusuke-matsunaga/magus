
/// @file GdsScanner.cc
/// @brief GdsScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsScanner.h"
#include "ym_gds/Msg.h"
#include "GdsRecTable.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II の読み込みを行うクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 入力ストリームを指定する．
GdsScanner::GdsScanner(istream& is) :
  mIs(is),
  mCurPos(0),
  mDataBuff(NULL),
  mBuffSize(0)
{
  mBuffSize = 1024;
  mDataBuff = new ymuint8[mBuffSize];
}

// デストラクタ
GdsScanner::~GdsScanner()
{
  delete [] mDataBuff;
}

// @brief レコード一つ分の読み込みを行う．
// @retval true 読み込みが成功した．
// @retval false エラーが起った場合や末尾に達した場合
bool
GdsScanner::read_rec()
{
  ymuint32 size = 0;
  ymuint32 offset = 0;
  while ( size == 0 ) {
    if ( mIs.eof() ) {
      return false;
    }
    offset = mCurPos;
    size = read_2byte_uint();
    // null word をスキップする．
  }

  if ( size < 4 || (size & 1) ) {
    // 変なサイズ
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "illegal size (" << size << ")";
    msg_end();
    return false;
  }

  ymuint32 dsize = size - 4;
  mCurRtype = static_cast<tGdsRtype>(read_1byte_uint());
  mCurDtype = static_cast<tGdsDtype>(read_1byte_uint());

  // データの integrity check を行う．
  const GdsRecTable& table = GdsRecTable::obj();
  if ( table.dtype(mCurRtype) != mCurDtype ) {
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "data type mismatch: record type = "
      << table.rtype_string(mCurRtype)
      << ", data type = " << table.dtype_string(mCurDtype);
    msg_end();
    return false;
  }
  int unit_size = 1;
  switch ( mCurDtype ) {
  case kGdsNodata:                   break;
  case kGdsBitArray: unit_size =  2; break;
  case kGds2Int:     unit_size =  2; break;
  case kGds4Int:     unit_size =  4; break;
  case kGds4Real:    unit_size =  4; break;
  case kGds8Real:    unit_size =  8; break;
  case kGdsString:   unit_size = -2; break;
  }
  int exp_dsize = unit_size * table.data_num(mCurRtype);
  if ( exp_dsize >= 0 ) {
    if ( exp_dsize != static_cast<int>(dsize) ) {
      error_header(__FILE__, __LINE__, "GdsScanner", offset)
	<< "data size mismatch: record type = "
	<< table.rtype_string(mCurRtype)
	<< " expected data size = "
	<< exp_dsize
	<< ", real data size = "
	<< dsize;
      msg_end();
      return false;
    }
  }
  else {
    // 可変長だが exp_dsize の絶対値の倍数でなければならない．
    int unit = -exp_dsize;
    if ( dsize % unit != 0 ) {
      error_header(__FILE__, __LINE__, "GdsScanner", offset)
	<< "data size mismatch: record type = "
	<< table.rtype_string(mCurRtype)
	<< " expected data size = "
	<< unit
	<< " x N, real data size = "
	<< dsize;
      msg_end();
      return false;
    }
  }

  mCurOffset = offset;
  mCurSize = size;

  alloc_buff(dsize);
  mIs.read(reinterpret_cast<char*>(mDataBuff), dsize);
  mCurPos += dsize;

  if ( !mIs.good() ) {
    error_header(__FILE__, __LINE__, "GdsScanner", offset)
      << "unknown error occured during read";
    msg_end();
    return false;
  }

  return true;
}

// ストリームから1バイト読んで符号なし整数に変換する．
ymuint8
GdsScanner::read_1byte_uint()
{
  char buf[1] = { 0 };
  mIs.read(buf, 1);
  mCurPos += 1;
  return static_cast<ymuint8>(buf[0]);
}

// ストリームから2バイト読んで符号なし整数に変換する．
ymuint16
GdsScanner::read_2byte_uint()
{
  char buf[2] = { 0, 0 };
  mIs.read(buf, 2);
  mCurPos += 2;
  ymuint16 ans = static_cast<ymuint8>(buf[0]) << 8;
  ans += static_cast<ymuint8>(buf[1]);
  return ans;
}

// @brief バッファを確保する．
void
GdsScanner::alloc_buff(ymuint32 req_size)
{
  ymuint old_size = mBuffSize;
  while ( mBuffSize < req_size ) {
    mBuffSize <<= 1;
  }

  if ( mBuffSize != old_size ) {
    delete [] mDataBuff;
    mDataBuff = new ymuint8[mBuffSize];
  }
}

END_NAMESPACE_YM_GDS
