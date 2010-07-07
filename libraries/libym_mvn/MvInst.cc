
/// @file libym_mvn/MvInst.cc
/// @brief MvInst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvInst.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvInst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] submodule 子のモジュール
MvInst::MvInst(MvModule* module,
	       MvModule* submodule) :
  MvNode(module, submodule->input_num(), submodule->output_num()),
  mSubmodule(submodule)
{
}

// @brief デストラクタ
MvInst::~MvInst()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvInst::type() const
{
  return kInst;
}

// @brief 下位モジュールを得る．
// @note type() が kInst の時のみ意味を持つ．
const MvModule*
MvInst::module() const
{
  return mSubmodule;
}

END_NAMESPACE_YM_MVN
