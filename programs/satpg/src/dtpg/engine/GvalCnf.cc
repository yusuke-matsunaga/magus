
/// @file GvalCnf.cc
/// @brief GvalCnf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GvalCnf.h"
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

// @brief 変数マップを得る．
const VidMap&
GvalCnf::var_map() const
{
  return mVarMap;
}

END_NAMESPACE_YM_SATPG
