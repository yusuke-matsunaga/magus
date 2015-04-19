#ifndef FVALCNF_H
#define FVALCNF_H

/// @file FvalCnf.h
/// @brief FvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "GenVidMap.h"
#include "Val3.h"
#include "YmLogic/Bool3.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FvalCnf FvalCnf.h "FvalCnf.h"
/// @brief 故障回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class FvalCnf
{
  friend class SatEngine;

public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値
  /// @param[in] gval_cnf 正常回路のCNFを作るクラス
  FvalCnf(ymuint max_node_id,
	  GvalCnf& gval_cnf);

  /// @brief デストラクタ
  ~FvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値
  void
  init(ymuint max_node_id);

  /// @brief 十分割当リストを求める．
  /// @param[in] sat_model SAT問題の解
  /// @param[in] fault 故障
  /// @param[in] node_set 故障に関連するノード集合
  /// @param[out] suf_list 十分割当リストを格納する変数
  /// @param[out] pi_suf_list 外部入力上の十分割当リストを格納する変数
  void
  get_pi_suf_list(const vector<Bool3>& sat_model,
		  TpgFault* fault,
		  const NodeSet& node_set,
		  NodeValList& suf_list,
		  NodeValList& pi_suf_list);

  /// @brief 正常回路のCNFを生成するクラスを返す．
  GvalCnf&
  gval_cnf();

  /// @brief 正常回路の変数マップを得る．
  const VidMap&
  gvar_map() const;

  /// @brief 故障回路の変数マップを得る．
  const VidMap&
  fvar_map() const;

  /// @brief 故障検出用の変数番号を返す．
  VarId
  fd_var() const;

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障回路のCNFを作る．
  /// @param[in] engine SATエンジン
  /// @param[in] fault 故障
  /// @param[in] node_set 関係するノード集合
  /// @param[in] detect 検出条件
  ///
  /// detect = kVal0: 検出しないCNFを作る．
  ///        = kVal1: 検出するCNFを作る．
  ///        = kValX: fd_var() で制御するCNFを作る．
  void
  make_cnf(SatEngine& engine,
	   TpgFault* fault,
	   const NodeSet& node_set,
	   Val3 detect);

  /// @brief TFO にマークをつけてCNF式を作る．
  /// @param[in] engine SATエンジン
  /// @param[in] node ノード
  void
  mark_tfo(SatEngine& engine,
	   const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxId;

  // 正常回路のCNFを作るクラス
  GvalCnf& mGvalCnf;

  // 処理済みのノードの印
  vector<bool> mMark;

  // 故障値の変数マップ
  GenVidMap mFvarMap;

  // 故障伝搬値の変数マップ
  GenVidMap mDvarMap;

  // 故障検出用の変数番号
  VarId mFdVar;

  // 故障に関係するノードのリスト
  vector<const TpgNode*> mFconeList;

  // 故障に関係する外部出力ノードのリスト
  vector<const TpgNode*> mOutputList;

};

END_NAMESPACE_YM_SATPG

#endif // FVALCNF_H
