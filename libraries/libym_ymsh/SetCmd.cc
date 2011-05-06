
/// @file libym_ymsh/SetCmd.cc
/// @brief SetCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SetCmd.h"


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
  if ( argv.size() == 1 ) {
    vector<const char*> var_list;
    ymsh()->get_var_name_list(var_list);
    for (vector<const char*>::iterator p = var_list.begin();
	 p != var_list.end(); ++ p ) {
      const char* name = *p;
      YmshVar* var = ymsh()->get_var(name);
      assert_cond( var != NULL, __FILE__, __LINE__);
      cout << name << "\t";
      const vector<string>& value = var->value();
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
    for (ymuint i = 1; i < argv.size(); ++ i) {
      string tmp = argv[i];
      ymuint
    }
  }
}

END_NAMESPACE_YM_YMSH
