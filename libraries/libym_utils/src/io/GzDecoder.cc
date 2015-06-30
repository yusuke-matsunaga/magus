
/// @file GzDecoder.cc
/// @brief GzDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "GzDecoder.h"
#include "gz_common.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GzDecoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GzDecoder::GzDecoder()
{
}

// @brief デストラクタ
GzDecoder::~GzDecoder()
{
  close();
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzDecoder::open(const char* filename)
{
  bool stat = mBuff.open(filename, O_RDONLY, 0);
  if ( !stat ) {
    return false;
  }

  ymuint8 flags = 0;

  // ヘッダを解釈する．
  ymuint8 header[10];
  ymuint hsize = mBuff.read(header, sizeof(header));
  if ( hsize != sizeof(header) ||
       header[0] != GZIP_MAGIC0 ||
       ((header[1] != GZIP_MAGIC1) && (header[1] != GZIP_OMAGIC1)) ) {
       // ファイル形式が違う．
    goto error_out;
  }

  // CM(Compression Method)
  if ( header[2] != Z_DEFLATED ) {
    // maybe_err("unknown cpression method");
    goto error_out;
  }

  // FLG(FLaGs)
  flags = header[3];

  // MTIME(Modification TIME)
  // XFL (eXtra FLags)
  // OS(Operationg Systeam)
  // 全部無視

  if ( flags & EXTRA_FIELD ) {
    // EXTRA_FIELD がセットされていたら次の2バイトにそのサイズが書いてある．
    ymuint8 tmp_buff[2];
    if ( mBuff.read(tmp_buff, 2) != 2 ) {
      goto error_out;
    }
    ymuint size = static_cast<ymuint>(tmp_buff[0]) |
      (static_cast<ymuint>(tmp_buff[1]) << 8);
    if ( !mBuff.dummy_read(size) ) {
      goto error_out;
    }
  }

  if ( flags & ORIG_NAME ) {
    // ORIG_NAME がセットされていたら0終端の文字列が書いてある．
    ymuint8 c;
    for ( ; ; ) {
      if ( mBuff.read(&c, 1) != 1 ) {
	goto error_out;
      }
      if ( c == '\0' ) {
	break;
      }
    }
  }

  if ( flags & COMMENT ) {
    // COMMENT がセットされていたら0終端の文字列が書いてある．
    ymuint8 c;
    for ( ; ; ) {
      if ( mBuff.read(&c, 1) != 1 ) {
	goto error_out;
      }
      if ( c == '\0' ) {
	break;
      }
    }
  }

  if ( flags & HEAD_CRC ) {
    // HEAD_CRC がセットされていたら2バイトのCRCコードが書いてある．
    // でも無視する．
    ymuint8 tmp_buff[2];
    if ( mBuff.read(tmp_buff, 2) != 2 ) {
      goto error_out;
    }
  }

  // zstream の初期化
  if ( mZS.inflate_init2(- MAX_WBITS) != Z_OK ) {
    //maybe_err("failed to inflateInit2");
    goto error_out;
  }

  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0U;

  return true;

 error_out:
  close();
  return false;
}

// @brief ファイルをクローズする．
void
GzDecoder::close()
{
  mZS.inflate_end();
  mBuff.close();
}

// @brief 読み出せるデータがある時に true を返す．
bool
GzDecoder::is_ready() const
{
  return mBuff.is_ready();
}

BEGIN_NONAMESPACE

// 4バイトのデータを32ビットの符号なし整数に変換する．
inline
ymuint32
conv_4bytes(ymuint8 buff[])
{
  ymuint32 val0 = static_cast<ymuint32>(buff[0]);
  ymuint32 val1 = static_cast<ymuint32>(buff[1]);
  ymuint32 val2 = static_cast<ymuint32>(buff[2]);
  ymuint32 val3 = static_cast<ymuint32>(buff[3]);
  return val0 | (val1 << 8) | (val2 << 16) | (val3 << 24);
}

END_NONAMESPACE


// @brief 圧縮されたデータを伸長してバッファに書き込む．
// @param[in] buff 伸長したデータを格納するバッファ
// @param[in] size バッファの空きサイズ
ymint64
GzDecoder::read(ymuint8* buff,
		ymuint64 size)
{
  mZS.set_outbuf(buff, size);

  int ret = 0;
  for ( ; ; ) {
    // 入力バッファが空なら新たなデータを読み込む．
    if ( !mBuff.prepare() ) {
      // 読み込みに失敗した．
      return -1;
    }

    // 入力データをセットする．
    ymuint old_size = mBuff.buff_size();
    mZS.set_inbuf(mBuff.buff_ptr(), old_size);

    // 伸長する．
    ret = mZS.inflate(Z_FINISH);

    // 今回の inflate で消費した分だけ入力バッファを空読みする．
    ymuint in_size = old_size - mZS.avail_in();
    mBuff.seek(in_size);

    switch ( ret ) {
    case Z_OK:
      break;

    case Z_STREAM_END:
      break;

    case Z_BUF_ERROR:
      if ( mZS.avail_out() > 0 ) {
	// 出力バッファが満杯でないのに BUF_ERROR ということは
	// 入力データが枯渇したということ．
	ASSERT_COND( mZS.avail_in() == 0 );
	// もう一回繰り返す．
	continue;
      }
      // こちらは出力バッファが満杯になってしまった．
      break;

    case Z_NEED_DICT:
      cerr << "Z_NEED_DICT" << endl;
      return -1;

    case Z_DATA_ERROR:
      cerr << "Z_DATA_ERROR" << endl;
      return -1;

    case Z_STREAM_ERROR:
      cerr << "Z_STREAM_ERROR" << endl;
      return -1;

    case Z_MEM_ERROR:
      cerr << "Z_MEM_ERROR" << endl;
      return -1;

    default:
      cerr << "Unknown error codde (" << ret << ")" << endl;
      return -1;
    }
    break;
  }

  // バッファに書き込まれた量を計算する．
  ymuint wr = size - mZS.avail_out();
  if ( wr != 0 ) {
    // CRC 値を更新しておく．
    mCRC = crc32(mCRC, (const Bytef*)buff, wr);
    mOutSize += wr;
  }

  if ( ret == Z_STREAM_END ) {
    // データの末尾を読んだときの処理

    // データ末尾の次の4バイトは CRC コード
    ymuint8 tmp_buff[4];
    if ( mBuff.read(tmp_buff, 4) != 4 ) {
      // truncated input
      goto end;
    }

    ymuint32 orig_crc = conv_4bytes(tmp_buff);
    if ( orig_crc != mCRC ) {
      cerr << "CRC ERROR!" << endl;
      cerr << " original_crc = "
	   << hex << orig_crc << dec << endl;
      return -1;
    }

    // その次の4バイトは データ長
    if ( mBuff.read(tmp_buff, 4) != 4 ) {
      // truncated input
      goto end;
    }
    ymuint32 orig_len = conv_4bytes(tmp_buff);
    if ( orig_len != mOutSize ) {
      cerr << "data-length ERROR!" << endl;
      return -1;
    }
  }

 end:
  return wr;
}

END_NAMESPACE_YM
