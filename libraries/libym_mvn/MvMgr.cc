
/// @file libym_mvn/MvMgr.cc
/// @brief MvMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvMgr.h"

#include "ym_mvn/MvModule.h"

#include "MvInput.h"
#include "MvOutput.h"
#include "MvThrough.h"
#include "MvNot.h"
#include "MvLogOp.h"
#include "MvRop.h"
#include "MvRelOp.h"
#include "MvArithOp.h"
#include "MvIte.h"
#include "MvConcat.h"
#include "MvInst.h"
#include "MvConst.h"

#include "ym_mvn/MvNet.h"


BEGIN_NAMESPACE_YM_MVN

BEGIN_NONAMESPACE

// MvInputPin の内容を出力する．
void
dump_inputpin(ostream& s,
	      const MvInputPin* pin)
{
  s << "InputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
}

// MvOutputPin の内容を出力する．
void
dump_outputpin(ostream& s,
	       const MvOutputPin* pin)
{
  s << "OutputPin#" << pin->pos()
    << "(" << pin->bit_width() << ")" << endl;
}

// MvNode の内容を出力する．
void
dump_node(ostream& s,
	  const MvNode* node)
{
  s << "Node#" << node->id()
    << ":";
  switch ( node->type() ) {
  case MvNode::kInput:      s << "Input[" << node->pos() << "]"; break;
  case MvNode::kOutput:     s << "Output[" << node->pos() << "]"; break;
  case MvNode::kThrough:    s << "Through"; break;
  case MvNode::kNot:        s << "Not"; break;
  case MvNode::kAnd:        s << "And"; break;
  case MvNode::kOr:         s << "Or"; break;
  case MvNode::kXor:        s << "Xor"; break;
  case MvNode::kRand:       s << "Rand"; break;
  case MvNode::kRor:        s << "Ror"; break;
  case MvNode::kRxor:       s << "Rxor"; break;
  case MvNode::kEq:         s << "Eq"; break;
  case MvNode::kLt:         s << "Lt"; break;
  case MvNode::kSll:        s << "Sll"; break;
  case MvNode::kSrl:        s << "Srl"; break;
  case MvNode::kSla:        s << "Sla"; break;
  case MvNode::kSra:        s << "Sra"; break;
  case MvNode::kAdd:        s << "Add"; break;
  case MvNode::kSub:        s << "Sub"; break;
  case MvNode::kMult:       s << "Mult"; break;
  case MvNode::kDiv:        s << "Div"; break;
  case MvNode::kMod:        s << "Mod"; break;
  case MvNode::kPow:        s << "Pow"; break;
  case MvNode::kIte:        s << "Ite"; break;
  case MvNode::kConcat:     s << "Concat"; break;
  case MvNode::kBitSelect:  s << "BitSelect"; break;
  case MvNode::kPartSelect: s << "PartSelect"; break;
  case MvNode::kInst:       s << "Inst of Module#"
			      << node->module()->id(); break;
  case MvNode::kConst:      s << "Const"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;
  ymuint ni = node->input_num();
  for (ymuint i = 0; i < ni; ++ i) {
    const MvInputPin* pin = node->input(i);
    dump_inputpin(s, pin);
  }
  ymuint no = node->output_num();
  for (ymuint i = 0; i < no; ++ i) {
    const MvOutputPin* pin = node->output(i);
    dump_outputpin(s, pin);
  }
}

// MvNet の内容を出力する．
void
dump_net(ostream& s,
	 const MvNet* net)
{
  const MvOutputPin* src_pin = net->src_pin();
  const MvInputPin* dst_pin = net->dst_pin();
  
  s << "Output#" << src_pin->pos() << "@Node#" << src_pin->node()->id()
    << "@Module#" << src_pin->node()->parent()->id() << endl
    << " ==>"
    << "Input#" << dst_pin->pos() << "@Node#" << dst_pin->node()->id()
    << "@Module#" << dst_pin->node()->parent()->id() << endl
    << endl;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス MvMgr
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
MvMgr::MvMgr()
{
}

// @brief デストラクタ
MvMgr::~MvMgr()
{
}

// @brief トップレベルモジュールのリストを得る．
// @param[out] module_list モジュールを格納するリスト
// @return 要素数を返す．
// @note この関数はトップモジュール数ではなく全モジュール数に比例した
// 実行時間を要する．
ymuint
MvMgr::topmodule_list(list<MvModule*>& module_list) const
{
  module_list.clear();
  for (vector<MvModule*>::const_iterator p = mModuleArray.begin();
       p != mModuleArray.end(); ++ p) {
    MvModule* module = *p;
    if ( module && module->parent() == NULL ) {
      module_list.push_back(module);
    }
  }
  return module_list.size();
}

// @brief モジュール番号の最大値+1を得る．
ymuint
MvMgr::max_module_id() const
{
  return mModuleArray.size();
}

// @brief モジュールIDをキーにしてモジュールにアクセスする．
// @param[in] id モジュールID ( 0 <= id < max_module_id() )
// @return 該当するモジュールを返す．
// @note 該当するモジュールがない場合は NULL を返す．
MvModule*
MvMgr::module(ymuint id) const
{
  return mModuleArray[id];
}

// @brief モジュールを生成する．
// @param[in] name 名前
// @param[in] ni 入力ノード数
// @param[in] no 出力ノード数
// @return 生成したモジュールを返す．
// @note 入出力ノードのビット幅は1で初期化される．
MvModule*
MvMgr::new_module(const char* name,
		  ymuint ni,
		  ymuint no)
{
  int tmp = mItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // IDが枯渇？
    return NULL;
  }
  mItvlMgr.erase(tmp);
  ymuint id = tmp;
  MvModule* module = new MvModule(name, ni, no);
  module->mId = id;
  while ( mModuleArray.size() <= id ) {
    mModuleArray.push_back(NULL);
  }
  mModuleArray[id] = module;

  for (ymuint i = 0; i < ni; ++ i) {
    new_input(module, i, 1);
  }
  for (ymuint i = 0; i < no; ++ i) {
    new_output(module, i, 1);
  }
  return module;
}

// @brief モジュールを削除する．
// @param[in] module 対象のモジュール
// @note モジュールインスタンスとして使われているモジュールは削除できない．
void
MvMgr::delete_module(MvModule* module)
{
  if ( module->parent() ) {
    // モジュールインスタンスは削除できない．
    return;
  }
  // module の要素を削除

  // module の下位モジュールを再帰的に削除

  mItvlMgr.add(module->mId);
  mModuleArray[module->mId] = NULL;
  delete module;
}

// @brief 入力ノードを生成する．
// @param[in] module 入力ノードが属するモジュール
// @param[in] pos 位置
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_input(MvModule* module,
		 ymuint pos,
		 ymuint bit_width)
{
  MvNode* node = new MvInput(module, pos, bit_width);
  reg_node(node);

  assert_cond( pos < module->input_num(), __FILE__, __LINE__);
  module->mInputArray[pos] = node;

  assert_cond( node->input_num() == 0, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief 出力ノードを生成する．
// @param[in] module 出力ノードが属するモジュール
// @param[in] pos 位置
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_output(MvModule* module,
		  ymuint pos,
		  ymuint bit_width)
{
  MvNode* node = new MvOutput(module, pos, bit_width);
  reg_node(node);

  assert_cond( pos < module->output_num(), __FILE__, __LINE__);
  module->mOutputArray[pos] = node;
  
  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 0, __FILE__, __LINE__);

  return node;
}

// @brief through ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_through(MvModule* module,
		   ymuint bit_width)
{
  MvNode* node = new MvThrough(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief not ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_not(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvNot(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_and(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvAnd(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_or(MvModule* module,
	      ymuint bit_width)
{
  MvNode* node = new MvOr(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_xor(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvXor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief reduction and ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_rand(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvRand(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);
  
  return node;
}

// @brief reduction or ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_ror(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvRor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief reduction xor ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_rxor(MvModule* module,
		ymuint bit_width)
{
  MvNode* node = new MvRxor(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 1, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief equal ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_equal(MvModule* module,
		 ymuint bit_width)
{
  MvNode* node = new MvEq(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief less than ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_lt(MvModule* module,
	      ymuint bit_width)
{
  MvNode* node = new MvLt(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief shift left logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sll(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSll(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief shift right logical ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_srl(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSrl(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief shift left arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sla(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSla(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief shift right arithmetic ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力データのビット幅
// @param[in] bit_width2 シフト量のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sra(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSra(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief add ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_add(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvAdd(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief sub ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_sub(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvSub(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief mult ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_mult(MvModule* module,
		ymuint bit_width1,
		ymuint bit_width2,
		ymuint bit_width3)
{
  MvNode* node = new MvMult(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief div ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_div(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvDiv(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief modulo ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_mod(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvMod(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief power ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width1 入力1のビット幅
// @param[in] bit_width2 入力2のビット幅
// @param[in] bit_width3 出力のビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_pow(MvModule* module,
	       ymuint bit_width1,
	       ymuint bit_width2,
	       ymuint bit_width3)
{
  MvNode* node = new MvPow(module, bit_width1, bit_width2, bit_width3);
  reg_node(node);

  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief condition ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @return 生成したノードを返す．
MvNode*
MvMgr::new_ite(MvModule* module,
	       ymuint bit_width)
{
  MvNode* node = new MvIte(module, bit_width);
  reg_node(node);

  assert_cond( node->input_num() == 3, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief concatenate ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] ni 入力数
// @return 生成したノードを返す．
MvNode*
MvMgr::new_concat(MvModule* module,
		  ymuint ni)
{
  MvNode* node = new MvConcat(module, ni);
  reg_node(node);

  return node;
}

#if 0
// @brief bit selectノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] idx ビット位置を表す定数ノード
// @return 生成したノードを返す．
MvNode*
MvMgr::new_bitselect(MvModule* module,
		     ymuint bit_width,
		     MvNode* idx)
{
  MvNode* node = new MvBitSelect(module);
  reg_node(node);

  assert_cond( idx->type() == MvNode::kConst, __FILE__, __LINE__);
  assert_cond( node->input_num() == 2, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);
#if 0
  set_bit_width(node->input(0), bit_width);
  set_bit_width(node->input(1), idx->output(0)->bit_width());
  set_bit_width(node->output(0), 1);
#endif

  connect(idx->output(0), node->input(1));
  
  return node;
}

// @brief part select ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] msb 範囲指定の MSB
// @param[in] lsb 範囲指定の LSB
// @return 生成したノードを返す．
MvNode*
MvMgr::new_partselect(MvModule* module,
		      ymuint bit_width,
		      MvNode* msb,
		      MvNode* lsb)
{
  MvNode* node = new MvPartSelect(module);
  reg_node(node);

  assert_cond( msb->type() == MvNode::kConst, __FILE__, __LINE__);
  assert_cond( lsb->type() == MvNode::kConst, __FILE__, __LINE__);
  assert_cond( node->input_num() == 3, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);
#if 0
  set_bit_width(node->input(0), bit_width);
  set_bit_width(node->input(1), msb->output(0)->bit_width());
  set_bit_width(node->input(2), lsb->output(0)->bit_width());
  set_bit_width(node->output(0), 1);
#endif
  
  connect(msb->output(0), node->input(1));
  connect(lsb->output(0), node->input(2));
  
  return node;
}
#endif

// @brief module instance ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] submodule 下位モジュール
// @return 生成したノードを返す．
MvNode*
MvMgr::new_inst(MvModule* module,
		MvModule* submodule)
{
  MvNode* node = new MvInst(module, submodule);
  submodule->mParent = node;
  reg_node(node);

  return node;
}

// @brief constant ノードを生成する．
// @param[in] module ノードが属するモジュール
// @param[in] bit_width ビット幅
// @param[in] val 値
MvNode*
MvMgr::new_const(MvModule* module,
		 ymuint bit_width,
		 const vector<ymuint32>& val)
{
  MvNode* node = new MvConst(module, bit_width, val);
  reg_node(node);

  assert_cond( node->input_num() == 0, __FILE__, __LINE__);
  assert_cond( node->output_num() == 1, __FILE__, __LINE__);

  return node;
}

// @brief ノードを削除する．
// @param[in] node 対象のノード
// @note 入力ノード, 出力ノードは削除できない
// @note 接続が残っているノードは削除できない．
void
MvMgr::delete_node(MvNode* node)
{
  if ( node->type() == MvNode::kInput || node->type() == MvNode::kOutput ) {
    return;
  }
  for (ymuint i = 0; i < node->input_num(); ++ i) {
    if ( node->input(i)->net() ) {
      return;
    }
  }
  for (ymuint i = 0; i < node->output_num(); ++ i) {
    if ( !node->output(i)->net_list().empty() ) {
      return;
    }
  }
  unreg_node(node);
  delete node;
}

#if 0
// @brief ピンのビット幅を設定する．
// @param[in] pin 対象のピン
// @param[in] bit_width 設定するビット幅
void
MvMgr::set_bit_width(MvPin* pin,
		     ymuint bit_width)
{
  pin->mBitWidth = pin;
}
#endif

// @brief ピンとピンを接続する．
// @param[in] src_pin 入力元のピン
// @param[in] dst_pin 出力先のピン
// @return 接続を表すネットを返す．
// @note 接続が失敗したら NULLを返す．
// @note 接続が失敗するのは，
//  - ピンが異なるモジュールに属していた．
//  - ピンのビット幅が異なっていた．
MvNet*
MvMgr::connect(MvOutputPin* src_pin,
	       MvInputPin* dst_pin)
{
  if ( src_pin->node()->parent() != dst_pin->node()->parent() ) {
    return NULL;
  }
  if ( src_pin->bit_width() != dst_pin->bit_width() ) {
    return NULL;
  }
  MvNet* net = new MvNet(src_pin, dst_pin);
  src_pin->mNetList.push_back(net);
  return net;
}

// @brief 接続を取り除く
// @param[in] net 接続を表すネット
void
MvMgr::disconnect(MvNet* net)
{
  net->src_pin()->mNetList.erase(net);
}

// @brief ノードを登録する．
// @param[in] node 対象のノード
void
MvMgr::reg_node(MvNode* node)
{
  MvModule* module = node->parent();
  int tmp = module->mItvlMgr.avail_num();
  if ( tmp == -1 ) {
    // ID番号が枯渇？
    return;
  }
  module->mItvlMgr.erase(tmp);
  ymuint id = tmp;
  node->mId = id;
  while ( module->mNodeArray.size() <= id ) {
    module->mNodeArray.push_back(NULL);
  }
  module->mNodeArray[id] = node;

  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    list<MvNode*>& nodelist = module->mNodeList;
    nodelist.push_back(node);
    node->mSelfRef = nodelist.end();
    -- node->mSelfRef;
    assert_cond( *node->mSelfRef == node , __FILE__, __LINE__);
  }
}

// @brief ノードの登録を削除する．
// @param[in] node 対象のノード
void
MvMgr::unreg_node(MvNode* node)
{
  MvModule* module = node->parent();
  module->mItvlMgr.add(node->id());
  module->mNodeArray[node->id()] = NULL;
  if ( node->type() != MvNode::kInput && node->type() != MvNode::kOutput ) {
    module->mNodeList.erase(node->mSelfRef);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MvNet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] src 入力元のピン
// @param[in] dst 出力先のピン
MvNet::MvNet(MvOutputPin* src,
	     MvInputPin* dst) :
  mSrc(src),
  mDst(dst)
{
}

// @brief デストラクタ
MvNet::~MvNet()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvPin::MvPin()
{
}

// @brief デストラクタ
MvPin::~MvPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvInputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvInputPin::MvInputPin()
{
}

// @brief デストラクタ
MvInputPin::~MvInputPin()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvOutputPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MvOutputPin::MvOutputPin()
{
}

// @brief デストラクタ
MvOutputPin::~MvOutputPin()
{
}


//////////////////////////////////////////////////////////////////////
// グローバル関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する
// @param[in] s 出力先のストリーム
// @param[in] mgr MvMgr
/// @note デバッグ用
void
dump(ostream& s,
     const MvMgr& mgr)
{
  ymuint n = mgr.max_module_id();
  for (ymuint i = 0; i < n; ++ i) {
    MvModule* module = mgr.module(i);
    if ( module == NULL ) continue;
    s << "Module#" << module->id() << "(";
    if ( module->name() ) {
      s << module->name();
    }
    s << ")" << endl;
    MvNode* pnode = module->parent();
    if ( pnode ) {
      s << "  parent node: Module#" << pnode->parent()->id()
	<< ":" << pnode->id() << endl;
    }
    else {
      s << "  toplevel module" << endl;
    }

#if 1
    ymuint ni = module->input_num();
    for (ymuint j = 0; j < ni; ++ j) {
      dump_node(s, module->input(j));
    }
    ymuint no = module->output_num();
    for (ymuint j = 0;j < no; ++ j) {
      dump_node(s, module->output(j));
    }
    for (list<MvNode*>::const_iterator p = module->nodes_begin();
	 p != module->nodes_end(); ++ p) {
      MvNode* node = *p;
      dump_node(s, node);
    }
#else
    ymuint n = module->max_node_id();
    for (ymuint i = 0; i < n; ++ i) {
      MvNode* node = module->node(i);
      if ( node ) {
	dump_node(s, node);
      }
    }
#endif
    s << endl;
  }
}

END_NAMESPACE_YM_MVN
