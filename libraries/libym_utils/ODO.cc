
/// @file ODO.cc
/// @brief ODO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/ODO.h"
#include "ym_utils/FileODO.h"
#include "ym_utils/StreamODO.h"
#include "ym_utils/CompODO.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/FileRegion.h"

#include <fcntl.h>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス ODO
//////////////////////////////////////////////////////////////////////

// @brief write() を読み出して結果をチェックする．
inline
void
ODO::_write(const ymuint8* buff,
	    ymuint64 n)
{
  ssize_t ret = write(buff, n);
  if ( static_cast<ymuint64>(ret) != n ) {
    ostringstream buf;
    buf << "ODO::write(" << n << ") failed. wrote " << ret << " bytes.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    FileRegion(),
		    kMsgFailure,
		    "ODO",
		    buf.str());
  }
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_8(ymuint8 val)
{
  ymuint8 buff[1];

  buff[0] = val & 255U;
  _write(buff, 1);
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_16(ymuint16 val)
{
  ymuint8 buff[2];

  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U;
  _write(buff, 2);
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_32(ymuint32 val)
{
  ymuint8 buff[4];

  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U;
  _write(buff, 4);
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_64(ymuint64 val)
{
  ymuint8 buff[8];

  buff[0] = val & 255U; val >>= 8;
  buff[1] = val & 255U; val >>= 8;
  buff[2] = val & 255U; val >>= 8;
  buff[3] = val & 255U; val >>= 8;
  buff[4] = val & 255U; val >>= 8;
  buff[5] = val & 255U; val >>= 8;
  buff[6] = val & 255U; val >>= 8;
  buff[7] = val & 255U;
  _write(buff, 8);
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_float(float val)
{
  ymuint8 buff[sizeof(float)];

  // かなり強引
  *(reinterpret_cast<float*>(buff)) = val;
  _write(buff, sizeof(float));
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_double(double val)
{
  ymuint8 buff[sizeof(double)];

  // かなり強引
  *(reinterpret_cast<double*>(buff)) = val;
  _write(buff, sizeof(double));
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
void
ODO::write_str(const char* val)
{
  if ( val ) {
    ymuint64 l = strlen(val);
    write_64(l);
    _write(reinterpret_cast<const ymuint8*>(val), l);
  }
  else {
    write_64(0);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StreamODO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
StreamODO::StreamODO(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
StreamODO::~StreamODO()
{
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
StreamODO::write(const ymuint8* buff,
		 ymuint64 n)
{
  mS.write(reinterpret_cast<const char*>(buff), n);
  if ( mS.bad() ) {
    return -1;
  }
  else {
    return n;
  }
}

#if 0
//////////////////////////////////////////////////////////////////////
// クラス CompODO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
CompODO::CompODO(ymuint bits)
{
  mCoder = new CompressCoder();
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
CompODO::CompODO(const char* filename,
		 ymuint bits)
{
  mCoder = new CompressCoder();
  open(filename, bits);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
CompODO::CompODO(const string& filename,
		 ymuint bits)
{
  mCoder = new CompressCoder();
  open(filename, bits);
}

// @brief デストラクタ
CompODO::~CompODO()
{
  delete mCoder;
}

// @brief 書き込み可能なら true を返す．
CompODO::operator bool() const
{
  return mCoder->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] bits 初期ビットサイズ (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
CompODO::open(const char* filename,
	      ymuint bits)
{
  return mCoder->open(filename, bits);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
CompODO::open(const string& filename,
	      ymuint bits)
{
  return open(filename.c_str(), bits);
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
CompODO::close()
{
  mCoder->close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
CompODO::write(const ymuint8* buff,
	       ymuint64 n)
{
  return mCoder->write(buff, n);
}
#endif

END_NAMESPACE_YM
