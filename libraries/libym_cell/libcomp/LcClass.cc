
/// @file LdClass.cc
/// @brief LdClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdClass.h"
#include "LdGroup.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LdClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] repfunc 代表関数
LdClass::LdClass(ymuint id,
		 const TvFunc& repfunc) :
  mId(id),
  mRepFunc(TvFuncM(vector<TvFunc>(1, repfunc))),
  mCellClass(NULL)
{
}

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] repfunc 代表関数
LdClass::LdClass(ymuint id,
		 const TvFuncM& repfunc) :
  mId(id),
  mRepFunc(repfunc),
  mCellClass(NULL)
{
}

// @brief デストラクタ
LdClass::~LdClass()
{
}

// @brief ID番号を返す．
ymuint
LdClass::id() const
{
  return mId;
}

// @brief 代表関数を返す．
TvFuncM
LdClass::repfunc() const
{
  return mRepFunc;
}

// @brief 実体を返す．
const CellClass*
LdClass::cell_class() const
{
  return mCellClass;
}

// @brief このクラスに属しているグループのリストを返す．
const vector<LdGroup*>&
LdClass::group_list() const
{
  return mGroupList;
}

// @brief グループを追加する．
void
LdClass::add_group(LdGroup* group,
		   const NpnMapM& map)
{
  mGroupList.push_back(group);
  group->mParent = this;
  group->mMap = map;
}


END_NAMESPACE_YM_CELL_LIBCOMP

