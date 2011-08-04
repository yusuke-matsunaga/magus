#ifndef YM_UTILS_BINIO_H
#define YM_UTILS_BINIO_H

/// @file ym_utils/BinIO.h
/// @brief BinIO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BinIO BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief バイナリ形式でダンプ/リストアするためのクラス
/// @note 実はクラスメンバ関数(静的メンバ関数)のみのクラス
//////////////////////////////////////////////////////////////////////
class BinIO
{
public:

  /// @brief 1バイトの書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_8(ostream& s,
	  ymuint8 val);

  /// @brief 2バイトの書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_16(ostream& s,
	   ymuint16 val);

  /// @brief 4バイトの書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_32(ostream& s,
	   ymuint32 val);

  /// @brief 8バイトの書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_64(ostream& s,
	   ymuint64 val);

  /// @brief 単精度浮動小数点数の書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_float(ostream& s,
	      float val);

  /// @brief 倍精度浮動小数点数の書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_double(ostream& s,
	       double val);

  /// @brief 文字列の書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_str(ostream& s,
	    const char* val);

  /// @brief 文字列の書き込み
  /// @param[in] s 出力先のストリーム
  /// @param[in] val 値
  static
  void
  write_str(ostream& s,
	    const string& val);

  /// @brief 1バイトの読み出し
  /// @param[in] s 入力元のストリーム
  static
  ymuint8
  read_8(istream& s);

  /// @brief 2バイトの読み出し
  /// @param[in] s 入力元のストリーム
  static
  ymuint16
  read_16(istream& s);

  /// @brief 4バイトの読み出し
  /// @param[in] s 入力元のストリーム
  static
  ymuint32
  read_32(istream& s);

  /// @brief 8バイトの読み出し
  /// @param[in] s 入力元のストリーム
  static
  ymuint64
  read_64(istream& s);

  /// @brief 単精度不動週数点数の読み出し
  /// @param[in] s 入力元のストリーム
  static
  float
  read_float(istream& s);

  /// @brief 倍精度不動週数点数の読み出し
  /// @param[in] s 入力元のストリーム
  static
  double
  read_double(istream& s);

  /// @brief 文字列の読み出し
  /// @param[in] s 入力元のストリーム
  static
  string
  read_str(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バッファ
  static
  ymuint8 mBuf[8];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
inline
void
BinIO::write_str(ostream& s,
		 const string& val)
{
  write_str(s, val.c_str());
}

END_NAMESPACE_YM

#endif // YM_UTILS_BINIO_H
