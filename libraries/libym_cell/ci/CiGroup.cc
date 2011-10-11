
/// @file CiGroup.cc
/// @brief CiGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"

#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/Cell.h"


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

// @brief ID番号を返す．
// @note CellLibrary::group(id) で返されるオブジェクトの id() は id となる．
ymuint
CiGroup::id() const
{
  return mId;
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
	      const vector<const Cell*>& cell_list,
	      AllocBase& alloc)
{
  mCellClass = cell_class;
  mMap = map;
  mCellNum = cell_list.size();

  alloc_array(alloc);
  for (ymuint i = 0; i < mCellNum; ++ i) {
    mCellList[i] = cell_list[i];
  }
}

// @brief バイナリダンプを行う．
// @param[in] bos 出力先のストリーム
void
CiGroup::dump(BinO& bos) const
{
  ymuint32 parent_id = mCellClass->id();
  bos << parent_id
      << mMap
      << mCellNum;
  for (ymuint i = 0; i < mCellNum; ++ i) {
    ymuint32 cell_id = mCellList[i]->id();
    bos << cell_id;
  }
}

// @brief バイナリファイルを読み込む．
// @param[in] bis 入力元のストリーム
// @param[in] library セルライブラリ
// @param[in] alloc メモリアロケータ
void
CiGroup::restore(BinI& bis,
		 const CellLibrary& library,
		 AllocBase& alloc)
{
  ymuint32 parent_id;
  bis >> parent_id
      >> mMap
      >> mCellNum;
  mCellClass = library.npn_class(parent_id);

  alloc_array(alloc);
  for (ymuint i = 0; i < mCellNum; ++ i) {
    ymuint32 cell_id;
    bis >> cell_id;
    mCellList[i] = library.cell(cell_id);
  }
}

// @brief メモリ領域の確保を行う．
// @param[in] alloc メモリアロケータ
void
CiGroup::alloc_array(AllocBase& alloc)
{
  if ( mCellNum > 0 ) {
    void* p = alloc.get_memory(sizeof(const Cell*) * mCellNum);
    mCellList = new (p) const Cell*[mCellNum];
  }
  else {
    mCellList = NULL;
  }
}

END_NAMESPACE_YM_CELL
