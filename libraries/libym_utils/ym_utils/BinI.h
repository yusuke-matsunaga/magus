#ifndef YM_UTILS_BINI_H
#define YM_UTILS_BINI_H

/// @file ym_utils/BinI.h
/// @brief BinI のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BinI BinI.h "ym_utils/BinI.h"
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
/// @class BinIStream BinI.h "ym_utils/BinI.h"
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

#endif // YM_UTILS_BINI_H
