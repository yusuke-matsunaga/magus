
/// @file libym_mvn/MvArithOp.cc
/// @brief MvArithOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvArithOp.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvAdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvAdd::MvAdd(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvAdd::~MvAdd()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvAdd::type() const
{
  return kAdd;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSub
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSub::MvSub(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvSub::~MvSub()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSub::type() const
{
  return kSub;
}


//////////////////////////////////////////////////////////////////////
// クラス MvMult
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvMult::MvMult(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvMult::~MvMult()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvMult::type() const
{
  return kMult;
}


//////////////////////////////////////////////////////////////////////
// クラス MvDiv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvDiv::MvDiv(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvDiv::~MvDiv()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDiv::type() const
{
  return kDiv;
}


//////////////////////////////////////////////////////////////////////
// クラス MvMod
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvMod::MvMod(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvMod::~MvMod()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvMod::type() const
{
  return kMod;
}


//////////////////////////////////////////////////////////////////////
// クラス MvPow
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvPow::MvPow(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvPow::~MvPow()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvPow::type() const
{
  return kPow;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSll
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSll::MvSll(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvSll::~MvSll()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSll::type() const
{
  return kSll;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSrl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSrl::MvSrl(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvSrl::~MvSrl()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSrl::type() const
{
  return kSrl;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSla
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSla::MvSla(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvSla::~MvSla()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSla::type() const
{
  return kSla;
}


//////////////////////////////////////////////////////////////////////
// クラス MvSra
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvSra::MvSra(MvModule* module,
	     ymuint bit_width1,
	     ymuint bit_width2,
	     ymuint bit_width3) :
  MvBinaryOp(module, bit_width1, bit_width2, bit_width2)
{
}

// @brief デストラクタ
MvSra::~MvSra()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvSra::type() const
{
  return kSra;
}

END_NAMESPACE_YM_MVN
