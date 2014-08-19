
/// @file GdsScanner.cc
/// @brief GdsScanner の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/GdsScanner.h"
#include "gds/Msg.h"
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
    if ( !read_2byte_uint(mCurSize) ) {
      return false;
    }
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
  ymuint tmp_word;
  if ( !read_2byte_uint(tmp_word) ) {
    return false;
  }
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

  if ( !read_block(dsize) ) {
    return false;
  }

  return true;
}

// @brief 直前の read_rec() で読んだレコードのデータを2バイト整数に変換する．
// @param[in] pos 位置 (2バイト分で1つ)
ymint16
GdsScanner::conv_2byte_int(ymuint pos) const
{
  ymuint32 offset = pos * 2;
  ymuint16 ans;
  ans  = (mDataBuff[offset + 0] << 8);
  ans += (mDataBuff[offset + 1] << 0);
  return static_cast<ymint16>(ans);
}

// @brief 直前の read_rec() で読んだレコードのデータを4バイト整数に変換する．
// @param[in] pos 位置 (4バイト分で1つ)
ymint32
GdsScanner::conv_4byte_int(ymuint pos) const
{
  ymuint32 offset = pos * 4;
  ymuint32 ans;
  ans  = (mDataBuff[offset + 0] << 24);
  ans += (mDataBuff[offset + 1] << 16);
  ans += (mDataBuff[offset + 2] <<  8);
  ans += (mDataBuff[offset + 3] <<  0);
  return static_cast<ymint32>(ans);
}

// @brief 直前の read_rec() で読んだレコードのデータを4バイト浮動小数点数に変換する．
// @param[in] pos 位置 (4バイト分で1つ)
double
GdsScanner::conv_4byte_real(ymuint pos) const
{
  ymuint32 offset = pos * 4;
  bool zero = true;
  ymuint v[4];
  for (ymuint i = 0; i < 4; ++ i) {
    v[i] = mDataBuff[offset + i];
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

// @brief 直前の read_rec() で読んだレコードのデータを8バイト浮動小数点数に変換する．
// @param[in] pos 位置 (8バイト分で1つ)
double
GdsScanner::conv_8byte_real(ymuint pos) const
{
  ymuint offset = pos * 8;
  bool zero = true;
  ymuint v[8];
  for (ymuint i = 0; i < 8; ++ i) {
    v[i] = mDataBuff[offset + i];
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

// @brief 2バイト読んで符号なし整数に変換する．
// @param[out] val 読み込んだ値を格納する変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
GdsScanner::read_2byte_uint(ymuint& val)
{
  ymuint val0;
  if ( !read_1byte(val0) ) {
    return false;
  }

  ymuint val1;
  if ( !read_1byte(val1) ) {
    return false;
  }

  val  = val0 << 8;
  val += val1 << 0;

  return true;
}

// @brief 1バイト読み込む．
// @param[out] val 読み込んだ値を格納する変数
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
GdsScanner::read_1byte(ymuint& val)
{
  while ( mReadPos >= mEndPos ) {
    bool stat = raw_read();
    if ( !stat ) {
      return false;
    }
  }

  val = mBuff[mReadPos];
  ++ mReadPos;
  ++ mCurPos;

  return true;
}

// @brief データブロックを読み込む．
// @param[in] dsize 読み込むサイズ
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
// @note 結果は mDataBuff に格納される．
bool
GdsScanner::read_block(ymuint dsize)
{
  alloc_buff(dsize);

  ymuint wpos = 0;
  while ( dsize > 0 ) {
    ymuint n = dsize;
    if ( mReadPos + n >= mEndPos ) {
      n = mEndPos - mReadPos;
    }
    if ( n > 0 ) {
      ymuint8* sp = mBuff + mReadPos;
      ymuint8* dp = mDataBuff + wpos;
      for (ymuint i = 0; i < n; ++ i, ++ sp, ++ dp) {
	*dp = *sp;
      }
      mReadPos += n;
      mCurPos += n;
      wpos += n;
      dsize -= n;
    }
    else {
      if ( !raw_read() ) {
	return false;
      }
    }
  }

  return true;
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
    error_header(__FILE__, __LINE__, "GdsScanner", mCurPos)
      << "error occured in 'read()'";
    msg_end();
    return false;
  }
  if ( n == 0 ) {
    // EOF
    return false;
  }

  mEndPos = static_cast<ymuint>(n);
  mReadPos = 0;

  return true;
}

END_NAMESPACE_YM_GDS
