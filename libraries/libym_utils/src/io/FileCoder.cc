
/// @file FileCoder.cc
/// @brief FileCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileCoder.h"
#include "ThruCoder.h"
#include "ZCoder.h"
#include "YmUtils/MsgMgr.h"

#if defined(ZLIB_FOUND)
#include "GzCoder.h"
#endif

#if defined(BZIP2_FOUND)
#include "BzCoder.h"
#endif

#if defined(LIBLZMA_FOUND)
#include "LzmaCoder.h"
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileCoder
//////////////////////////////////////////////////////////////////////

// @brief FileCoder のインスタンスを作る関数
// @param[in] type FileCoder の型
// @return 作成したインスタンスを返す．
// @note type が不正な値の場合は NULL を返す．
FileCoder*
FileCoder::new_coder(CodecType type)
{
  FileCoder* coder = NULL;

  if ( type == kCodecThrough ) {
    coder = new ThruCoder();
  }
  else if ( type == kCodecZ ) {
    coder = new ZCoder();
  }
  else if ( type == kCodecGzip ) {
#if defined(ZLIB_FOUND)
    coder = new GzCoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileCoder",
		    "gzip format is not supported on this system");
#endif
  }
  else if ( type == kCodecBzip2 ) {
#if defined(BZIP2_FOUND)
    coder = new BzCoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileCoder",
		    "bzip2 format is not supported on this system");
#endif
  }
  else if ( type == kCodecLzma ) {
#if defined(LIBLZMA_FOUND)
    coder = new LzmaCoder();
#else
    MsgMgr::put_msg(__FILE__, __LINE__,
		    kMsgError,
		    "FileCoder",
		    "lzma(xz) format is not supported on this system");
#endif
  }

  return coder;
}

// @brief thru coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileCoder*
FileCoder::new_thru_coder()
{
  return new ThruCoder();
}

// @brief compress coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileCoder*
FileCoder::new_z_coder()
{
  return new ZCoder();
}

// @brief gzip coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileCoder*
FileCoder::new_gzip_coder()
{
#if defined(ZLIB_FOUND)
  return new GzCoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileCoder",
		  "gzip format is not supported on this system");
  return NULL;
#endif
}

// @brief bzip2 coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileCoder*
FileCoder::new_bzip2_coder()
{
#if defined(BZIP2_FOUND)
  return new BzCoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileCoder",
		  "bzip2 format is not supported on this system");
  return NULL;
#endif
}

// @brief xz coder のインスタンスを作る関数
// @return 作成したインスタンスを返す．
FileCoder*
FileCoder::new_xz_coder()
{
#if defined(LIBLZMA_FOUND)
  return new LzmaCoder();
#else
  MsgMgr::put_msg(__FILE__, __LINE__,
		  kMsgError,
		  "FileCoder",
		  "xz format is not supported on this system");
  return NULL;
#endif
}

END_NAMESPACE_YM
