#ifndef YM_UTILS_FILEIDO_H
#define YM_UTILS_FILEIDO_H

/// @file ym_utils/FileIDO.h
/// @brief FileIDO のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/IDO.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/CodecType.h"


BEGIN_NAMESPACE_YM

class FileDecoder;

//////////////////////////////////////////////////////////////////////
/// @class FileIDO FileIDO.h "ym_utils/FileIDO.h"
/// @ingroup YmUtils
/// @brief 通常のファイルを用いた IDO の継承クラス
//////////////////////////////////////////////////////////////////////
class FileIDO :
  public IDO
{
public:

  /// @brief コンストラクタ
  /// @param[in] codec_type Decoder の種類
  explicit
  FileIDO(tCodecType codec_type = kCodecThrough);

#if 0
  /// @brief コンストラクタ
  /// @param[in] codec_type Decoder の種類
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 意味的にはコンストラクタ + open()
  FileIDO(tCodecType codec_type,
	  const char* filename,
	  const FileLoc& parent_loc = FileLoc());

  /// @brief コンストラクタ
  /// @param[in] codec_type Decoder の種類
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 意味的にはコンストラクタ + open()
  FileIDO(tCodecType codec_type,
	  const string& filename,
	  const FileLoc& parent_loc = FileLoc());
#endif

  /// @brief デストラクタ
  virtual
  ~FileIDO();


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

  /// @brief データを読み込む．
  /// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
  /// @param[in] n 読み込むデータサイズ
  /// @return 実際に読み込んだ量を返す．
  /// @note エラーが起こったら負の数を返す．
  virtual
  ssize_t
  read(ymuint8* buff,
       ymuint64 n);


public:
  //////////////////////////////////////////////////////////////////////
  // FileIDO の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 他のファイルを開いていたら強制的に close する．
  bool
  open(const char* filename,
       const FileLoc& parent_loc = FileLoc());

  /// @brief ファイルを開く
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @note 他のファイルを開いていたら強制的に close する．
  bool
  open(const string& filename,
       const FileLoc& parent_loc = FileLoc());

  /// @brief ファイルを閉じる．
  /// @note 以降の読み出しは行われない．
  void
  close();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  FileInfo mFileInfo;

  // ファイル復号器
  FileDecoder* mDecoder;

};

END_NAMESPACE_YM

#endif // YM_UTILS_FILEIDO_H
