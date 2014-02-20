#ifndef FILEINFOMGR_H
#define FILEINFOMGR_H

/// @file FileInfoMgr.h
/// @brief FileInfoMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "utils/FileLoc.h"
#include "utils/StrBuff.h"


BEGIN_NAMESPACE_YM

class FileInfo;
class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class FileInfoMgr FileInfoMgr.h "FileInfoMgr.h"
/// @brief FileInfoMgr の実際の処理を行うクラス
/// @sa FileInfo FileLoc FileRegion
//////////////////////////////////////////////////////////////////////
class FileInfoMgr
{
public:

  /// @brief コンストラクタ
  FileInfoMgr();

  /// @brief デストラクタ
  /// @note このクラスで生成したデータはすべて削除される．
  ~FileInfoMgr();


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

END_NAMESPACE_YM

#endif // FILEINFOMGR_H
