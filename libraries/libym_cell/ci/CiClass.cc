
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

// @brief ID番号を返す．
// @note CellLibrary::npn_class(id) で返されるオブジェクトの id() は id となる．
ymuint
CiClass::id() const
{
  return mId;
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
// @param[in] id ID番号
// @param[in] idmap_list 同位体変換リスト
// @param[in] group_list グループのリスト
// @param[in] alloc メモリアロケータ
void
CiClass::init(ymuint id,
	      const vector<NpnMapM>& idmap_list,
	      const vector<const CellGroup*>& group_list,
	      AllocBase& alloc)
{
  mId = id;
  mIdmapList.clear();
  mIdmapList.reserve(idmap_list.size());
  for (vector<NpnMapM>::const_iterator p = idmap_list.begin();
       p != idmap_list.end(); ++ p) {
    mIdmapList.push_back(*p);
  }

  mGroupNum = group_list.size();
  void* p = alloc.get_memory(sizeof(const CellGroup*) * mGroupNum);
  mGroupList = new (p) const CellGroup*[mGroupNum];
  for (ymuint i = 0; i < mGroupNum; ++ i) {
    mGroupList[i] = group_list[i];
  }
}

END_NAMESPACE_YM_CELL
