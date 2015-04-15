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


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FvalCnf FvalCnf.h "FvalCnf.h"
/// @brief 故障回路のCNF式を作るためのクラス
//////////////////////////////////////////////////////////////////////
class FvalCnf
{
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

  /// @brief 故障回路のCNFを作る．
  /// @param[in] engine SATエンジン
  /// @param[in] fault 故障
  void
  make_cnf(SatEngine& engine,
	   TpgFault* fault);

  /// @brief 変数マップを得る．
  const VidMap&
  var_map() const;

  /// @brief 故障検出用の変数番号を返す．
  VarId
  fd_var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

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
