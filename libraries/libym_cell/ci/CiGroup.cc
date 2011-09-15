
/// @file CiGroup.cc
/// @brief CiGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiGroup::CiGroup() :
  mCellClass(NULL),
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
CiGroup::~CiGroup()
{
  // mCellList は CellMgr が管理する．
}

// @brief 属している CellClass を返す．
const CellClass*
CiGroup::cell_class() const
{
  return mCellClass;
}

// @brief 代表クラスに対する変換マップを返す．
const NpnMapM&
CiGroup::map() const
{
  return mMap;
}

// @brief セル数を返す．
ymuint
CiGroup::cell_num() const
{
  return mCellNum;
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
const Cell*
CiGroup::cell(ymuint pos) const
{
  assert_cond( pos < mCellNum, __FILE__, __LINE__);
  return mCellList[pos];
}

// @brief 初期化する．
// @param[in] cell_class 代表クラス
// @param[in] map 変換マップ
// @param[in] cell_list セルのリスト
// @param[in] alloc メモリアロケータ
void
CiGroup::init(const CellClass* cell_class,
	      const NpnMapM& map,
	      ymuint cell_num,
	      AllocBase& alloc)
{
  mCellClass = cell_class;
  mMap = map;
  mCellNum = cell_num;
  void* p = alloc.get_memory(sizeof(const Cell*) * cell_num);
  mCellList = new (p) const Cell*[cell_num];
}

// @brief セルを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
// @param[in] cell セル
// @note 必ず init() の後に呼び出すこと
void
CiGroup::set_cell(ymuint pos,
		  const Cell* cell)
{
  assert_cond( pos < cell_num(), __FILE__, __LINE__);
  mCellList[pos] = cell;
}

END_NAMESPACE_YM_CELL
