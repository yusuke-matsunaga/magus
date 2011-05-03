#ifndef YM_UTILS_FILELOC_H
#define YM_UTILS_FILELOC_H

/// @file ym_utils/FileLoc.h
/// @brief FileLocのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileLoc.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_utils/FileInfo.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileLoc FileLoc.h "ym_utils/FileLoc.h"
/// @ingroup YmUtils
/// @brief ファイル位置を表すクラス
///
/// 基本的には
/// - ファイル情報
/// - 行番号
/// - コラム位置
/// の情報を持つ
/// @sa FileInfo FileInfoMgr FileRegion
//////////////////////////////////////////////////////////////////////
class FileLoc
{
  friend class FileRegion;

public:

  /// @brief 空のコンストラクタ
  /// @note 無効なデータを持つ
  FileLoc();

  /// @brief 内容を指定するコンストラクタ
  /// @param[in] file_info ファイル情報
  /// @param[in] line 行番号
  /// @param[in] column コラム番号
  FileLoc(FileInfo file_info,
	  ymuint line,
	  ymuint column);

  /// @brief デストラクタ
  ~FileLoc();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データの妥当性のチェック
  /// @retval true 意味のある値を持っている時
  /// @retval false 無効なデータの時
  bool
  is_valid() const;

  /// @brief ファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  file_info() const;

  /// @brief ファイル名を返す．
  string
  filename() const;

  /// @brief インクルード元のファイル位置を返す．
  /// @note インクルードされていないファイルの場合には無効なデータが返される．
  FileLoc
  parent_loc() const;

  /// @brief インクルード元のファイル位置の情報をすべてリストに積む．
  /// @param[out] loc_list ファイルの位置情報のリスト
  /// @note トップレベルのファイルが先頭になる．
  void
  parent_loc_list(list<FileLoc>& loc_list) const;

  /// @brief 行番号の取得
  /// @return 行番号
  ymuint
  line() const;

  /// @brief コラム位置の取得
  /// @return コラム位置
  ymuint
  column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル情報
  FileInfo mFileInfo;

  // 行番号とコラム位置
  // 上位 20 ビットが行番号
  // 下位 12 ビットがコラム位置
  ymuint32 mLineColumn;

};


//////////////////////////////////////////////////////////////////////
/// @name FileLoc の出力関数
/// @{

/// @relates FileLoc
/// @brief FileLoc を表示するための関数
/// @param[in] s 出力ストリーム
/// @param[in] file_loc ファイル位置の情報
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileLoc& file_loc);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
// 無効なデータを持つ
inline
FileLoc::FileLoc() :
  mLineColumn(0)
{
  // FileInfo のデフォルトコンストラクタは無効なデータで初期化する．
}

// @brief 内容を指定するコンストラクタ
// @param[in] file_info ファイル情報
// @param[in] line 行番号
// @param[in] column コラム番号
inline
FileLoc::FileLoc(FileInfo file_info,
		 ymuint line,
		 ymuint column) :
  mFileInfo(file_info),
  mLineColumn((line << 12) | (column & 0xfff))
{
}

// @brief デストラクタ
inline
FileLoc::~FileLoc()
{
}

// @brief データの妥当性のチェック
// @retval true 意味のある値を持っている時
// @retval false 無効なデータの時
inline
bool
FileLoc::is_valid() const
{
  return mFileInfo.is_valid();
}

// @brief ファイル記述子の取得
// @return ファイル記述子を返す．
inline
FileInfo
FileLoc::file_info() const
{
  return mFileInfo;
}

// @brief ファイル名を返す．
inline
string
FileLoc::filename() const
{
  return file_info().filename();
}

// @brief インクルード元のファイル位置を返す．
// @note インクルードされていないファイルの場合には無効なデータが返される．
inline
FileLoc
FileLoc::parent_loc() const
{
  return file_info().parent_loc();
}

// @brief 行番号の取得
// @return 行番号
inline
ymuint
FileLoc::line() const
{
  return mLineColumn >> 12;
}

// @brief コラム位置の取得
// @return コラム位置
inline
ymuint
FileLoc::column() const
{
  return mLineColumn & 0xfff;
}

END_NAMESPACE_YM

#endif // YM_UTILS_FILELOC_H
