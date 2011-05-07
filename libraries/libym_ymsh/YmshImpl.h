#ifndef LIBYM_YMSH_YMSHIMPL_H
#define LIBYM_YMSH_YMSHIMPL_H

/// @file libym_ymsh/YmshImpl.h
/// @brief YmshImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_ymsh/ymsh_nsdef.h"
#include "ym_utils/StrBuff.h"
#include "YmshCmdTable.h"
#include "YmshObjTable.h"


BEGIN_NAMESPACE_YM_YMSH

//////////////////////////////////////////////////////////////////////
/// @class YmshImpl YmshImpl "YmshImpl.h"
/// @brief Ymsh の実装クラス
//////////////////////////////////////////////////////////////////////
class YmshImpl
{
public:

  /// @brief コンストラクタ
  YmshImpl();

  /// @brief デストラクタ
  ~YmshImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // メイン関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドループを実行する．
  void
  run();


public:
  //////////////////////////////////////////////////////////////////////
  // コマンドに関連した関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドを登録する．
  /// @param[in] cmd コマンド
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名のコマンドがすでに登録済み
  bool
  reg_command(YmshCmd* cmd);

  /// @brief コマンドの登録を削除する．
  /// @param[in] name 削除するコマンド名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_command(const char* name);

  /// @brief コマンド名に対応したコマンドを返す．
  /// @param[in] name コマンド名
  /// @note なければ NULL を返す．
  YmshCmd*
  find_command(const char* name);

  /// @brief コマンド名のリストを得る．
  /// @param[out] command_list 結果のリストを格納するための変数
  void
  get_command_name_list(vector<const char*>& command_list);


public:
  //////////////////////////////////////////////////////////////////////
  // エイリアスに関連した関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エイリアスが登録されているか調べる．
  /// @param[in] name エイリアス名
  /// @return name という名のエイリアスが登録されていたら true を返す．
  bool
  check_alias(const char* name);

  /// @brief エイリアスが登録されているか調べる．
  /// @param[in] name エイリアス名
  /// @return name という名のエイリアスが登録されていたら true を返す．
  bool
  check_alias(const string& name);

  /// @brief エイリアスを得る．
  /// @param[in] name エイリアス名
  /// @return 対応する値を返す．
  /// @note 登録してなかったら新しいエイリアスを作る．
  vector<string>&
  get_alias(const char* name);

  /// @brief エイリアスを得る．
  /// @param[in] name エイリアス名
  /// @return 対応する値を返す．
  /// @note 登録してなかったら新しいエイリアスを作る．
  vector<string>&
  get_alias(const string& name);

  /// @brief エイリアスの登録を解除する．
  /// @param[in] name 登録を解除するエイリアス名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_alias(const char* name);

  /// @brief エイリアスの登録を解除する．
  /// @param[in] name 登録を解除するエイリアス名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_alias(const string& name);

  /// @brief エイリアス名のリストを得る．
  /// @param[out] alias_list 結果のリストを格納するための変数
  void
  get_alias_name_list(vector<const char*>& alias_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 変数に関連した関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数が登録されているか調べる．
  /// @param[in] name 変数名
  /// @return name という名の変数が登録されていたら true を返す．
  bool
  check_var(const char* name);

  /// @brief 変数が登録されているか調べる．
  /// @param[in] name 変数名
  /// @return name という名の変数が登録されていたら true を返す．
  bool
  check_var(const string& name);

  /// @brief 変数を得る．
  /// @param[in] name 変数名
  /// @return 対応する変数の値
  /// @note なければ登録する．
  vector<string>&
  get_var(const char* name);

  /// @brief 変数を得る．
  /// @param[in] name 変数名
  /// @return 対応する変数の値
  /// @note なければ登録する．
  vector<string>&
  get_var(const string& name);

  /// @brief 変数の登録を解除する．
  /// @param[in] name 登録を解除する変数名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_var(const char* name);

  /// @brief 変数の登録を解除する．
  /// @param[in] name 登録を解除する変数名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_var(const string& name);

  /// @brief 変数名のリストを得る．
  /// @param[out] var_list 結果のリストを格納するための変数
  void
  get_var_name_list(vector<const char*>& var_list);


public:
  //////////////////////////////////////////////////////////////////////
  // コマンドの挙動を制御する関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // コマンドのテーブル
  YmshCmdTable mCmdTable;

  // エイリアスのテーブル
  YmshObjTable mAliasTable;

  // 変数のテーブル
  YmshObjTable mVarTable;

  // プロンプト文字列1
  StrBuff mPrompt1;

  // プロンプト文字列2
  StrBuff mPrompt2;

  // プロンプト文字列3
  StrBuff mPrompt3;

  // 最大ヒストリ数
  ymuint32 mMaxHistory;

  // 現在のヒストリ数
  ymuint32 mHistoryNum;

  // ctrl-D での終了を許すかどうかのフラグ
  bool mAllowCtrlDExit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief エイリアスが登録されているか調べる．
// @param[in] name エイリアス名
// @return name という名のエイリアスが登録されていたら true を返す．
inline
bool
YmshImpl::check_alias(const string& name)
{
  return check_alias(name.c_str());
}

// @brief エイリアスを得る．
// @param[in] name エイリアス名
// @return 対応する値を返す．
// @note 登録してなかったら新しいエイリアスを作る．
inline
vector<string>&
YmshImpl::get_alias(const string& name)
{
  return get_alias(name.c_str());
}

// @brief エイリアスの登録を解除する．
// @param[in] name 登録を解除するエイリアス名
// @note 登録されていなくてもエラーとはならない．
inline
void
YmshImpl::unreg_alias(const string& name)
{
  return unreg_alias(name.c_str());
}

// @brief 変数が登録されているか調べる．
// @param[in] name 変数名
// @return name という名の変数が登録されていたら true を返す．
inline
bool
YmshImpl::check_var(const string& name)
{
  return check_var(name.c_str());
}

// @brief 変数を得る．
// @param[in] name 変数名
// @return 対応する変数の値
// @note なければ登録する．
inline
vector<string>&
YmshImpl::get_var(const string& name)
{
  return get_var(name.c_str());
}

// @brief 変数の登録を解除する．
// @param[in] name 登録を解除する変数名
// @note 登録されていなくてもエラーとはならない．
inline
void
YmshImpl::unreg_var(const string& name)
{
  return unreg_var(name.c_str());
}

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHIMPL_H
