
/// @file BzCoder.cc
/// @brief BzCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BzCoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BzCoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BzCoder::BzCoder()
{
}

// @brief デストラクタ
BzCoder::~BzCoder()
{
  close();
}

// @brief 書き込み可能の時に true を返す．
bool
BzCoder::is_ready() const
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
BzCoder::open(const char* filename,
	      mode_t mode,
	      ymuint level)
{
  bool stat = mBuff.open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
  if ( !stat ) {
    return false;
  }

  if ( level == 0 ) {
    level = 9;
  }
  int rcode = mCompEngine.compress_init(level, 0, 0);
  if ( rcode != BZ_OK ) {
    cerr << "Error in BzStream::compress_init(" << level << ", 0, 0)" << endl;
    cerr << " return code = " << rcode << endl;
    mBuff.close();
    return false;
  }

  mCompEngine.set_outbuf(mBuff.buff_ptr(), mBuff.buff_size());
  mNextAction = BZ_RUN;

  return true;
}

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
BzCoder::close()
{
  if ( !mBuff.is_ready() ) {
    return;
  }

  // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
  for ( ; ; ) {
    int rcode = mCompEngine.compress(BZ_FINISH);
    if ( rcode != BZ_FINISH_OK && rcode != BZ_STREAM_END ) {
      cerr << "Error in BzCompEngine::compress(BZ_FINISH): error = " << rcode << endl;
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
    if ( rcode == BZ_STREAM_END ) {
      // 返り値が BZ_FINISH_OK ならまだ内部に処理されていないデータが残っている．
      break;
    }
  }
  {
    // 終了処理
    int stat = mCompEngine.compress_end();
    if ( stat != BZ_OK ) {
      // compress_end でエラー
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
BzCoder::write(const ymuint8* buff,
	       ymuint64 n)
{
  if ( !mBuff.is_ready() ) {
    return -1;
  }

  if ( n == 0 ) {
    mNextAction = BZ_FINISH;
  }

  mCompEngine.set_inbuf(buff, n);

  for ( ; ; ) {
    // データを圧縮する．
    int rcode = mCompEngine.compress(mNextAction);
    if ( rcode == BZ_RUN_OK ) {
      mNextAction = BZ_RUN;
    }
    else if ( rcode == BZ_FLUSH_OK ) {
      mNextAction = BZ_FLUSH;
    }
    else if ( rcode == BZ_FINISH_OK ) {
      mNextAction = BZ_FINISH;
    }
    else if ( rcode == BZ_PARAM_ERROR ||
	      rcode == BZ_SEQUENCE_ERROR ||
	      rcode == BZ_MEM_ERROR ) {
      // エラーが起きた．
      cerr << "Error in BzCompEngine::compress()" << endl;
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
