
/// @file MvnNodeBase.cc
/// @brief MvnNodeBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnNodeBase.h"
#include "ym_networks/MvnMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvnNodeBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] type 型
// @param[in] input_num 入力数
// @param[in] output_num 出力数
MvnNodeBase::MvnNodeBase(MvnModule* parent,
			 tType type,
			 ymuint input_num) :
  MvnNode(parent),
  mType(type),
  mInputNum(input_num),
  mInputArray(new MvnInputPin[input_num])
{
  for (ymuint i = 0; i < input_num; ++ i) {
    mInputArray[i].init(this, i);
  }
}

// @brief デストラクタ
MvnNodeBase::~MvnNodeBase()
{
}

// @brief ノードの種類を得る．
MvnNode::tType
MvnNodeBase::type() const
{
  return mType;
}

// @brief 入力ピン数を得る．
ymuint
MvnNodeBase::input_num() const
{
  return mInputNum;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
const MvnInputPin*
MvnNodeBase::input(ymuint pos) const
{
  assert_cond( pos < mInputNum, __FILE__, __LINE__);
  return mInputArray + pos;
}

// @brief 入力ピンを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() )
MvnInputPin*
MvnNodeBase::_input(ymuint pos)
{
  assert_cond( pos < mInputNum, __FILE__, __LINE__);
  return mInputArray + pos;
}

// @brief クロック信号の極性を得る．
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::clock_pol() const
{
  return 0;
}

// @brief 非同期セット信号の極性を得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @retval 1 正極性(posedge)
// @retval 0 負極性(negedge)
// @note type() が kDff の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::control_pol(ymuint pos) const
{
  return 0;
}

// @brief 非同期セットの値を表す定数ノードを得る．
// @param[in] pos 位置 ( 0 <= pos < input_num() - 2 )
// @note デフォルトの実装では NULL を返す．
const MvnNode*
MvnNodeBase::control_val(ymuint pos) const
{
  return NULL;
}

// @brief ビット位置を得る．
// @note type() が kConstBitSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::bitpos() const
{
  return 0;
}

// @brief 範囲指定の MSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::msb() const
{
  return 0;
}

// @brief 範囲指定の LSB を得る．
// @note type() が kConstPartSelect の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::lsb() const
{
  return 0;
}

// @brief 定数値を得る．
// @param[out] val 値を格納するベクタ
// @note type() が kConst の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
void
MvnNodeBase::const_value(vector<ymuint32>& val) const
{
}

// @brief Xマスクを得る．
// @param[out] val 値を格納するベクタ
// @note type() が kEqX の時のみ意味を持つ．
// @note デフォルトの実装ではなにもしない．
void
MvnNodeBase::xmask(vector<ymuint32>& val) const
{
}

// @brief セルを得る．
// @note type() が kCell の時のみ意味をモツ．
// @note デフォルトの実装では NULL を返す．
const Cell*
MvnNodeBase::cell() const
{
  return NULL;
}

// @brief セルの出力ピン番号を返す．
// @note type() が kCell の時のみ意味を持つ．
// @note デフォルトの実装では 0 を返す．
ymuint
MvnNodeBase::cell_opin_pos() const
{
  return 0;
}

// @brief 多出力セルノードの場合の代表ノードを返す．
// @note type() が kCell の時のみ意味を持つ．
// @note 1出力セルノードの時には自分自身を返す．
// @note デフォルトの実装では NULL を返す．
const MvnNode*
MvnNodeBase::cell_node() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// MvnMgr のノード生成関数
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_input(MvnModule* module,
		  ymuint bit_width)
{
  MvnNode* node = new MvnNodeBase(module, MvnNode::kInput, 0);
  reg_node(node);
  node->mBitWidth = bit_width;
  return node;
}

// @brief 出力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_output(MvnModule* module,
		   ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kOutput,
			       bit_width, bit_width);
  return node;
}

// @brief 入出力ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_inout(MvnModule* module,
		  ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kInout,
			       bit_width, bit_width);
  return node;
}

// @brief ラッチノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_latch(MvnModule* module,
		  ymuint bit_width)
{
  MvnNode* node = new_binary_op(module, MvnNode::kLatch,
				bit_width, 1, bit_width);
  return node;
}

// @brief through ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_through(MvnModule* module,
		    ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kThrough,
			       bit_width, bit_width);
  return node;
}

// @brief not ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_not(MvnModule* module,
		ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kNot,
			       bit_width, bit_width);
  return node;
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
  MvnNode* node = new_log_op(module, MvnNode::kAnd,
			     input_num, bit_width);
  return node;
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
  MvnNode* node = new_log_op(module, MvnNode::kOr,
			     input_num, bit_width);
  return node;
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
  MvnNode* node = new_log_op(module, MvnNode::kXor,
			     input_num, bit_width);
  return node;
}

// @brief reduction and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rand(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kRand,
			       bit_width, 1);
  return node;
}

// @brief reduction or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ror(MvnModule* module,
		ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kRor,
			       bit_width, 1);
  return node;
}

// @brief reduction xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_rxor(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kRxor,
			       bit_width, 1);
  return node;
}

