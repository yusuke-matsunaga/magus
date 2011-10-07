
/// @file CiClass.cc
/// @brief CiClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiClass.h"
#include "ym_logic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiClass::CiClass() :
  mGroupNum(0),
  mGroupList(NULL)
{
}

// @brief デストラクタ
CiClass::~CiClass()
{
  // mGroupList は CellMgr が管理している．
}

// @brief 同位体変換リストを得る．
const vector<NpnMapM>&
CiClass::idmap_list() const
{
  return mIdmapList;
}

// @brief NPN同値類の数を返す．
ymuint
CiClass::group_num() const
{
  return mGroupNum;
}

// @brief CellGroup を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
const CellGroup*
CiClass::cell_group(ymuint pos) const
{
  assert_cond( pos < mGroupNum, __FILE__, __LINE__);
  return mGroupList[pos];
}

// @brief 初期化する．
// @param[in] idmap_list 同位体変換リスト
// @param[in] group_num グループ数
// @param[in] alloc メモリアロケータ
void
CiClass::init(const vector<NpnMapM>& idmap_list,
	      ymuint group_num,
	      AllocBase& alloc)
{
  mIdmapList.clear();
  mIdmapList.reserve(idmap_list.size());
  for (vector<NpnMapM>::const_iterator p = idmap_list.begin();
       p != idmap_list.end(); ++ p) {
    mIdmapList.push_back(*p);
  }

  mGroupNum = group_num;
  void* p = alloc.get_memory(sizeof(const CellGroup*) * group_num);
  mGroupList = new (p) const CellGroup*[group_num];
}

// @brief グループを設定する．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
// @param[in] group グループ
// @note 必ず init() の後に呼び出すこと
void
CiClass::set_group(ymuint pos,
		   const CellGroup* group)
{
  assert_cond( pos < mGroupNum, __FILE__, __LINE__);
  mGroupList[pos] = group;
}

END_NAMESPACE_YM_CELL
