
/// @file LzmaCoder.cc
/// @brief LzmaCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "LzmaCoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LzmaCoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LzmaCoder::LzmaCoder()
{
}

// @brief デストラクタ
LzmaCoder::~LzmaCoder()
{
  close();
}

// @brief 書き込み可能の時に true を返す．
bool
LzmaCoder::is_ready() const
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
LzmaCoder::open(const char* filename,
		mode_t mode,
		ymuint level)
{
  bool stat = mBuff.open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if ( !stat ) {
    return false;
  }

  lzma_ret rcode = mCompEngine.easy_encoder(level);
  if ( rcode != LZMA_OK ) {
    cerr << "Error in LzmaStream::easy_encoder(" << level << ")" << endl;
    cerr << " return code = " << rcode << endl;
    mBuff.close();
    return false;
  }

  mCompEngine.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
  mNextAction = LZMA_RUN;

  return true;
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
LzmaCoder::close()
{
  if ( !mBuff.is_ready() ) {
    return;
  }

  // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
  for ( ; ; ) {
    lzma_ret rcode = mCompEngine.code(LZMA_FINISH);
    if ( rcode != LZMA_OK && rcode != LZMA_STREAM_END ) {
      cerr << "Error in LzmaCompEngine::code(LZMA_FINISH): error = "
	   << rcode << endl;
      goto end;
    }

    ymuint64 wr = mBuff.buff_size() - mCompEngine.avail_out();
    if ( wr > 0 ) {
      mBuff.seek(wr);
      bool stat = mBuff.flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto end;
      }
      mCompEngine.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
    }
    if ( rcode == LZMA_STREAM_END ) {
      // 返り値が LZMA_OK ならまだ内部に処理されていないデータが残っている．
      break;
    }
  }

  // 終了処理
  mCompEngine.end();

 end:
  mBuff.close();
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ymint64
LzmaCoder::write(const ymuint8* buff,
		 ymuint64 n)
{
  if ( !mBuff.is_ready() ) {
    return -1;
  }

  if ( n == 0 ) {
    mNextAction = LZMA_FINISH;
  }

  mCompEngine.set_inbuf(buff, n);

  for ( ; ; ) {
    // データを圧縮する．
    lzma_ret rcode = mCompEngine.code(mNextAction);
    if ( rcode != LZMA_OK &&
	 rcode != LZMA_STREAM_END ) {
      // エラーが起きた．
      cerr << "Error in LzmaCompEngine::code()" << endl;
      goto error_out;
    }

    // 今の処理で書き込まれた分だけバッファのポインタを進める．
    ymuint64 wr = mBuff.buff_size() - mCompEngine.avail_out();
    mBuff.seek(wr);

    if ( mCompEngine.avail_out() == 0 ) {
      // 出力バッファが一杯になったのでファイルに書き出す．
      bool stat = mBuff.flush();
      if ( !stat ) {
	// 書き込みエラー
	cerr << "Error in write()" << endl;
	goto error_out;
      }
      mCompEngine.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
    }

    if ( mCompEngine.avail_in() == 0 ) {
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
