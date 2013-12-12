#ifndef GZIDO_H
#define GZIDO_H

/// @file GzIDO.h
/// @brief GzIDO のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"
#include "ym_utils/zstream.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/FileInfo.h"


BEGIN_NAMESPACE_YM

class FileBuff;

//////////////////////////////////////////////////////////////////////
/// @class GzIDO GzIDO.h "GzIDO.h"
/// @brief gzip'ed ファイルを読み込む IDO の継承クラスクラス
//////////////////////////////////////////////////////////////////////
class GzIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 意味的にはコンストラクタ + open()
  explicit
  GzIDO(const char* filename,
	const FileLoc& parent_loc = FileLoc());

  /// @brief コンストラクタ
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 意味的にはコンストラクタ + open()
  explicit
  GzIDO(const string& filename,
	const FileLoc& parent_loc = FileLoc());

  /// @brief デストラクタ
  ~GzIDO();


public:
  //////////////////////////////////////////////////////////////////////
  // IDO の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 読み出し可能なら true を返す．
  virtual
  operator bool() const;

  /// @brief オープン中のファイル情報を得る．
  virtual
  const FileInfo&
  file_info() const;

  /// @brief 現在のファイル情報を書き換える．
  /// @param[in] file_info 新しいファイル情報
  /// @note プリプロセッサのプラグマなどで用いることを想定している．
  /// @note 通常は使わないこと．
  virtual
  void
  set_file_info(const FileInfo& file_info);

  /// @brief 圧縮されたデータを伸長してバッファに書き込む．
  /// @param[in] buff 伸長したデータを格納するバッファ
  /// @param[in] size バッファの空きサイズ
  /// @return バッファに書き込まれたデータサイズを返す．
  /// @note エラーが起こった場合には負の値が返される．
  virtual
  ssize_t
  read(ymuint8* buff,
       size_t size);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルをオープンする．
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @retval true オープンが成功した．
  /// @retval false オープンが失敗した．
  bool
  open(const char* filename,
       const FileLoc& parent_loc = FileLoc());

  /// @brief ファイルをクローズする．
  void
  close();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  FileInfo mFileInfo;

  // 入力ファイル用のバッファ
  FileBuff* mBuff;

  // zstream
  zstream mZS;

  // CRC コード
  ymuint32 mCRC;

  // 出力バッファに書き込まれたサイズ
  ymuint32 mOutSize;

};

END_NONAMESPACE

#endif // GZIDO_H
