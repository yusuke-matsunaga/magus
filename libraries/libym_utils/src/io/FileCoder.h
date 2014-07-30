#ifndef FILECODER_H
#define FILECODER_H

/// @file FileCoder.h
/// @brief FileCoder のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmUtils/CodecType.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @calss FileCoder FileCoder.h "FileCoder.h"
/// @brief ファイルの圧縮を行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class FileCoder
{
public:

  /// @brief FileCoder のインスタンスを作る関数
  /// @param[in] type FileCoder の型
  /// @return 作成したインスタンスを返す．
  /// @note type が不正な値の場合は NULL を返す．
  static
  FileCoder*
  new_coder(CodecType type);

  /// @brief thru coder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileCoder*
  new_thru_coder();

  /// @brief compress coder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileCoder*
  new_z_coder();

  /// @brief gzip coder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileCoder*
  new_gzip_coder();

  /// @brief bzip2 coder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileCoder*
  new_bzip2_coder();

  /// @brief xz coder のインスタンスを作る関数
  /// @return 作成したインスタンスを返す．
  static
  FileCoder*
  new_xz_coder();

  /// @brief デストラクタ
  virtual
  ~FileCoder() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  virtual
  bool
  open(const char* filename,
       mode_t mode = 0666,
       ymuint level = 0) = 0;

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] mode ファイル作成用のモード
  /// @param[in] level 圧縮レベル
  /// @retval true 成功した
  /// @retval false 失敗した．
  ///
  /// 失敗する理由は以下の通り
  ///  - ファイルに対する書き込み許可がない．
  bool
  open(const string& filename,
       mode_t mode = 0666,
       ymuint level = 0);

  /// @brief ファイルを閉じる．
  virtual
  void
  close() = 0;

  /// @brief 書き込み可能の時に true を返す．
  virtual
  bool
  is_ready() const = 0;

  /// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
  /// @param[in] rbuff 圧縮するデータを格納するバッファ
  /// @param[in] num 書き込むデータ数(バイト)
  /// @return 実際に書き込んだバイト数を返す．
  /// @note エラーが起こったら -1 を返す．
  virtual
  ssize_t
  write(const ymuint8* rbuff,
	ymuint64 num) = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] level 圧縮レベル
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルに対する書き込み許可がない．
inline
bool
FileCoder::open(const string& filename,
		mode_t mode,
		ymuint level)
{
  return open(filename.c_str(), level);
}

END_NAMESPACE_YM

#endif // FILECODER_H
