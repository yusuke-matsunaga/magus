
/// @file BinIO.cc
/// @brief BinIO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinI.h"
#include "ym_utils/BinO.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 作業用のバッファ
ymuint8 buff[8];

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス BinO
//////////////////////////////////////////////////////////////////////

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_8(ymuint8 val)
{
  buff[0] = val & 255U;
  write(1, buff);
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_16(ymuint16 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U;
  write(2, buff);
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_32(ymuint32 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U;
  write(4, buff);
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_64(ymuint64 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U; val >>= 8;
  buff[4] = val & 255U; val >>= 8;
  buff[5] = val & 255U; val >>= 8;
  buff[6] = val & 255U; val >>= 8;
  buff[7] = val & 255U;
  write(8, buff);
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_float(float val)
{
  // かなり強引
  *(reinterpret_cast<float*>(buff)) = val;
  write(sizeof(float), buff);
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_double(double val)
{
  // かなり強引
  *(reinterpret_cast<double*>(buff)) = val;
  write(sizeof(double), buff);
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_str(const char* val)
{
  if ( val ) {
    ymuint64 l = strlen(val);
    write_64(l);
    write(l, reinterpret_cast<const ymuint8*>(val));
  }
  else {
    write_64(0);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileBinO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] filename ファイル名
FileBinO::FileBinO(const char* filename)
{
  mFd = open(filename, O_WRONLY | O_CREAT | O_TRUNC);
  mPos = 0;
}

// @brief コンストラクタ
// @param[in] filename ファイル名
FileBinO::FileBinO(const string& filename)
{
  mFd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
  mPos = 0;
}

// @brief デストラクタ
FileBinO::~FileBinO()
{
  close();
}

// @brief 書き込み可能なら true を返す．
bool
FileBinO::ok() const
{
  return mFd >= 0;
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
FileBinO::close()
{
  if ( mFd >= 0 ) {
    if ( mPos > 0 ) {
      ::write(mFd, reinterpret_cast<void*>(mBuff), mPos);
    }
    ::close(mFd);
  }
  mFd = -1;
}

// @brief データを書き出す．
// @param[in] n データサイズ
// @param[in] buff データを収めた領域のアドレス
// @return 実際に書き出した量を返す．
ymuint
FileBinO::write(ymuint64 n,
		const ymuint8* buff)
{
  if ( mFd < 0 ) {
    return 0;
  }

  ymuint count = 0;
  while ( n > 0 ) {
    if ( mPos + n < BUFF_SIZE ) {
      memcpy(reinterpret_cast<void*>(mBuff + mPos), reinterpret_cast<const void*>(buff), n);
      mPos += n;
      count += n;
      break;
    }
    else {
      ymuint n1 = BUFF_SIZE - mPos;
      memcpy(reinterpret_cast<void*>(mBuff + mPos), reinterpret_cast<const void*>(buff), n1);
      ::write(mFd, reinterpret_cast<void*>(mBuff), BUFF_SIZE);
      mPos = 0;
      count += n1;
      buff += n1;
      n -= n1;
    }
  }

  return count;
}


//////////////////////////////////////////////////////////////////////
// クラス BinOStream
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
BinOStream::BinOStream(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
BinOStream::~BinOStream()
{
}

// @brief データを書き出す．
// @param[in] n データサイズ
// @param[in] buff データを収めた領域のアドレス
// @return 実際に書き出した量を返す．
ymuint
BinOStream::write(ymuint64 n,
		  const ymuint8* buff)
{
  mS.write(reinterpret_cast<const char*>(buff), n);
  return n;
}


//////////////////////////////////////////////////////////////////////
// クラス BinI
//////////////////////////////////////////////////////////////////////

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
BinI::read_8()
{
  read(1, buff);
  return buff[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
BinI::read_16()
{
  read(2, buff);
  ymuint16 val =
    (static_cast<ymuint16>(buff[0]) <<  0) |
    (static_cast<ymuint16>(buff[1]) <<  8);
  return val;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint32
BinI::read_32()
{
  read(4, buff);
  ymuint32 val =
    (static_cast<ymuint32>(buff[0]) <<  0) |
    (static_cast<ymuint32>(buff[1]) <<  8) |
    (static_cast<ymuint32>(buff[2]) << 16) |
    (static_cast<ymuint32>(buff[3]) << 24);
  return val;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint64
BinI::read_64()
{
  read(8, buff);
  ymuint64 val =
    (static_cast<ymuint64>(buff[0]) <<  0) |
    (static_cast<ymuint64>(buff[1]) <<  8) |
    (static_cast<ymuint64>(buff[2]) << 16) |
    (static_cast<ymuint64>(buff[3]) << 24) |
    (static_cast<ymuint64>(buff[4]) << 32) |
    (static_cast<ymuint64>(buff[5]) << 40) |
    (static_cast<ymuint64>(buff[6]) << 48) |
    (static_cast<ymuint64>(buff[7]) << 56);
  return val;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
float
BinI::read_float()
{
  // かなり強引
  read(sizeof(float), buff);
  return *(reinterpret_cast<float*>(buff));
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
double
BinI::read_double()
{
  // かなり強引
  read(sizeof(double), buff);
  return *(reinterpret_cast<double*>(buff));
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
string
BinI::read_str()
{
  ymuint64 l = read_64();
  if ( l > 0 ) {
    ymuint8* strbuf = new ymuint8[l + 1];
    read(l, strbuf);
    strbuf[l] = '\0';
    string ans(reinterpret_cast<char*>(strbuf));
    delete [] strbuf;
    return ans;
  }
  else {
    return string();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス BinIStream
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 入力ストリーム
BinIStream::BinIStream(istream& s) :
  mS(s)
{
}

// @brief デストラクタ
BinIStream::~BinIStream()
{
}

// @brief データを読み込む．
// @param[in] n 読み込むデータサイズ
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @return 実際に読み込んだ量を返す．
ymuint
BinIStream::read(ymuint64 n,
		 ymuint8* buff)
{
  mS.read(reinterpret_cast<char*>(buff), n);
  return n;
}

END_NAMESPACE_YM
