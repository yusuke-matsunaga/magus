
/// @file DirCmd.cc
/// @brief DirCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DirCmd.h"
#include "YmshImpl.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス PwdCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PwdCmd::PwdCmd() :
  YmshCmd("pwd")
{
}

// @brief デストラクタ
PwdCmd::~PwdCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
PwdCmd::exec(const vector<string>& argv)
{
  if ( argv.size() != 1 ) {
    // USAGE を出力する．
  }
  else {
    char* buff = getcwd(NULL, 0);
    cout << buff << endl;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス CdCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CdCmd::CdCmd() :
  YmshCmd("cd")
{
}

// @brief デストラクタ
CdCmd::~CdCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
CdCmd::exec(const vector<string>& argv)
{
  if ( argv.size() != 2 ) {
    // USAGE を出力する．
  }
  else {
    string dir = argv[1];
    int code = chdir(dir.c_str());
    return code;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス MkdirCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MkdirCmd::MkdirCmd() :
  YmshCmd("mkdir")
{
}

// @brief デストラクタ
MkdirCmd::~MkdirCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
MkdirCmd::exec(const vector<string>& argv)
{
  if ( argv.size() != 2 ) {
    // USAGE を出力する．
  }
  else {
    string dir = argv[1];
    int code = mkdir(dir.c_str(), 0777);
    return code;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス RmdirCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RmdirCmd::RmdirCmd() :
  YmshCmd("rmdir")
{
}

// @brief デストラクタ
RmdirCmd::~RmdirCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
RmdirCmd::exec(const vector<string>& argv)
{
  if ( argv.size() != 2 ) {
    // USAGE を出力する．
  }
  else {
    string dir = argv[1];
    int code = rmdir(dir.c_str());
    return code;
  }
  return 0;
}

END_NAMESPACE_YM_YMSH
