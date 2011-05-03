#ifndef YM_UTILS_FILEINFOMGR_H
#define YM_UTILS_FILEINFOMGR_H

/// @file ym_utils/FinleInfoMgr.h
/// @brief FinleInfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FinleInfoMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/FileInfo.h"


BEGIN_NAMESPACE_YM

class FileLoc;

//////////////////////////////////////////////////////////////////////
/// @class FileInfoMgr FileInfoMgr.h "ym_utils/FileInfoMgr.h"
/// @brief FileInfo を生成する管理クラス
/// @note このクラスは static menber function しか持たない．
/// @sa FileInfo FileLoc FileRegion
//////////////////////////////////////////////////////////////////////
class FileInfoMgr
{
private:

  FileInfoMgr();

public:

  /// @brief 新しい FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @return 生成された FileInfo
  static
  FileInfo
  new_file_info(const char* filename);

  /// @brief 新しい FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @return 生成された FileInfo
  static
  FileInfo
  new_file_info(const string& filename);

  /// @brief 新しい FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @param[in] parent_loc インクルード元の親ファイルの情報
  /// @return 生成された FileInfo
  static
  FileInfo
  new_file_info(const char* filename,
		const FileLoc& parent_loc);

  /// @brief 新しい FileInfo を生成する．
  /// @param[in] filename ファイル名
  /// @param[in] parent_file_loc インクルード元の親ファイルの情報
  /// @return 生成された FileInfo
  static
  FileInfo
  new_file_info(const string& filename,
		const FileLoc& parent_loc);

  /// @brief 生成したすべての FileInfo を削除する．
  static
  void
  clear();

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 新しい FileInfo を生成する．
// @param[in] filename ファイル名
// @return 生成された FileInfo
inline
FileInfo
FileInfoMgr::new_file_info(const string& filename)
{
  return new_file_info(filename.c_str());
}

// @brief 新しい FileInfo を生成する．
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
// @return 生成された FileInfo
inline
FileInfo
FileInfoMgr::new_file_info(const string& filename,
			   const FileLoc& parent_loc)
{
  return new_file_info(filename.c_str(), parent_loc);
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILEINFOMGR_H
