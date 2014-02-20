#ifndef UTILS_CODEC_TYPE_H
#define UTILS_CODEC_TYPE_H

/// @file utils/CodecType.h
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
  kCodecGzip,
  /// @brief bzip2
  kCodecBzip2,
  /// @brief lzma (xz)
  kCodecLzma
};

END_NAMESPACE_YM

#endif // UTILS_CODEC_TYPE_H
