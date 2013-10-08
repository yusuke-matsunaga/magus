
/// @file GzIDO.cc
/// @brief GzIDO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GzIDO.h"
#include "ym_utils/GzODO.h"
#include "../FileBuff.h"
#include <sys/cdefs.h>


#define GZIP_MAGIC0	0x1F
#define GZIP_MAGIC1	0x8B
#define GZIP_OMAGIC1	0x9E

#define GZIP_TIMESTAMP	(off_t)4
#define GZIP_ORIGNAME	(off_t)10

#define HEAD_CRC	0x02
#define EXTRA_FIELD	0x04
#define ORIG_NAME	0x08
#define COMMENT		0x10

#define OS_CODE		3	/* Unix */


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス GzIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @note 意味的にはコンストラクタ + open()
GzIDO::GzIDO(const char* filename,
	     const FileLoc& parent_loc)
{
  mBuff = new FileBuff();
  open(filename, parent_loc);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @note 意味的にはコンストラクタ + open()
GzIDO::GzIDO(const string& filename,
	     const FileLoc& parent_loc)
{
  mBuff = new FileBuff();
  open(filename.c_str(), parent_loc);
}

// @brief デストラクタ
GzIDO::~GzIDO()
{
  close();
  delete mBuff;
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzIDO::open(const char* filename,
	    const FileLoc& parent_loc)
{
  bool stat = mBuff->open(filename, O_RDONLY);
  if ( !stat ) {
    return false;
  }

  mFileInfo = FileInfo(filename, parent_loc);

  // ヘッダを解釈する．
  ymuint8 header[10];
  ymuint hsize = mBuff->read(header, sizeof(header));
  if ( hsize != sizeof(header) ||
       header[0] != GZIP_MAGIC0 ||
       (header[1] != GZIP_MAGIC1) && (header[1] != GZIP_OMAGIC1) ) {
       // ファイル形式が違う．
    return false;
  }

  // CM(Compression Method)
  if ( header[2] != Z_DEFLATED ) {
    // maybe_err("unknown cpression method");
    return false;
  }

  // FLG(FLaGs)
  ymuint8 flags = header[3];

  // MTIME(Modification TIME)
  // XFL (eXtra FLags)
  // OS(Operationg Systeam)
  // 全部無視

  if ( flags & EXTRA_FIELD ) {
    // EXTRA_FIELD がセットされていたら次の2バイトにそのサイズが書いてある．
    ymuint8 tmp_buff[2];
    if ( mBuff->read(tmp_buff, 2) != 2 ) {
      return false;
    }
    ymuint size = static_cast<ymuint>(tmp_buff[0]) |
      (static_cast<ymuint>(tmp_buff[1]) << 8);
    if ( !mBuff->dummy_read(size) ) {
      return false;
    }
  }

  if ( flags & ORIG_NAME ) {
    // ORIG_NAME がセットされていたら0終端の文字列が書いてある．
    ymuint8 c;
    for ( ; ; ) {
      if ( mBuff->read(&c, 1) != 1 ) {
	return false;
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
      if ( mBuff->read(&c, 1) != 1 ) {
	return false;
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
    if ( mBuff->read(tmp_buff, 2) != 2 ) {
      return false;
    }
  }

  // zstream の初期化
  if ( mZS.inflate_init2(- MAX_WBITS) != Z_OK ) {
    //maybe_err("failed to inflateInit2");
    return false;
  }

  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0U;

  return true;
}

// @brief ファイルをクローズする．
void
GzIDO::close()
{
  mBuff->close();
  mFileInfo = FileInfo();
}

// @brief 読み出し可能なら true を返す．
GzIDO::operator bool() const
{
  return mBuff->is_ready();
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
GzIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
GzIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
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
ssize_t
GzIDO::read(ymuint8* buff,
	    size_t size)
{
  mZS.set_outbuf(buff, size);

  int inflate_stat = 0;

 start:
  {
    // 入力バッファが空なら新たなデータを読み込む．
    if ( !mBuff->prepare() ) {
      // 読み込みに失敗した．
      return -1;
    }

    // 入力データをセットする．
    ymuint old_size = mBuff->buff_size();
    mZS.set_inbuf(mBuff->buff_ptr(), old_size);

    // 伸長する．
    inflate_stat = mZS.inflate(Z_FINISH);

    // 今回の inflate で消費した分だけ入力バッファを空読みする．
    ymuint in_size = old_size - mZS.avail_in();
    mBuff->seek(in_size);
  }
  switch ( inflate_stat ) {
  case Z_BUF_ERROR:
    // Z_BUF_ERROR goes with Z_FINISH ...
    if ( mZS.avail_out() > 0 ) {
      // 出力バッファが満杯でないのに BUF_ERROR ということは
      // 入力データが枯渇したということ．
      assert_cond( mZS.avail_in() == 0, __FILE__, __LINE__);
      // もう一回繰り返す．
      goto start;
    }
    break;

  case Z_OK:
    break;

  case Z_STREAM_END:
    break;

  case Z_NEED_DICT:
    //meybe_err("Z_NEED_DICT error");
    break;

  case Z_DATA_ERROR:
    //maybe_err("data stream error");
    break;

  case Z_STREAM_ERROR:
    //maybe_err("internal stream error");
    break;

  case Z_MEM_ERROR:
    //maybe_err("memory allocation error");
    break;

  default:
    //maybe_err("unknown error from inflate(): %d", status);
    break;
  }

  // バッファに書き込まれた量を計算する．
  ymuint wr = size - mZS.avail_out();
  if ( wr != 0 ) {
    // CRC 値を更新しておく．
    mCRC = crc32(mCRC, (const Bytef*)buff, wr);
    mOutSize += wr;
  }

  if ( inflate_stat == Z_STREAM_END ) {
    // データの末尾を読んだときの処理
    mZS.inflate_end();

    // データ末尾の次の4バイトは CRC コード
    ymuint8 tmp_buff[4];
    if ( mBuff->read(tmp_buff, 4) != 4 ) {
      // truncated input
      goto end;
    }

    ymuint32 orig_crc = conv_4bytes(tmp_buff);
    if ( orig_crc != mCRC ) {
      cerr << "CRC ERROR!" << endl;
      cerr << " original_crc = "
	   << hex << orig_crc << dec << endl;
      return 0;
    }

    // その次の4バイトは データ長
    if ( mBuff->read(tmp_buff, 4) != 4 ) {
      // truncated input
      goto end;
    }
    ymuint32 orig_len = conv_4bytes(tmp_buff);
    if ( orig_len != mOutSize ) {
      cerr << "data-length ERROR!" << endl;
      return 0;
    }

    close();
  }

 end:
  return wr;
}


//////////////////////////////////////////////////////////////////////
// クラス GzODO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
GzODO::GzODO()
{
  mBuff = new FileBuff();
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
GzODO::GzODO(const char* filename,
	     ymuint level)
{
  mBuff = new FileBuff();
  open(filename, level);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
GzODO::GzODO(const string& filename,
	     ymuint level)
{
  mBuff = new FileBuff();
  open(filename, level);
}

// @brief デストラクタ
GzODO::~GzODO()
{
  close();
  delete mBuff;
}

// @brief 書き込み可能なら true を返す．
GzODO::operator bool() const
{
  return mBuff->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzODO::open(const char* filename,
	    ymuint level)
{
  static ymuint8 header[] = { GZIP_MAGIC0, GZIP_MAGIC1, Z_DEFLATED, 0,
			      0, 0, 0, 0,
			      0, OS_CODE };

  bool stat = mBuff->open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if ( !stat ) {
    return false;
  }

  // ヘッダを書き込む．
  assert_cond( mBuff->buff_size() > sizeof(header), __FILE__, __LINE__);
  memcpy(mBuff->buff_ptr(), header, sizeof(header));
  mBuff->seek(sizeof(header));

  {
    if ( level == 0 ) {
      level = 6;
    }
    int error = mZ.deflate_init2(level, Z_DEFLATED, (-MAX_WBITS), 8, Z_DEFAULT_STRATEGY);
    if ( error != Z_OK ) {
      // なんか知らないけどエラーが起きた．
      goto error_out;
    }
  }

  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0;

  mZ.set_outbuf(mBuff->buff_ptr(), mBuff->buff_size());

  return true;

 error_out:
  mBuff->close();
  return false;
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzODO::open(const string& filename,
	    ymuint level)
{
  return open(filename.c_str(), level);
}

BEGIN_NONAMESPACE

// 32ビットの符号なし整数を4バイトのデータに変換する．
inline
void
conv_to_4bytes(ymuint32 data,
	       ymuint8 buff[])
{
  buff[0] = (data >>  0) & 0xFF;
  buff[1] = (data >>  8) & 0xFF;
  buff[2] = (data >> 16) & 0xFF;
  buff[3] = (data >> 24) & 0xFF;
}

END_NONAMESPACE

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
GzODO::close()
{
  // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
  for ( ; ; ) {
    int rcode = mZ.deflate(Z_FINISH);
    if ( rcode != Z_OK && rcode != Z_STREAM_END ) {
      cerr << "Error in deflate(Z_FINISH): error = " << rcode << endl;
      goto end;
    }

    ymuint64 wr = mBuff->buff_size() - mZ.avail_out();
    if ( wr > 0 ) {
      mBuff->seek(wr);
      bool stat = mBuff->flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto end;
      }
      mZ.set_outbuf(mBuff->buff_ptr(), mBuff->buff_size());
    }
    if ( rcode == Z_STREAM_END ) {
      // 返り値が Z_OK ならまだ内部に処理されていないデータが残っている．
      break;
    }
  }
  {
    // 終了処理
    int stat = mZ.deflate_end();
    if ( stat != Z_OK ) {
      // deflate_end でエラー
      goto end;
    }
  }

  {
    ymuint8 trail[8];
    conv_to_4bytes(mCRC, &trail[0]);
    conv_to_4bytes(mOutSize, &trail[4]);
    ssize_t tsize = mBuff->write(trail, sizeof(trail));
    if ( tsize != sizeof(trail) ) {
      // エラー
      goto end;
    }
  }

 end:
  mBuff->close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
GzODO::write(const ymuint8* buff,
	     ymuint64 n)
{
  if ( n == 0 ) {
    return 0;
  }

  mZ.set_inbuf(buff, n);
  mCRC = crc32(mCRC, buff, n);
  mOutSize += n;

  for ( ; ; ) {
    if ( mZ.avail_out() == 0 ) {
      // 出力バッファが一杯になったのでファイルに書き出す．
      bool stat = mBuff->flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto error_out;
      }
      mZ.set_outbuf(mBuff->buff_ptr(), mBuff->buff_size());
    }

    // データを伸長する．
    int rcode = mZ.deflate(Z_NO_FLUSH);
    if ( rcode != Z_OK && rcode != Z_STREAM_END ) {
      // エラーが起きた．
      cerr << "Error in defalte()" << endl;
      goto error_out;
    }

    ymuint64 wr = mBuff->buff_size() - mZ.avail_out();
    mBuff->seek(wr);

    if ( mZ.avail_in() == 0 ) {
      // 入力データをすべて処理したら終わり．
      return n;
    }
  }

 error_out:
  // エラーが起きたらファイルをクローズして終わる．
  mBuff->close();
  return -1;
}

END_NONAMESPACE
