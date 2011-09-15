#ifndef CIGROUP_H
#define CIGROUP_H

/// @file CiGroup.h
/// @brief CiGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiGroup CiGroup.h "CiGroup.h"
/// @brief CellGroup の実装クラス
//////////////////////////////////////////////////////////////////////
class CiGroup :
  public CellGroup
{
public:

  /// @brief コンストラクタ
  CiGroup();

  /// @brief デストラクタ
  virtual
  ~CiGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellClass を返す．
  virtual
  const CellClass*
  cell_class() const;

  /// @brief 代表クラスに対する変換マップを返す．
  virtual
  const NpnMapM&
  map() const;


public:
  //////////////////////////////////////////////////////////////////////
  // このグループに属しているセルの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  virtual
  ymuint
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] cell_class 代表クラス
  /// @param[in] map 変換マップ
  /// @param[in] cell_num セル数
  /// @param[in] alloc メモリアロケータ
  void
  init(const CellClass* cell_class,
       const NpnMapM& map,
       ymuint cell_num,
       AllocBase& alloc);

  /// @brief セルを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  /// @param[in] cell セル
  /// @note 必ず init() の後に呼び出すこと
  void
  set_cell(ymuint pos,
	   const Cell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属している CellClass
  const CellClass* mCellClass;

  // CellClass に対する入出力の変換関数
  NpnMapM mMap;

  // セル数
  ymuint32 mCellNum;

  // セルのポインタの配列
  const Cell** mCellList;

};

END_NAMESPACE_YM_CELL

#endif // CIGROUP_H
