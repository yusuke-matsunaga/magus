
/// @file libym_mvn/MvnLogOp.cc
/// @brief MvnLogOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnLogOp.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnLogOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @note ビット幅はすべての入力，出力で同一
MvnLogOp::MvnLogOp(MvnModule* module,
		   ymuint input_num,
		   ymuint bit_width) :
  MvnNode(module, input_num, 1)
{
  for (ymuint i = 0; i < input_num; ++ i) {
    set_ipin_bit_width(i, bit_width);
  }
  set_opin_bit_width(0, bit_width);
}

// @brief デストラクタ
MvnLogOp::~MvnLogOp()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvnAnd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvnAnd::MvnAnd(MvnModule* module,
	       ymuint input_num,
	       ymuint bit_width) :
  MvnLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvnAnd::~MvnAnd()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnAnd::type() const
{
  return kAnd;
}

// @brief and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_and(MvnModule* module,
		ymuint input_num,
		ymuint bit_width)
{
  MvnNode* node = new MvnAnd(module, input_num, bit_width);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnOr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvnOr::MvnOr(MvnModule* module,
	     ymuint input_num,
	     ymuint bit_width) :
  MvnLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvnOr::~MvnOr()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnOr::type() const
{
  return kOr;
}

// @brief or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_or(MvnModule* module,
	       ymuint input_num,
	       ymuint bit_width)
{
  MvnNode* node = new MvnOr(module, input_num, bit_width);
  reg_node(node);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnXor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvnXor::MvnXor(MvnModule* module,
	       ymuint input_num,
	       ymuint bit_width) :
  MvnLogOp(module, input_num, bit_width)
{
}

// @brief デストラクタ
MvnXor::~MvnXor()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnXor::type() const
{
  return kXor;
}

// @brief xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_xor(MvnModule* module,
		ymuint input_num,
		ymuint bit_width)
{
  MvnNode* node = new MvnXor(module, input_num, bit_width);
  reg_node(node);

  return node;
}

END_NAMESPACE_YM_MVN
