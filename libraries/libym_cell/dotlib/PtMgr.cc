
/// @file libym_cell/dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "PtNode.h"
#include "PtNodeImpl.h"
#include "PtCell.h"
#include "PtValue.h"
#include "PtValueImpl.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc(4096)
{
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

// @brief simple attribute を表す PtNode を生成する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
PtNode*
PtMgr::new_ptsimple(ShString attr_name,
		    const FileRegion& attr_loc,
		    const PtValue* value)
{
  void* p = mAlloc.get_memory(sizeof(PtSimpleNode));
  return new (p) PtSimpleNode(attr_name, attr_loc, value);
}

// @brief complex attribute を表す PtNode を生成する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値のリスト
PtNode*
PtMgr::new_ptcomplex(ShString attr_name,
		     const FileRegion& attr_loc,
		     const vector<const PtValue*>& value_list)
{
  void* p = mAlloc.get_memory(sizeof(PtComplexNode));
  return new (p) PtComplexNode(attr_name, attr_loc, value_list);
}

// @brief group statement を表す PtNode を生成する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value_list 値のリスト
PtNode*
PtMgr::new_ptgroup(ShString attr_name,
		   const FileRegion& attr_loc,
		   const vector<const PtValue*>& value_list)
{
  void* p = mAlloc.get_memory(sizeof(PtGroupNode));
  return new (p) PtGroupNode(attr_name, attr_loc, value_list);
}

// @brief PtCell を生成する．
// @param[in] attr_name 属性名
// @param[in] attr_loc ファイル上の位置
// @param[in] value 値
PtCell*
PtMgr::new_ptcell(ShString attr_name,
		  const FileRegion& attr_loc,
		  const PtValue* value)
{
  void* p = mAlloc.get_memory(sizeof(PtCell));
  return new (p) PtCell(attr_name, attr_loc, value);
}

// @brief 整数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_int(int value,
	       const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtInt));
  return new (p) PtInt(value, loc);
}

// @brief 実数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_float(double value,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtFloat));
  return new (p) PtFloat(value, loc);
}

// @brief 定数シンボルを表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_string(ShString value,
		  const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtString));
  return new (p) PtString(value, loc);
}

// @brief 演算子を表す PtValue を生成する．
// @param[in] type 型
// @param[in] opr1, opr2 オペランド
PtValue*
PtMgr::new_opr(tTokenType type,
		 PtValue* opr1,
		 PtValue* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(type, opr1, opr2);
}

END_NAMESPACE_YM_CELL_DOTLIB
