
/// @file libym_ymsh/YmshImpl.cc
/// @brief YmshImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmshImpl.h"
#include "ym_ymsh/YmshCmd.h"
#include "ym_ymsh/YmshAlias.h"
#include "ym_ymsh/YmshVar.h"
#include <readline/readline.h>
#include <readline/history.h>


BEGIN_NAMESPACE_YM_YMSH

BEGIN_NONAMESPACE

// 文字列をソートするための比較関数
struct StrCmp
{
  bool
  operator()(const char* left,
	     const char* right)
  {
    return strcmp(left, right);
  }
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス YmshImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmshImpl::YmshImpl()
{
  mPrompt1 = ">";
  mPrompt2 = "?";
  mPrompt3 = "?";
  mMaxHistory = 100;

  mHistoryNum = 0;

  mAllowCtrlDExit = false;
}

// @brief デストラクタ
YmshImpl::~YmshImpl()
{
}


BEGIN_NONAMESPACE

bool
parse_line(const char* line,
	   vector<string>& argv)
{
  StrBuff buff;
  const char* bp = line;
  char c;

 ST_INIT:
  c = *bp ++;
  if ( c == '\0' ) {
    return true;
  }
  if ( c == ' ' || c == '\t' ) {
    goto ST_INIT;
  }
  if ( c == '\\' ) {
    goto ST_ESC;
  }
  if ( c == '"' ) {
    goto ST_DQ;
  }
  buff.put_char(c);
  goto ST_STR;

 ST_ESC:
  c = *bp ++;
  if ( c == '\0' ) {
    buff.put_char('\\');
    argv.push_back(buff.c_str());
    return true;
  }
  if ( c == ' ' || c == '\t' ) {
    buff.put_char(c);
  }
  else {
    buff.put_char('\\');
    buff.put_char(c);
  }
  goto ST_STR;

 ST_DQ:
  c = *bp ++;
  if ( c == '\0' ) {
    cout << "Error: \" mismatch." << endl;
    return false;
  }
  if ( c == '"' ) {
    goto ST_STR;
  }
  buff.put_char(c);
  goto ST_DQ;

 ST_STR:
  c = *bp ++;
  if ( c == '\0' ) {
    argv.push_back(buff.c_str());
    return true;
  }
  if ( c == ' ' || c == '\t' ) {
    argv.push_back(buff.c_str());
    buff.clear();
    goto ST_INIT;
  }
  buff.put_char(c);
  goto ST_STR;
}

END_NONAMESPACE

// @brief コマンドループを実行する．
void
YmshImpl::run()
{
  for ( ; ; ) {
    char* line = readline(mPrompt1.c_str());
    if ( line == NULL ) {
      if ( mAllowCtrlDExit ) {
	break;
      }
      printf("\n");
      continue;
    }

    add_history(line);
    ++ mHistoryNum;
    if ( mHistoryNum > mMaxHistory ) {
      // あふれたヒストリを削除する．
      HIST_ENTRY* history = remove_history(0);
      free(history);
      -- mHistoryNum;
    }

    // コマンドラインの文字列を空白で切り分ける．
    vector<string> argv;
    parse_line(line, argv);

    string cmdname = argv[0];

    // まずエイリアスを探す．
    if ( cmdname[0] != '\\' ) {
      YmshAlias* alias = get_alias(cmdname.c_str());
      if ( alias != NULL ) {
	// argv[0] を alias->value() で置き換える．
	argv.erase(argv.begin());
	argv.insert(argv.begin(), alias->value().begin(), alias->value().end());
      }
    }

    // コマンドを探す．
    cmdname = argv[0];
    if ( cmdname[0] == '\\' ) {
      cmdname.erase(cmdname.begin());
    }
    YmshCmd* cmd = get_command(cmdname.c_str());
    if ( cmd != NULL ) {
      int code = cmd->exec(argv);
    }
    else {
      cout << cmdname << ": command not found." << endl;
    }

    // 文字列領域を解放する．
    free(line);
  }
}

// @brief コマンドを登録する．
// @param[in] cmd コマンド
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名のコマンドがすでに登録済み
bool
YmshImpl::reg_command(YmshCmd* cmd)
{
  if ( mCmdTable.count(cmd->name()) > 0 ) {
    // 同名のコマンドが登録済み
    return false;
  }

  mCmdTable.insert(make_pair(cmd->name(), cmd));
  cmd->mYmsh = this;
  return true;
}

// @brief コマンドの登録を削除する．
// @param[in] name 削除するコマンド名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_command(const char* name)
{
  mCmdTable.erase(name);
}

// @brief コマンド名に対応したコマンドを返す．
// @param[in] name コマンド名
// @note なければ NULL を返す．
YmshCmd*
YmshImpl::get_command(const char* name)
{
  hash_map<const char*, YmshCmd*>::iterator p = mCmdTable.find(name);
  if ( p == mCmdTable.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief コマンド名のリストを得る．
// @param[out] command_list 結果のリストを格納するための変数
void
YmshImpl::get_command_name_list(vector<const char*>& command_list)
{
  command_list.clear();
  for (hash_map<const char*, YmshCmd*>::iterator p = mCmdTable.begin();
       p != mCmdTable.end(); ++ p) {
    const char* name = p->first;
    command_list.push_back(name);
  }
  sort(command_list.begin(), command_list.end(), StrCmp());
}

// @brief エイリアスを登録する．
// @param[in] alias エイリアス
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名のエイリアスがすでに登録済み
bool
YmshImpl::reg_alias(YmshAlias* alias)
{
  if ( mAliasTable.count(alias->name()) > 0 ) {
    // 同名のエイリアスがすでに登録されていた．
    return false;
  }

  mAliasTable.insert(make_pair(alias->name(), alias));
  return true;
}

// @brief エイリアスの登録を解除する．
// @param[in] name 登録を解除するエイリアス名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_alias(const char* name)
{
  mAliasTable.erase(name);
}

// @brief エイリアスを得る．
// @param[in] name エイリアス名
// @note 登録されていなかったら NULL を返す．
YmshAlias*
YmshImpl::get_alias(const char* name)
{
  hash_map<const char*, YmshAlias*>::iterator p = mAliasTable.find(name);
  if ( p == mAliasTable.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief エイリアス名のリストを得る．
// @param[out] alias_list 結果のリストを格納するための変数
void
YmshImpl::get_alias_name_list(vector<const char*>& alias_list)
{
  alias_list.clear();
  for (hash_map<const char*, YmshAlias*>::iterator p = mAliasTable.begin();
       p != mAliasTable.end(); ++ p) {
    const char* name = p->first;
    alias_list.push_back(name);
  }
  sort(alias_list.begin(), alias_list.end(), StrCmp());
}

// @brief 変数を登録する．
// @param[in] var 変数
// @retval true 登録が成功した．
// @retval false 登録が失敗した．同名の変数がすでに登録済み
bool
YmshImpl::reg_var(YmshVar* var)
{
  if ( mVarTable.count(var->name()) > 0 ) {
    // 同名の変数が登録されていた．
    return false;
  }

  mVarTable.insert(make_pair(var->name(), var));
  return true;
}

// @brief 変数の登録を解除する．
// @param[in] name 登録を解除する変数名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_var(const char* name)
{
  mVarTable.erase(name);
}

// @brief 変数を得る．
// @param[in] name 変数名
// @note 見つからなければ NULL を返す．
YmshVar*
YmshImpl::get_var(const char* name)
{
  hash_map<const char*, YmshVar*>::iterator p = mVarTable.find(name);
  if ( p == mVarTable.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief 変数名のリストを得る．
// @param[out] var_list 結果のリストを格納するための変数
void
YmshImpl::get_var_name_list(vector<const char*>& var_list)
{
  var_list.clear();
  for (hash_map<const char*, YmshVar*>::iterator p = mVarTable.begin();
       p != mVarTable.end(); ++ p) {
    const char* name = p->first;
    var_list.push_back(name);
  }
  sort(var_list.begin(), var_list.end(), StrCmp());
}

END_NAMESPACE_YM_YMSH
