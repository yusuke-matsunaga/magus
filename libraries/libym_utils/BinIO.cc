
/// @file BinIO.cc
/// @brief BinIO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinI.h"
#include "ym_utils/FileBinI.h"
#include "ym_utils/StreamBinI.h"

#include "ym_utils/BinO.h"
#include "ym_utils/FileBinO.h"
#include "ym_utils/StreamBinO.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/FileRegion.h"

#include "FileBuff.h"

#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// 作業用のバッファ
ymuint8 buff[8];

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス BinO
//////////////////////////////////////////////////////////////////////

// @brief write() を読み出して結果をチェックする．
inline
void
BinO::_write(ymuint64 n,
	     const ymuint8* buff)
{
  ssize_t ret = write(n, buff);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "BinO::write(" << n << ") failed. wrote " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "BinO",
		    buf.str());
  }
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_8(ymuint8 val)
{
  buff[0] = val & 255U;
  _write(1, buff);
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_16(ymuint16 val)
{
  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U;
  _write(2, buff);
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
  _write(4, buff);
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
  _write(8, buff);
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_float(float val)
{
  // かなり強引
  *(reinterpret_cast<float*>(buff)) = val;
  _write(sizeof(float), buff);
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
BinO::write_double(double val)
{
  // かなり強引
  *(reinterpret_cast<double*>(buff)) = val;
  _write(sizeof(double), buff);
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
    _write(l, reinterpret_cast<const ymuint8*>(val));
  }
  else {
    write_64(0);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス FileBinO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] buff_size バッファサイズ
FileBinO::FileBinO(ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] buff_size バッファサイズ
FileBinO::FileBinO(const char* filename,
		   ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] buff_size バッファサイズ
FileBinO::FileBinO(const string& filename,
		   ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief デストラクタ
FileBinO::~FileBinO()
{
  close();
  delete mFileBuff;
}

// @brief 書き込み可能なら true を返す．
FileBinO::operator bool() const
{
  return mFileBuff->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
void
FileBinO::open(const char* filename)
{
  close();
  mFileBuff->open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
void
FileBinO::open(const string& filename)
{
  open(filename.c_str());
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
FileBinO::close()
{
  mFileBuff->close();
}

// @brief データを書き出す．
// @param[in] n データサイズ
// @param[in] buff データを収めた領域のアドレス
// @return 実際に書き出した量を返す．
ssize_t
FileBinO::write(ymuint64 n,
		const ymuint8* buff)
{
  return mFileBuff->write(buff, n);
}


//////////////////////////////////////////////////////////////////////
// クラス StreamBinO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
StreamBinO::StreamBinO(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamBinO::~StreamBinO()
{
}

// @brief データを書き出す．
// @param[in] n データサイズ
// @param[in] buff データを収めた領域のアドレス
// @return 実際に書き出した量を返す．
ssize_t
StreamBinO::write(ymuint64 n,
		  const ymuint8* buff)
{
  mS.write(reinterpret_cast<const char*>(buff), n);
  return n;
}


//////////////////////////////////////////////////////////////////////
// クラス BinI
//////////////////////////////////////////////////////////////////////

// @brief read() を呼び出して結果をチェックする．
void
BinI::_read(ymuint64 n,
	    ymuint8* buff)
{
  ssize_t ret = read(n, buff);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "BinI::read(" << n << ") failed. read " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "BinI",
		    buf.str());
  }
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
BinI::read_8()
{
  _read(1, buff);
  return buff[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
BinI::read_16()
{
  _read(2, buff);
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
  _read(4, buff);
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
  _read(8, buff);
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
  _read(sizeof(float), buff);
  return *(reinterpret_cast<float*>(buff));
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
double
BinI::read_double()
{
  // かなり強引
  _read(sizeof(double), buff);
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
    _read(l, strbuf);
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
// クラス FileBinI
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
FileBinI::FileBinI(ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
FileBinI::FileBinI(const char* filename,
		   ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
FileBinI::FileBinI(const string& filename,
		   ymuint buff_size)
{
  mFileBuff = new FileBuff(buff_size);
  open(filename);
}

// @brief デストラクタ
FileBinI::~FileBinI()
{
  close();
  delete mFileBuff;
}

// @brief 読み出し可能なら true を返す．
FileBinI::operator bool() const
{
  return mFileBuff->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
bool
FileBinI::open(const char* filename)
{
  close();
  return mFileBuff->open(filename, O_RDONLY);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
bool
FileBinI::open(const string& filename)
{
  return open(filename.c_str());
}

// @brief ファイルを閉じる．
// @note 以降の読み出しは行われない．
void
FileBinI::close()
{
  mFileBuff->close();
}

// @brief データを読み込む．
// @param[in] n 読み込むデータサイズ
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @return 実際に読み込んだ量を返す．
ssize_t
FileBinI::read(ymuint64 n,
	       ymuint8* buff)
{
  return mFileBuff->read(buff, n);
}


//////////////////////////////////////////////////////////////////////
// クラス StreamBinI
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 入力ストリーム
StreamBinI::StreamBinI(istream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamBinI::~StreamBinI()
{
}

// @brief データを読み込む．
// @param[in] n 読み込むデータサイズ
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @return 実際に読み込んだ量を返す．
ssize_t
StreamBinI::read(ymuint64 n,
		 ymuint8* buff)
{
  mS.read(reinterpret_cast<char*>(buff), n);
  return n;
}

END_NAMESPACE_YM
