#ifndef YM_UTILS_CODEC_TYPE_H
#define YM_UTILS_CODEC_TYPE_H

/// @file ym_utils/CodecType.h
/// @brief tCodecType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief FileCoder/FileDecoder の種類
//////////////////////////////////////////////////////////////////////
enum tCodecType {
  /// @brief 無変換
  kCodecThrough,
  /// @brief compress (Z)
  kCodecZ,
  /// @brief gzip
  kCodecGzip
};

END_NAMESPACE_YM

#endif // YM_UTILS_CODEC_TYPE_H
