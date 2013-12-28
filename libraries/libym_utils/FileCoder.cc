
/// @file FileCoder.cc
/// @brief FileCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileCoder.h"
#include "CompressCoder.h"
#include "GzCoder.h"
#include "ThruCoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileCoder
//////////////////////////////////////////////////////////////////////

// @brief FileCoder のインスタンスを作る関数
// @param[in] type FileCoder の型
// @return 作成したインスタンスを返す．
// @note type が不正な値の場合は NULL を返す．
FileCoder*
FileCoder::new_coder(tCodecType type)
{
  FileCoder* coder = NULL;

  if ( type == kCodecThrough ) {
    coder = new ThruCoder();
  }
  else if ( type == kCodecCompress ) {
    coder = new CompressCoder();
  }
  else if ( type == kCodecGzip ) {
    coder = new GzCoder();
  }

  return coder;
}

END_NAMESPACE_YM
