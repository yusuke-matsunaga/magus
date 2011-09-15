#ifndef CICLASS_H
#define CICLASS_H

/// @file CiClass.h
/// @brief CiClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellClass.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiClass CiClass.h "CiClass.h"
/// @brief CellClass の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class CiClass :
  public CellClass
{
public:

  /// @brief コンストラクタ
  CiClass();

  /// @brief デストラクタ
  virtual
  ~CiClass();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルグループの数を返す．
  virtual
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
  virtual
  const CellGroup*
  cell_group(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] group_num グループ数
  /// @param[in] alloc メモリアロケータ
  void
  init(ymuint group_num,
       AllocBase& alloc);

  /// @brief グループを設定する．
  /// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
  /// @param[in] group グループ
  /// @note 必ず init() の後に呼び出すこと
  void
  set_group(ymuint pos,
	    const CellGroup* group);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループの数
  ymuint32 mGroupNum;

  // グループの配列
  const CellGroup** mGroupList;

};

END_NAMESPACE_YM_CELL

#endif // CICLASS_H
