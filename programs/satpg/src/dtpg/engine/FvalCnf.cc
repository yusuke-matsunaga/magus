
/// @file FvalCnf.cc
/// @brief FvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FvalCnf.h"
#include "GvalCnf.h"
#include "SatEngine.h"
#include "TpgNode.h"
#include "TpgFault.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス FvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
// @param[in] gval_cnf 正常回路のCNFを作るクラス
FvalCnf::FvalCnf(ymuint max_node_id,
		 GvalCnf& gval_cnf) :
  mMaxId(max_node_id),
  mGvalCnf(gval_cnf),
  mMark(max_node_id),
  mFvarMap(max_node_id),
  mDvarMap(max_node_id)
{
}

// @brief デストラクタ
FvalCnf::~FvalCnf()
{
}

// @brief 初期化する．
// @param[in] max_node_id ノード番号の最大値
void
FvalCnf::init(ymuint max_node_id)
{
  mMaxId = max_node_id;
  mMark.clear();
  mMark.resize(max_node_id, false);
  mFvarMap.init(max_node_id);
  mDvarMap.init(max_node_id);
}

// @brief 故障回路のCNFを作る．
// @param[in] engine SATエンジン
// @param[in] fault 故障
void
FvalCnf::make_cnf(SatEngine& engine,
		  TpgFault* fault)
{
  const TpgNode* fnode = fault->node();

  mFdVar = engine.new_var();

  mFconeList.clear();
  mOutputList.clear();
  mark_tfo(engine, fnode);

  ymuint nfc = mFconeList.size();
  for (ymuint i = 0; i < nfc; ++ i) {
    const TpgNode* node = mFconeList[i];

    mGvalCnf.make_cnf(engine, node);

    if ( node == fnode ) {
      engine.make_fault_cnf(fault, mGvalCnf.var_map(), mFvarMap);
    }
    else {
      ymuint ni = node->fanin_num();
      for (ymuint i = 0; i < ni; ++ i) {
	const TpgNode* inode = node->fanin(i);
	if ( !mMark[inode->id()] ) {
	  // 故障ファンアウトコーンの外側(境界)のノードの場合には
	  // 正常回路の変数番号をコピーしておく．
	  const VidMap& gvar_map = mGvalCnf.var_map();
	  mFvarMap.set_vid(inode, gvar_map(inode));
	}
      }

      engine.make_node_cnf(node, mFvarMap);
    }
    engine.make_dchain_cnf(node, mGvalCnf.var_map(), mFvarMap, mDvarMap);
  }

  ymuint npo = mOutputList.size();
  engine.tmp_lits_begin(npo + 1);
  Literal fdlit(mFdVar);
  for (ymuint i = 0; i < npo; ++ i) {
    const TpgNode* node = mOutputList[i];
    Literal dlit(mDvarMap(node));
    engine.tmp_lits_add(dlit);
    engine.add_clause(fdlit, ~dlit);
  }
  engine.tmp_lits_add(~fdlit);
  engine.tmp_lits_end();

  for (const TpgNode* node = fnode; node != NULL; node = node->imm_dom()) {
    Literal dlit(mDvarMap(node), false);
    engine.add_clause(~fdlit, dlit);
  }
}

// @brief 変数マップを得る．
const VidMap&
FvalCnf::var_map() const
{
  return mFvarMap;
}

// @brief 故障検出用の変数番号を返す．
VarId
FvalCnf::fd_var() const
{
  return mFdVar;
}

// @brief TFO にマークをつけてCNF式を作る．
// @param[in] engine SATエンジン
// @param[in] node ノード
void
FvalCnf::mark_tfo(SatEngine& engine,
		  const TpgNode* node)
{
  if ( mMark[node->id()] ) {
    return;
  }
  mMark[node->id()] = true;

  VarId fvar = engine.new_var();
  mFvarMap.set_vid(node, fvar);
  VarId dvar = engine.new_var();
  mDvarMap.set_vid(node, dvar);

  mFconeList.push_back(node);
  if ( node->is_output() ) {
    mOutputList.push_back(node);
  }

  ymuint nfo = node->active_fanout_num();
  for (ymuint i = 0; i < nfo; ++ i) {
    const TpgNode* onode = node->active_fanout(i);
    mark_tfo(engine, onode);
  }
}

END_NAMESPACE_YM_SATPG
