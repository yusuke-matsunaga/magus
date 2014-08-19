
/// @file CiTiming.cc
/// @brief CiTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CiTiming.h"
#include "YmCell/CellLut.h"


BEGIN_NAMESPACE_YM_CELL

BEGIN_NONAMESPACE

void
dump_lut(ODO& s,
	 const CellLut* lut)
{
  if ( lut ) {
    lut->dump(s);
  }
  else {
    s.write_str(string());
  }
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス CiTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] type タイミング条件の型
// @param[in] cond タイミング条件を表す式
CiTiming::CiTiming(ymuint id,
		   tCellTimingType type,
		   const Expr& cond) :
  mId(id),
  mType(type),
  mCond(cond)
{
}

// @brief デストラクタ
CiTiming::~CiTiming()
{
}

// @brief ID番号の取得
// @note timing = cell->timing(id); の時，timing->id() = id となる．
ymuint
CiTiming::id() const
{
  return mId;
}

// @brief 型の取得
tCellTimingType
CiTiming::type() const
{
  return mType;
}

// @brief タイミング条件式の取得
// @note ない場合には定数1の式が返される．
Expr
CiTiming::timing_cond() const
{
  return mCond;
}

// @brief 立ち上がり固有遅延の取得
CellTime
CiTiming::intrinsic_rise() const
{
  return CellTime(0.0);
}

