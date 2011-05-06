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
  get_command(const char* name);

  /// @brief コマンド名のリストを得る．
  /// @param[out] command_list 結果のリストを格納するための変数
  void
  get_command_name_list(vector<const char*>& command_list);


public:
  //////////////////////////////////////////////////////////////////////
  // エイリアスに関連した関数
  //////////////////////////////////////////////////////////////////////

  /// @brief エイリアスを登録する．
  /// @param[in] alias エイリアス
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名のエイリアスがすでに登録済み
  bool
  reg_alias(YmshAlias* alias);

  /// @brief エイリアスの登録を解除する．
  /// @param[in] name 登録を解除するエイリアス名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_alias(const char* name);

  /// @brief エイリアスを得る．
  /// @param[in] name エイリアス名
  /// @note 登録されていなかったら NULL を返す．
  YmshAlias*
  get_alias(const char* name);

  /// @brief エイリアス名のリストを得る．
  /// @param[out] alias_list 結果のリストを格納するための変数
  void
  get_alias_name_list(vector<const char*>& alias_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 変数に関連した関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を登録する．
  /// @param[in] var 変数
  /// @retval true 登録が成功した．
  /// @retval false 登録が失敗した．同名の変数がすでに登録済み
  bool
  reg_var(YmshVar* var);

  /// @brief 変数の登録を解除する．
  /// @param[in] name 登録を解除する変数名
  /// @note 登録されていなくてもエラーとはならない．
  void
  unreg_var(const char* name);

  /// @brief 変数を得る．
  /// @param[in] name 変数名
  /// @note 見つからなければ NULL を返す．
  YmshVar*
  get_var(const char* name);

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
  hash_map<const char*, YmshCmd*> mCmdTable;

  // エイリアスのテーブル
  hash_map<const char*, YmshAlias*> mAliasTable;

  // 変数のテーブル
  hash_map<const char*, YmshVar*> mVarTable;

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

END_NAMESPACE_YM_YMSH

#endif // LIBYM_YMSH_YMSHIMPL_H
