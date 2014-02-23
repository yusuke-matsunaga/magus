
/// @file FileIDO.cc
/// @brief FileIDO の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/FileIDO.h"
#include "FileDecoder.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] codec_type Decoder の種類
FileIDO::FileIDO(tCodecType codec_type)
{
  mDecoder = FileDecoder::new_decoder(codec_type);
}

// @brief デストラクタ
FileIDO::~FileIDO()
{
  close();
  delete mDecoder;
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
bool
FileIDO::open(const char* filename,
	      const FileLoc& parent_loc)
{
  if ( mDecoder == NULL ) {
    return false;
  }

  bool stat = mDecoder->open(filename);
  if ( stat ) {
    mFileInfo = FileInfo(filename, parent_loc);
  }
  return stat;
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
bool
FileIDO::open(const string& filename,
	      const FileLoc& parent_loc)
{
  return open(filename.c_str(), parent_loc);
}

// @brief ファイルを閉じる．
// @note 以降の読み出しは行われない．
void
FileIDO::close()
{
  if ( mDecoder != NULL ) {
    mDecoder->close();
  }
}

// @brief 読み出し可能なら true を返す．
bool
FileIDO::is_ready() const
{
  if ( mDecoder == NULL ) {
    return false;
  }

  return mDecoder->is_ready();
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
FileIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
FileIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief データを読み込む．
// @param[in] buff 読み込んだデータを格納する領域の先頭アドレス．
// @param[in] n 読み込むデータサイズ
// @return 実際に読み込んだ量を返す．
ssize_t
FileIDO::read(ymuint8* buff,
	      ymuint64 n)
{
  if ( mDecoder == NULL ) {
    return -1;
  }

  return mDecoder->read(buff, n);
}

END_NAMESPACE_YM
