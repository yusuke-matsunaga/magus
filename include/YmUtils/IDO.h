#ifndef YMUTILS_IDO_H
#define YMUTILS_IDO_H

/// @file YmUtils/IDO.h
/// @brief IDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-204 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class FileInfo;

//////////////////////////////////////////////////////////////////////
/// @class IDO IDO.h "YmUtils/IDO.h"
/// @ingroup YmUtils
/// @brief 入力データオブジェクト (Input Data Object)
//////////////////////////////////////////////////////////////////////
class IDO
{
public:

  /// @brief デストラクタ
  virtual
  ~IDO() { }


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

  /// @brief 一行分の文字列の読み出し．
  string
  read_line();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

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
  ssize_t
  read(ymuint8* buff,
       ymuint64 n) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief read() を呼び出して結果をチェックする．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @note n バイトの読み込みに失敗したらエラーメッセージを出力する．
  void
  _read(ymuint8* buff,
	ymuint64 n);

};


//////////////////////////////////////////////////////////////////////
// IDO に対するストリーム入力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   bool& val);

/// @brief 1バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint8& val);

/// @brief 2バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint16& val);

/// @brief 4バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint32& val);

/// @brief 8バイトの読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   ymuint64& val);

/// @brief 単精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   float& val);

/// @brief 倍精度不動週数点数の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   double& val);

/// @brief 文字列の読み出し
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @return IDO を返す．
IDO&
operator>>(IDO& s,
	   string& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ブール値の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   bool& val)
{
  val = s.read_8();
  return s;
}

// @brief 1バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint8& val)
{
  val = s.read_8();
  return s;
}

// @brief 2バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint16& val)
{
  val = s.read_16();
  return s;
}

// @brief 4バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint32& val)
{
  val = s.read_32();
  return s;
}

// @brief 8バイトの読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   ymuint64& val)
{
  val = s.read_64();
  return s;
}

// @brief 単精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   float& val)
{
  val = s.read_float();
  return s;
}

// @brief 倍精度不動週数点数の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   double& val)
{
  val = s.read_double();
  return s;
}

// @brief 文字列の読み出し
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @return IDO を返す．
inline
IDO&
operator>>(IDO& s,
	   string& val)
{
  val = s.read_str();
  return s;
}

END_NAMESPACE_YM

#endif // YMUTILS_IDO_H
