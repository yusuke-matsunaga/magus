
/// @file libym_mislib/MislibMgrImpl.cc
/// @brief MislibMgrImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibMgrImpl.h"
#include "MislibGate.h"
#include "MislibNot.h"
#include "MislibBop.h"
#include "MislibPin.h"
#include "MislibList.h"
#include "MislibSymbol.h"
#include "MislibConst.h"
#include "MislibNum.h"
#include "MislibStr.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibMgrImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibMgrImpl::MislibMgrImpl() :
  mAlloc(4096)
{
}

// デストラクタ
MislibMgrImpl::~MislibMgrImpl()
{
  clear();
}

// 今までに生成したすべてのオブジェクトを解放する．
void
MislibMgrImpl::clear()
{
  mAlloc.destroy();
  mGateList = NULL;
}

// @brief ゲートのリストを返す．
const MislibNode*
MislibMgrImpl::gate_list() const
{
  return mGateList;
}

// 文字列ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_str(const FileRegion& loc,
		       ShString str)
{
  void* p = mAlloc.get_memory(sizeof(MislibStr));
  return new (p) MislibStr(loc, str);
}

// 数値ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_num(const FileRegion& loc,
		       double num)
{
  void* p = mAlloc.get_memory(sizeof(MislibNum));
  return new (p) MislibNum(loc, num);
}

// NONINV ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_noninv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibNoninv));
  return new (p) MislibNoninv(loc);
}

// INV ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_inv(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibInv));
  return new (p) MislibInv(loc);
}

// UNKNOWN ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_unknown(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibUnknown));
  return new (p) MislibUnknown(loc);
}

// 定数0ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_const0(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibConst0));
  return new (p) MislibConst0(loc);
}

// 定数1ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_const1(const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(MislibConst1));
  return new (p) MislibConst1(loc);
}

// NOT ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_not(const FileRegion& loc,
		       const MislibNode* child1)
{
  void* p = mAlloc.get_memory(sizeof(MislibNot));
  return new (p) MislibNot(loc, child1);
}

// AND ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_and(const FileRegion& loc,
		       const MislibNode* child1,
		       const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibAnd));
  return new (p) MislibAnd(loc, child1, child2);
}

// OR ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_or(const FileRegion& loc,
		      const MislibNode* child1,
		      const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibOr));
  return new (p) MislibOr(loc, child1, child2);
}

// XOR ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_xor(const FileRegion& loc,
		       const MislibNode* child1,
		       const MislibNode* child2)
{
  void* p = mAlloc.get_memory(sizeof(MislibXor));
  return new (p) MislibXor(loc, child1, child2);
}

// リストノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_list()
{
  void* p = mAlloc.get_memory(sizeof(MislibList));
  return new (p) MislibList();
}

// PIN ノードを生成する．
MislibNodeImpl*
MislibMgrImpl::new_pin(const FileRegion& loc,
		       const MislibNode* name,
		       const MislibNode* phase,
		       const MislibNode* input_load,
		       const MislibNode* max_load,
		       const MislibNode* rise_block_delay,
		       const MislibNode* rise_fanout_delay,
		       const MislibNode* fall_block_delay,
		       const MislibNode* fall_fanout_delay)
{
  void* p = mAlloc.get_memory(sizeof(MislibPin));
  return new (p) MislibPin(loc,
			   name, phase,
			   input_load, max_load,
			   rise_block_delay, rise_fanout_delay,
			   fall_block_delay, fall_fanout_delay);
}

// GATE ノードを生成する．
void
MislibMgrImpl::new_gate1(const FileRegion& loc,
			 const MislibNode* pt_name,
			 const MislibNode* pt_area,
			 const MislibNode* pt_oname,
			 const MislibNode* pt_expr,
			 const MislibNode* pt_ipin_list)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin_list, __FILE__, __LINE__);
  assert_cond(pt_ipin_list->type() == MislibNode::kList, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, pt_ipin_list);
  add_gate(gate);
}

// GATE ノードを生成する．
void
MislibMgrImpl::new_gate2(const FileRegion& loc,
			 const MislibNode* pt_name,
			 const MislibNode* pt_area,
			 const MislibNode* pt_oname,
			 const MislibNode* pt_expr,
			 const MislibNode* pt_ipin)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);
  assert_cond(pt_ipin, __FILE__, __LINE__);
  assert_cond(pt_ipin->type() == MislibNode::kPin, __FILE__, __LINE__);
  assert_cond(pt_ipin->name() == NULL, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, pt_ipin);
  add_gate(gate);
}

// GATE ノードを生成する．
void
MislibMgrImpl::new_gate3(const FileRegion& loc,
			 const MislibNode* pt_name,
			 const MislibNode* pt_area,
			 const MislibNode* pt_oname,
			 const MislibNode* pt_expr)
{
  assert_cond(pt_name, __FILE__, __LINE__);
  assert_cond(pt_name->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_area, __FILE__, __LINE__);
  assert_cond(pt_area->type() == MislibNode::kNum, __FILE__, __LINE__);
  assert_cond(pt_oname, __FILE__, __LINE__);
  assert_cond(pt_oname->type() == MislibNode::kStr, __FILE__, __LINE__);
  assert_cond(pt_expr, __FILE__, __LINE__);
  assert_cond(pt_expr->is_expr(), __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(MislibGate));
  MislibNodeImpl* gate = new (p) MislibGate(loc, pt_name, pt_area,
					    pt_oname, pt_expr, new_list());
  add_gate(gate);
}

// @brief ゲートを追加する．
void
MislibMgrImpl::add_gate(MislibNodeImpl* gate)
{
  if ( mGateList == NULL ) {
    mGateList = new_list();
  }
  mGateList->push_back(gate);
}

END_NAMESPACE_YM_MISLIB
