
/// @file IDO.cc
/// @brief IDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/IDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StrListIDO.h"
#include "YmUtils/StrBuff.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/FileRegion.h"

#include <fcntl.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス IDO
//////////////////////////////////////////////////////////////////////

// @brief read() を呼び出して結果をチェックする．
void
IDO::_read(ymuint8* buff,
	   ymuint64 n)
{
  ssize_t ret = read(buff, n);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "IDO::read(" << n << ") failed. read " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "IDO",
		    buf.str());
  }
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint8
IDO::read_8()
{
  ymuint8 buff[1];

  _read(buff, 1);
  return buff[0];
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint16
IDO::read_16()
{
  ymuint8 buff[2];

  _read(buff, 2);
  ymuint16 val =
    (static_cast<ymuint16>(buff[0]) <<  0) |
    (static_cast<ymuint16>(buff[1]) <<  8);
  return val;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
ymuint32
IDO::read_32()
{
  ymuint8 buff[4];

  _read(buff, 4);
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
IDO::read_64()
{
  ymuint8 buff[8];

  _read(buff, 8);
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
IDO::read_float()
{
  ymuint8 buff[sizeof(float)];

  // かなり強引
  _read(buff, sizeof(float));
  return *(reinterpret_cast<float*>(buff));
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
double
IDO::read_double()
{
  ymuint8 buff[sizeof(double)];

  // かなり強引
  _read(buff, sizeof(double));
  return *(reinterpret_cast<double*>(buff));
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
string
IDO::read_str()
{
  ymuint64 l = read_64();
  if ( l > 0 ) {
    ymuint8* strbuf = new ymuint8[l + 1];
    _read(strbuf, l);
    strbuf[l] = '\0';
    string ans(reinterpret_cast<char*>(strbuf));
    delete [] strbuf;
    return ans;
  }
  else {
    return string();
  }
}

// @brief 一行分の文字列の読み出し．
string
IDO::read_line()
{
  StrBuff buf;
  ymuint8 c;
  while ( (c = read_8()) != '\n' && c != '\0' ) {
    buf.put_char(c);
  }
  return buf;
}


//////////////////////////////////////////////////////////////////////
// クラス StreamIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 入力ストリーム
StreamIDO::StreamIDO(istream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamIDO::~StreamIDO()
{
}

// @brief 読み出し可能なら true を返す．
bool
StreamIDO::is_ready() const
{
  return mS;
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
StreamIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
StreamIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ssize_t
StreamIDO::read(ymuint8* buff,
		ymuint64 n)
{
  return mS.readsome(reinterpret_cast<char*>(buff), n);
}

//////////////////////////////////////////////////////////////////////
// クラス StrListIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] str_list 文字列のリスト
StrListIDO::StrListIDO(const vector<string>& str_list) :
  mStrList(str_list)
{
  mLineNum = 0;
  mColumnNum = 0;
}

// @brief デストラクタ
StrListIDO::~StrListIDO()
{
}

// @brief 読み出し可能なら true を返す．
bool
StrListIDO::is_ready() const
{
  if ( mLineNum < mStrList.size() ) {
    return true;
  }
  else {
    return false;
  }
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
StrListIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
StrListIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ssize_t
StrListIDO::read(ymuint8* buff,
		 ymuint64 n)
{
  ymuint count = 0;
  while ( n > 0 && mLineNum < mStrList.size() ) {
    const string& str = mStrList[mLineNum];
    ymuint src_size = str.size() - mColumnNum;
    ymuint n1 = src_size;
    if ( n1 > n ) {
      n1 = n;
    }
    for (ymuint i = 0; i < n1; ++ i, ++ mColumnNum) {
      buff[count + i] = str[mColumnNum];
    }
    n -= n1;
    count += n1;
    if ( mColumnNum >= str.size() ) {
      ++ mLineNum;
      mColumnNum = 0;
    }
  }
  return count;
}

END_NAMESPACE_YM
