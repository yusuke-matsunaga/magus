#ifndef CIBUS_H
#define CIBUS_H

/// @file CiBus.h
/// @brief CiBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/CellBus.h"
#include "utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiBus CiBus.h "CiBus.h"
/// @brief CellBus の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBus :
  public CellBus
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiBus();

  /// @brief デストラクタ
  ~CiBus();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief バスの型の取得
  virtual
  const CellBusType*
  bus_type() const;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  virtual
  const CellPin*
  pin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // バスの型
  const CellBusType* mBusType;

  // ピン数
  ymuint32 mPinNum;

  // ピンの配列
  CellPin** mPinList;

};

END_NAMESPACE_YM_CELL

#endif // CIBUS_H
