#ifndef BTZDD_H
#define BTZDD_H

/// @file BtZdd.h
/// @brief BtZdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"
#include "YmLogic/ZddMgr.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtZdd BtZdd.h "BtZdd.h"
/// @brief Zdd を用いた backtracer
//////////////////////////////////////////////////////////////////////
class BtZdd :
  public BtJustBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvmgr TvMgr
  BtZdd(TvMgr& tvmgr);

  /// @brief デストラクタ
  virtual
  ~BtZdd();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードID番号の最大値を設定する．
  /// @param[in] max_id ID番号の最大値
  virtual
  void
  set_max_id(ymuint max_id);

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  TestVector*
  operator()(TpgNode* fnode,
	     const vector<Bool3>& model,
	     const vector<TpgNode*>& input_list,
	     const vector<TpgNode*>& output_list);


protected:
  //////////////////////////////////////////////////////////////////////
  // BtJustBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief clear_justified() 中で呼ばれるフック関数
  virtual
  void
  clear_justified_hook(TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの値を正当化する入力の組み合わせを求める．
  /// @param[in] node 対象のノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  Zdd
  justify(TpgNode* node,
	  const vector<Bool3>& model);

  /// @brief すべてのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  Zdd
  just_sub1(TpgNode* node,
	    const vector<Bool3>& model);

  /// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
  /// @param[in] node 対象のノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  /// @param[in] val 値
  Zdd
  just_sub2(TpgNode* node,
	    const vector<Bool3>& model,
	    Bool3 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ZDD マネージャ
  ZddMgr mMgr;

  // node->id() をキーにして ZDD を納める配列
  vector<Zdd> mJustArray;

};

END_NAMESPACE_YM_SATPG

#endif // BTZDD_H
