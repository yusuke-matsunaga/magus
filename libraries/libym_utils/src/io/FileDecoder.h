#ifndef FILEDECODER_H
#define FILEDECODER_H

/// @file FileDecoder.h
/// @brief FileDecoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/CodecType.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileDecoder FileDecoder.h "FileDecoder.h"
/// @brief ファイルの伸長を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class FileDecoder
{
public:

  /// @brief FileDecoder のインスタンスを作る関数
  /// @param[in] type FileDecoder の型
  /// @return 作成したインスタンスを返す．
  /// @note type が不正な値の場合は NULL を返す．
  static
  FileDecoder*
  new_decoder(CodecType type);

  /// @brief thru decoder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileDecoder*
  new_thru_decoder();

  /// @brief compress decoder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileDecoder*
  new_z_decoder();

  /// @brief gzip decoder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileDecoder*
  new_gzip_decoder();

  /// @brief bzip2 decoder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileDecoder*
  new_bzip2_decoder();

  /// @brief lzma(xz) decoder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileDecoder*
  new_xz_decoder();

  /// @brief デストラクタ
  virtual
  ~FileDecoder() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  virtual
  bool
  open(const char* filename) = 0;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルが存在しない．
  ///  - ファイルに対する読み出し許可がない．
  ///  - ファイルの形式が異なる．
  bool
  open(const string& filename);

  /// @brief ファイルを閉じる．
  virtual
  void
  close() = 0;

  /// @brief 読み出せるデータがある時に true を返す．
  virtual
  bool
  is_ready() const = 0;

  /// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
  /// @param[in] rbuff 展開したデータを格納するバッファ
  /// @param[in] num 読み出すデータ数(バイト)
  /// @return 実際に読み出したバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  ymint64
  read(ymuint8* rbuff,
       ymuint64 num) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイルを開く
// @param[in] filename ファイル名
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルが存在しない．
//  - ファイルに対する読み出し許可がない．
//  - ファイルの形式が異なる．
inline
bool
FileDecoder::open(const string& filename)
{
  return open(filename.c_str());
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILEDECODER_H
