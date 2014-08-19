
/// @file ElbProxy.cc
/// @brief ElbProxy の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbProxy.h"
#include "ElbEnv.h"
#include "ModuleGen.h"
#include "DeclGen.h"
#include "ItemGen.h"
#include "StmtGen.h"
#include "ExprGen.h"
#include "AttrGen.h"

#include "ElbExpr.h"
#include "YmVerilog/pt/PtExpr.h"
#include "YmVerilog/pt/PtArray.h"
#include "PtDumper.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] elb 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ElbProxy::ElbProxy(Elaborator& elab,
		   ElbMgr& elb_mgr,
		   ElbFactory& elb_factory) :
  mElaborator(elab),
  mMgr(elb_mgr),
  mFactory(elb_factory)
{
}

// @brief デストラクタ
ElbProxy::~ElbProxy()
{
}

// @brief 初期化を行う．
// @param[in] module_gen モジュール生成用のオブジェクト
// @param[in] decl_gen 宣言要素生成用のオブジェクト
// @param[in] item_gen 構成要素生成用のオブジェクト
// @param[in] stmt_gen ステートメント生成用のオブジェクト
// @param[in] expr_gen 式生成用のオブジェクト
void
ElbProxy::init(ModuleGen* module_gen,
	       DeclGen* decl_gen,
	       ItemGen* item_gen,
	       StmtGen* stmt_gen,
	       ExprGen* expr_gen,
	       AttrGen* attr_gen)
{
  mModuleGen = module_gen;
  mDeclGen = decl_gen;
  mItemGen = item_gen;
  mStmtGen = stmt_gen;
  mExprGen = expr_gen;
  mAttrGen = attr_gen;
}

// @brief 対応する宣言に範囲があり，IO宣言に範囲がない記述を認めるか
bool
ElbProxy::allow_empty_io_range()
{
  return mElaborator.mAllowEmptyIORange;
}

// @brief module の中身のうちスコープに関係するインスタンス化を行う．
// @param[in] modle モジュール
// @param[in] pt_module モジュール定義
// @param[in] param_con パラメータ割り当ての情報
void
ElbProxy::phase1_module_item(ElbModule* module,
			     const PtModule* pt_module,
			     const ElbParamCon* param_con)
{
  return mModuleGen->phase1_module_item(module, pt_module, param_con);
}

// @brief parameter と genvar を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
// @param[in] force_to_local true なら parameter を localparam にする．
void
ElbProxy::phase1_decl(const VlNamedObj* parent,
		      PtDeclHeadArray pt_head_array,
		      bool force_to_local)
{
  mDeclGen->phase1_decl(parent, pt_head_array, force_to_local);
}

// @brief IO宣言要素を実体化する．
// @param[in] module 親のモジュール
// @param[in] task 親のタスク
// @param[in] function 親の function
// @param[in] pt_head_array IO宣言ヘッダの配列
// @note module, task, function は1つのみが値を持つ．残りは NULL．
void
ElbProxy::instantiate_iodecl(ElbModule* module,
			     ElbTaskFunc* taskfunc,
			     PtIOHeadArray pt_head_array)
{
  mDeclGen->instantiate_iodecl(module, taskfunc, pt_head_array);
}

// @brief 宣言要素のリストをインスタンス化する．
// @param[in] parent 親のスコープ
// @param[in] pt_head_array 宣言ヘッダの配列
void
ElbProxy::instantiate_decl(const VlNamedObj* parent,
			   PtDeclHeadArray pt_head_array)
{
  mDeclGen->instantiate_decl(parent, pt_head_array);
}

// @brief スコープに関係する要素を実体化する．
// @param[in] parent 親のスコープ
// @param[in] pt_item_array 要素定義の配列
void
ElbProxy::phase1_item(const VlNamedObj* parent,
		      PtItemArray pt_item_array)
{
  mItemGen->phase1_item(parent, pt_item_array);
}

// @brief constant function の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_function 関数定義
ElbTaskFunc*
ElbProxy::instantiate_constant_function(const VlNamedObj* parent,
					const PtItem* pt_function)
{
  return mItemGen->instantiate_constant_function(parent, pt_function);
}

// @brief スコープに関係するステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] pt_stmt 対象のステートメント
// @param[in] cf constant function 中のステートメントの時 true
void
ElbProxy::phase1_stmt(const VlNamedObj* parent,
		      const PtStmt* pt_stmt,
		      bool cf)
{
  mStmtGen->phase1_stmt(parent, pt_stmt, cf);
}

// @brief ステートメントの実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
ElbProxy::instantiate_stmt(const VlNamedObj* parent,
			   ElbProcess* process,
			   const ElbEnv& env,
			   const PtStmt* pt_stmt)
{
  return mStmtGen->instantiate_stmt(parent, process, env, pt_stmt);
}

