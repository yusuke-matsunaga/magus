#ifndef YM_CELL_CELLBUS_H
#define YM_CELL_CELLBUS_H

/// @file　ym_cell/CellBus.h
/// @brief CellBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellBus.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "ym_cell/cell_nsdef.h"
#include "ym_cell/cell_type.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellBusType CellBus.h <ym_cell/CellBus.h>
//////////////////////////////////////////////////////////////////////
class CellBusType
{
  friend class CellManip;
  
public:

  /// @brief base_type を表す型
  enum tBaseType {
    kArrayType
  };
  
  /// @brief data_type を表す型
  enum tDataType {
    kBitType
  };
  

private:

  /// @brief コンストラクタ
  CellBusType(ShString name,
	      tBaseType base_type,
	      tDataType data_type,
	      ymint32 bit_from,
	      ymint32 bit_to);

  /// @brief デストラクタ
  ~CellBusType();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////
  
  /// @brief 名前の取得
  ShString
  name() const;
  
  /// @brief base_type の取得
  tBaseType
  base_type() const;
  
  /// @brief data_type の取得
  tDataType
  data_type() const;

  /// @brief ビット幅の取得
  ymuint32
  bit_width() const;

  /// @brief 開始ビットの取得
  ymint32
  bit_from() const;

  /// @brief 終了ビットの取得
  ymint32
  bit_to() const;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  bool
  downto() const;
  
  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 名前
  ShString mName;
  
  // base_type
  tBaseType mBaseType;

  // data_type
  tDataType mDataType;

  // ビット幅
  ymuint32 mBitWidth;

  // 開始ビット
  ymint32 mBitFrom;

  // 終了ビット
  ymint32 mBitTo;

  // 向き
  bool mDownTo;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellBus CellBus.h <ym_cell/CellBus.h>
/// @brief バスを表すクラス
//////////////////////////////////////////////////////////////////////
class CellBus
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  /// @param[in] name バス名
  /// @param[in] bus_type バス型
  CellBus(ShString name,
	  const CellBusType* bus_type);

  /// @brief デストラクタ
  ~CellBus();

  
public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  ShString
  name() const;
  
  /// @brief バスの型の取得
  const CellBusType*
  bus_type() const;

  /// @brief ピン数の取得
  ymuint32
  n_pins() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
  const CellPin*
  pin(ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 名前
  ShString mName;
  
  // バスの型
  const CellBusType* mBusType;
  
  // ピンのリスト
  vector<CellPin*> mPinList;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////
  
// @brief 名前の取得
inline
ShString
CellBusType::name() const
{
  return mName;
}
  
// @brief base_type の取得
inline
CellBusType::tBaseType
CellBusType::base_type() const
{
  return mBaseType;
}

// @brief data_type の取得
inline
CellBusType::tDataType
CellBusType::data_type() const
{
  return mDataType;
}

// @brief ビット幅の取得
inline
ymuint32
CellBusType::bit_width() const
{
  return mBitWidth;
}

// @brief 開始ビットの取得
inline
int
CellBusType::bit_from() const
{
  return mBitFrom;
}

// @brief 終了ビットの取得
inline
int
CellBusType::bit_to() const
{
  return mBitTo;
}

// @brief 向きの取得
// @note true の時，降順を表す．
inline
bool
CellBusType::downto() const
{
  return mDownTo;
}

// @brief 名前の取得
inline
ShString
CellBus::name() const
{
  return mName;
}
  
// @brief バスの型の取得
inline
const CellBusType*
CellBus::bus_type() const
{
  return mBusType;
}

// @brief ピン数の取得
inline
ymuint32
CellBus::n_pins() const
{
  return mPinList.size();
}

// @brief ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < n_pins() )
inline
const CellPin*
CellBus::pin(ymuint32 pos) const
{
  return mPinList[pos];
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLBUS_H
