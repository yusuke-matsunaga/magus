
/// @file libym_cell/dotlib/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "PtValue.h"


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

// @brief 整数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_value(int value,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtInt));
  return new (p) PtInt(value, loc);
}

// @brief 実数値を表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_value(double value,
		 const FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtFloat));
  return new (p) PtFloat(value, loc);
}

// @brief 定数シンボルを表す PtValue を生成する．
// @param[in] value 値
// @param[in] loc ファイル上の位置
PtValue*
PtMgr::new_value(const string& value,
		 cponst FileRegion& loc)
{
  void* p = mAlloc.get_memory(sizeof(PtString));
  return new (p) PtString(value, loc);
}

// @brief 演算子を表す PtValue を生成する．
// @param[in] type 型
// @param[in] opr1, opr2 オペランド
PtValue*
PtMgr::new_value(tTokenType type,
		 PtValue* opr1,
		 PtValue* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtOpr));
  return new (p) PtOpr(type, opr1, opr2);
}

END_NAMESPACE_YM_CELL_DOTLIB
