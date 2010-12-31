
/// @file libym_utils/BinIO.cc
/// @brief BinIO の実装ファイル
/// @author Yusuke Matsunaga
///
/// $Id: File.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM

ymuint8 BinIO::mBuf[8];

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinIO::write_8(ostream& s,
	       ymuint8 val)
{
  mBuf[0] = val & 255U;
  s.write(reinterpret_cast<char*>(mBuf), 1);
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinIO::write_16(ostream& s,
		ymuint16 val)
{
  mBuf[0] = val & 255U; val >>= 8;
  mBuf[1] = val & 255U;
  s.write(reinterpret_cast<char*>(mBuf), 2);
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinIO::write_32(ostream& s,
		ymuint32 val)
{
  mBuf[0] = val & 255U; val >>= 8;
  mBuf[1] = val & 255U; val >>= 8;
  mBuf[2] = val & 255U; val >>= 8;
  mBuf[3] = val & 255U;
  s.write(reinterpret_cast<char*>(mBuf), 4);
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinIO::write_64(ostream& s,
		ymuint64 val)
{
  mBuf[0] = val & 255U; val >>= 8;
  mBuf[1] = val & 255U; val >>= 8;
  mBuf[2] = val & 255U; val >>= 8;
  mBuf[3] = val & 255U; val >>= 8;
  mBuf[4] = val & 255U; val >>= 8;
  mBuf[5] = val & 255U; val >>= 8;
  mBuf[6] = val & 255U; val >>= 8;
  mBuf[7] = val & 255U;
  s.write(reinterpret_cast<char*>(mBuf), 8);
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
BinIO::read_8(istream& s)
{
  s.read(reinterpret_cast<char*>(mBuf), 1);
  return mBuf[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
BinIO::read_16(istream& s)
{
  s.read(reinterpret_cast<char*>(mBuf), 2);
  ymuint16 val =
    (static_cast<ymuint16>(mBuf[0]) <<  0) |
    (static_cast<ymuint16>(mBuf[1]) <<  8);
  return val;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint32
BinIO::read_32(istream& s)
{
  s.read(reinterpret_cast<char*>(mBuf), 4);
  ymuint32 val =
    (static_cast<ymuint32>(mBuf[0]) <<  0) |
    (static_cast<ymuint32>(mBuf[1]) <<  8) |
    (static_cast<ymuint32>(mBuf[2]) << 16) |
    (static_cast<ymuint32>(mBuf[3]) << 24);
  return val;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint64
BinIO::read_64(istream& s)
{
  s.read(reinterpret_cast<char*>(mBuf), 8);
  ymuint64 val =
    (static_cast<ymuint64>(mBuf[0]) <<  0) |
    (static_cast<ymuint64>(mBuf[1]) <<  8) |
    (static_cast<ymuint64>(mBuf[2]) << 16) |
    (static_cast<ymuint64>(mBuf[3]) << 24) |
    (static_cast<ymuint64>(mBuf[4]) << 32) |
    (static_cast<ymuint64>(mBuf[5]) << 40) |
    (static_cast<ymuint64>(mBuf[6]) << 48) |
    (static_cast<ymuint64>(mBuf[7]) << 56);
  return val;
}

END_NAMESPACE_YM
