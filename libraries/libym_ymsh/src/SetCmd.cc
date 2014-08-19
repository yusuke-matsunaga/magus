
/// @file SetCmd.cc
/// @brief SetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SetCmd.h"
#include "YmshImpl.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス SetCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetCmd::SetCmd() :
  YmshCmd("set")
{
}

// @brief デストラクタ
SetCmd::~SetCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
SetCmd::exec(const vector<string>& argv)
{
  ymuint n = argv.size();
  if ( n == 1 ) {
    // 現在の変数の値を表示する．
    vector<const char*> var_list;
    ymsh()->get_var_name_list(var_list);
    for (vector<const char*>::iterator p = var_list.begin();
	 p != var_list.end(); ++ p ) {
      const char* name = *p;
      vector<string>& value = ymsh()->get_var(name);
      cout << name << "\t";
      ymuint n = value.size();
      if ( n == 1 ) {
	cout << value[0];
      }
      else if ( n > 2 ) {
	cout << "(";
	const char* sp = "";
	for (ymuint i = 0; i < n; ++ i) {
	  cout << sp << value[i];
	  sp = " ";
	}
	cout << ")";
      }
      cout << endl;
    }
  }
  else {
    // <変数名>=<値> の形をパースして値の設定を行う．
    // ただし，<変数名> '=' <値>の形もある．
    for (ymuint pos = 1; pos < n; ) {
      string tmp = argv[pos];
      string::size_type index = tmp.find("=");
      string varname;
      string value_str;
      if ( index == string::npos ) {
	varname = tmp;
	++ pos;
	if ( pos >= n ) {
	  goto end;
	}
	if ( argv[pos] == "=" ) {
	  ++ pos;
	  if ( pos >= n ) {
	    goto end;
	  }
	  value_str = argv[pos];
	  ++ pos;
	}
      }
      else {
	varname = tmp.substr(0, index);
	value_str = tmp.substr(index + 1);
	++ pos;
      }
    end:
      vector<string>& value = ymsh()->get_var(varname);
      if ( value_str == string() ) {
	value.clear();
      }
      else {
	value.resize(1);
	value[0] = value_str;
      }
    }
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス UnsetCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UnsetCmd::UnsetCmd() :
  YmshCmd("unset")
{
}

// @brief デストラクタ
UnsetCmd::~UnsetCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
UnsetCmd::exec(const vector<string>& argv)
{
  if ( argv.size() == 1 ) {
    cout << argv[0] << ": Too few arguments." << endl;
    return -1;
  }

  for (ymuint i = 1; i < argv.size(); ++ i) {
    ymsh()->unreg_var(argv[i]);
  }
  return 0;
}

END_NAMESPACE_YM_YMSH
