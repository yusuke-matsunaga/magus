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
/// @class BinO BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief バイナリ出力ストリーム
//////////////////////////////////////////////////////////////////////
class BinO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  BinO(ostream& s);

  /// @brief ストリームを得る．
  ostream&
  s();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};


//////////////////////////////////////////////////////////////////////
/// @class BinI BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief バイナリ入力ストリーム
//////////////////////////////////////////////////////////////////////
class BinI
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  BinI(istream& s);

  /// @brief ストリームを得る．
  istream&
  s();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

};


//////////////////////////////////////////////////////////////////////
/// @class BinIO BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief バイナリ形式でダンプ/リストアするためのクラス
/// @note 実際にはクラスメソッドしか持たない．
//////////////////////////////////////////////////////////////////////
class BinIO
{
public:
  //////////////////////////////////////////////////////////////////////
  // クラスメソッド
  //////////////////////////////////////////////////////////////////////

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
// BinIO に対するストリーム出力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   bool val);

/// @brief 1バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint8 val);

/// @brief 2バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint16 val);

/// @brief 4バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint32 val);

/// @brief 8バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   ymuint64 val);

/// @brief 単精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   float val);

/// @brief 倍精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   double val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   const char* val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return BinO を返す．
BinO&
operator<<(BinO& s,
	   const string& val);

/// @brief ブール値の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   bool& val);

/// @brief 1バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   ymuint8& val);

/// @brief 2バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   ymuint16& val);

/// @brief 4バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   ymuint32& val);

/// @brief 8バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   ymuint64& val);

/// @brief 単精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   float& val);

/// @brief 倍精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   double& val);

/// @brief 文字列の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return BinI を返す．
BinI&
operator>>(BinI& s,
	   string& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ストリームを得る．
inline
ostream&
BinO::s()
{
  return mS;
}

// @brief ブール値の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   bool val)
{
  BinIO::write_8(s.s(), val);
  return s;
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint8 val)
{
  BinIO::write_8(s.s(), val);
  return s;
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint16 val)
{
  BinIO::write_16(s.s(), val);
  return s;
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint32 val)
{
  BinIO::write_32(s.s(), val);
  return s;
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   ymuint64 val)
{
  BinIO::write_64(s.s(), val);
  return s;
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   float val)
{
  BinIO::write_float(s.s(), val);
  return s;
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   double val)
{
  BinIO::write_double(s.s(), val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   const char* val)
{
  BinIO::write_str(s.s(), val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return BinO を返す．
inline
BinO&
operator<<(BinO& s,
	   const string& val)
{
  BinIO::write_str(s.s(), val.c_str());
  return s;
}

// @brief ストリームを得る．
inline
istream&
BinI::s()
{
  return mS;
}

// @brief ブール値の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   bool& val)
{
  val = BinIO::read_8(s.s());
  return s;
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   ymuint8& val)
{
  val = BinIO::read_8(s.s());
  return s;
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   ymuint16& val)
{
  val = BinIO::read_16(s.s());
  return s;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   ymuint32& val)
{
  val = BinIO::read_32(s.s());
  return s;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   ymuint64& val)
{
  val = BinIO::read_64(s.s());
  return s;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   float& val)
{
  val = BinIO::read_float(s.s());
  return s;
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   double& val)
{
  val = BinIO::read_double(s.s());
  return s;
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return BinI を返す．
inline
BinI&
operator>>(BinI& s,
	   string& val)
{
  val = BinIO::read_str(s.s());
  return s;
}

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
