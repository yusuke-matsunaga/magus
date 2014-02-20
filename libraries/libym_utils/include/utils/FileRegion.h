#ifndef UTILS_FILEREGION_H
#define UTILS_FILEREGION_H

/// @file utils/FileRegion.h
/// @brief FileRegion のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "utils/FileInfo.h"
#include "utils/FileLoc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FileRegion FileRegion.h "utils/FileRegion.h"
/// @ingroup YmUtils
/// @brief ファイル上の領域を表すクラス
/// @sa FileLoc FileInfo FileInfoMgr
//////////////////////////////////////////////////////////////////////
class FileRegion
{
public:

  /// @brief 空のデフォルトコンストラクタ
  /// @note 結果は invalid な情報を持つオブジェクトとなる．
  FileRegion();

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] file_info ファイル情報
  /// @param[in] start_line 先頭の行番号
  /// @param[in] start_column 先頭のコラム番号
  /// @param[in] end_line 末尾の行番号
  /// @param[in] end_column 末尾のコラム番号
  FileRegion(FileInfo file_info,
	     ymuint start_line,
	     ymuint start_column,
	     ymuint end_line,
	     ymuint end_column);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_info 先頭のファイル情報
  /// @param[in] start_line 先頭の行番号
  /// @param[in] start_column 先頭のコラム番号
  /// @param[in] end_file_info 末尾のファイル情報
  /// @param[in] end_line 末尾の行番号
  /// @param[in] end_column 末尾のコラム番号
  FileRegion(FileInfo start_file_info,
	     ymuint start_line,
	     ymuint start_column,
	     FileInfo end_file_info,
	     ymuint end_line,
	     ymuint end_column);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] file_loc ファイル上の位置
  FileRegion(const FileLoc& file_loc);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_loc 先頭のファイル上の位置
  /// @param[in] end_file_loc 末尾ののファイル上の位置
  FileRegion(const FileLoc& start_file_loc,
	     const FileLoc& end_file_loc);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_region 先頭のファイル上の位置
  /// @param[in] end_file_region 末尾ののファイル上の位置
  FileRegion(const FileRegion& start_file_region,
	     const FileRegion& end_file_region);

  /// @brief デストラクタ
  ~FileRegion();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データの妥当性のチェック
  /// @return 意味のある値を持っているとき true を返す．
  bool
  is_valid() const;

  /// @brief 先頭のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  start_loc() const;

  /// @brief 先頭のファイル情報の取得
  /// @return ファイル情報を返す．
  FileInfo
  start_file_info() const;

  /// @brief 先頭の行番号の取得
  /// @return 行番号
  ymuint
  start_line() const;

  /// @brief 先頭のコラム位置の取得
  /// @return コラム位置
  ymuint
  start_column() const;

  /// @brief 末尾のファイル位置の取得
  /// @return ファイル位置
  FileLoc
  end_loc() const;

  /// @brief 末尾のファイル情報の取得
  FileInfo
  end_file_info() const;

  /// @brief 末尾の行番号の取得
  /// @return 行番号
  ymuint
  end_line() const;

  /// @brief 末尾のコラム位置の取得
  /// @return コラム位置
  ymuint
  end_column() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 先頭のファイル情報
  FileInfo mStartFileInfo;

  // 末尾のファイル情報
  FileInfo mEndFileInfo;

  // 先頭の行番号とコラム位置
  ymuint32 mStartLineColumn;

  // 末尾の行番号とコラム位置
  ymuint32 mEndLineColumn;

};


//////////////////////////////////////////////////////////////////////
/// @name FileRegion の出力関数
/// @{

/// @relates FileRegion
/// @brief FileRegion を表示するための関数
/// @param[in] s 出力ストリーム
/// @param[in] file_region ファイル領域の情報
/// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileRegion& file_region);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のデフォルトコンストラクタ
// @note 結果は invalid な情報を持つオブジェクトとなる．
inline
FileRegion::FileRegion() :
  mStartLineColumn(0U),
  mEndLineColumn(0U)
{
  // FileInfo のデフォルトコンストラクタは無効なIDで初期化する．
}

