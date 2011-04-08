
/// @file libym_mvn/MvnInst.cc
/// @brief MvnInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnInst.h"
#include "ym_mvn/MvnMgr.h"
#include "ym_mvn/MvnModule.h"



BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnInst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] submodule 子のモジュール
MvnInst::MvnInst(MvnModule* module,
		 MvnModule* submodule) :
  MvnNode(module, submodule->input_num(), submodule->output_num()),
  mSubmodule(submodule)
{
}

// @brief デストラクタ
MvnInst::~MvnInst()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnInst::type() const
{
  return kInst;
}

// @brief 下位モジュールを得る．
// @note type() が kInst の時のみ意味を持つ．
const MvnModule*
MvnInst::module() const
{
  return mSubmodule;
}

// @brief module instance ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] submodule 下位モジュール
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_inst(MvnModule* module,
		 MvnModule* submodule)
{
  MvnNode* node = new MvnInst(module, submodule);
  submodule->mParent = node;
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
