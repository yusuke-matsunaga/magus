
/// @file CodecType.sip
/// @brief tCodecType の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.

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