// @brief PtExpr から ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_expr(const VlNamedObj* parent,
			   const ElbEnv& env,
			   const PtExpr* pt_expr)
{
  return mExprGen->instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr から定数式の ElbExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_constant_expr(const VlNamedObj* parent,
				    const PtExpr* pt_expr)
{
  return mExprGen->instantiate_constant_expr(parent, pt_expr);
}

// @brief PtExpr からイベント式の ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_event_expr(const VlNamedObj* parent,
				 const ElbEnv& env,
				 const PtExpr* pt_expr)
{
  return mExprGen->instantiate_event_expr(parent, env, pt_expr);
}

// @brief PtExpr からシステム関数の引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_arg(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  return mExprGen->instantiate_arg(parent, env, pt_expr);
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_lhs(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  return mExprGen->instantiate_lhs(parent, env, pt_expr);
}

// @brief PtExpr から右辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[in] lhs 左辺式
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ElbProxy::instantiate_rhs(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr,
			  ElbExpr* lhs)
{
  ElbExpr* expr = mExprGen->instantiate_expr(parent, env, pt_expr);
  if ( expr ) {
    // lhs の型を expr に設定する．
    expr->set_reqsize(lhs->value_type());
  }
  return expr;
}

// @brief PtExpr(primary) から named_event を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
ElbExpr*
ElbProxy::instantiate_namedevent(const VlNamedObj* parent,
				 const PtExpr* pt_expr)
{
  return mExprGen->instantiate_namedevent(parent, pt_expr);
}

// @brief PtDelay から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_delay 遅延を表すパース木
ElbDelay*
ElbProxy::instantiate_delay(const VlNamedObj* parent,
			    const PtDelay* pt_delay)
{
  return mExprGen->instantiate_delay(parent, pt_delay);
}

// @brief PtOrderedCon から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 順序付きわりあて式
// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
// 遅延なのかわからないので PtOrderedCon で表していることによる．
ElbDelay*
ElbProxy::instantiate_delay(const VlNamedObj* parent,
			    const PtItem* pt_head)
{
  return mExprGen->instantiate_delay(parent, pt_head);
}

// @brief 式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ElbProxy::evaluate_expr(const VlNamedObj* parent,
			const PtExpr* pt_expr,
			bool put_error)
{
  return mExprGen->evaluate_expr(parent, pt_expr, put_error);
}

// @brief PtExpr を評価し int 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ElbProxy::evaluate_int(const VlNamedObj* parent,
		       const PtExpr* pt_expr,
		       int& value,
		       bool put_error)
{
  return mExprGen->evaluate_int(parent, pt_expr, value, put_error);
}

// @brief PtExpr を評価しスカラー値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ElbProxy::evaluate_scalar(const VlNamedObj* parent,
			  const PtExpr* pt_expr,
			  VlScalarVal& value,
			  bool put_error)
{
  return mExprGen->evaluate_scalar(parent, pt_expr, value, put_error);
}

// @brief PtExpr を評価し bool 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ElbProxy::evaluate_bool(const VlNamedObj* parent,
			const PtExpr* pt_expr,
			bool& value,
			bool put_error)
{
  return mExprGen->evaluate_bool(parent, pt_expr, value, put_error);
}

// @brief PtExpr を評価しビットベクタ値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ElbProxy::evaluate_bitvector(const VlNamedObj* parent,
			     const PtExpr* pt_expr,
			     BitVector& value,
			     bool put_error)
{
  return mExprGen->evaluate_bitvector(parent, pt_expr, value, put_error);
}

// @brief 範囲を表す式を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_left 範囲のMSBを表すパース木
// @param[in] pt_right 範囲のLSBを表すパース木
// @param[in] left_val 範囲の MSB の値
// @param[in] right_val 範囲の LSB の値
bool
ElbProxy::evaluate_range(const VlNamedObj* parent,
			 const PtExpr* pt_left,
			 const PtExpr* pt_right,
			 int& left_val,
			 int& right_val)
{
  left_val = 0;
  right_val = 0;
  if ( pt_left && pt_right ) {
    bool stat1 = evaluate_int(parent, pt_left, left_val, true);
    bool stat2 = evaluate_int(parent, pt_right, right_val, true);
    if ( !stat1 || !stat2 ) {
      return false;
    }
  }
  return true;
}

// @brief PtAttrInst から属性リストを生成し，オブジェクトに付加する．
// @param[in] pt_attr_array 属性を表すパース木
// @param[in] def 定義側の属性の時 true とするフラグ
// @param[in] obj 付加する対象のオブジェクト
void
ElbProxy::instantiate_attribute(PtAttrInstArray pt_attr_array,
				bool def,
				const VlObj* obj)
{
  return mAttrGen->instantiate_attribute(pt_attr_array, def, obj);
}

END_NAMESPACE_YM_VERILOG
