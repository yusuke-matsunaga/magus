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
/// @brief バイナリ出力ストリームの基底クラス
//////////////////////////////////////////////////////////////////////
class BinO
{
public:

  /// @brief コンストラクタ
  BinO() { }

  /// @brief デストラクタ
  virtual
  ~BinO() { }


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの書き込み
  /// @param[in] val 値
  void
  write_8(ymuint8 val);

  /// @brief 2バイトの書き込み
  /// @param[in] val 値
  void
  write_16(ymuint16 val);

  /// @brief 4バイトの書き込み
  /// @param[in] val 値
  void
  write_32(ymuint32 val);

  /// @brief 8バイトの書き込み
  /// @param[in] val 値
  void
  write_64(ymuint64 val);

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

  /// @brief 文字列の書き込み
  /// @param[in] val 値
  void
  write_str(const string& val);


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief nバイトのデータを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class BinOStream BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief ostream を用いた BinO の継承クラス
//////////////////////////////////////////////////////////////////////
class BinOStream :
  public BinO
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  BinOStream(ostream& s);

  /// @brief デストラクタ
  virtual
  ~BinOStream();


public:
  //////////////////////////////////////////////////////////////////////
  // BinO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを書き出す．
  /// @param[in] n データサイズ
  /// @param[in] buff データを収めた領域のアドレス
  /// @return 実際に書き出した量を返す．
  virtual
  ymuint
  write(ymuint64 n,
	const ymuint8* buff);


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
/// @brief バイナリ入力ストリームの基底クラス
//////////////////////////////////////////////////////////////////////
class BinI
{
public:

  /// @brief コンストラクタ
  BinI() { }

  /// @brief デストラクタ
  virtual
  ~BinI() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint8
  read_8();

  /// @brief 2バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint16
  read_16();

  /// @brief 4バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint32
  read_32();

  /// @brief 8バイトの読み出し
  /// @return 読み込んだ値を返す．
  ymuint64
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
  string
  read_str();


public:
  //////////////////////////////////////////////////////////////////////
  // BinI の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] n 読み込むデータサイズ
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @return 実際に読み込んだ量を返す．
  virtual
  ymuint
  read(ymuint64 n,
       ymuint8* buff) = 0;

};


//////////////////////////////////////////////////////////////////////
/// @class BinIStream BinIO.h "ym_utils/BinIO.h"
/// @ingroup YmUtils
/// @brief istream を用いた BinI の継承クラス
//////////////////////////////////////////////////////////////////////
class BinIStream :
  public BinI
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 入力ストリーム
  BinIStream(istream& s);

  /// @brief デストラクタ
  virtual
  ~BinIStream();


public:
  //////////////////////////////////////////////////////////////////////
  // BinI の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込む．
  /// @param[in] n 読み込むデータサイズ
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @return 実際に読み込んだ量を返す．
  virtual
  ymuint
  read(ymuint64 n,
       ymuint8* buff);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ストリーム
  istream& mS;

};


//////////////////////////////////////////////////////////////////////
// BinO に対するストリーム出力演算子
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


//////////////////////////////////////////////////////////////////////
// BinI に対するストリーム入力演算子
//////////////////////////////////////////////////////////////////////

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

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
inline
void
BinO::write_str(const string& val)
{
  write_str(val.c_str());
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
  s.write_8(val);
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
  s.write_8(val);
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
  s.write_16(val);
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
  s.write_32(val);
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
  s.write_64(val);
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
  s.write_float(val);
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
  s.write_double(val);
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
  s.write_str(val);
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
  s.write_str(val.c_str());
  return s;
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
  val = s.read_8();
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
  val = s.read_8();
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
  val = s.read_16();
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
  val = s.read_32();
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
  val = s.read_64();
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
  val = s.read_float();
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
  val = s.read_double();
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
  val = s.read_str();
  return s;
}

END_NAMESPACE_YM

#endif // YM_UTILS_BINIO_H
