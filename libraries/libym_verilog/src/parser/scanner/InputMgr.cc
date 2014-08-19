
/// @file InputMgr.cc
/// @brief InputMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "InputMgr.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/FileInfo.h"


// ファイル末尾に改行がなくても warning としない時に 1
#define ALLOW_EOF_WITHOUT_NL 1


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// InputMgr のパブリックなメンバ関数
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lex 親の Lex
InputMgr::InputMgr(RawLex& lex) :
  mLex(lex),
  mCurFile(NULL)
{
}

// @brief デストラクタ
InputMgr::~InputMgr()
{
  clear();
}

// @brief 初期状態に戻す．
// 読み込みの途中でこれを呼ぶと大変なことになる．
// 普通は正常に読み終わったあとか, エラーの後で呼ぶ．
void
InputMgr::clear()
{
  delete_file(mCurFile);
  mCurFile = NULL;
  for (vector<InputFile*>::iterator p = mFileStack.begin();
       p != mFileStack.end(); ++ p) {
    delete_file(*p);
  }
  mFileStack.clear();
}


//////////////////////////////////////////////////////////////////////
// サーチパスとファイル関係
//////////////////////////////////////////////////////////////////////

// @brief サーチパスリストを設定する．
// @param[in] searchpath セットするサーチパス
void
InputMgr::set_searchpath(const SearchPathList& searchpath)
{
  mSearchPathList = searchpath;
}

// @brief 設定されているサーチパスを考慮して filename を探す．
// @param[in] filename ファイル名
// @return 実際の絶対パス
PathName
InputMgr::search_file(const string& filename)
{
  // filename が存在して読めなければならない．
  return mSearchPathList.search(PathName(filename));
}

// @brief ファイルをオープンする．
// @param[in] filename ファイル名
// @param[in] parent_file インクルード元のファイル情報
// @retval true オープンに成功した．
// @retval false ファイルが開けなかった
// 設定されているサーチパスを考慮して filename を探す．
bool
InputMgr::open_file(const string& filename,
		    const FileLoc& parent_file)
{
  PathName pathname = search_file(filename);
  if ( !pathname.is_valid() ) {
    return false;
  }
  // 本当のパス名
  string realname = pathname.str();

  InputFile* new_file = new InputFile(mLex);
  if ( !new_file->open(realname, parent_file) ) {
    return false;
  }

  if ( mCurFile ) {
    mFileStack.push_back(mCurFile);
  }
  mCurFile = new_file;

  return true;
}

// @brief 現在のファイル位置を強制的に書き換える．
// @param[in] new_filename 新しいファイル名
// @param[in] line     新しい行番号
// @param[in] level
//           - 0 インクルード関係のレベル変化無し
//           - 1 新しいファイルはインクルードされたファイル
//           - 2 新しいファイルはインクルードもとのファイル
void
InputMgr::set_file_loc(const char* new_filename,
		       ymuint line,
		       ymuint level)
{
  if ( mCurFile == NULL ) {
    // ないと思うけど念のため
    return;
  }

  FileInfo cur_fi = mCurFile->file_info();
  switch ( level ) {
  case 0: // レベルの変化無し
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      FileLoc flp = cur_fi.parent_loc();
      cur_fi = FileInfo(new_filename, flp);
    }
    break;

  case 1: // 新しいインクルードファイル．
    {
      FileLoc parent_loc(cur_fi, cur_file()->cur_loc().end_line(), 1);
      cur_fi = FileInfo(new_filename, parent_loc);
    }
    break;

  case 2: // インクルードの終り
    cur_fi = cur_fi.parent_loc().file_info();
    if ( cur_fi.filename() != new_filename ) {
      // 新しい FileInfo を作る．
      FileLoc flp = cur_fi.parent_loc();
      cur_fi = FileInfo(new_filename, flp);
    }
    break;
  }
  mCurFile->set_file_info(cur_fi);
}

// @brief 現在のファイルを返す．
InputFile*
InputMgr::cur_file() const
{
  return mCurFile;
}

// @brief 現在のファイル名を返す．
string
InputMgr::cur_filename() const
{
  return mCurFile->file_info().filename();
}

// @brief 現在の InputFile が EOF を返したときの処理
// @return 処理を続けられる時 true を返す．
bool
InputMgr::wrap_up()
{
  for ( ; ; ) {
    delete_file(mCurFile);
    if ( mFileStack.empty() ) {
      // もうファイルが残っていない．
      mCurFile = NULL;
      return false;
    }

    mCurFile = mFileStack.back();
    mFileStack.pop_back();

    int c = mCurFile->peek();
    if ( c != EOF ) {
      return true;
    }
  }
}

// @brief ファイルのオープン済チェック
// @param[in] name チェックするファイル名
// @retval true name という名のファイルがオープンされている．
// @retval false name というなのファイルはオープンされていない．
bool
InputMgr::check_file(const char* name) const
{
  if ( cur_filename() == name ) {
    return true;
  }
  for (vector<InputFile*>::const_iterator p = mFileStack.begin();
       p != mFileStack.end(); ++ p) {
    if ( (*p)->file_info().filename() == name ) {
      return true;
    }
  }
  return false;
}

// @brief InputFile を削除する．
void
InputMgr::delete_file(InputFile* file)
{
  delete file;
}

END_NAMESPACE_YM_VERILOG
