#ifndef YM_UTILS_LZMAENGINE_H
#define YM_UTILS_LZMAENGINE_H

/// @file ym_utils/LzmaEngine.h
/// @brief LzmaEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#if defined(HAVE_LZMA)
#include <lzma.h>
#else
#error "\"ym_utils/LzmaEngine.h\" requires <lzma.h>"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LzmaEngine LzmaEngine.h "ym_utils/LzmaEngine.h"
/// @brief lzma_stream の C++ 用ラッパクラス
//////////////////////////////////////////////////////////////////////
class LzmaEngine
{
public:

  /// @brief コンストラクタ
  /// @param[in] af alloc 関数
  LzmaEngine(lzma_allocator* af = NULL);

  /// @brief デストラクタ
  ~LzmaEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 初期化/終了処理を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief xz 圧縮用の初期化を行う．
  /// @param[in] preset 圧縮レベル ( 0 - 9: 6 がデフォルト )
  /// @param[in] check 検査方法
  /// @return 終了コードを返す．
  ///
  /// 検査方法は以下のとおり
  ///  - LZMA_CHECK_NONE
  ///  - LZMA_CHECK_CRC32
  ///  - LZMA_CHECK_CRC64
  ///  - LZMA_CHECK_SHA256
  ///
  /// 成功したら LZMA_OK を返す．
  int
  easy_encoder(ymuint32 preset = 6,
	       lzma_check check = LZMA_CHECK_CRC64);

  /// @brief xz 伸長用の初期化を行う．
  /// @param[in] memlimit 割り当てるメモリの上限
  /// @param[in] flags 動作制御用のフラグ
  /// @return 終了コードを返す．
  int
  stream_decoder(ymuint64 memlimit = 128 * 1024 * 1024,
		 ymuint32 flags = 0);

  /// @brief 終了処理を行う．
  void
  end();


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮/伸長を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮/伸長を行う．
  /// @param[in] action 動作コード
  /// @return 終了コードを返す．
  ///
  /// action のとりうる値は以下のとおり
  ///  - LZMA_RUN
  ///  - LZMA_SYNC_FLUSH
  ///  - LZMA_FULL_FLUSH
  ///  - LZMA_FINISH
  int
  code(lzma_action action);


public:
  //////////////////////////////////////////////////////////////////////
  // バッファの操作関数
  //////////////////////////////////////////////////////////////////////

  /// @brief in バッファを設定する．
  /// @param[in] buf バッファ本体
  /// @param[in] size バッファのサイズ
  void
  set_inbuf(const ymuint8* buf,
	    ymuint64 size);

  /// @brief out バッファを設定する．
  /// @param[in] buf バッファ本体
  /// @param[in] size バッファのサイズ
  void
  set_outbuf(ymuint8* buf,
	     ymuint64 size);

  /// @brief 読み出せるデータのバイト数を返す．
  ymuint
  avail_in();

  /// @brief 書き込めるデータのバイト数を返す．
  ymuint
  avail_out();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当の構造体
  lzma_stream mLzmaStream;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] af alloc 関数
inline
LzmaEngine::LzmaEngine(lzma_allocator* af)
{
  mLzmaStream = (lzma_stream)LZMA_STREAM_INIT;
  if ( af != NULL ) {
    mLzmaStream.allocator = af;
  }
}

// @brief デストラクタ
inline
LzmaEngine::~LzmaEngine()
{
  end();
}

// @brief xz 圧縮用の初期化を行う．
// @param[in] preset 圧縮レベル ( 0 - 9: 6 がデフォルト )
// @param[in] check 検査方法
// @return 終了コードを返す．
inline
int
LzmaEngine::easy_encoder(ymuint32 preset,
			 lzma_check check)
{
  return lzma_easy_encoder(&mLzmaStream, preset, check);
}

// @brief xz 伸長用の初期化を行う．
// @param[in] memlimit 割り当てるメモリの上限
// @param[in] flags 動作制御用のフラグ
// @return 終了コードを返す．
inline
int
LzmaEngine::stream_decoder(ymuint64 memlimit,
			   ymuint32 flags)
{
  return lzma_stream_decoder(&mLzmaStream, memlimit, flags);
}

// @brief 終了処理を行う．
inline
void
LzmaEngine::end()
{
  lzma_end(&mLzmaStream);
}

// @brief 圧縮/伸長を行う．
// @param[in] action 動作コード
// @return 終了コードを返す．
inline
int
LzmaEngine::code(lzma_action action)
{
  return lzma_code(&mLzmaStream, action);
}

// @brief in バッファを設定する．
// @param[in] buf バッファ本体
// @param[in] size バッファのサイズ
inline
void
LzmaEngine::set_inbuf(const ymuint8* buf,
		      ymuint64 size)
{
#if 0
  // void* を利用した巧妙なキャスト
  // といっても本当は良くないコード
  void* tmp = const_cast<ymuint8*>(buf);
  mLzmaStream.next_in = static_cast<char*>(tmp);
#else
  mLzmaStream.next_in = buf;
#endif
  mLzmaStream.avail_in = size;
}

// @brief out バッファを設定する．
// @param[in] buf バッファ本体
// @param[in] size バッファのサイズ
inline
void
LzmaEngine::set_outbuf(ymuint8* buf,
		       ymuint64 size)
{
#if 0
  // void* を利用した巧妙なキャスト
  // といっても本当は良くないコード
  void* tmp = buf;
  mLzmaStream.next_out = static_cast<char*>(tmp);
#else
  mLzmaStream.next_out = buf;
#endif
  mLzmaStream.avail_out = size;
}

// @brief 読み出せるデータのバイト数を返す．
inline
ymuint
LzmaEngine::avail_in()
{
  return mLzmaStream.avail_in;
}

// @brief 書き込めるデータのバイト数を返す．
inline
ymuint
LzmaEngine::avail_out()
{
  return mLzmaStream.avail_out;
}

END_NAMESPACE_YM

#endif // YM_UTILS_LZMAENGINE_H
