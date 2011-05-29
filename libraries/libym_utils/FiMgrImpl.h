#ifndef LIBYM_UTILS_FIMGRIMPL_H
#define LIBYM_UTILS_FIMGRIMPL_H

/// @file libym_utils/FiMgrImpl.h
/// @brief FiMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FinleInfoMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM

class FileInfo;
class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class FiMgrImpl FiMgrImpl.h "ym_utils/FiMgrImpl.h"
/// @brief FileInfoMgr の実際の処理を行うクラス
/// @sa FileInfoMgr FileInfo FileLoc FileRegion
//////////////////////////////////////////////////////////////////////
class FiMgrImpl
{
public:

  /// @brief コンストラクタ
  FiMgrImpl();

  /// @brief デストラクタ
  /// @note このクラスで生成したデータはすべて削除される．
  ~FiMgrImpl();


public:

  /// @brief 新しい _FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @return 生成された _FileInfo の ID 番号
  ymuint
  new_file_info(const char* filename);

  /// @brief 新しい _FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @return 生成された _FileInfo の ID 番号
  ymuint
  new_file_info(const char* filename,
		const FileLoc& parent_loc);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル名を返す．
  /// @param[in] id _FileInfo の ID 番号
  string
  filename(ymuint id);

  /// @brief インクルード元のファイル位置を返す．
  /// @param[in] id _FileInfo の ID 番号
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc(ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ファイル情報の実体
  struct _FileInfo
  {
    /// @brief 空のコンストラクタ
    _FileInfo();

    /// @brief 親のない場合のコンストラクタ
    /// @param[in] filename ファイル名
    _FileInfo(const char* filename);

    /// @brief 親のある場合(インクルードされている場合)のコンストラクタ
    /// @param[in] filename ファイル名
    /// @param[in] file_loc インクルード元のファイル位置
    _FileInfo(const char* filename,
	      const FileLoc& file_loc);

    /// @brief ファイル名
    StrBuff mFileName;

    /// @brief インクルード元の情報
    /// インクルードされていない場合には無効な FileInfo が入っている．
    FileLoc mParentLoc;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FileInfo の配列
  vector<_FileInfo> mFiArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス FiMgrImpl
//////////////////////////////////////////////////////////////////////

// @brief ファイル名を返す．
// @param[in] id _FileInfo の ID 番号
inline
string
FiMgrImpl::filename(ymuint id)
{
  assert_cond( id < mFiArray.size(), __FILE__, __LINE__);
  const _FileInfo& fi = mFiArray[id];
  return fi.mFileName;
}

// @brief インクルード元のファイル位置を返す．
// @param[in] id _FileInfo の ID 番号
// @note インクルードされていないファイルの場合には無効なデータが返される．
inline
FileLoc
FiMgrImpl::parent_loc(ymuint id)
{
  assert_cond( id < mFiArray.size(), __FILE__, __LINE__);
  const _FileInfo& fi = mFiArray[id];
  return fi.mParentLoc;
}


//////////////////////////////////////////////////////////////////////
// クラス FiMgrImpl::_FileInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
FiMgrImpl::_FileInfo::_FileInfo()
{
}

// @brief 親のない場合のコンストラクタ
// @param[in] filename ファイル名
inline
FiMgrImpl::_FileInfo::_FileInfo(const char* filename) :
  mFileName(filename)
{
}

// @brief 親のある場合(インクルードされている場合)のコンストラクタ
// @param[in] filename ファイル名
// @param[in] file_loc インクルード元のファイル位置
inline
FiMgrImpl::_FileInfo::_FileInfo(const char* filename,
				  const FileLoc& file_loc) :
  mFileName(filename),
  mParentLoc(file_loc)
{
}


END_NAMESPACE_YM

#endif // YM_UTILS_FILEINFOMGR_H
