
/// @file ODO.sip
/// @brief ODO の sip ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.

class ODO
{

%TypeHeaderCode
#include "utils/ODO.h"
using namespace nsYm;
%End

public:

  /// @brief 1バイトの書き込み
  /// @param[in] val 値
  void
  write_8(unsigned char val);

  /// @brief 2バイトの書き込み
  /// @param[in] val 値
  void
  write_16(unsigned int val);

  /// @brief 4バイトの書き込み
  /// @param[in] val 値
  void
  write_32(unsigned long val);

  /// @brief 8バイトの書き込み
  /// @param[in] val 値
  void
  write_64(unsigned long long val);

  /// @brief 単精度浮動小数点数の書き込み
  /// @param[in] val 値
  void
  write_float(float val);

  /// @brief 倍精度浮動小数点数の書き込み
  /// @param[in] val 値
  void
  write_double(double val);

  /// @brief 文字列の書き込み
  /// @param[in] val 値
  void
  write_str(const char* val);

  /// @brief nバイトのデータを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  long
  write(const unsigned char* buff,
	unsigned long n) = 0;

};
