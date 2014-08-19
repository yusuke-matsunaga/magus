#ifndef CIBUSTYPE_H
#define CIBUSTYPE_H

/// @file CiBusType.h
/// @brief CiBusType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/CellBusType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiBusType CiBusType.h "CiBusType.h"
/// @brief CellBusType の実装クラス
//////////////////////////////////////////////////////////////////////
class CiBusType :
  public CellBusType
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] bit_from 開始位置
  /// @param[in] bit_to 終了位置
  CiBusType(ShString name,
	      ymint bit_from,
	      ymint bit_to);

  /// @brief デストラクタ
  virtual
  ~CiBusType();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief base_type の取得
  virtual
  tBaseType
  base_type() const;

  /// @brief data_type の取得
  virtual
  tDataType
  data_type() const;

  /// @brief ビット幅の取得
  virtual
  ymuint
  bit_width() const;

  /// @brief 開始ビットの取得
  virtual
  ymint
  bit_from() const;

  /// @brief 終了ビットの取得
  virtual
  ymint
  bit_to() const;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  virtual
  bool
  downto() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ビット幅
  ymuint32 mBitWidth;

  // 開始ビット
  ymint32 mBitFrom;

  // 終了ビット
  ymint32 mBitTo;

  // 向き
  bool mDownTo;

};

END_NAMESPACE_YM_CELL

#endif // CIBUS_H
