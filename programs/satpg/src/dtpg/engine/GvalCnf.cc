
/// @file GvalCnf.cc
/// @brief GvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GvalCnf.h"
#include "NodeSet.h"
#include "NodeValList.h"
#include "SatEngine.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス GvalCnf
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_node_id ノード番号の最大値
GvalCnf::GvalCnf(ymuint max_node_id) :
  mMaxId(max_node_id),
  mMark(max_node_id, false),
  mVarMap(max_node_id)
{
}

// @brief デストラクタ
GvalCnf::~GvalCnf()
{
}

// @brief 初期化する．
// @param[in] max_node_id ノード番号の最大値
void
GvalCnf::init(ymuint max_node_id)
{
  mMaxId = max_node_id;
  mMark.clear();
  mMark.resize(max_node_id, false);
  mVarMap.init(max_node_id);
}

#if 0
// @brief NodeSet の内容に応じてCNFを作る．
// @param[in] engine SATエンジン
// @param[in] node_set 対象のノード集合
//
// 具体的には tfo_tfi_node() を対象にする．
void
GvalCnf::make_cnf(SatEngine& engine,
		  const NodeSet& node_set)
{
  ymuint n = node_set.tfo_tfi_size();
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( mMark[node->id()] ) {
      continue;
    }
    VarId gvar = engine.new_var();
    mVarMap.set_vid(node, gvar);
  }
  for (ymuint i = 0; i < n; ++ i) {
    const TpgNode* node = node_set.tfo_tfi_node(i);
    if ( mMark[node->id()] ) {
      continue;
    }
    mMark[node->id()] = true;
    engine.make_node_cnf(node, mVarMap);
  }
}

// @brief ノードのTFI全体のCNFを作る．
// @param[in] engine SATエンジン
// @param[in] node ノード
void
GvalCnf::make_cnf(SatEngine& engine,
		  const TpgNode* node)
{
  if ( mMark[node->id()] ) {
    return;
  }
  mMark[node->id()] = true;

  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const TpgNode* inode = node->fanin(i);
    make_cnf(engine, inode);
  }

  VarId gvar = engine.new_var();
  mVarMap.set_vid(node, gvar);
  engine.make_node_cnf(node, mVarMap);
}

// @brief 割当リストに対応する仮定を追加する．
// @param[in] engine SATエンジン
// @param[in] assign_list 割当リスト
void
GvalCnf::add_assumption(SatEngine& engine,
			const NodeValList& assign_list)
{
  ymuint n = assign_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    NodeVal nv = assign_list[i];
    const TpgNode* node = nv.node();
    make_cnf(engine, node);
    Literal alit(mVarMap(node), false);
    if ( nv.val() ) {
      engine.assumption_add(alit);
    }
    else {
      engine.assumption_add(~alit);
    }
  }
}
#endif

END_NAMESPACE_YM_SATPG
