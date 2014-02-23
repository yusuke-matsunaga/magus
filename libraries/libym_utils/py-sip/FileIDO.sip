
/// @file FileIDO.sip
/// @brief FileIDO の sip ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class FileIDO : public IDO {
public:
%TypeHeaderCode
#include "utils/FileIDO.h"
using nsYm::FileIDO;
using nsYm::IDO;
using nsYm::tCodecType;
using nsYm::kCodecThrough;
using nsYm::kCodecZ;
using nsYm::kCodecGzip;
using nsYm::kCodecBzip2;
using nsYm::kCodecLzma;
%End

  /// @brief コンストラクタ
  /// @param[in] codec_type Decoder の種類
  explicit
  FileIDO(tCodecType codec_type = kCodecThrough);

  /// @brief 読み出し可能なら true を返す．
  bool
  is_ready() const;
  %MethodCode
  sipRes = sipCpp->operator bool();
  %End

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(FileInfo file_info);

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  /// @note エラーが起こったら負の数を返す．
  virtual
  long long
  read(unsigned char* buff,
       unsigned long long n);

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 他のファイルを開いていたら強制的に close する．
  bool
  open(const char* filename,
       FileLoc parent_loc = FileLoc());

  /// @brief ファイルを閉じる．
  /// @note 以降の読み出しは行われない．
  void
  close();


private:

  FileIDO(const FileIDO& src);

  FileIDO &operator=(FileIDO&);

};
