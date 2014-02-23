
/// @file FileODO.sip
/// @brief FileODO の sip ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


class FileODO :
  public ODO
{
%TypeHeaderCode
#include "utils/FileODO.h"
using nsYm::FileODO;
using nsYm::ODO;
using nsYm::tCodecType;
using nsYm::kCodecThrough;
using nsYm::kCodecZ;
using nsYm::kCodecGzip;
using nsYm::kCodecBzip2;
using nsYm::kCodecLzma;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] codec_type Coder の種類
  FileODO(tCodecType codec_type = kCodecThrough);

  /// @brief 書き込み可能なら true を返す．
  operator bool() const;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] opt 圧縮用のオプション
  bool
  open(const char* filename,
       int mode = 0666,
       unsigned int opt = 0);

  /// @brief ファイルを閉じる．
  /// @note 以降の書き込みは行われない．
  void
  close();

  /// @brief データを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  long
  write(const unsigned char* buff,
	unsigned long n);

};
