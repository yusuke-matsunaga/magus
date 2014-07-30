#ifndef YMUTILS_ODO_H
#define YMUTILS_ODO_H

/// @file YmUtils/ODO.h
/// @brief ODO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ODO ODO.h "YmUtils/ODO.h"
/// @ingroup YmUtils
/// @brief バイナリ出力ストリームの基底クラス
//////////////////////////////////////////////////////////////////////
class ODO
{
public:

  /// @brief デストラクタ
  virtual
  ~ODO() { }


public:
  //////////////////////////////////////////////////////////////////////
  // ODO の公開関数
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
  // ODO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief nバイトのデータを書き出す．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  /// @return 実際に書き出した量を返す．
  virtual
  ssize_t
  write(const ymuint8* buff,
	ymuint64 n) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief write() を呼び出して結果をチェックする．
  /// @param[in] buff データを収めた領域のアドレス
  /// @param[in] n データサイズ
  void
  _write(const ymuint8* buff,
	 ymuint64 n);

};


//////////////////////////////////////////////////////////////////////
// ODO に対するストリーム出力演算子
//////////////////////////////////////////////////////////////////////

/// @brief ブール値の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   bool val);

/// @brief 1バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   ymuint8 val);

/// @brief 2バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   ymuint16 val);

/// @brief 4バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   ymuint32 val);

/// @brief 8バイトの書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   ymuint64 val);

/// @brief 単精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   float val);

/// @brief 倍精度浮動小数点数の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   double val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   const char* val);

/// @brief 文字列の書き込み
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @return ODO を返す．
ODO&
operator<<(ODO& s,
	   const string& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
inline
void
ODO::write_str(const string& val)
{
  write_str(val.c_str());
}

// @brief ブール値の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   bool val)
{
  s.write_8(val);
  return s;
}

// @brief 1バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   ymuint8 val)
{
  s.write_8(val);
  return s;
}

// @brief 2バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   ymuint16 val)
{
  s.write_16(val);
  return s;
}

// @brief 4バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   ymuint32 val)
{
  s.write_32(val);
  return s;
}

// @brief 8バイトの書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   ymuint64 val)
{
  s.write_64(val);
  return s;
}

// @brief 単精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   float val)
{
  s.write_float(val);
  return s;
}

// @brief 倍精度浮動小数点数の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   double val)
{
  s.write_double(val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   const char* val)
{
  s.write_str(val);
  return s;
}

// @brief 文字列の書き込み
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @return ODO を返す．
inline
ODO&
operator<<(ODO& s,
	   const string& val)
{
  s.write_str(val.c_str());
  return s;
}

END_NAMESPACE_YM

#endif // YMUTILS_ODO_H
