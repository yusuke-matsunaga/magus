
/// @file libym_ymsh/YmshImpl.cc
/// @brief YmshImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmshImpl.h"
#include "ym_ymsh/YmshCmd.h"

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
    return ( strcmp(left, right) <= 0 );
  }
};

END_NONAMESPACE


BEGIN_NONAMESPACE

// Ymsh
YmshImpl* gYmsh = NULL;

/* Generator function for command completion.  STATE lets us
   know whether to start from scratch; without any state
   (i.e. STATE == 0), then we start at the top of the list. */
char *
command_generator (const char *text,
		   int state)
{
  static int len;
  static vector<const char*> command_list;
  static vector<const char*>::iterator list_index;

  /* If this is a new word to complete, initialize now.  This
     includes saving the length of TEXT for efficiency, and
     initializing the index variable to 0. */
  if ( !state ) {
    len = strlen (text);
    gYmsh->get_command_name_list(command_list);
    list_index = command_list.begin();
  }

  /* Return the next name which partially matches from the
     command list. */
  while ( list_index != command_list.end() ) {
    const char* name = *list_index;
    ++ list_index;

    if ( strncmp (name, text, len) == 0 ) {
      ymuint size = strlen(name);
      char* str = static_cast<char*>(malloc(size + 1));
      for (ymuint i = 0; i < size; ++ i) {
	str[i] = name[i];
      }
      str[size] = '\0';
      return str;
    }
  }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/* Attempt to complete on the contents of TEXT.  START and END
   bound the region of rl_line_buffer that contains the word to
   complete.  TEXT is the word to complete.  We can use the entire
   contents of rl_line_buffer in case we want to do some simple
   parsing.  Returnthe array of matches, or NULL if there aren't any. */
char **
ymsh_completion (const char *text,
		 int start,
		 int end)
{
  char **matches;

  matches = (char **)NULL;

  /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
  if ( start == 0 )
    matches = rl_completion_matches (text, command_generator);

  return (matches);
}

// readline の completion 機能の設定を行う．
void
initialize_rl_completion(YmshImpl* impl)
{
  gYmsh = impl;

  rl_attempted_completion_function = ymsh_completion;

}

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

  mLoop = false;

  initialize_rl_completion(this);
}

// @brief デストラクタ
YmshImpl::~YmshImpl()
{
}


BEGIN_NONAMESPACE

char*
chop(char* line)
{
  char* s;
  for (s = line; *s && isspace(*s); ++ s) ;
  if ( *s == 0 ) {
    return s;
  }

  char* t = line + strlen(line) - 1;
  for ( ; t > s && isspace(*t); -- t) ;
  ++ t;
  *t = '\0';
  return s;
}

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
  for (mLoop = true; mLoop; ) {
    char* line = readline(mPrompt1.c_str());
    if ( line == NULL ) {
      if ( mAllowCtrlDExit ) {
	mLoop = false;
      }
      printf("\n");
      continue;
    }

    char* str = chop(line);
    if ( *str == '\0' ) {
      continue;
    }

    add_history(str);
    ++ mHistoryNum;
    if ( mHistoryNum > mMaxHistory ) {
      // あふれたヒストリを削除する．
      HIST_ENTRY* history = remove_history(0);
      free(history);
      -- mHistoryNum;
    }

    // コマンドラインの文字列を空白で切り分ける．
    vector<string> argv;
    parse_line(str, argv);

    string cmdname = argv[0];

    // まずエイリアスを探す．
    if ( cmdname[0] != '\\' ) {
      if ( check_alias(cmdname) ) {
	vector<string>& value = get_alias(cmdname);
	// argv[0] を value で置き換える．
	argv.erase(argv.begin());
	argv.insert(argv.begin(), value.begin(), value.end());
      }
    }

    // コマンドを探す．
    cmdname = argv[0];
    if ( cmdname[0] == '\\' ) {
      cmdname.erase(cmdname.begin());
    }
    YmshCmd* cmd = find_command(cmdname.c_str());
    if ( cmd != NULL ) {
      int code = cmd->exec(argv);
    }
    else {
      cout << cmdname << ": command not found." << endl;
      vector<const char*> tmp_list;
      get_command_name_list(tmp_list);
      cout << "Possible commands are :";
      for (vector<const char*>::iterator p = tmp_list.begin();
	   p != tmp_list.end(); ++ p) {
	cout << " " << *p;
      }
      cout << endl;
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
  bool stat = mCmdTable.reg_obj(cmd);
  if ( stat ) {
    cmd->mYmsh = this;
  }
  return stat;
}

// @brief コマンドの登録を削除する．
// @param[in] name 削除するコマンド名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_command(const char* name)
{
  mCmdTable.unreg_obj(name);
}

// @brief コマンド名に対応したコマンドを返す．
// @param[in] name コマンド名
// @note なければ NULL を返す．
YmshCmd*
YmshImpl::find_command(const char* name)
{
  return mCmdTable.find_obj(name);
}

// @brief コマンド名のリストを得る．
// @param[out] command_list 結果のリストを格納するための変数
void
YmshImpl::get_command_name_list(vector<const char*>& command_list)
{
  mCmdTable.get_name_list(command_list);
  sort(command_list.begin(), command_list.end(), StrCmp());
}

// @brief エイリアスが登録されているか調べる．
// @param[in] name エイリアス名
// @return name という名のエイリアスが登録されていたら true を返す．
bool
YmshImpl::check_alias(const char* name)
{
  return mAliasTable.check_obj(name);
}

// @brief エイリアスを得る．
// @param[in] name エイリアス名
// @return 対応する値を返す．
// @note 登録してなかったら新しいエイリアスを作る．
vector<string>&
YmshImpl::get_alias(const char* name)
{
  return mAliasTable.get_obj(name);
}

// @brief エイリアスの登録を解除する．
// @param[in] name 登録を解除するエイリアス名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_alias(const char* name)
{
  mAliasTable.unreg_obj(name);
}

// @brief エイリアス名のリストを得る．
// @param[out] alias_list 結果のリストを格納するための変数
void
YmshImpl::get_alias_name_list(vector<const char*>& alias_list)
{
  mAliasTable.get_name_list(alias_list);
  sort(alias_list.begin(), alias_list.end(), StrCmp());
}

// @brief 変数を得る．
// @param[in] name 変数名
// @return 対応する変数の値
// @note なければ登録する．
vector<string>&
YmshImpl::get_var(const char* name)
{
  return mVarTable.get_obj(name);
}

// @brief 変数の登録を解除する．
// @param[in] name 登録を解除する変数名
// @note 登録されていなくてもエラーとはならない．
void
YmshImpl::unreg_var(const char* name)
{
  mVarTable.unreg_obj(name);
}

// @brief 変数名のリストを得る．
// @param[out] var_list 結果のリストを格納するための変数
void
YmshImpl::get_var_name_list(vector<const char*>& var_list)
{
  mVarTable.get_name_list(var_list);
  sort(var_list.begin(), var_list.end(), StrCmp());
}

// @brief コマンドループを終了する．
void
YmshImpl::exit()
{
  mLoop = false;
}

END_NAMESPACE_YM_YMSH
