#ifndef YM_SMTLiBV2_SMTSHELL_H
#define YM_SMTLiBV2_SMTSHELL_H

/// @file SmtLibMgr.h
/// @brief SmtLibMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"
#include "ym_smtlibv2/SmtMgr.h"
#include "ym_utils/StrBuff.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibParser;
class SmtLibNode;

//////////////////////////////////////////////////////////////////////
/// @class SmtShell SmtShell.h "SmtShell.h"
/// @brief SmtLib の S式の evaluation を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtShell
{
public:

  /// @brief コンストラクタ
  SmtShell();

  /// @brief デストラクタ
  ~SmtShell();


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
  const SmtSort*
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
  /// @param[out] sorted_var 結果を格納する変数
  /// @retval true 変換が成功した．
  /// @retval false 変換が失敗した．
  bool
  eval_to_sorted_var(const SmtLibNode* node,
		     SmtSortedVar& sorted_var);

  /// @brief S式を var_binding に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] var_binding 結果を格納する変数
  /// @retval true 変換が成功した．
  /// @retval false 変換が失敗した．
  bool
  eval_to_var_binding(const SmtLibNode* node,
		      SmtVarBinding& var_binding);

  /// @brief S式を attribute に変換する．
  /// @param[in] node S式を表すノード
  /// @param[out] attr_list 結果の attribute のリストを格納する変数
  bool
  eval_to_attr(const SmtLibNode* node,
	       vector<SmtAttr>& attr_list);

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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサー
  SmtLibParser* mParser;

  // 処理を行う本体
  SmtMgr mMgr;

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

};

END_NAMESPACE_YM_SMTLIBV2

#endif //  YM_SMTLiBV2_SMTSHELL_H