// @brief 立ち下がり固有遅延の取得
CellTime
CiTiming::intrinsic_fall() const
{
  return CellTime(0.0);
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
CiTiming::slope_rise() const
{
  return CellTime(0.0);
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
CiTiming::slope_fall() const
{
  return CellTime(0.0);
}

// @brief 立ち上がり遷遅延の取得
CellResistance
CiTiming::rise_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CiTiming::fall_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
CiTiming::rise_pin_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CiTiming::fall_pin_resistance() const
{
  return CellResistance(0.0);
}

// @brief 立ち上がり？？？
CellTime
CiTiming::rise_delay_intercept() const
{
  return CellTime(0.0);
}

// @brief 立ち下がり？？？
CellTime
CiTiming::fall_delay_intercept() const
{
  return CellTime(0.0);
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
CiTiming::rise_transition() const
{
  return NULL;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
CiTiming::fall_transition() const
{
  return NULL;
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
CiTiming::rise_propagation() const
{
  return NULL;
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
CiTiming::fall_propagation() const
{
  return NULL;
}

// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
CiTiming::cell_rise() const
{
  return NULL;
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
CiTiming::cell_fall() const
{
  return NULL;
}

// @brief 共通な情報をダンプする．
// @param[in] s 出力先のストリーム
// @param[in] type_id 型の ID
void
CiTiming::dump_common(ODO& s,
		      ymuint8 type_id) const
{
  s << type_id
    << static_cast<ymuint8>(type())
    << timing_cond();
}


//////////////////////////////////////////////////////////////////////
// クラス CiTimingGB
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] timing_type タイミングの型
// @param[in] cond タイミング条件を表す式
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
CiTimingGP::CiTimingGP(ymuint id,
		       tCellTimingType timing_type,
		       const Expr& cond,
		       CellTime intrinsic_rise,
		       CellTime intrinsic_fall,
		       CellTime slope_rise,
		       CellTime slope_fall) :
  CiTiming(id, timing_type, cond),
  mIntrinsicRise(intrinsic_rise),
  mIntrinsicFall(intrinsic_fall),
  mSlopeRise(slope_rise),
  mSlopeFall(slope_fall)
{
}

// @brief デストラクタ
CiTimingGP::~CiTimingGP()
{
}

// @brief 立ち上がり固有遅延の取得
CellTime
CiTimingGP::intrinsic_rise() const
{
  return mIntrinsicRise;
}

// @brief 立ち下がり固有遅延の取得
CellTime
CiTimingGP::intrinsic_fall() const
{
  return mIntrinsicFall;
}

// @brief 立ち上がりスロープ遅延の取得
CellTime
CiTimingGP::slope_rise() const
{
  return mSlopeRise;
}

// @brief 立ち下がりスロープ遅延の取得
CellTime
CiTimingGP::slope_fall() const
{
  return mSlopeFall;
}


//////////////////////////////////////////////////////////////////////
// クラス CiTimingGeneric
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] timing_type タイミングの型
// @param[in] cond タイミング条件を表す式
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
// @param[in] rise_resistance 立ち上がり遷移遅延パラメータ
// @param[in] fall_resistance 立ち下がり遷移遅延パラメータ
CiTimingGeneric::CiTimingGeneric(ymuint id,
				 tCellTimingType timing_type,
				 const Expr& cond,
				 CellTime intrinsic_rise,
				 CellTime intrinsic_fall,
				 CellTime slope_rise,
				 CellTime slope_fall,
				 CellResistance rise_resistance,
				 CellResistance fall_resistance) :
  CiTimingGP(id, timing_type, cond,
	     intrinsic_rise, intrinsic_fall,
	     slope_rise, slope_fall),
  mRiseResistance(rise_resistance),
  mFallResistance(fall_resistance)
{
}

// @brief デストラクタ
CiTimingGeneric::~CiTimingGeneric()
{
}


// @brief 立ち上がり遷移遅延の取得
CellResistance
CiTimingGeneric::rise_resistance() const
{
  return mRiseResistance;
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CiTimingGeneric::fall_resistance() const
{
  return mFallResistance;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiTimingGeneric::dump(ODO& s) const
{
  dump_common(s, 0);

  s << intrinsic_rise()
    << intrinsic_fall()
    << slope_rise()
    << slope_fall()
    << rise_resistance()
    << fall_resistance();
}


//////////////////////////////////////////////////////////////////////
// クラス CiTimingPiecewise
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] timing_type タイミングの型
// @param[in] cond タイミング条件を表す式
// @param[in] intrinsic_rise 立ち上がり固有遅延
// @param[in] intrinsic_fall 立ち下がり固有遅延
// @param[in] slope_rise 立ち上がりスロープ遅延
// @param[in] slope_fall 立ち下がりスロープ遅延
CiTimingPiecewise::CiTimingPiecewise(ymuint id,
				     tCellTimingType timing_type,
				     const Expr& cond,
				     CellTime intrinsic_rise,
				     CellTime intrinsic_fall,
				     CellTime slope_rise,
				     CellTime slope_fall,
				     CellResistance rise_pin_resistance,
				     CellResistance fall_pin_resistance) :
  CiTimingGP(id, timing_type, cond,
	     intrinsic_rise, intrinsic_fall,
	     slope_rise, slope_fall),
  mRisePinResistance(rise_pin_resistance),
  mFallPinResistance(fall_pin_resistance)
{
}

// @brief デストラクタ
CiTimingPiecewise::~CiTimingPiecewise()
{
}

// @brief 立ち上がり遷移遅延の取得
CellResistance
CiTimingPiecewise::rise_pin_resistance() const
{
  return mRisePinResistance;
}

// @brief 立ち下がり遷移遅延の取得
CellResistance
CiTimingPiecewise::fall_pin_resistance() const
{
  return mFallPinResistance;
}

// @brief 立ち上がり？？？
CellTime
CiTimingPiecewise::rise_delay_intercept() const
{
#warning "TODO: CiTimingPiecewise::rise_delay_intercept"
  return CellTime(0.0);
}

// @brief 立ち下がり？？？
CellTime
CiTimingPiecewise::fall_delay_intercept() const
{
#warning "TODO: CiTimingPiecewise::fall_delay_intercept"
  return CellTime(0.0);
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiTimingPiecewise::dump(ODO& s) const
{
  dump_common(s, 1);

  s << intrinsic_rise()
    << intrinsic_fall()
    << slope_rise()
    << slope_fall();
}


//////////////////////////////////////////////////////////////////////
// クラス CiTimingLut1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] timing_type タイミングの型
// @param[in] cond タイミング条件を表す式
// @param[in] cell_rise 立ち上がりセル遅延テーブル
// @param[in] cell_fall 立ち下がりセル遅延テーブル
// @param[in] rise_transition 立ち上がり遷移遅延テーブル
// @param[in] fall_transition 立ち下がり遷移遅延テーブル
CiTimingLut1::CiTimingLut1(ymuint id,
			   tCellTimingType timing_type,
			   const Expr& cond,
			   CellLut* cell_rise,
			   CellLut* cell_fall,
			   CellLut* rise_transition,
			   CellLut* fall_transition) :
  CiTiming(id, timing_type, cond),
  mCellRise(cell_rise),
  mCellFall(cell_fall),
  mRiseTransition(rise_transition),
  mFallTransition(fall_transition)
{
}

// @brief デストラクタ
CiTimingLut1::~CiTimingLut1()
{
}

// @brief 立ち上がりセル遅延テーブルの取得
const CellLut*
CiTimingLut1::cell_rise() const
{
  return mCellRise;
}

// @brief 立ち下がりセル遅延テーブルの取得
const CellLut*
CiTimingLut1::cell_fall() const
{
  return mCellFall;
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
CiTimingLut1::rise_transition() const
{
  return mRiseTransition;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
CiTimingLut1::fall_transition() const
{
  return mFallTransition;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiTimingLut1::dump(ODO& s) const
{
  dump_common(s, 2);

  dump_lut(s, cell_rise());
  dump_lut(s, cell_fall());
  dump_lut(s, rise_transition());
  dump_lut(s, fall_transition());
}


//////////////////////////////////////////////////////////////////////
// クラス CiTimingLut2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] timing_type タイミングの型
// @param[in] cond タイミング条件を表す式
// @param[in] rise_transition 立ち上がり遷移遅延テーブル
// @param[in] fall_transition 立ち下がり遷移遅延テーブル
// @param[in] rise_propagation 立ち上がり伝搬遅延テーブル
// @param[in] fall_propagation 立ち下がり伝搬遅延テーブル
CiTimingLut2::CiTimingLut2(ymuint id,
			   tCellTimingType timing_type,
			   const Expr& cond,
			   CellLut* rise_transition,
			   CellLut* fall_transition,
			   CellLut* rise_propagation,
			   CellLut* fall_propagation) :
  CiTiming(id, timing_type, cond),
  mRiseTransition(rise_transition),
  mFallTransition(fall_transition),
  mRisePropagation(rise_propagation),
  mFallPropagation(fall_propagation)
{
}

// @brief デストラクタ
CiTimingLut2::~CiTimingLut2()
{
}

// @brief 立ち上がり遷移遅延テーブルの取得
const CellLut*
CiTimingLut2::rise_transition() const
{
  return mRiseTransition;
}

// @brief 立ち下がり遷移遅延テーブルの取得
const CellLut*
CiTimingLut2::fall_transition() const
{
  return mFallTransition;
}

// @brief 立ち上がり伝搬遅延テーブルの取得
const CellLut*
CiTimingLut2::rise_propagation() const
{
  return mRisePropagation;
}

// @brief 立ち下がり伝搬遅延テーブルの取得
const CellLut*
CiTimingLut2::fall_propagation() const
{
  return mFallPropagation;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiTimingLut2::dump(ODO& s) const
{
  dump_common(s, 3);

  dump_lut(s, rise_transition());
  dump_lut(s, fall_transition());
  dump_lut(s, rise_propagation());
  dump_lut(s, fall_propagation());
}

END_NAMESPACE_YM_CELL
