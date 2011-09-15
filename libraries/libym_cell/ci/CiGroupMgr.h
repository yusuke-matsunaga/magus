#ifndef CIGROUPMGR_H
#define CIGROUPMGR_H

/// @file CiGroupMgr.h
/// @brief CiGroupMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "CiClass.h"
#include "CiGroup.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiGroupMgr CiGroupMgr.h "CiGroupMgr.h"
/// @brief セルのグループ情報を管理するクラス
//////////////////////////////////////////////////////////////////////
class CiGroupMgr
{
public:

  /// @brief コンストラクタ
  CiGroupMgr();

  /// @brief デストラクタ
  ~CiGroupMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NPN同値クラスの個数を返す．
  ymuint
  npn_class_num() const;

  /// @brief NPN同値クラスを返す．
  /// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
  const CellClass*
  npn_class(ymuint id) const;

  /// @brief セルグループの個数を返す．
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] id グループ番号 ( 0 <= id < group_num() )
  const CellGroup*
  group(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief クラス数を設定する．
  /// @param[in] nc クラス数
  /// @param[in] alloc メモリアロケータ
  /// @note 同時にクラスの配列の確保を行う．
  void
  set_class_num(ymuint nc,
		AllocBase& alloc);

  /// @brief クラスを得る．
  /// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
  CiClass*
  _npn_class(ymuint id);

  /// @brief グループ数を設定する．
  /// @param[in] ng グループ数
  /// @param[in] alloc メモリアロケータ
  /// @note 同時にグループの配列の確保を行う．
  void
  set_group_num(ymuint ng,
		AllocBase& alloc);

  /// @brief グループを得る．
  /// @param[in] id グループ番号 ( 0 <= id < group_num() )
  CiGroup*
  _group(ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NPN同値クラスの数
  ymuint32 mClassNum;

  // NPN同値クラスの配列
  CiClass* mClassArray;

  // セルグループの数
  ymuint32 mGroupNum;

  // セルグループの配列
  CiGroup* mGroupArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief NPN同値クラスの個数を返す．
inline
ymuint
CiGroupMgr::npn_class_num() const
{
  return mClassNum;
}

// @brief NPN同値クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
inline
const CellClass*
CiGroupMgr::npn_class(ymuint id) const
{
  return &mClassArray[id];
}

// @brief セルグループの個数を返す．
inline
ymuint
CiGroupMgr::group_num() const
{
  return mGroupNum;
}

// @brief セルグループを返す．
// @param[in] id グループ番号 ( 0 <= id < group_num() )
inline
const CellGroup*
CiGroupMgr::group(ymuint id) const
{
  return &mGroupArray[id];
}

END_NAMESPACE_YM_CELL

#endif // CIGROUPMGR_H