// @brief cmpl ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
// @note 2の補数を計算する．
MvnNode*
MvnMgr::new_cmpl(MvnModule* module,
		 ymuint bit_width)
{
  MvnNode* node = new_unary_op(module, MvnNode::kCmpl,
			       bit_width, bit_width);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kAdd,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kSub,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kMult,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kDiv,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kMod,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kPow,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kSll,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kSrl,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kSla,
				bit_width1, bit_width2, bit_width3);
  return node;
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
  MvnNode* node = new_binary_op(module, MvnNode::kSra,
				bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief equal ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_equal(MvnModule* module,
		  ymuint bit_width)
{
  MvnNode* node = new_binary_op(module, MvnNode::kEq,
				bit_width, bit_width, 1);
  return node;
}

// @brief less than ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_lt(MvnModule* module,
	       ymuint bit_width)
{
  MvnNode* node = new_binary_op(module, MvnNode::kLt,
				bit_width, bit_width, 1);
  return node;
}

// @brief 可変 bit-selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 ビット指定入力のビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_bitselect(MvnModule* module,
		      ymuint bit_width1,
		      ymuint bit_width2)
{
  MvnNode* node = new_binary_op(module, MvnNode::kBitSelect,
				bit_width1, bit_width2, 1);
  return node;
}

// @brief 可変 part-select ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 範囲指定入力のビット幅
// @param[in] bit_width3 範囲のビット幅(出力のビット幅)
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_partselect(MvnModule* module,
		       ymuint bit_width1,
		       ymuint bit_width2,
		       ymuint bit_width3)
{
  MvnNode* node = new_binary_op(module, MvnNode::kPartSelect,
				bit_width1, bit_width2, bit_width3);
  return node;
}

// @brief condition ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvnNode*
MvnMgr::new_ite(MvnModule* module,
		ymuint bit_width)
{
  MvnNode* node = new_ternary_op(module, MvnNode::kIte,
				 1, bit_width, bit_width, bit_width);
  return node;
}

// @brief concatenate ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ibitwidth_array 入力のビット幅の配列
// @return 生成したノードを返す．
// @note 出力のビット幅は入力のビット幅の和となる．
MvnNode*
MvnMgr::new_concat(MvnModule* module,
		   const vector<ymuint>& ibitwidth_array)
{
  ymuint ni = ibitwidth_array.size();
  ymuint obitwidth = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    obitwidth += ibitwidth_array[i];
  }
  return new_nary_op(module, MvnNode::kConcat, ibitwidth_array, obitwidth);
}

// @brief 多入力論理演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] input_num 入力数
// @param[in] bit_width ビット幅
MvnNode*
MvnMgr::new_log_op(MvnModule* module,
		   MvnNode::tType type,
		   ymuint input_num,
		   ymuint bit_width)
{
  vector<ymuint> ibitwidth_array(input_num);
  for (ymuint i = 0; i < input_num; ++ i) {
    ibitwidth_array[i] = bit_width;
  }
  return new_nary_op(module, type, ibitwidth_array, bit_width);
}

// @brief 1入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力のビット幅
// @param[in] bit_width2 出力のビット幅
MvnNode*
MvnMgr::new_unary_op(MvnModule* module,
		     MvnNode::tType type,
		     ymuint bit_width1,
		     ymuint bit_width2)
{
  MvnNode* node = new MvnNodeBase(module, type, 1);
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->mBitWidth = bit_width2;

  return node;
}

// @brief 2入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
MvnNode*
MvnMgr::new_binary_op(MvnModule* module,
		      MvnNode::tType type,
		      ymuint bit_width1,
		      ymuint bit_width2,
		      ymuint bit_width3)
{
  MvnNode* node = new MvnNodeBase(module, type, 2);
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->_input(1)->mBitWidth = bit_width2;
  node->mBitWidth = bit_width3;

  return node;
}

// @brief 3入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 入力3のビット幅
// @param[in] bit_width4 出力のビット幅
MvnNode*
MvnMgr::new_ternary_op(MvnModule* module,
		       MvnNode::tType type,
		       ymuint bit_width1,
		       ymuint bit_width2,
		       ymuint bit_width3,
		       ymuint bit_width4)
{
  MvnNode* node = new MvnNodeBase(module, type, 3);
  reg_node(node);

  node->_input(0)->mBitWidth = bit_width1;
  node->_input(1)->mBitWidth = bit_width2;
  node->_input(2)->mBitWidth = bit_width3;
  node->mBitWidth = bit_width4;

  return node;
}

// @brief 多入力演算ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] type 型
// @param[in] ibit_width_array 入力のビット幅の幅
// @param[in] obit_width 出力のビット幅
MvnNode*
MvnMgr::new_nary_op(MvnModule* module,
		    MvnNode::tType type,
		    const vector<ymuint>& ibit_width_array,
		    ymuint obit_width)
{
  ymuint ni = ibit_width_array.size();
  MvnNode* node = new MvnNodeBase(module, type, ni);
  reg_node(node);

  for (ymuint i = 0; i < ni; ++ i) {
    node->_input(i)->mBitWidth = ibit_width_array[i];
  }
  node->mBitWidth = obit_width;

  return node;
}

END_NAMESPACE_YM_NETWORKS_MVN

