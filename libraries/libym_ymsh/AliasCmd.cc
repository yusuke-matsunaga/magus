
/// @file libym_ymsh/AliasCmd.cc
/// @brief AliasCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "AliasCmd.h"
#include "YmshImpl.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
// クラス AliasCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
AliasCmd::AliasCmd() :
  YmshCmd("alias")
{
}

// @brief デストラクタ
AliasCmd::~AliasCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
AliasCmd::exec(const vector<string>& argv)
{
  ymuint n = argv.size();
  if ( n == 1 ) {
    vector<const char*> alias_list;
    ymsh()->get_alias_name_list(alias_list);
    for (vector<const char*>::iterator p = alias_list.begin();
	 p != alias_list.end(); ++ p ) {
      const char* name = *p;
      vector<string>& value = ymsh()->get_alias(name);
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
    vector<string>& value = ymsh()->get_alias(argv[1]);
    value.clear();
    value.resize(n - 2);
    for (ymuint i = 2; i < n; ++ i ) {
      value[i - 2] = argv[i];
    }
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス UnaliasCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
UnaliasCmd::UnaliasCmd() :
  YmshCmd("unalias")
{
}

// @brief デストラクタ
UnaliasCmd::~UnaliasCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] argv コマンド行の引数のベクタ(空白で切り分けてある)
// @return 実行後の状態を表すコードを返す．
int
UnaliasCmd::exec(const vector<string>& argv)
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
