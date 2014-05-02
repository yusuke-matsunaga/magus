#ifndef BTBASE_H
#define BTBASE_H

/// @file BtBase.h
/// @brief BtBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BackTracer.h"
#include "TpgNode.h"
#include "logic/Literal.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtBase BtBase.h "BtBase.h"
/// @brief BackTracer の基本クラス
//////////////////////////////////////////////////////////////////////
class BtBase :
  public BackTracer
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvmgr TvMgr
  BtBase(TvMgr& tvmgr);

  /// @brief デストラクタ
  virtual
  ~BtBase();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタを生成する．
  /// @note 結果は mCurPattern に格納される．
  TestVector*
  new_vector();

  /// @brief 入力ノードの値を記録する．
  /// @param[in] node 対象の外部入力ノード
  /// @param[in] model SAT の割り当て結果
  /// @note node の値を mCurPattern に記録する．
  void
  record_value(TpgNode* node,
	       const vector<Bool3>& model);

  /// @brief ノードの正常値を読み出す．
  /// @param[in] node 対象のノード
  /// @param[in] model SAT の割り当て結果
  static
  Bool3
  node_gval(TpgNode* node,
	    const vector<Bool3>& model);

  /// @brief ノードの故障値を読み出す．
  /// @param[in] node 対象のノード
  /// @param[in] model SAT の割り当て結果
  static
  Bool3
  node_fval(TpgNode* node,
	    const vector<Bool3>& model);

  /// @brief ノードの故障差を読み出す．
  /// @param[in] node 対象のノード
  /// @param[in] model SAT の割り当て結果
  static
  Bool3
  node_dval(TpgNode* node,
	    const vector<Bool3>& model);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数番号に対応する値を読み出す．
  /// @param[in] vid 変数番号
  /// @param[in] model SAT の割り当て結果
  static
  Bool3
  read_value(VarId vid,
	     const vector<Bool3>& model);

  /// @brief リテラルに対応する値を読み出す．
  /// @param[in] lit リテラル
  /// @param[in] model SAT の割り当て結果
  static
  Bool3
  read_value(Literal lit,
	     const vector<Bool3>& model);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // TvMgr
  TvMgr& mTvMgr;

  // 現在処理中のテストベクタ
  TestVector* mCurPattern;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノードの正常値を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
BtBase::node_gval(TpgNode* node,
		  const vector<Bool3>& model)
{
  return read_value(node->gvar(), model);
}

// @brief ノードの故障値を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
BtBase::node_fval(TpgNode* node,
		  const vector<Bool3>& model)
{
  return read_value(node->fvar(), model);
}

// @brief ノードの故障差を読み出す．
// @param[in] node 対象のノード
// @param[in] model SAT の割り当て結果
inline
Bool3
BtBase::node_dval(TpgNode* node,
		  const vector<Bool3>& model)
{
  return read_value(node->dvar(), model);
}

// @brief 変数番号に対応する値を読み出す．
// @param[in] vid 変数番号
// @param[in] model SAT の割り当て結果
inline
Bool3
BtBase::read_value(VarId vid,
		   const vector<Bool3>& model)
{
  return model[vid.val()];
}

// @brief リテラルに対応する値を読み出す．
// @param[in] lit リテラル
// @param[in] model SAT の割り当て結果
inline
Bool3
BtBase::read_value(Literal lit,
		   const vector<Bool3>& model)
{
  Bool3 val = read_value(lit.varid(), model);
  if ( lit.is_negative() ) {
    val = ~val;
  }
  return val;
}

END_NAMESPACE_YM_SATPG

#endif // BTBASE_H
