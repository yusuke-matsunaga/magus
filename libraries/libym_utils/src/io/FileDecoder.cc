
/// @file FileDecoder.cc
/// @brief FileDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileDecoder.h"
#include "ThruDecoder.h"
#include "ZDecoder.h"

#if defined(HAVE_ZLIB)
#include "GzDecoder.h"
#endif

#if defined(HAVE_BZLIB)
#include "BzDecoder.h"
#endif

#if defined(HAVE_LZMA)
#include "LzmaDecoder.h"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileDecoder
//////////////////////////////////////////////////////////////////////

// @brief FileDecoder のインスタンスを作る関数
// @param[in] type FileDecoder の型
// @return 作成したインスタンスを返す．
// @note type が不正な値の場合は NULL を返す．
FileDecoder*
FileDecoder::new_decoder(tCodecType type)
{
  FileDecoder* decoder = NULL;

  if ( type == kCodecThrough ) {
    decoder = new ThruDecoder();
  }
  else if ( type == kCodecZ ) {
    decoder = new ZDecoder();
  }
  else if ( type == kCodecGzip ) {
#if defined(HAVE_ZLIB)
    decoder = new GzDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileDecoder",
		    "gzip format is not supported on this system");
#endif
  }
  else if ( type == kCodecBzip2 ) {
#if defined(HAVE_BZLIB)
    decoder = new BzDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileDecoder",
		    "bzip2 format is not supported on this system");
#endif
  }
  else if ( type == kCodecLzma ) {
#if defined(HAVE_LZMA)
    decoder = new LzmaDecoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileCoder",
		    "lzma(xz) format is not supported on this system");
#endif
  }


  return decoder;
}

// @brief thru decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_thru_decoder()
{
  return new ThruDecoder();
}

// @brief compress decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_z_decoder()
{
  return new ZDecoder();
}

// @brief gzip decoder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_gzip_decoder()
{
#if defined(HAVE_ZLIB)
  return new GzDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileDecoder",
		  "gzip format is not supported on this system");
  return NULL;
#endif
}

// @brief bzip2 coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_bzip2_decoder()
{
#if defined(HAVE_BZLIB)
  return new BzDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileDecoder",
		  "bzip2 format is not supported on this system");
  return NULL;
#endif
}

// @brief xz coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileDecoder*
FileDecoder::new_xz_decoder()
{
#if defined(HAVE_LZMA)
  return new LzmaDecoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileDecoder",
		  "xz format is not supported on this system");
  return NULL;
#endif
}

END_NAMESPACE_YM
