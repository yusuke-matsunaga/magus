
/// @file GzCoder.cc
/// @brief GzCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GzCoder.h"
#include "gz_common.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GzCoder
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
GzCoder::GzCoder()
{
}

// @brief デストラクタ
GzCoder::~GzCoder()
{
  close();
}

// @brief 書き込み可能の時に true を返す．
bool
GzCoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzCoder::open(const char* filename,
	      mode_t mode,
	      ymuint level)
{
  static ymuint8 header[] = { GZIP_MAGIC0, GZIP_MAGIC1, Z_DEFLATED, 0,
			      0, 0, 0, 0,
			      0, OS_CODE };

  bool stat = mBuff.open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if ( !stat ) {
    return false;
  }

  // ヘッダを書き込む．
  assert_cond( mBuff.buff_size() > sizeof(header), __FILE__, __LINE__);
  memcpy(mBuff.buff_ptr(), header, sizeof(header));
  mBuff.seek(sizeof(header));

  {
    if ( level == 0 ) {
      level = 6;
    }
    int error = mZ.deflate_init2(level, Z_DEFLATED, (-MAX_WBITS),
				 8, Z_DEFAULT_STRATEGY);
    if ( error != Z_OK ) {
      // なんか知らないけどエラーが起きた．
      goto error_out;
    }
  }

  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0;

  mZ.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());

  return true;

 error_out:
  mBuff.close();
  return false;
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
GzCoder::close()
{
  if ( !mBuff.is_ready() ) {
    return;
  }

  // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
  for ( ; ; ) {
    int rcode = mZ.deflate(Z_FINISH);
    if ( rcode != Z_OK && rcode != Z_STREAM_END ) {
      cerr << "Error in deflate(Z_FINISH): error = " << rcode << endl;
      goto end;
    }

    ymuint64 wr = mBuff.buff_size() - mZ.avail_out();
    if ( wr > 0 ) {
      mBuff.seek(wr);
      bool stat = mBuff.flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto end;
      }
      mZ.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
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
    ssize_t tsize = mBuff.write(trail, sizeof(trail));
    if ( tsize != sizeof(trail) ) {
      // エラー
      goto end;
    }
  }

 end:
  mBuff.close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
GzCoder::write(const ymuint8* buff,
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
      bool stat = mBuff.flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto error_out;
      }
      mZ.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
    }

    // データを伸長する．
    int rcode = mZ.deflate(Z_NO_FLUSH);
    if ( rcode != Z_OK && rcode != Z_STREAM_END ) {
      // エラーが起きた．
      cerr << "Error in defalte()" << endl;
      goto error_out;
    }

    ymuint64 wr = mBuff.buff_size() - mZ.avail_out();
    mBuff.seek(wr);

    if ( mZ.avail_in() == 0 ) {
      // 入力データをすべて処理したら終わり．
      return n;
    }
  }

 error_out:
  // エラーが起きたらファイルをクローズして終わる．
  mBuff.close();
  return -1;
}

END_NAMESPACE_YM
