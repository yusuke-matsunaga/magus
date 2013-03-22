
/// @file VectSetList.cc
/// @brief VectSetlist の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "VectSetList.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
// クラス VectSetList
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] max_num 予想される最大要素数
// @param[in] max_setnum 予想される最大分割数
VectSetList::VectSetList(ymuint max_num,
			 ymuint max_setnum)
{
  mBody.reserve(max_num);
  mStartPos.reserve(max_setnum);
}

// @brief デストラクタ
VectSetList::~VectSetList()
{
}

// @brief 空の集合を追加する．
void
VectSetList::add_set()
{
  mStartPos.push_back(mBody.size());
}

// @brief 現在の集合に要素を追加する．
void
VectSetList::add_vect(RegVect* vect)
{
  mBody.push_back(vect);
}

// @brief 全要素数を得る．
ymuint
VectSetList::all_elem_num() const
{
  return mBody.size();
}

// @brief 集合の数を得る．
ymuint
VectSetList::set_num() const
{
  return mStartPos.size();
}

// @brief 指定された集合の要素数を得る．
// @param[in] set_id 集合のID番号 ( 0 <= set_id < set_num() )
ymuint
VectSetList::set_size(ymuint set_id) const
{
  if ( set_id < mStartPos.size() - 1 ) {
    return mStartPos[set_id + 1] - mStartPos[set_id];
  }
  else {
    return mBody.size() - mStartPos[set_id];
  }
}

// @brief 指定された集合の要素を得る．
// @param[in] set_id 集合のID番号 ( 0 <= set_id < set_num() )
// @param[in] vec_id ベクタのID番号 ( 0 <= vect_id < set_size(set_id) )
RegVect*
VectSetList::set_elem(ymuint set_id,
		      ymuint vect_id) const
{
  return mBody[mStartPos[set_id] + vect_id];
}

END_NAMESPACE_YM_IGF
