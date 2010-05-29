#ifndef LIBYM_CELL_CELLTIMINGIMPL_H
#define LIBYM_CELL_CELLTIMINGIMPL_H

/// @file　libym_cell/CellTimingImpl.h
/// @brief CellTiming の実装クラスのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellTimingImpl.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellTiming.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTimingBase CellTimingImpl.h "CellTimingImpl.h"
/// @brief CMOSジェネリック/折れ線近似タイプ共通の CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CellTimingBase :
  public CellTiming
{
protected:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrisic_rise 立ち上がり固有遅延
  /// @param[in] intrisic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  CellTimingBase(const vector<const CellPin*>& ipin_list,
		 CellPin* opin,
		 tCellTimingSense timing_sense,
		 tCellTimingType timing_type,
		 CellTime intrisic_rise,
		 CellTime intrisic_fall,
		 CellTime slope_rise,
		 CellTime slope_fall);

  /// @brief デストラクタ
  ~CellTimingBase();
  

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
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 立ち上がり固有遅延
  CellTime mIntrisicRise;

  // 立ち下がり固有遅延
  CellTime mIntrisicFall;

  // 立ち上がりスロープ遅延
  CellTime mSlopeRise;

  // 立ち下がりスロープ遅延
  CellTime mSlopeFall;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellTimingGeneric CellTimingImpl.h "CellTimingImpl.h"
/// @brief CMOSジェネリックタイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CellTimingGeneric :
  public CellTimingBase
{
  friend class CellManip;

private:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrisic_rise 立ち上がり固有遅延
  /// @param[in] intrisic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  /// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
  /// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
  CellTimingGeneric(const vector<const CellPin*>& ipin_list,
		    CellPin* opin,
		    tCellTimingSense timing_sense,
		    tCellTimingType timing_type,
		    CellTime intrisic_rise,
		    CellTime intrisic_fall,
		    CellTime slope_rise,
		    CellTime slope_fall,
		    CellResistance rise_resistance,
		    CellResistance fall_resistance);

  /// @brief デストラクタ
  ~CellTimingGeneric();


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
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 立ち上がり遷移遅延
  CellResistance mRiseResistance;

  // 立ち下がり遷移遅延
  CellResistance mFallResistance;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellTimingPiecewise CellTimingImpl.h "CellTimingImpl.h"
/// @brief CMOS折れ線近似タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CellTimingPiecewise :
  public CellTimingBase
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] intrisic_rise 立ち上がり固有遅延
  /// @param[in] intrisic_fall 立ち下がり固有遅延
  /// @param[in] slope_rise 立ち上がりスロープ遅延
  /// @param[in] slope_fall 立ち下がりスロープ遅延
  CellTimingPiecewise(const vector<const CellPin*>& ipin_list,
		      CellPin* opin,
		      tCellTimingSense timing_sense,
		      tCellTimingType timing_type,
		      CellTime intrisic_rise,
		      CellTime intrisic_fall,
		      CellTime slope_rise,
		      CellTime slope_fall, 
		      CellResistance rise_pin_resistance,
		      CellResistance fall_pin_resistance);

  /// @brief デストラクタ
  ~CellTimingPiecewise();


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

#if 0
  /// @brief 立ち上がり？？？
  virtual
  CellTime
  rise_delay_intercept() const;

  /// @brief 立ち下がり？？？
  virtual
  CellTime
  fall_delay_intercept() const;
#endif
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 立ち上がり遷移遅延
  CellResistance mRisePinResistance;

  // 立ち下がり遷移遅延
  CellResistance mFallPinResistance;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellTimingNonlinear1 CellTimingImpl.h "CellTimingImpl.h"
/// @brief CMOS非線形タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CellTimingNonlinear1 :
  public CellTiming
{
  friend class CellManip;

private:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] cell_rise 立ち上がりセル遅延テーブル
  /// @param[in] cell_fall 立ち下がりセル遅延テーブル
  CellTimingNonlinear1(const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type,
		       CellLut* cell_rise,
		       CellLut* cell_fall);
	     

  /// @brief デストラクタ
  ~CellTimingNonlinear1();

  
public:
  //////////////////////////////////////////////////////////////////////
  // CMOS非線形遅延モデルの属性
  //////////////////////////////////////////////////////////////////////
  
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
  
  // 立ち上がりセル遅延テーブル
  const CellLut* mCellRise;

  // 立ち下がりセル遅延テーブル
  const CellLut* mCellFall;
  
};


//////////////////////////////////////////////////////////////////////
/// @class CellTimingNonlinear2 CellTimingImpl.h "CellTimingImpl.h"
/// @brief CMOS非線形タイプの CellTiming の実装クラス
//////////////////////////////////////////////////////////////////////
class CellTimingNonlinear2 :
  public CellTiming
{
  friend class CellManip;
  
private:

  /// @brief コンストラクタ
  /// @param[in] ipin_list 開始ピンのリスト
  /// @param[in] opin 終点ピン
  /// @param[in] timing_sense タイミング条件
  /// @param[in] timing_type タイミングの型
  /// @param[in] rise_transition 立ち上がり遷移遅延テーブル
  /// @param[in] fall_transition 立ち下がり遷移遅延テーブル
  /// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
  /// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
  CellTimingNonlinear2(const vector<const CellPin*>& ipin_list,
		       CellPin* opin,
		       tCellTimingSense timing_sense,
		       tCellTimingType timing_type,
		       CellLut* rise_transition,
		       CellLut* fall_transition,
		       CellLut* rise_propagation,
		       CellLut* fall_propagation);
	     

  /// @brief デストラクタ
  ~CellTimingNonlinear2();

  
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
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////
  
  // 立ち上がり遷移遅延テーブル
  const CellLut* mRiseTransition;

  // 立ち下がり遷移遅延テーブル
  const CellLut* mFallTransition;

  // 立ち上がり伝搬遅延テーブル
  const CellLut* mRisePropagation;

  // 立ち下がり伝搬遅延テーブル
  const CellLut* mFallPropagation;
  
};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_CELLTIMINGIMPL_H
