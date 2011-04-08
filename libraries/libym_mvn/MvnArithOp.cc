
/// @file libym_mvn/MvnArithOp.cc
/// @brief MvnArithOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnArithOp.h"
#include "ym_mvn/MvnMgr.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnAdd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnAdd::MvnAdd(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnAdd::~MvnAdd()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnAdd::type() const
{
  return kAdd;
}

// @brief add ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_add(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnAdd(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSub
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnSub::MvnSub(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnSub::~MvnSub()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSub::type() const
{
  return kSub;
}

// @brief sub ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sub(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnSub(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnMult
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnMult::MvnMult(MvnModule* module,
		 ymuint bit_width1,
		 ymuint bit_width2,
		 ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnMult::~MvnMult()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnMult::type() const
{
  return kMult;
}

// @brief mult ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_mult(MvnModule* module,
		 ymuint bit_width1,
		 ymuint bit_width2,
		 ymuint bit_width3)
{
  MvnNode* node = new MvnMult(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnDiv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnDiv::MvnDiv(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnDiv::~MvnDiv()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnDiv::type() const
{
  return kDiv;
}

// @brief div ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_div(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnDiv(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnMod
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnMod::MvnMod(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnMod::~MvnMod()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnMod::type() const
{
  return kMod;
}

// @brief modulo ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_mod(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnMod(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnPow
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnPow::MvnPow(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnPow::~MvnPow()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnPow::type() const
{
  return kPow;
}

// @brief power ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_pow(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnPow(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSll
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnSll::MvnSll(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnSll::~MvnSll()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSll::type() const
{
  return kSll;
}

// @brief shift left logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sll(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnSll(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSrl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnSrl::MvnSrl(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnSrl::~MvnSrl()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSrl::type() const
{
  return kSrl;
}

// @brief shift right logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_srl(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnSrl(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSla
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnSla::MvnSla(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnSla::~MvnSla()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSla::type() const
{
  return kSla;
}

// @brief shift left arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sla(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnSla(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnSra
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnSra::MvnSra(MvnModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3) :
  MvnBinaryOp(module, bit_width1, bit_width2, bit_width3)
{
}

// @brief デストラクタ
MvnSra::~MvnSra()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnSra::type() const
{
  return kSra;
}

// @brief shift right arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_sra(MvnModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvnNode* node = new MvnSra(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

END_NAMESPACE_YM_MVN
