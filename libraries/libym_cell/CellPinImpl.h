#ifndef LIBYM_CELL_CELLPINIMPL_H
#define LIBYM_CELL_CELLPINIMPL_H

/// @file　libym_cell/CellPinImpl.h
/// @brief CellPinImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellPinImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.

#include <ym_cell/CellPin.h>


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellInputPin CellPinImpl.h "CellPinImpl.h"
/// @brief セルの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CellInputPin :
  public CellPin
{
  friend class CellManip;
  
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CellInputPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CellInputPin();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 負荷
  CellCapacitance mCapacitance;
  
  // rise 負荷
  CellCapacitance mRiseCapacitance;
  
  // fall 負荷
  CellCapacitance mFallCapacitance;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellOutputPin CellPinImpl.h "CellPinImpl.h"
/// @brief セルの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class CellOutputPin :
  public CellPin
{
  friend class CellManip;
  
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CellOutputPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CellOutputPin();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 出力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 最大ファンアウト容量を返す．
  virtual
  CellCapacitance
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  virtual
  CellCapacitance
  min_fanout() const;
  
  /// @brief 最大負荷容量を返す．
  virtual
  CellCapacitance
  max_capacitance() const;
  
  /// @brief 最小負荷容量を返す．
  virtual
  CellCapacitance
  min_capacitance() const;
  
  /// @brief 最大遷移時間を返す．
  virtual
  CellTime
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  virtual
  CellTime
  min_transition() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // ファンアウトの負荷
  CellCapacitance mFanoutLoad;

  // 最大ファンアウト容量
  CellCapacitance mMaxFanout;

  // 最小ファンアウト容量
  CellCapacitance mMinFanout;

  // 最大遷移時間
  CellTime mMaxTransition;

  // 最小遷移時間
  CellTime mMinTransition;

  // 最大負荷容量
  CellCapacitance mMaxCapacitance;

  // 最小負荷容量
  CellCapacitance mMinCapacitance;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellInoutPin CellPinImpl.h "CellPinImpl.h"
/// @brief セルの入出力ピンを表すクラス
/// @note 多重継承はオーバーヘッドがかかるので愚直な実装を用いている．
//////////////////////////////////////////////////////////////////////
class CellInoutPin :
  public CellOutputPin
{
  friend class CellManip;
  
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  CellInoutPin(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~CellInoutPin();
  
  
public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 方向を返す．
  virtual
  tCellDirection
  direction() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ピンの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 負荷容量を返す．
  virtual
  CellCapacitance
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  virtual
  CellCapacitance
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  virtual
  CellCapacitance
  fall_capacitance() const;
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 負荷
  CellCapacitance mCapacitance;
  
  // rise 負荷
  CellCapacitance mRiseCapacitance;
  
  // fall 負荷
  CellCapacitance mFallCapacitance;
  
};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CELLPIN_H
