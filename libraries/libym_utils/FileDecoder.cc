
/// @file FileDecoder.cc
/// @brief FileDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileDecoder.h"
#include "ThruDecoder.h"
#include "ZDecoder.h"

#if defined(HAVE_ZLIB)
#include "GzDecoder.h"
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
  return new GzDecoder();
}

END_NAMESPACE_YM