// @brief 内容を指定したコンストラクタ
// @param[in] file_info ファイル情報
// @param[in] start_line 先頭の行番号
// @param[in] start_column 先頭のコラム番号
// @param[in] end_line 末尾の行番号
// @param[in] end_column 末尾のコラム番号
inline
FileRegion::FileRegion(FileInfo file_info,
		       ymuint start_line,
		       ymuint start_column,
		       ymuint end_line,
		       ymuint end_column) :
  mStartFileInfo(file_info),
  mEndFileInfo(file_info),
  mStartLineColumn((start_line << 12) | (start_column & 0xfff)),
  mEndLineColumn((end_line << 12) | (end_column & 0xfff))
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_info 先頭のファイル情報
// @param[in] start_line 先頭の行番号
// @param[in] start_column 先頭のコラム番号
// @param[in] end_file_info 末尾のファイル情報
// @param[in] end_line 末尾の行番号
// @param[in] end_column 末尾のコラム番号
inline
FileRegion::FileRegion(FileInfo start_file_info,
		       ymuint start_line,
		       ymuint start_column,
		       FileInfo end_file_info,
		       ymuint end_line,
		       ymuint end_column) :
  mStartFileInfo(start_file_info),
  mEndFileInfo(end_file_info),
  mStartLineColumn((start_line << 12) | (start_column & 0xfff)),
  mEndLineColumn((end_line << 12) | (end_column & 0xfff))
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] file_loc ファイル上の位置
inline
FileRegion::FileRegion(const FileLoc& file_loc) :
  mStartFileInfo(file_loc.file_info()),
  mEndFileInfo(file_loc.file_info()),
  mStartLineColumn(file_loc.mLineColumn),
  mEndLineColumn(file_loc.mLineColumn)
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_loc 先頭のファイル上の位置
// @param[in] end_file_loc 末尾ののファイル上の位置
inline
FileRegion::FileRegion(const FileLoc& start_file_loc,
		       const FileLoc& end_file_loc) :
  mStartFileInfo(start_file_loc.file_info()),
  mEndFileInfo(end_file_loc.file_info()),
  mStartLineColumn(start_file_loc.mLineColumn),
  mEndLineColumn(end_file_loc.mLineColumn)
{
}

// @brief 内容を指定したコンストラクタ
// @param[in] start_file_region 先頭のファイル上の位置
// @param[in] end_file_region 末尾ののファイル上の位置
inline
FileRegion::FileRegion(const FileRegion& start_file_region,
		       const FileRegion& end_file_region) :
  mStartFileInfo(start_file_region.mStartFileInfo),
  mEndFileInfo(end_file_region.mEndFileInfo),
  mStartLineColumn(start_file_region.mStartLineColumn),
  mEndLineColumn(end_file_region.mEndLineColumn)
{
}

// @brief デストラクタ
inline
FileRegion::~FileRegion()
{
}

// @brief データの妥当性のチェック
// @return 意味のある値を持っているとき true を返す．
inline
bool
FileRegion::is_valid() const
{
  return mStartFileInfo.is_valid();
}

// @brief 先頭のファイル位置の取得
// @return ファイル位置
inline
FileLoc
FileRegion::start_loc() const
{
  return FileLoc(mStartFileInfo, start_line(), start_column());
}

// @brief 先頭のファイル情報の取得
// @return ファイル情報を返す．
inline
FileInfo
FileRegion::start_file_info() const
{
  return mStartFileInfo;
}

// @brief 先頭の行番号の取得
// @return 行番号
inline
ymuint
FileRegion::start_line() const
{
  return mStartLineColumn >> 12;
}

// @brief 先頭のコラム位置の取得
// @return コラム位置
inline
ymuint
FileRegion::start_column() const
{
  return mStartLineColumn & 0xfff;
}

// @brief 末尾のファイル位置の取得
// @return ファイル位置
inline
FileLoc
FileRegion::end_loc() const
{
  return FileLoc(mEndFileInfo, end_line(), end_column());
}

// @brief 末尾のファイル情報の取得
inline
FileInfo
FileRegion::end_file_info() const
{
  return mEndFileInfo;
}

// @brief 末尾の行番号の取得
// @return 行番号
inline
ymuint
FileRegion::end_line() const
{
  return mEndLineColumn >> 12;
}

// @brief 末尾のコラム位置の取得
// @return コラム位置
inline
ymuint
FileRegion::end_column() const
{
  return mEndLineColumn & 0xfff;
}

END_NAMESPACE_YM

#endif // UTILS_FILEREGION_H
