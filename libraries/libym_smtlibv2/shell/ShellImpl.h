#ifndef SHELLIMPL_H
#define SHELLIMPL_H

/// @file ShellImpl.h
/// @brief ShellImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_logic/SmtSolver.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;
class IdMgr;
class SortElem;
class SortMgr;
class NameMgr;
class NameObj;
class StackPage;
class SmtLibParser;
class SmtLibNode;
class SmtTermMgr;

//////////////////////////////////////////////////////////////////////
/// @class ShellImpl ShellImpl.h "ShellImpl.h"
/// @brief SmtLib の S式の evaluation を行うクラス
//////////////////////////////////////////////////////////////////////
class ShellImpl
{
public:

  /// @brief コンストラクタ
  ShellImpl();

  /// @brief デストラクタ
  ~ShellImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 実行する．
  void
  run();


private:
  //////////////////////////////////////////////////////////////////////
  // コマンドを実行する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief set-logic の処理を行う．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  set_logic(const SmtLibNode* arg_top);

  /// @brief オプションを設定する．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  set_option(const SmtLibNode* arg_top);

  /// @brief 情報を設定する．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  set_info(const SmtLibNode* arg_top);

  /// @brief sort の宣言を行う．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  declare_sort(const SmtLibNode* arg_top);

  /// @brief sort の alias を定義する．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  define_sort(const SmtLibNode* arg_top);

  /// @brief 関数の宣言を行う．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  declare_fun(const SmtLibNode* arg_top);

  /// @brief 関数の定義を行う．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  define_fun(const SmtLibNode* arg_top);

  /// @brief assertion を追加する．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  assert(const SmtLibNode* arg_top);

  /// @brief assertion スタックにプッシュする．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  push(const SmtLibNode* arg_top);

  /// @brief assertion スタックからポップする．
  /// @param[in] arg_top 引数の先頭ノード
  /// @return ポップが成功したら true を返す．
  bool
  pop(const SmtLibNode* arg_top);

  /// @brief 充足可能性判定を行なう．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  check_sat(const SmtLibNode* arg_top);

  /// @brief assertion を得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_assertions(const SmtLibNode* arg_top);

  /// @brief 証明を得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_proof(const SmtLibNode* arg_top);

  /// @brief unsat core を得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_unsat_core(const SmtLibNode* arg_top);

  /// @brief 値を得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_value(const SmtLibNode* arg_top);

  /// @brief 割り当てを得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_assignment(const SmtLibNode* arg_top);

  /// @brief オプションを得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_option(const SmtLibNode* arg_top);

  /// @brief 情報を得る．
  /// @param[in] arg_top 引数の先頭ノード
  bool
  get_info(const SmtLibNode* arg_top);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 識別子を返す．
  /// @param[in] name 名前
  /// @param[in] index_list インデックスのリスト
  /// @return 同じ識別子があればそれを返す．なければ作る．
  ///
  /// この関数は通常は成功するはず．
  const SmtId*
  make_id(const ShString& name,
	  const vector<ymuint32>& index_list = vector<ymuint32>(0));

