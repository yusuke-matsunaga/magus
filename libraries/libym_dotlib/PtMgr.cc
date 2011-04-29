
/// @file libym_dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "ym_dotlib/PtAttr.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc(4096)
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
}

// @brief 初期化する．
// @note 以前，生成されたオブジェクトは(デストラクタ抜きで)破壊される．
void
PtMgr::init()
{
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
  return new (p) PtInt(value, loc);
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
  return new (p) PtFloat(value, loc);
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
  return new (p) PtString(value, loc);
}

// @brief + 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_plus(PtNodeImpl* opr1,
		PtNodeImpl* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(PtNode::kPlus, opr1, opr2);
}

// @brief - 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_minus(PtNodeImpl* opr1,
		 PtNodeImpl* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(PtNode::kMinus, opr1, opr2);
}

// @brief * 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_mult(PtNodeImpl* opr1,
		PtNodeImpl* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(PtNode::kMult, opr1, opr2);
}

// @brief / 演算子を表す PtNode を生成する．
// @param[in] opr1, opr2 オペランド
PtNodeImpl*
PtMgr::new_div(PtNodeImpl* opr1,
	       PtNodeImpl* opr2)
{
  ++ mOprNum;
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(PtNode::kDiv, opr1, opr2);
}

// @brief リストを表す PtNode を生成する．
PtNodeImpl*
PtMgr::new_list()
{
  ++ mListNum;
  void* p = mAlloc.get_memory(sizeof(PtList));
  return new (p) PtList();
}

// @brief グループを表す PtNode を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtNodeImpl*
PtMgr::new_group(const PtNodeImpl* value,
		 const FileRegion& loc)
{
  ++ mGroupNum;
  void* p = mAlloc.get_memory(sizeof(PtGroup));
  return new (p) PtGroup(value, loc);
}

// @brief PtAttr を生成する．
PtAttr*
PtMgr::new_attr(const ShString& attr_name,
		const PtNode* value)
{
  ++ mAttrNum;
  void* p = mAlloc.get_memory(sizeof(PtAttr));
  return new (p) PtAttr(attr_name, value);
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

END_NAMESPACE_YM_DOTLIB
