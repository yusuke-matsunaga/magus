
/// @file libym_dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"

#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc(4096),
  mTop(NULL)
{
  mIntNum = 0;
  mFloatNum = 0;
  mStrNum = 0;
  mOprNum = 0;
  mListNum = 0;
  mGroupNum = 0;
  mAttrNum = 0;
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
  clear();
}

// @brief 初期化する．
// @note 以前，生成されたオブジェクトは破壊される．
void
PtMgr::clear()
{
  for (PtNodeImpl* node = mTop; node; ) {
    PtNodeImpl* next = node->mClearLink;
    node->~PtNodeImpl();
    node = next;
  }
  mTop = NULL;
  mAlloc.destroy();
}

// @brief 整数値を表す PtNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtNodeImpl*
PtMgr::new_int(int value,
	       const FileRegion& loc)
{
  ++ mIntNum;
  void* p = mAlloc.get_memory(sizeof(PtInt));
  PtNodeImpl* node = new (p) PtInt(value, loc);
  add_node(node);
  return node;
}

// @brief 実数値を表す PtNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtNodeImpl*
PtMgr::new_float(double value,
		 const FileRegion& loc)
{
  ++ mFloatNum;
  void* p = mAlloc.get_memory(sizeof(PtFloat));
  PtNodeImpl* node = new (p) PtFloat(value, loc);
  add_node(node);
  return node;
}

// @brief 定数シンボルを表す PtNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtNodeImpl*
PtMgr::new_string(ShString value,
		  const FileRegion& loc)
{
  ++ mStrNum;
  void* p = mAlloc.get_memory(sizeof(PtString));
  PtNodeImpl* node = new (p) PtString(value, loc);
  add_node(node);
  return node;
}

// @brief + 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_plus(const DotlibNode* opr1,
		const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  PtNodeImpl* node = new (p) PtOpr(DotlibNode::kPlus, opr1, opr2);
  add_node(node);
  return node;
}

// @brief - 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_minus(const DotlibNode* opr1,
		 const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  PtNodeImpl* node = new (p) PtOpr(DotlibNode::kMinus, opr1, opr2);
  add_node(node);
  return node;
}

// @brief * 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_mult(const DotlibNode* opr1,
		const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  PtNodeImpl* node =  new (p) PtOpr(DotlibNode::kMult, opr1, opr2);
  add_node(node);
  return node;
}

// @brief / 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_div(const DotlibNode* opr1,
	       const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  PtNodeImpl* node = new (p) PtOpr(DotlibNode::kDiv, opr1, opr2);
  add_node(node);
  return node;
}

// @brief リストを表す PtNode を生成する．
PtNodeImpl*
PtMgr::new_list()
{
  ++ mListNum;
  void* p = mAlloc.get_memory(sizeof(PtList));
  PtNodeImpl* node =  new (p) PtList();
  add_node(node);
  return node;
}

// @brief グループを表す PtNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtNodeImpl*
PtMgr::new_group(const DotlibNode* value,
		 const FileRegion& loc)
{
  ++ mGroupNum;
  void* p = mAlloc.get_memory(sizeof(PtGroup));
  PtNodeImpl* node =  new (p) PtGroup(value, loc);
  add_node(node);
  return node;
}

// @brief PtAttr を生成する．
PtNodeImpl*
PtMgr::new_attr(const ShString& attr_name,
		const DotlibNode* value,
		const FileRegion& loc)
{
  ++ mAttrNum;
  void* p = mAlloc.get_memory(sizeof(PtAttr));
  PtNodeImpl* node =  new (p) PtAttr(attr_name, value, loc);
  add_node(node);
  return node;
}

// @brief 使用メモリ量の一覧を出力する．
// @param[in] s 出力先のストリーム
void
PtMgr::show_stats(ostream& s)
{
  s << "PtInt:          " << setw(7) << mIntNum
    << " x " << setw(3) << sizeof(PtInt)
    << " = " << setw(10) << mIntNum * sizeof(PtInt) << endl

    << "PtFloat:        " << setw(7) << mFloatNum
    << " x " << setw(3) << sizeof(PtFloat)
    << " = " << setw(10) << mFloatNum * sizeof(PtFloat) << endl

    << "PtString:       " << setw(7) << mStrNum
    << " x " << setw(3) << sizeof(PtString)
    << " = " << setw(10) << mStrNum * sizeof(PtString) << endl

    << "PtOpr:          " << setw(7) << mOprNum
    << " x " << setw(3) << sizeof(PtOpr)
    << " = " << setw(10) << mOprNum * sizeof(PtOpr) << endl

    << "PtList:         " << setw(7) << mListNum
    << " x " << setw(3) << sizeof(PtList)
    << " = " << setw(10) << mListNum * sizeof(PtList) << endl

    << "PtGroup:        " << setw(7) << mGroupNum
    << " x " << setw(3) << sizeof(PtGroup)
    << " = " << setw(10) << mGroupNum * sizeof(PtGroup) << endl

    << "PtAttr:         " << setw(7) << mAttrNum
    << " x " << setw(3) << sizeof(PtAttr)
    << " = " << setw(10) << mAttrNum * sizeof(PtAttr) << endl

    << "Total memory:                 = "
    << setw(10) << mAlloc.used_size() << endl
    << endl

    << "Allocated memory:             = "
    << setw(10) << mAlloc.allocated_size() << endl

    << "ShString:                     = "
    << setw(10) << ShString::allocated_size() << endl;
}

// @brief 生成したノードをリンクにつなぐ．
void
PtMgr::add_node(PtNodeImpl* node)
{
  node->mClearLink = mTop;
  mTop = node;
}

END_NAMESPACE_YM_DOTLIB
