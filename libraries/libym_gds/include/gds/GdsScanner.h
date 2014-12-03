#ifndef GDS_GDSSCANNER_H
#define GDS_GDSSCANNER_H

/// @file gds/GdsScanner.h
/// @brief GdsScanner のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsScanner GdsScanner.h "gds/GdsScanner.h"
/// @brief GDS-II の読み込みを行うクラス
//////////////////////////////////////////////////////////////////////
class GdsScanner
{
public:

  /// @brief コンストラクタ
  GdsScanner();

  /// @brief デストラクタ
  ~GdsScanner();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true オープンに成功した．
  /// @retval false オープンに失敗した．
  bool
  open_file(const string& filename);

  /// @brief ファイルを閉じる．
  void
  close_file();

  /// @brief レコード一つ分の読み込みを行う．
  /// @retval true 読み込みが成功した．
  /// @retval false エラーが起った場合や末尾に達した場合
  bool
  read_rec();

  /// @brief 直前の read_rec() で読んだレコードのオフセットを得る．
  ymuint32
  cur_offset() const;

  /// @brief 直前の read_rec() で読んだレコードのサイズを得る．
  ymuint32
  cur_size() const;

  /// @brief 直前の read_rec() で読んだレコードの型を得る．
  GdsRtype
  cur_rtype() const;

  /// @brief 直前の read_rec() で読んだレコードのデータ型を得る．
  GdsDtype
  cur_dtype() const;

  /// @brief 直前お read_rec() で読んだレコードのデータサイズを得る．
  ymuint32
  cur_dsize() const;

  /// @brief 直前の read_rec() で読んだレコードのデータを得る．
  ymuint8*
  cur_data() const;

  /// @brief 直前の read_rec() で読んだレコードのデータを2バイト整数に変換する．
  /// @param[in] pos 位置 (2バイト分で1つ)
  ymint16
  conv_2byte_int(ymuint pos) const;

  /// @brief 直前の read_rec() で読んだレコードのデータを4バイト整数に変換する．
  /// @param[in] pos 位置 (4バイト分で1つ)
  ymint32
  conv_4byte_int(ymuint pos) const;

  /// @brief 直前の read_rec() で読んだレコードのデータを4バイト浮動小数点数に変換する．
  /// @param[in] pos 位置 (4バイト分で1つ)
  double
  conv_4byte_real(ymuint pos) const;

  /// @brief 直前の read_rec() で読んだレコードのデータを8バイト浮動小数点数に変換する．
  /// @param[in] pos 位置 (8バイト分で1つ)
  double
  conv_8byte_real(ymuint pos) const;

  /// @brief 現在のファイル上の位置を返す．
  ymuint32
  cur_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2バイト読んで符号なし整数に変換する．
  /// @param[out] val 読み込んだ値を格納する変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_2byte_uint(ymuint& val);

  /// @brief 1バイト読み込む．
  /// @param[out] val 読み込んだ値を格納する変数
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_1byte(ymuint& val);

  /// @brief データブロックを読み込む．
  /// @param[in] dsize 読み込むサイズ
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  /// @note 結果は mDataBuff に格納される．
  bool
  read_block(ymuint dsize);

  /// @brief データバッファを確保する．
  void
  alloc_buff(ymuint32 req_size);

  /// @brief 低レベルの読み込みを行う．
  bool
  raw_read();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のファイル記述子
  int mFd;

  // ファイルバッファ
  ymuint8 mBuff[4096];

  // mBuff の読み出し位置
  ymuint16 mReadPos;

  // mBuff の終端位置
  ymuint16 mEndPos;

  // 入力ストリームから読み込んだバイト数
  ymuint32 mCurPos;

  // 現在のレコードのオフセット
  ymuint32 mCurOffset;

  // 現在のレコードのサイズ
  ymuint32 mCurSize;

  // 現在のレコードの型
  GdsRtype mCurRtype;

  // 現在のレコードのデータ型
  GdsDtype mCurDtype;

  // 現在のレコードのデータバッファ
  ymuint8* mDataBuff;

  // mCurData のサイズ
  ymuint32 mBuffSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前の read_rec() で読んだレコードのオフセットを得る．
inline
ymuint32
GdsScanner::cur_offset() const
{
  return mCurOffset;
}

// @brief 直前の read_rec() で読んだレコードのサイズを得る．
inline
ymuint32
GdsScanner::cur_size() const
{
  return mCurSize;
}

// @brief 直前の read_rec() で読んだレコードの型を得る．
inline
GdsRtype
GdsScanner::cur_rtype() const
{
  return mCurRtype;
}

// @brief 直前の read_rec() で読んだレコードのデータ型を得る．
inline
GdsDtype
GdsScanner::cur_dtype() const
{
  return mCurDtype;
}

// @brief 直前お read_rec() で読んだレコードのデータサイズを得る．
inline
ymuint32
GdsScanner::cur_dsize() const
{
  return cur_size() - 4;
}

// @brief 現在のファイル上の位置を返す．
inline
ymuint32
GdsScanner::cur_pos() const
{
  return mCurPos;
}

// @brief 直前の read_rec() で読んだレコードのデータを得る．
inline
ymuint8*
GdsScanner::cur_data() const
{
  return mDataBuff;
}

END_NAMESPACE_YM_GDS

#endif // GDS_GDSSCANNER_H
