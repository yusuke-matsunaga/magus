#ifndef LIBYM_CELL_MISLIB_MISLIBTIMING_H
#define LIBYM_CELL_MISLIB_MISLIBTIMING_H

/// @file libym_cell/mislib/MislibTiming.h
/// @brief MislibTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibTiming MislibTiming.h "MislibTiming.h"
/// @brief Mislib 用の CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class MislibTiming :
  public CellTiming
{
  friend class MislibParserImpl;

protected:

  /// @brief コンストラクタ
  MislibTiming();

  /// @brief デストラクタ
  virtual
  ~MislibTiming();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連する終点ピンの取得
  virtual
  const CellPin*
  opin() const;

  /// @brief 条件の取得
  virtual
  tCellTimingSense
  timing_sense() const;

  /// @brief 型の取得
  virtual
  tCellTimingType
  timing_type() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック/CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり固有遅延の取得
  virtual
  CellTime
  intrinsic_rise() const;

  /// @brief 立ち下がり固有遅延の取得
  virtual
  CellTime
  intrinsic_fall() const;

  /// @brief 立ち上がりスロープ遅延の取得
  virtual
  CellTime
  slope_rise() const;

  /// @brief 立ち下がりスロープ遅延の取得
  virtual
  CellTime
  slope_fall() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOSジェネリック遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_resistance() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS折れ線近似遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延の取得
  virtual
  CellResistance
  rise_pin_resistance() const;

  /// @brief 立ち下がり遷移遅延の取得
  virtual
  CellResistance
  fall_pin_resistance() const;

  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 立ち上がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  rise_transition() const;

  /// @brief 立ち下がり遷移遅延テーブルの取得
  virtual
  const CellLut*
  fall_transition() const;

  /// @brief 立ち上がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  rise_propagation() const;

  /// @brief 立ち下がり伝搬遅延テーブルの取得
  virtual
  const CellLut*
  fall_propagation() const;

  /// @brief 立ち上がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_rise() const;

  /// @brief 立ち下がりセル遅延テーブルの取得
  virtual
  const CellLut*
  cell_fall() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力ピン
  const CellPin* mOpin;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibTiming1 MislibTiming.h "MislibTiming.h"
/// @brief 入力ピンが1つのタイプの継承クラス
//////////////////////////////////////////////////////////////////////
class MislibTiming1 :
  public MislibTiming
{
  friend class MislibParserImpl;

private:

  /// @brief コンストラクタ
  MislibTiming1();

  /// @brief デストラクタ
  virtual
  ~MislibTiming1();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連する開始ピン数の取得
  virtual
  ymuint
  ipin_num() const;

  /// @brief 関連する開始ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < ipin_num() )
  virtual
  const CellPin*
  ipin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピン
  const CellPin* mIpin;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibTiming2 MislibTiming.h "MislibTiming.h"
/// @brief 入力ピンが複数のタイプの継承クラス
//////////////////////////////////////////////////////////////////////
class MislibTiming2 :
  public MislibTiming
{
  friend class MislibParserImpl;

private:

  /// @brief コンストラクタ
  MislibTiming2();

  /// @brief デストラクタ
  virtual
  ~MislibTiming2();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief 関連する開始ピン数の取得
  virtual
  ymuint
  ipin_num() const;

  /// @brief 関連する開始ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < ipin_num() )
  virtual
  const CellPin*
  ipin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力ピンの数
  ymuint32 mIpinNum;

  // 入力ピンの配列
  const CellPin** mIpinArray;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBTIMING_H
