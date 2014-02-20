
/// @file FileInfoMgr.cc
/// @brief FileInfoMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FileInfoMgr.h"
#include "utils/FileLoc.h"



BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FileInfoMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FileInfoMgr::FileInfoMgr()
{
}

// @brief デストラクタ
// @note このクラスで生成したデータはすべて削除される．
FileInfoMgr::~FileInfoMgr()
{
}

// @brief ファイル名を返す．
// @param[in] id _FileInfo の ID 番号
string
FileInfoMgr::filename(ymuint id)
{
  assert_cond( id < mFiArray.size(), __FILE__, __LINE__);
  const _FileInfo& fi = mFiArray[id];
  return fi.mFileName;
}

// @brief インクルード元のファイル位置を返す．
// @param[in] id _FileInfo の ID 番号
// @note インクルードされていないファイルの場合には無効なデータが返される．
FileLoc
FileInfoMgr::parent_loc(ymuint id)
{
  assert_cond( id < mFiArray.size(), __FILE__, __LINE__);
  const _FileInfo& fi = mFiArray[id];
  return fi.mParentLoc;
}

// @brief 新しい _FileInfo を生成する．
// @param[in] filename ファイル名
// @return 生成された _FileInfo の ID 番号
ymuint
FileInfoMgr::new_file_info(const char* filename)
{
  ymuint id = mFiArray.size();
  mFiArray.push_back(_FileInfo(filename));
  return id;
}

// @brief 新しい _FileInfo を生成する．
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @return 生成された _FileInfo の ID 番号
ymuint
FileInfoMgr::new_file_info(const char* filename,
			   const FileLoc& parent_loc)
{
  ymuint id = mFiArray.size();
  mFiArray.push_back(_FileInfo(filename, parent_loc));
  return id;
}


//////////////////////////////////////////////////////////////////////
// クラス FileInfoMgr::_FileInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
FileInfoMgr::_FileInfo::_FileInfo()
{
}

// @brief 親のない場合のコンストラクタ
// @param[in] filename ファイル名
inline
FileInfoMgr::_FileInfo::_FileInfo(const char* filename) :
  mFileName(filename)
{
}

// @brief 親のある場合(インクルードされている場合)のコンストラクタ
// @param[in] filename ファイル名
// @param[in] file_loc インクルード元のファイル位置
inline
FileInfoMgr::_FileInfo::_FileInfo(const char* filename,
				  const FileLoc& file_loc) :
  mFileName(filename),
  mParentLoc(file_loc)
{
}

END_NAMESPACE_YM
