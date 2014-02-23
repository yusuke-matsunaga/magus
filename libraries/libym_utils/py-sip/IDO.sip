
/// @file IDO.sip
/// @brief IDO の sip ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class IDO
{

%TypeHeaderCode
#include "utils/IDO.h"
using nsYm::IDO;
%End

public:

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  unsigned int
  read_8();

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  unsigned int
  read_16();

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  unsigned long
  read_32();

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  unsigned long long
  read_64();

  /// @brief 単精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  float
  read_float();

  /// @brief 倍精度不動週数点数の読み出し
  /// @return 読み込んだ値を返す．
  double
  read_double();

  /// @brief 文字列の読み出し
  /// @return 読み込んだ値を返す．
  const char*
  read_str();
  %MethodCode
  sipRes = sipCpp->read_str().c_str();
  %End

  /// @brief 一行分の文字列の読み出し．
  const char*
  read_line();
  %MethodCode
  sipRes = sipCpp->read_line().c_str();
  %End

  /// @brief 読み出し可能なら true を返す．
  virtual
  bool
  is_ready() const = 0;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const = 0;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info) = 0;

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  virtual
  long
  read(unsigned char* buff,
       unsigned long n) = 0;

};