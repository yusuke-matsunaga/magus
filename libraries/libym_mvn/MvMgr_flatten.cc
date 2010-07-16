
/// @file libym_mvn/MvMgr_flatten.cc
/// @brief MvMgr::flatten() の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

// @brief 階層構造をなくす．
void
MvMgr::flatten()
{
  ymuint n = max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvModule* module = this->module(i);
    if ( module == NULL || module->parent() != NULL ) continue;
    flatten_sub(module, module);
  }
}

// @brief flatten() の下請け関数
// @param[in] module 対象のモジュール
// @param[in] top_module トップモジュール
// @note 大まかには module のノードを top_module に移す．
void
MvMgr::flatten_sub(MvModule* module,
		   MvModule* top_module)
{
  ymuint n = module->max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvNode* node = module->node(i);
    if ( node == NULL ) continue;
    if ( node->type() == MvNode::kInst ) {
      MvModule* module1 = node->_module();
      flatten_sub(module1, top_module);
      // 入出力をバイパスする．
    }
    else if ( module != top_module ) {
      // node を module から top_module に移す．
    }
  }
}

END_NAMESPACE_YM_MVN
