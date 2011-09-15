
/// @file CiGroupMgr.cc
/// @brief CiGroupMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "CiGroupMgr.h"
#include "CiClass.h"
#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiGroupMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiGroupMgr::CiGroupMgr() :
  mClassNum(0),
  mClassArray(NULL),
  mGroupNum(0),
  mGroupArray(NULL)
{
}

// @brief デストラクタ
CiGroupMgr::~CiGroupMgr()
{
  // メモリは別のオブジェクトが管理している．
}

// @brief クラス数を設定する．
// @param[in] nc クラス数
// @param[in] alloc メモリアロケータ
// @note 同時にクラスの配列の確保を行う．
void
CiGroupMgr::set_class_num(ymuint nc,
			  AllocBase& alloc)
{
  mClassNum = nc;
  void* p = alloc.get_memory(sizeof(CiClass) * nc);
  mClassArray = new (p) CiClass[nc];
}

// @brief クラスを得る．
// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
CiClass*
CiGroupMgr::_npn_class(ymuint id)
{
  return &mClassArray[id];
}

// @brief グループ数を設定する．
// @param[in] ng グループ数
// @param[in] alloc メモリアロケータ
// @note 同時にグループの配列の確保を行う．
void
CiGroupMgr::set_group_num(ymuint ng,
			  AllocBase& alloc)
{
  mGroupNum = ng;
  void* p = alloc.get_memory(sizeof(CiGroup) * ng);
  mGroupArray = new (p) CiGroup[ng];
}

// @brief グループを得る．
// @param[in] id グループ番号 ( 0 <= id < group_num() )
CiGroup*
CiGroupMgr::_group(ymuint id)
{
  return &mGroupArray[id];
}

END_NAMESPACE_YM_CELL

