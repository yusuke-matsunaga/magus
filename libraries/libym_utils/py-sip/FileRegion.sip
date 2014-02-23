
/// @file utils/FileRegion.h
/// @brief FileRegion のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

class FileRegion {

%TypeHeaderCode
#include "utils/FileRegion.h"
using nsYm::FileRegion;
%End

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
	     unsigned int start_line,
	     unsigned int start_column,
	     unsigned int end_line,
	     unsigned int end_column);

  /// @brief 内容を指定したコンストラクタ
  /// @param[in] start_file_info 先頭のファイル情報
  /// @param[in] start_line 先頭の行番号
  /// @param[in] start_column 先頭のコラム番号
  /// @param[in] end_file_info 末尾のファイル情報
  /// @param[in] end_line 末尾の行番号
  /// @param[in] end_column 末尾のコラム番号
  FileRegion(FileInfo start_file_info,
	     unsigned int start_line,
	     unsigned int start_column,
	     FileInfo end_file_info,
	     unsigned int end_line,
	     unsigned int end_column);

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
  unsigned int
  start_line() const;

  /// @brief 先頭のコラム位置の取得
  /// @return コラム位置
  unsigned int
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
  unsigned int
  end_line() const;

  /// @brief 末尾のコラム位置の取得
  /// @return コラム位置
  unsigned int
  end_column() const;

};