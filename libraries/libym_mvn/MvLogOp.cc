
/// @file libym_mvn/MvLogOp.cc
/// @brief MvLogOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvLogOp.h"
#include "ym_mvn/MvMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvLogOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @note ビット幅はすべての入力，出力で同一
MvLogOp::MvLogOp(MvModule* module,
		 ymuint input_num,
		 ymuint bit_width) :
  MvNode(module, input_num, 1)
{
  for (ymuint i = 0; i < input_num; ++ i) {
    set_ipin_bit_width(i, bit_width);
  }
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvLogOp::~MvLogOp()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvAnd::MvAnd(MvModule* module,
	     ymuint input_num,
	     ymuint bit_width) :
  MvLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvAnd::~MvAnd()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvAnd::type() const
{
  return kAnd;
}

// @brief and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_and(MvModule* module,
	       ymuint input_num,
	       ymuint bit_width)
{
  MvNode* node = new MvAnd(module, input_num, bit_width);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvOr::MvOr(MvModule* module,
	   ymuint input_num,
	   ymuint bit_width) :
  MvLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvOr::~MvOr()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvOr::type() const
{
  return kOr;
}

// @brief or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_or(MvModule* module,
	      ymuint input_num,
	      ymuint bit_width)
{
  MvNode* node = new MvOr(module, input_num, bit_width);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvXor::MvXor(MvModule* module,
	     ymuint input_num,
	     ymuint bit_width) :
  MvLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvXor::~MvXor()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvXor::type() const
{
  return kXor;
}

// @brief xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_xor(MvModule* module,
	       ymuint input_num,
	       ymuint bit_width)
{
  MvNode* node = new MvXor(module, input_num, bit_width);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
