#ifndef LIBYM_CELL_MISLIB_MISLIBPIN_H
#define LIBYM_CELL_MISLIB_MISLIBPIN_H

/// @file libym_cell/mislib/MislibPin.h
/// @brief MislibPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellPin.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibPin MislibPin.h "MislibPin.h"
/// @brief Mislib 用の CellPin の実装クラス
//////////////////////////////////////////////////////////////////////
class MislibPin :
  public CellPin
{
  friend class MislibParseImpl;

protected:

  /// @brief コンストラクタ
  MislibPin();

  /// @brief デストラクタ
  virtual
  ~MislibPin();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通属性
  //////////////////////////////////////////////////////////////////////

  /// @brief ピン名を返す．
  virtual
  string
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibInputPin MislibPin.h "MislibPin.h"
/// @brief 入力ピンを表す MislibPin の継承クラス
//////////////////////////////////////////////////////////////////////
class MislibInputPin :
  public MislibPin
{
  friend class MislibParseImpl;

private:

  /// @brief コンストラクタ
  MislibInputPin();

  /// @brief デストラクタ
  virtual
  ~MislibInputPin();


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

  // 負荷容量
  CellCapacitance mCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibOutputPin MislibPin.h "MislibPin.h"
/// @brief 出力ピン用の MislibPin の継承クラス
//////////////////////////////////////////////////////////////////////
class MislibOutputPin :
  public MislibPin
{
  friend class MislibParseImpl;

private:

  /// @brief コンストラクタ
  MislibOutputPin();

  /// @brief デストラクタ
  virtual
  ~MislibOutputPin();


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

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBPIN_H
