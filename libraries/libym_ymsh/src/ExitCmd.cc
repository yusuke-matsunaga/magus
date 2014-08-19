
/// @file ExitCmd.cc
/// @brief ExitCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExitCmd.h"
#include "YmshImpl.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス ExitCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ExitCmd::ExitCmd() :
  YmshCmd("exit")
{
}

// @brief デストラクタ
ExitCmd::~ExitCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
ExitCmd::exec(const vector<string>& argv)
{
  // 引数は無視する．
  ymsh()->exit();
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス UmaskCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UmaskCmd::UmaskCmd() :
  YmshCmd("umask")
{
}

// @brief デストラクタ
UmaskCmd::~UmaskCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
UmaskCmd::exec(const vector<string>& argv)
{
  ymuint n = argv.size();
  if ( n == 1 ) {
    // 値を出力する．
    mode_t old_umask = umask(0777);
    cout << oct << old_umask << dec << endl;
    umask(old_umask);
  }
  else if ( n == 2 ) {
    string str = argv[1];
    bool error = false;
    ymuint mask = 0U;
    if ( str.size() > 3 ) {
      error = true;
    }
    else {
      for (ymuint i = 0; i < str.size(); ++ i) {
	char c = str[i];
	if ( c < '0' || '7' < c ) {
	  error = true;
	  break;
	}
	mask |= (static_cast<ymuint>(c - '0') << (i * 3));
      }
    }
    if ( error ) {
      cout << "umask: Improper mask." << endl;
      return -1;
    }
    else {
      umask(mask);
      return 0;
    }
  }
  else {
    // USAGE を出力する．
  }
  return 0;
}

END_NAMESPACE_YM_YMSH
