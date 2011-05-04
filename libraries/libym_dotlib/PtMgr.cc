
/// @file libym_dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"

#include "DotlibNodeImpl.h"


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
  for (DotlibNodeImpl* node = mTop; node; ) {
    DotlibNodeImpl* next = node->mClearLink;
    node->~DotlibNodeImpl();
    node = next;
  }
  mTop = NULL;
  mAlloc.destroy();
}

// @brief 整数値を表す DotlibNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibNodeImpl*
PtMgr::new_int(int value,
	       const FileRegion& loc)
{
  ++ mIntNum;
  void* p = mAlloc.get_memory(sizeof(DotlibInt));
  DotlibNodeImpl* node = new (p) DotlibInt(value, loc);
  add_node(node);
  return node;
}

// @brief 実数値を表す DotlibNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibNodeImpl*
PtMgr::new_float(double value,
		 const FileRegion& loc)
{
  ++ mFloatNum;
  void* p = mAlloc.get_memory(sizeof(DotlibFloat));
  DotlibNodeImpl* node = new (p) DotlibFloat(value, loc);
  add_node(node);
  return node;
}

// @brief 定数シンボルを表す DotlibNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibNodeImpl*
PtMgr::new_string(ShString value,
		  const FileRegion& loc)
{
  ++ mStrNum;
  void* p = mAlloc.get_memory(sizeof(DotlibString));
  DotlibNodeImpl* node = new (p) DotlibString(value, loc);
  add_node(node);
  return node;
}

// @brief + 演算子を表す DotlibNode を生成する．
// @param[in] opr1, opr2 オペランド
DotlibNodeImpl*
PtMgr::new_plus(const DotlibNode* opr1,
		const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(DotlibOpr));
  DotlibNodeImpl* node = new (p) DotlibOpr(DotlibNode::kPlus, opr1, opr2);
  add_node(node);
  return node;
}

// @brief - 演算子を表す DotlibNode を生成する．
// @param[in] opr1, opr2 オペランド
DotlibNodeImpl*
PtMgr::new_minus(const DotlibNode* opr1,
		 const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(DotlibOpr));
  DotlibNodeImpl* node = new (p) DotlibOpr(DotlibNode::kMinus, opr1, opr2);
  add_node(node);
  return node;
}

// @brief * 演算子を表す DotlibNode を生成する．
// @param[in] opr1, opr2 オペランド
DotlibNodeImpl*
PtMgr::new_mult(const DotlibNode* opr1,
		const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(DotlibOpr));
  DotlibNodeImpl* node =  new (p) DotlibOpr(DotlibNode::kMult, opr1, opr2);
  add_node(node);
  return node;
}

// @brief / 演算子を表す DotlibNode を生成する．
// @param[in] opr1, opr2 オペランド
DotlibNodeImpl*
PtMgr::new_div(const DotlibNode* opr1,
	       const DotlibNode* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(DotlibOpr));
  DotlibNodeImpl* node = new (p) DotlibOpr(DotlibNode::kDiv, opr1, opr2);
  add_node(node);
  return node;
}

// @brief リストを表す DotlibNode を生成する．
DotlibNodeImpl*
PtMgr::new_list()
{
  ++ mListNum;
  void* p = mAlloc.get_memory(sizeof(DotlibList));
  DotlibNodeImpl* node =  new (p) DotlibList();
  add_node(node);
  return node;
}

// @brief グループを表す DotlibNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
DotlibNodeImpl*
PtMgr::new_group(const DotlibNode* value,
		 const FileRegion& loc)
{
  ++ mGroupNum;
  void* p = mAlloc.get_memory(sizeof(DotlibGroup));
  DotlibNodeImpl* node =  new (p) DotlibGroup(value, loc);
  add_node(node);
  return node;
}

// @brief DotlibAttr を生成する．
DotlibNodeImpl*
PtMgr::new_attr(const ShString& attr_name,
		const DotlibNode* value,
		const FileRegion& loc)
{
  ++ mAttrNum;
  void* p = mAlloc.get_memory(sizeof(DotlibAttr));
  DotlibNodeImpl* node =  new (p) DotlibAttr(attr_name, value, loc);
  add_node(node);
  return node;
}

// @brief 使用メモリ量の一覧を出力する．
// @param[in] s 出力先のストリーム
void
PtMgr::show_stats(ostream& s)
{
  s << "DotlibInt:          " << setw(7) << mIntNum
    << " x " << setw(3) << sizeof(DotlibInt)
    << " = " << setw(10) << mIntNum * sizeof(DotlibInt) << endl

    << "DotlibFloat:        " << setw(7) << mFloatNum
    << " x " << setw(3) << sizeof(DotlibFloat)
    << " = " << setw(10) << mFloatNum * sizeof(DotlibFloat) << endl

    << "DotlibString:       " << setw(7) << mStrNum
    << " x " << setw(3) << sizeof(DotlibString)
    << " = " << setw(10) << mStrNum * sizeof(DotlibString) << endl

    << "DotlibOpr:          " << setw(7) << mOprNum
    << " x " << setw(3) << sizeof(DotlibOpr)
    << " = " << setw(10) << mOprNum * sizeof(DotlibOpr) << endl

    << "DotlibList:         " << setw(7) << mListNum
    << " x " << setw(3) << sizeof(DotlibList)
    << " = " << setw(10) << mListNum * sizeof(DotlibList) << endl

    << "DotlibGroup:        " << setw(7) << mGroupNum
    << " x " << setw(3) << sizeof(DotlibGroup)
    << " = " << setw(10) << mGroupNum * sizeof(DotlibGroup) << endl

    << "DotlibAttr:         " << setw(7) << mAttrNum
    << " x " << setw(3) << sizeof(DotlibAttr)
    << " = " << setw(10) << mAttrNum * sizeof(DotlibAttr) << endl

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
PtMgr::add_node(DotlibNodeImpl* node)
{
  node->mClearLink = mTop;
  mTop = node;
}

END_NAMESPACE_YM_DOTLIB
