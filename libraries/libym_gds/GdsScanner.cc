
/// @file GdsScanner.cc
/// @brief GdsScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsScanner.h"
#include "ym_gds/Msg.h"
#include "GdsRecTable.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// GDS-II の読み込みを行うクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
GdsScanner::GdsScanner() :
  mFd(-1),
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
  close_file();
  delete [] mDataBuff;
}

// @brief ファイルを開く
// @param[in] filenmae ファイル名
// @retval true オープンに成功した．
// @retval false オープンに失敗した．
bool
GdsScanner::open_file(const string& filename)
{
  mCurPos = 0;
  mFd = open(filename.c_str(), O_RDONLY);
  return ( mFd >= 0 );
}

// @brief ファイルを閉じる．
void
GdsScanner::close_file()
{
  if ( mFd >= 0 ) {
    close(mFd);
    mFd = -1;
  }
}

// @brief レコード一つ分の読み込みを行う．
// @retval true 読み込みが成功した．
// @retval false エラーが起った場合や末尾に達した場合
bool
GdsScanner::read_rec()
{
  mCurSize = 0;
  while ( mCurSize == 0 ) {
    if ( is_eof() ) {
      return false;
    }
    mCurSize = read_2byte_uint();
    // null word をスキップする．
  }

  mCurOffset = mCurPos;
  if ( mCurSize < 4 || (mCurSize & 1) ) {
    // 変なサイズ
    error_header(__FILE__, __LINE__, "GdsScanner", mCurPos)
      << "illegal size (" << mCurSize << ")";
    msg_end();
    return false;
  }

  ymuint32 dsize = mCurSize - 4;
  ymuint16 tmp_word = read_2byte_uint();
  mCurRtype = static_cast<tGdsRtype>(tmp_word >> 8);
  mCurDtype = static_cast<tGdsDtype>(tmp_word & 0xFF);

  // データの integrity check を行う．
  const GdsRecTable& table = GdsRecTable::obj();
  if ( table.dtype(mCurRtype) != mCurDtype ) {
    error_header(__FILE__, __LINE__, "GdsScanner", mCurPos)
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
      error_header(__FILE__, __LINE__, "GdsScanner", mCurPos)
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
      error_header(__FILE__, __LINE__, "GdsScanner", mCurPos)
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

  block_read(dsize);
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

// ストリームから2バイト読んで符号なし整数に変換する．
ymuint16
GdsScanner::read_2byte_uint()
{
  char buf[2] = { 0, 0 };

  if ( mReadPos < mEndPos ) {
    buf[0] = mBuff[mReadPos];
    ++ mReadPos;

  mIs.read(buf, 2);
  mCurPos += 2;
  ymuint16 ans = static_cast<ymuint8>(buf[0]) << 8;
  ans += static_cast<ymuint8>(buf[1]);
  return ans;
}

// @brief ブロック読み込みを行う．
// @param[in] dsize 読み込むサイズ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
GdsScanner::read_block(ymuint dsize)
{
  alloc_buff(req_size);
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

// @brief 低レベルの読み込みを行う．
bool
GdsScanner::raw_read()
{
  if ( mFd < 0 ) {
    return false;
  }

  ssize_t n = read(mFd, mBuff, 4096);
  if ( n < 0 ) {
    // エラー
    return false;
  }
  mEndPos = static_cast<ymuint>(n);
  mReadPos = 0;

  return true;
}

END_NAMESPACE_YM_GDS
