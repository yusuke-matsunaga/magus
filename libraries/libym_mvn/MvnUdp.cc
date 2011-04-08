
/// @file libym_mvn/MvnUdp.cc
/// @brief MvnUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnUdp.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnCombUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvnCombUdp::MvnCombUdp(MvnModule* module,
		       ymuint ni) :
  MvnNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_ipin_bit_width(i, 1);
  }
  set_opin_bit_width(0, 1);
}

// @brief デストラクタ
MvnCombUdp::~MvnCombUdp()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnCombUdp::type() const
{
  return kCombUdp;
}

// @brief combinational UDP ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ni 入力数
// @note ビット幅はすべて1ビット
MvnNode*
MvnMgr::new_combudp(MvnModule* module,
		    ymuint ni)
{
  MvnNode* node = new MvnCombUdp(module, ni);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSeqUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvnSeqUdp::MvnSeqUdp(MvnModule* module,
		     ymuint ni) :
  MvnNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_ipin_bit_width(i, 1);
  }
  set_opin_bit_width(0, 1);
}

// @brief デストラクタ
MvnSeqUdp::~MvnSeqUdp()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSeqUdp::type() const
{
  return kSeqUdp;
}

// @brief sequential UDP ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ni 入力数
// @note ビット幅はすべて1ビット
MvnNode*
MvnMgr::new_sequdp(MvnModule* module,
		   ymuint ni)
{
  MvnNode* node = new MvnSeqUdp(module, ni);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