  /// @brief 型を返す．
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] elem_list 要素のリスト
  /// @return 同じ型があればそれを返す．なければ作る．
  /// @note エラーの場合には NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の型が登録されていなかった．
  ///  - 登録されている型と elem_list のサイズが異なった．
  const SmtSort*
  make_sort(const SmtId* name_id,
	    const vector<const SmtSort*>& elem_list = vector<const SmtSort*>(0));

  /// @brief 名前から変数か関数を探す
  /// @param[in] name_id 名前を表す識別子
  /// @return 指定された名前の変数または関数を返す．
  /// @note エラーの場合には NULL を返す．
  ///
  /// エラーの原因は以下のとおり
  ///  - name_id という名の関数が登録されていなかった．
  const NameObj*
  find_obj(const SmtId* name_id);

  /// @brief let 文の処理を行なう．
  /// @param[in] node 引数の先頭ノード
  const SmtTerm*
  eval_to_let(const SmtLibNode* node);

  /// @brief forall 文の処理を行なう．
  /// @param[in] node 引数の先頭ノード
  const SmtTerm*
  eval_to_forall(const SmtLibNode* node);

  /// @brief exists 文の処理を行なう．
  /// @param[in] node 引数の先頭ノード
  const SmtTerm*
  eval_to_exists(const SmtLibNode* node);

  /// @brief attr 文の処理を行なう．
  /// @param[in] node 引数の先頭ノード
  const SmtTerm*
  eval_to_attr_term(const SmtLibNode* node);

  /// @brief S式を識別子に変換する．
  /// @param[in] node S式を表すノード
  const SmtId*
  eval_to_id(const SmtLibNode* node);

  /// @brief S式を sort に変換する．
  /// @param[in] node S式を表すノード
  const SmtSort*
  eval_to_sort(const SmtLibNode* node);

  /// @brief S式を sort に変換する．
  /// @param[in] node S式を表すノード
  const SortElem*
  eval_to_sort_template(const SmtLibNode* node,
			const vector<const SmtId*>& param_list);

  /// @brief S式を term に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_term(const SmtLibNode* node);

  /// @brief S式を s-expr に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_expr(const SmtLibNode* node);

  /// @brief S式を qual_identifier に変換する．
  /// @param[in] node S式を表すノード
  const SmtTerm*
  eval_to_qid(const SmtLibNode* node);

  /// @brief S式を sorted_var に変換する．
  /// @param[in] node S式を表すノード
  const SmtVar*
  eval_to_sorted_var(const SmtLibNode* node);

  /// @brief S式を var_binding に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] id 変数のID番号
  /// @param[out] term 置き換える式
  /// @retval true 変換が成功した．
  /// @retval false 変換が失敗した．
  bool
  eval_to_var_binding(const SmtLibNode* node,
		      ymuint32& id,
		      const SmtTerm*& term);

  /// @brief 引数のリストをパーズする．
  /// @param[in] arg_top 引数の先頭のノード
  /// @param[in] arg_num 引数の数
  /// @param[out] arg_list 引数を格納するリスト
  /// @retval true 引数の数が arg_num と一致した．
  /// @retval false 引数の数が arg_num と一致しなかった．
  bool
  parse_args(const SmtLibNode*  arg_top,
	     ymuint arg_num,
	     vector<const SmtLibNode*>& arg_list);


private:
  //////////////////////////////////////////////////////////////////////
  // readline 関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1行を評価する．
  int
  eval_line();


private:
  //////////////////////////////////////////////////////////////////////
  // Sort/Fun を管理するための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の SortMgr を返す．
  SortMgr&
  sort_mgr();

  /// @brief 現在の NameMgr を返す．
  NameMgr&
  name_mgr();

  /// @brief 現在のアロケータを返す．
  Alloc&
  alloc();

  /// @brief 現在の assertion リストを返す．
  vector<const SmtTerm*>&
  assertion_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー
  SmtLibParser* mParser;

  // 通常のプロンプト文字列
  StrBuff mPrompt1;

  // 継続行のプロンプト文字列
  StrBuff mPrompt2;

  // ヒストリの最大数
  ymuint32 mMaxHistory;

  // 現在覚えているヒストリの数．
  ymuint32 mHistoryNum;

  // ctrl-D で終了を許すかどうかのフラグ
  bool mAllowCtrlDExit;

  // 行バッファ
  vector<string> mStrList;

  // ループフラグ
  bool mLoop;

  // エラーメッセージ用のバッファ
  ostringstream mErrBuf;

  // SMT ソルバ
  SmtSolver* mSolver;

  // スタック
  vector<StackPage*> mStack;

  // SmtId を管理するクラス
  IdMgr* mIdMgr;

  // SmtTerm を管理するクラス
  SmtTermMgr* mTermMgr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif //  SHELLIMPL_H
