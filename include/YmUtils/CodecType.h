#ifndef YMUTILS_CODECTYPE_H
#define YMUTILS_CODECTYPE_H

/// @file YmUtils/CodecType.h
/// @brief CodecType の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief FileCoder/FileDecoder の種類
//////////////////////////////////////////////////////////////////////
enum CodecType {
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

#endif // YMUTILS_CODECTYPE_H
