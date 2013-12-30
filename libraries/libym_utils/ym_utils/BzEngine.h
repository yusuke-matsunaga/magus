#ifndef YM_UTILS_BZENGINE_H
#define YM_UTILS_BZENGINE_H

/// @file ym_utils/BzEngine.h
/// @brief BzEngine のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#if defined(HAVE_BZLIB)
#include <bzlib.h>
#else
#error "\"ym_utils/BzEngine.h\" requires <bzlib.h>"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BzEngine BzEngine.h "ym_utils/BzEngine.h"
/// @brief bz_stream の C++ 用ラッパクラス
//////////////////////////////////////////////////////////////////////
class BzEngine
{
public:
  //////////////////////////////////////////////////////////////////////
  // 関数の型定義
  //////////////////////////////////////////////////////////////////////

  /// @brief alloc 関数の型定義
  typedef void* (*alloc_func)(void *, int, int);

  /// @brief free 関数の型定義
  typedef void (*free_func)(void *, void *);


public:

  /// @brief コンストラクタ
  /// @param[in] af alloc 関数
  /// @param[in] ff free 関数
  /// @param[in] op opaque オブジェクト
  BzEngine(alloc_func af = NULL,
	   free_func ff = NULL,
	   void* op = NULL);

  /// @brief デストラクタ
  ~BzEngine();


public:
  //////////////////////////////////////////////////////////////////////
  // 圧縮用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮用の初期化を行う．
  /// @param[in] block_size_100k 作業用のメモリサイズ ( 0 - 9 )
  /// @param[in] verbosity デバッグ用の出力レベル ( 0 - 4 )
  /// @param[in] work_factor 圧縮アルゴリズム用のパラメータ ( 0 - 250 )
  int
  compress_init(int block_size_100k,
		int verbosity,
		int work_factor);

  /// @brief 圧縮を行う．
  /// @param[in] action 動作コード
  ///
  /// action のとりうる値は以下のとおり
  ///  - BZ_RUN
  ///  - BZ_FLUSH
  ///  - BZ_FINISH
  int
  compress(int action);

  /// @brief 圧縮を終了する．
  int
  compress_end();


public:
  //////////////////////////////////////////////////////////////////////
  // 伸張用の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 伸張用のの初期化を行う．
  /// @param[in] verbosity デバッグ用の出力レベル ( 0 - 4 )
  /// @param[in] small 伸張アルゴリズム用のパラメータ ( 0 か 非0 )
  int
  decompress_init(int verbosity,
		  int small);

  /// @brief 伸張を行う．
  int
  decompress();

  /// @brief 伸張を終了する．
  int
  decompress_end();


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
  bz_stream mBzStream;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] af alloc 関数
// @param[in] ff free 関数
// @param[in] op opaque オブジェクト
inline
BzEngine::BzEngine(alloc_func af,
		   free_func ff,
		   void* op)
{
  mBzStream.bzalloc = af;
  mBzStream.bzfree = ff;
  mBzStream.opaque = op;
}

// @brief デストラクタ
inline
BzEngine::~BzEngine()
{
}

// @brief 圧縮用の初期化を行う．
// @param[in] block_size_100k
// @param[in] verbosity
// @param[in] work_factor
inline
int
BzEngine::compress_init(int block_size_100k,
			int verbosity,
			int work_factor)
{
  return BZ2_bzCompressInit(&mBzStream, block_size_100k, verbosity, work_factor);
}

// @brief 圧縮を行う．
// @param[in] action
inline
int
BzEngine::compress(int action)
{
  return BZ2_bzCompress(&mBzStream, action);
}

// @brief 圧縮を終了する．
inline
int
BzEngine::compress_end()
{
  return BZ2_bzCompressEnd(&mBzStream);
}

// @brief 伸張用のの初期化を行う．
// @param[in] verbosity
// @param[in] small
inline
int
BzEngine::decompress_init(int verbosity,
			  int small)
{
  return BZ2_bzDecompressInit(&mBzStream, verbosity, small);
}

// @brief 伸張を行う．
inline
int
BzEngine::decompress()
{
  return BZ2_bzDecompress(&mBzStream);
}

// @brief 伸張を終了する．
inline
int
BzEngine::decompress_end()
{
  return BZ2_bzDecompressEnd(&mBzStream);
}

// @brief in バッファを設定する．
// @param[in] buf バッファ本体
// @param[in] size バッファのサイズ
inline
void
BzEngine::set_inbuf(const ymuint8* buf,
		    ymuint64 size)
{
  // void* を利用した巧妙なキャスト
  // といっても本当は良くないコード
  void* tmp = const_cast<ymuint8*>(buf);
  mBzStream.next_in = static_cast<char*>(tmp);
  mBzStream.avail_in = size;
}

// @brief out バッファを設定する．
// @param[in] buf バッファ本体
// @param[in] size バッファのサイズ
inline
void
BzEngine::set_outbuf(ymuint8* buf,
		     ymuint64 size)
{
  // void* を利用した巧妙なキャスト
  // といっても本当は良くないコード
  void* tmp = buf;
  mBzStream.next_out = static_cast<char*>(tmp);
  mBzStream.avail_out = size;
}

// @brief 読み出せるデータのバイト数を返す．
inline
ymuint
BzEngine::avail_in()
{
  return mBzStream.avail_in;
}

// @brief 書き込めるデータのバイト数を返す．
inline
ymuint
BzEngine::avail_out()
{
  return mBzStream.avail_out;
}

END_NAMESPACE_YM

#endif // YM_UTILS_BZENGINE_H
