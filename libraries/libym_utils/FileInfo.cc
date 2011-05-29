
/// @file libym_utils/FileInfo.cc
/// @brief FileInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: FileLoc.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/FileInfo.h"
#include "ym_utils/FileLoc.h"
#include "ym_utils/FileRegion.h"

#include "FileInfoMgr.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

// スタティックオブジェクト
FileInfoMgr gTheMgr;

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス FileInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 無効な ID で初期化される．
FileInfo::FileInfo() :
  mId(0xFFFF)
{
}

// @brief ファイル名を指定したコンストラクタ
// @param[in] filename ファイル名
FileInfo::FileInfo(const char* filename)
{
  mId = gTheMgr.new_file_info(filename);
}

// @brief ファイル名を指定したコンストラクタ
// @param[in] filename ファイル名
FileInfo::FileInfo(const string& filename)
{
  mId = gTheMgr.new_file_info(filename.c_str());
}

// @brief ファイル名とインクルード元の親ファイルの情報
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
FileInfo::FileInfo(const char* filename,
		   const FileLoc& parent_loc)
{
  mId = gTheMgr.new_file_info(filename, parent_loc);
}

// @brief ファイル名とインクルード元の親ファイルの情報
// @param[in] filename ファイル名
// @param[in] parent_loc インクルード元の親ファイルの情報
FileInfo::FileInfo(const string& filename,
		   const FileLoc& parent_loc)
{
  mId = gTheMgr.new_file_info(filename.c_str(), parent_loc);
}

// @brief デストラクタ
FileInfo::~FileInfo()
{
}

// @brief ファイル名を返す．
string
FileInfo::filename() const
{
  return gTheMgr.filename(mId);
}

// @brief インクルード元のファイル位置を返す．
// @note インクルードされていないファイルの場合には無効なデータが返される．
FileLoc
FileInfo::parent_loc() const
{
  return gTheMgr.parent_loc(mId);
}

// @brief インクルード元のファイル位置の情報をすべてリストに積む．
// @param[out] loc_list ファイルの位置情報のリスト
// @note トップレベルのファイルが先頭になる．
void
FileInfo::parent_loc_list(list<FileLoc>& loc_list) const
{
  loc_list.clear();
  for (FileLoc loc = parent_loc(); loc.is_valid(); loc = loc.parent_loc()) {
    loc_list.push_front(loc);
  }
}

// @relates FileInfo
// @brief FileInfo 用のストリーム出力演算子
// @param[in] s 出力ストリーム
// @param[in] file_info ファイル情報
// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   FileInfo file_info)
{
  list<FileLoc> loc_list;
  file_info.parent_loc_list(loc_list);
  for (list<FileLoc>::const_iterator p = loc_list.begin();
       p != loc_list.end(); ++ p) {
    const FileLoc& loc = *p;
    s << "In file included from " << loc.filename()
      << ": line " << loc.line() << ":" << endl;
  }
  s << file_info.filename();

  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス FileLoc
//////////////////////////////////////////////////////////////////////

// @brief インクルード元のファイル位置の情報をすべてリストに積む．
// @param[out] loc_list ファイルの位置情報のリスト
// @note トップレベルのファイルが先頭になる．
void
FileLoc::parent_loc_list(list<FileLoc>& loc_list) const
{
  loc_list.clear();
  for (FileLoc loc = parent_loc(); loc.is_valid(); loc = loc.parent_loc()) {
    loc_list.push_front(loc);
  }
}

// @brief FileLoc を表示するための関数
// @param[in] s 出力ストリーム
// @param[in] file_loc ファイル位置の情報
// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileLoc& file_loc)
{
  if ( file_loc.is_valid() ) {
    s << file_loc.file_info()
      << ": line " << file_loc.line()
      << ", column " << file_loc.column();
  }
  else {
    s << "invalid file_loc";
  }

  return s;
}

// @brief FileRegion を表示するための関数
// @param[in] s 出力ストリーム
// @param[in] file_region ファイル領域の情報
// @return s をそのまま返す
ostream&
operator<<(ostream& s,
	   const FileRegion& file_region)
{
  FileInfo first_fi = file_region.start_file_info();
  FileInfo last_fi = file_region.end_file_info();
  ymuint first_line = file_region.start_line();
  ymuint first_column = file_region.start_column();
  ymuint last_line = file_region.end_line();
  ymuint last_column = file_region.end_column();

  if ( first_fi.is_valid() ) {
    // 意味のあるファイル記述子
    if ( first_fi == last_fi ) {
      // 同じファイル
      s << first_fi;
      if ( first_line == last_line ) {
	// 同じ行番号
	s << ": line " << first_line;
	if ( first_column == last_column ) {
	  // 同じコラム位置
	  s << ", column " << first_column;
	}
	else {
	  // 異なるコラム
	  s << ", column " << first_column
	    << " - " << last_column;
	}
      }
      else {
	// 異なる行
	s << ": line " << first_line
	  << ", column " << first_column
	  << " - line " << last_line
	  << ", column " << last_column;
      }
    }
    else {
      // 異なるファイル
      s << first_fi
	<< ": line " << first_line
	<< ", column " << first_column
	<< " - "
	<< last_fi
	<< ": line " << last_line
	<< ", column " << last_column;
    }
  }
  else {
    s << "---";
  }

  return s;
}

END_NAMESPACE_YM
