
/// @file libym_mvn/MvUdp.cc
/// @brief MvUdp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvUdp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvCombUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvCombUdp::MvCombUdp(MvModule* module,
		     ymuint ni) :
  MvNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_bit_width(_input(i), 1);
  }
  set_bit_width(_output(0), 1);
}

// @brief デストラクタ
MvCombUdp::~MvCombUdp()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvCombUdp::type() const
{
  return kCombUdp;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSeqUdp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] ni 入力数
MvSeqUdp::MvSeqUdp(MvModule* module,
		   ymuint ni) :
  MvNode(module, ni, 1)
{
  for (ymuint i = 0; i < ni; ++ i) {
    set_bit_width(_input(i), 1);
  }
  set_bit_width(_output(0), 1);
}

// @brief デストラクタ
MvSeqUdp::~MvSeqUdp()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSeqUdp::type() const
{
  return kSeqUdp;
}

END_NAMESPACE_YM_MVN
