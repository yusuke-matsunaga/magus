
/// @file GzIDO.cc
/// @brief GzIDO の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/GzIDO.h"
#include "ym_utils/GzODO.h"

#include "GzCoder.h"
#include "GzDecoder.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス GzIDO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @note 意味的にはコンストラクタ + open()
GzIDO::GzIDO(const char* filename,
	     const FileLoc& parent_loc)
{
  mDecoder = new GzDecoder();
  open(filename, parent_loc);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @note 意味的にはコンストラクタ + open()
GzIDO::GzIDO(const string& filename,
	     const FileLoc& parent_loc)
{
  mDecoder = new GzDecoder();
  open(filename.c_str(), parent_loc);
}

// @brief デストラクタ
GzIDO::~GzIDO()
{
  close();
  delete mDecoder;
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzIDO::open(const char* filename,
	    const FileLoc& parent_loc)
{
  bool stat = mDecoder->open(filename);
  if ( stat ) {
    mFileInfo = FileInfo(filename, parent_loc);
  }
  return stat;
}

// @brief ファイルをクローズする．
void
GzIDO::close()
{
  mDecoder->close();
}

// @brief 読み出し可能なら true を返す．
GzIDO::operator bool() const
{
  return mDecoder->is_ready();
}

// @brief オープン中のファイル情報を得る．
const FileInfo&
GzIDO::file_info() const
{
  return mFileInfo;
}

// @brief 現在のファイル情報を書き換える．
// @param[in] new_info 新しいファイル情報
// @note プリプロセッサのプラグマなどで用いることを想定している．
// @note 通常は使わないこと．
void
GzIDO::set_file_info(const FileInfo& file_info)
{
  mFileInfo = file_info;
}

// @brief 圧縮されたデータを伸長してバッファに書き込む．
// @param[in] buff 伸長したデータを格納するバッファ
// @param[in] size バッファの空きサイズ
ssize_t
GzIDO::read(ymuint8* buff,
	    size_t size)
{
  return mDecoder->read(buff, size);
}


//////////////////////////////////////////////////////////////////////
// クラス GzODO
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
GzODO::GzODO()
{
  mCoder = new GzCoder();
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
GzODO::GzODO(const char* filename,
	     ymuint level)
{
  mCoder = new GzCoder();
  open(filename, level);
}

// @brief コンストラクタ
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
GzODO::GzODO(const string& filename,
	     ymuint level)
{
  mCoder = new GzCoder();
  open(filename, level);
}

// @brief デストラクタ
GzODO::~GzODO()
{
  delete mCoder;
}

// @brief 書き込み可能なら true を返す．
GzODO::operator bool() const
{
  return mCoder->is_ready();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzODO::open(const char* filename,
	    ymuint level)
{
  return mCoder->open(filename, level);
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] level 圧縮レベル (0 でデフォルト値を用いる)
// @retval true オープンが成功した．
// @retval false オープンが失敗した．
bool
GzODO::open(const string& filename,
	    ymuint level)
{
  return open(filename.c_str(), level);
}

// @brief データを書き出す．
// @param[in] buff データを収めた領域のアドレス
// @param[in] n データサイズ
// @return 実際に書き出した量を返す．
ssize_t
GzODO::write(const ymuint8* buff,
	     ymuint64 n)
{
  return mCoder->write(buff, n);
}

END_NONAMESPACE
