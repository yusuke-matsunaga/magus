
/// @file FileDecoder.cc
/// @brief FileDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileDecoder.h"
#include "CompressDecoder.h"
#include "GzDecoder.h"
#include "ThruDecoder.h"


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
  else if ( type == kCodecCompress ) {
    decoder = new CompressDecoder();
  }
  else if ( type == kCodecGzip ) {
    decoder = new GzDecoder();
  }

  return decoder;
}

END_NAMESPACE_YM
