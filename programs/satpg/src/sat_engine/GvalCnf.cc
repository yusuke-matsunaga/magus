
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


END_NAMESPACE_YM_SATPG
