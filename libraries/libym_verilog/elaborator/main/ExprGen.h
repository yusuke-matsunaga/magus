#ifndef LIBYM_VERILOG_ELABORATOR_EXPRGEN_H
#define LIBYM_VERILOG_ELABORATOR_EXPRGEN_H

/// @file libym_verilog/elaborator/ExprGen.h
/// @brief ExprGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_verilog/pt/PtP.h"
#include "ElbProxy.h"


BEGIN_NAMESPACE_YM_VERILOG

class ElbEnv;

//////////////////////////////////////////////////////////////////////
/// @class ExprGen ExprGen.h "ExprGen.h"
/// @brief ElbExpr を生成するクラス
//////////////////////////////////////////////////////////////////////
class ExprGen :
  public ElbProxy
{
public:

  /// @brief コンストラクタ
  /// @param[in] elab 生成器
  /// @param[in] elb_mgr Elbオブジェクトを管理するクラス
  /// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
  ExprGen(Elaborator& elab,
	  ElbMgr& elb_mgr,
	  ElbFactory& elb_factory);

  /// @brief デストラクタ
  ~ExprGen();


public:
  //////////////////////////////////////////////////////////////////////
  // ExprGen の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtExpr から通常の ElbiExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_expr(const VlNamedObj* parent,
		   const ElbEnv& env,
		   const PtExpr* pt_expr);

  /// @brief PtExpr からイベント式の ElbiExpr を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_event_expr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr);

  /// @brief PtExpr からシステム関数の引数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_arg(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr から左辺式を生成する
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された ElbExpr のポインタを返す．
  /// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
  ElbExpr*
  instantiate_lhs(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtExpr(primary) から named_event を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  ElbExpr*
  instantiate_namedevent(const VlNamedObj* parent,
			 const PtExpr* pt_expr);

  /// @brief PtExpr を評価し int 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_int(const VlNamedObj* parent,
		    const PtExpr* pt_expr,
		    int& value);

  /// @brief PtExpr を評価し bool 値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_bool(const VlNamedObj* parent,
		     const PtExpr* pt_expr,
		     bool& value);

  /// @brief PtExpr を評価しビットベクタ値を返す．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[out] value 評価値を格納する変数
  /// @note 定数でなければエラーメッセージを出力し false を返す．
  bool
  evaluate_expr_bitvector(const VlNamedObj* parent,
			  const PtExpr* pt_expr,
			  BitVector& value);

  /// @brief PtDelay から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_delay 遅延を表すパース木
  ElbDelay*
  instantiate_delay(const VlNamedObj* parent,
		    const PtDelay* pt_delay);

  /// @brief PtOrderedCon から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_head 順序付きわりあて式
  /// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
  /// 遅延なのかわからないので PtOrderedCon で表していることによる．
  ElbDelay*
  instantiate_delay(const VlNamedObj* parent,
		    const PtItem* pt_head);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief PtPrimary から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_primary(const VlNamedObj* parent,
		      const ElbEnv& env,
		      const PtExpr* pt_expr);

  /// @brief PtPrimary から左辺用の ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_lhs_primary(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr);

  /// @brief PtOpr から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_opr(const VlNamedObj* parent,
		  const ElbEnv& env,
		  const PtExpr* pt_expr);

  /// @brief PtFuncCall から ElbExpr を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_funccall(const VlNamedObj* parent,
		       const ElbEnv& env,
		       const PtExpr* pt_expr);

  /// @brief PtSysFuncCall から引数を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_sysfunccall(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr);
  
  /// @brief 単体の NamedEvent 用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] decl 宣言要素 (NamedEvent であることを期待している)
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_namedevent_primary1(const VlNamedObj* parent,
				  const PtExpr* pt_expr,
				  ElbDecl* decl);

  /// @brief 配列要素の NamedEvent 用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] decl 配列型の宣言要素 (NamedEventArray であることを期待している)
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_namedevent_primary2(const VlNamedObj* parent,
				  const PtExpr* pt_expr,
				  ElbDeclArray* decl);

  /// @brief 定数式用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_const_primary(const VlNamedObj* parent,
			    const ElbEnv& env,
			    const PtExpr* pt_expr);

  /// @brief constant function 内で用いられるプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] func 親の関数
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_cf_primary(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr,
			 const VlNamedObj* func);

  /// @brief 通常のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_normal_primary(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr);
  
  /// @brief 単体の宣言要素用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] decl 対象の宣言要素
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_decl_primary(const VlNamedObj* parent,
			   const ElbEnv& env,
			   const PtExpr* pt_expr,
			   ElbDecl* decl);
  
  /// @brief 配列要素用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] decl 対象の配列
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_array_primary(const VlNamedObj* parent,
			    const ElbEnv& env,
			    const PtExpr* pt_expr,
			    ElbDeclArray* decl);
  
  /// @brief parameter 宣言用のプライマリ式を生成する．
  /// @param[in] parent 親のスコープ
  /// @param[in] env 生成時の環境
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] param 対象の parameter 宣言
  /// @return 生成された式を返す．
  /// @note エラーが起きたらエラーメッセージを出力し，NULL を返す．
  ElbExpr*
  instantiate_param_primary(const VlNamedObj* parent,
			    const ElbEnv& env,
			    const PtExpr* pt_expr,
			    ElbParameter* param);
  
  /// @brief 添字の部分を実体化する．(単体のオブジェクト用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] obj 対象のオブジェクト
  /// @param[in] func 親の function
  /// @param[out] has_range_select 範囲指定を持っていたら true を返す．
  /// @param[out] has_bit_select ビット指定を持っていたら true を返す．
  /// @param[out] index1, index2 範囲指定/ビット指定の式を返す．
  bool
  resolve1(const VlNamedObj* parent,
	   const PtExpr* pt_expr,
	   VlDecl* obj,
	   const VlNamedObj* func,
	   bool& has_range_select,
	   bool& has_bit_select,
	   ElbExpr*& index1,
	   ElbExpr*& index2);
  
  /// @brief 添字の部分を実体化する．(配列要素のオブジェクト用)
  /// @param[in] parent 親のスコープ
  /// @param[in] pt_expr 式を表すパース木
  /// @param[in] obj 対象の配列型オブジェクト
  /// @param[in] func 親の function
  /// @param[out] index_list インデックス式のリスト
  /// @param[out] has_range_select 範囲指定を持っていたら true を返す．
  /// @param[out] has_bit_select ビット指定を持っていたら true を返す．
  /// @param[out] index1, index2 範囲指定/ビット指定の式を返す．
  bool
  resolve2(const VlNamedObj* parent,
	   const PtExpr* pt_expr,
	   VlDecl* obj,
	   const VlNamedObj* func,
	   vector<ElbExpr*>& index_list,
	   bool& has_range_select,
	   bool& has_bit_select,
	   ElbExpr*& index1,
	   ElbExpr*& index2);
  
};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_EXPRGEN_H
