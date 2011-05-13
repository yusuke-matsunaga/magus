
/// @file libym_cell/CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"
#include "CiPin.h"
#include "CiTiming.h"
#include "CiBus.h"
#include "CiBundle.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
CiCell::CiCell(ymuint id,
	       const ShString& name,
	       CellArea area) :
  mId(id),
  mName(name),
  mArea(area),
  mPinNum(0),
  mPinArray(NULL),
  mBusNum(0),
  mBusArray(NULL),
  mBundleNum(0),
  mBundleArray(NULL)
{
}

// @brief デストラクタ
CiCell::~CiCell()
{
  // メモリの確保は別のクラスが担当している．
}

// @brief ID番号の取得
// @note ここで返される番号は CellLibrary::cell() の引数に対応する．
ymuint
CiCell::id() const
{
  return mId;
}

// @brief 名前の取得
string
CiCell::name() const
{
  return mName;
}

// @brief 面積の取得
CellArea
CiCell::area() const
{
  return mArea;
}

// @brief 組み合わせ論理セルの時に true を返す．
// @note type() == kLogic と等価
bool
CiCell::is_logic() const
{
  return false;
}

// @brief FFセルの時に true を返す．
// @note type() == kFF と等価
bool
CiCell::is_ff() const
{
  return false;
}

// @brief ラッチセルの時に true を返す．
// @note type() == kLatch と等価
bool
CiCell::is_latch() const
{
  return false;
}

// @brief FSMセルの時に true を返す．
// @note type() == kFSM と等価
bool
CiCell::is_fsm() const
{
  return false;
}

// @brief 状態変数1の名前を返す．
// @note FFセルとラッチセルの時に意味を持つ．
string
CiCell::var1_name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return string();
}

// @brief 状態変数2の名前を返す．
// @note FFセルとラッチセルの時に意味を持つ．
string
CiCell::var2_name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return string();
}

// @brief next_state 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiCell::next_state() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief clocked_on 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiCell::clocked_on() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief data_in 関数の取得
LogExpr
CiCell::data_in() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief enable 関数の取得
LogExpr
CiCell::enable() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief enable_also 関数の取得
LogExpr
CiCell::enable_also() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief clocked_on_also 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiCell::clocked_on_also() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief clear 関数の取得
// @note FFセルとラッチセルの時に意味を持つ．
LogExpr
CiCell::clear() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief preset 関数の取得
// @note FFセルとラッチセルの時に意味を持つ．
LogExpr
CiCell::preset() const
{
  assert_not_reached(__FILE__, __LINE__);
  return LogExpr();
}

// @brief clear_preset_var1 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiCell::clear_preset_var1() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief clear_preset_var2 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiCell::clear_preset_var2() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief ピン数の取得
ymuint
CiCell::pin_num() const
{
  return mPinNum;
}

// @brief ピンの取得
// @param[in] pin_id ピン番号 ( 0 <= pin_id < pin_num()
const CellPin*
CiCell::pin(ymuint pin_id) const
{
  return mPinArray[pin_id];
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
CiCell::pin(const string& name) const
{
  // 未完
  return NULL;
}

// @brief バス数の取得
ymuint
CiCell::bus_num() const
{
  return mBusNum;
}

// @brief バスの取得
// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
const CellBus*
CiCell::bus(ymuint pos) const
{
  return &mBusArray[pos];
}

// @brief 名前からバスの取得
// @param[in] name バス名
// @return name という名前のバスを返す．
// @note なければ NULL を返す．
const CellBus*
CiCell::bus(const string& name) const
{
  // 未完
  return NULL;
}

// @brief バンドル数の取得
ymuint
CiCell::bundle_num() const
{
  return mBundleNum;
}

// @brief バンドルの取得
// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
const CellBundle*
CiCell::bundle(ymuint pos) const
{
  return &mBundleArray[pos];
}

// @brief 名前からバンドルの取得
const CellBundle*
CiCell::bundle(const string& name) const
{
  // 未完
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLogicCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
CiLogicCell::CiLogicCell(ymuint id,
			 const ShString& name,
			 CellArea area) :
  CiCell(id, name, area)
{
}

// @brief デストラクタ
CiLogicCell::~CiLogicCell()
{
}

// @brief 型の取得
Cell::tType
CiLogicCell::type() const
{
  return kLogic;
}

// @brief 組み合わせ論理セルの時に true を返す．
// @note type() == kLogic と等価
bool
CiLogicCell::is_logic() const
{
  return true;
}



//////////////////////////////////////////////////////////////////////
// クラス CiFLCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] var1, var2 状態変数名
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 "clear_preset_var1" の値
// @param[in] clear_preset_var2 "clear_preset_var2" の値
CiFLCell::CiFLCell(ymuint id,
		   const ShString& name,
		   CellArea area,
		   const ShString& var1,
		   const ShString& var2,
		   const LogExpr& clear,
		   const LogExpr& preset,
		   ymuint clear_preset_var1,
		   ymuint clear_preset_var2) :
  CiCell(id, name, area),
  mVar1(var1),
  mVar2(var2),
  mClear(clear),
  mPreset(preset),
  mClearPresetVal((clear_preset_var1 & 1U) | ((clear_preset_var2 & 1U) << 1))
{
}

// @brief デストラクタ
CiFLCell::~CiFLCell()
{
}

// @brief 状態変数1の名前を返す．
// @note FFセルとラッチセルの時に意味を持つ．
string
CiFLCell::var1_name() const
{
  return mVar1;
}

// @brief 状態変数2の名前を返す．
// @note FFセルとラッチセルの時に意味を持つ．
string
CiFLCell::var2_name() const
{
  return mVar2;
}

// @brief clear 関数の取得
// @note FFセルとラッチセルの時に意味を持つ．
LogExpr
CiFLCell::clear() const
{
  return mClear;
}

// @brief preset 関数の取得
// @note FFセルとラッチセルの時に意味を持つ．
LogExpr
CiFLCell::preset() const
{
  return mPreset;
}

// @brief clear_preset_var1 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiFLCell::clear_preset_var1() const
{
  return mClearPresetVal & 1U;
}

// @brief clear_preset_var2 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiFLCell::clear_preset_var2() const
{
  return (mClearPresetVal >> 1) & 1U;
}


//////////////////////////////////////////////////////////////////////
// クラス CiFFCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] var1, var2 状態変数名
// @param[in] next_state "next_state" 関数の式
// @param[in] clocked_on "clocked_on" 関数の式
// @param[in] clocked_on_also "clocked_on_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 "clear_preset_var1" の値
// @param[in] clear_preset_var2 "clear_preset_var2" の値
CiFFCell::CiFFCell(ymuint id,
		   const ShString& name,
		   CellArea area,
		   const ShString& var1,
		   const ShString& var2,
		   const LogExpr& next_state,
		   const LogExpr& clocked_on,
		   const LogExpr& clocked_on_also,
		   const LogExpr& clear,
		   const LogExpr& preset,
		   ymuint clear_preset_var1,
		   ymuint clear_preset_var2) :
  CiFLCell(id, name, area,
	   var1, var2, clear, preset,
	   clear_preset_var1,
	   clear_preset_var2),
  mNextState(next_state),
  mClockedOn(clocked_on),
  mClockedOnAlso(clocked_on_also)
{
}

// @brief デストラクタ
CiFFCell::~CiFFCell()
{
}

// @brief 型の取得
Cell::tType
CiFFCell::type() const
{
  return kFF;
}

// @brief FFセルの時に true を返す．
// @note type() == kFF と等価
bool
CiFFCell::is_ff() const
{
  return true;
}

// @brief next_state 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiFFCell::next_state() const
{
  return mNextState;
}

// @brief clocked_on 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiFFCell::clocked_on() const
{
  return mClockedOn;
}

// @brief clocked_on_also 関数の取得
// @note FFセルの時に意味を持つ．
LogExpr
CiFFCell::clocked_on_also() const
{
  return mClockedOnAlso;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLatchCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] var1, var2 状態変数名
// @param[in] data_in "data_in" 関数の式
// @param[in] enable "enable" 関数の式
// @param[in] enable_also "enable_also" 関数の式
// @param[in] clear "clear" 関数の式
// @param[in] preset "preset" 関数の式
// @param[in] clear_preset_var1 "clear_preset_var1" の値
// @param[in] clear_preset_var2 "clear_preset_var2" の値
CiLatchCell::CiLatchCell(ymuint id,
			 const ShString& name,
			 CellArea area,
			 const ShString& var1,
			 const ShString& var2,
			 const LogExpr& data_in,
			 const LogExpr& enable,
			 const LogExpr& enable_also,
			 const LogExpr& clear,
			 const LogExpr& preset,
			 ymuint clear_preset_var1,
			 ymuint clear_preset_var2) :
  CiFLCell(id, name, area,
	   var1, var2, clear, preset,
	   clear_preset_var1,
	   clear_preset_var2),
  mDataIn(data_in),
  mEnable(enable),
  mEnableAlso(enable_also)
{
}

// @brief デストラクタ
CiLatchCell::~CiLatchCell()
{
}

// @brief 型の取得
Cell::tType
CiLatchCell::type() const
{
  return kLatch;
}

// @brief ラッチセルの時に true を返す．
// @note type() == kLatch と等価
bool
CiLatchCell::is_latch() const
{
  return true;
}

// @brief data_in 関数の取得
LogExpr
CiLatchCell::data_in() const
{
  return mDataIn;
}

// @brief enable 関数の取得
LogExpr
CiLatchCell::enable() const
{
  return mEnable;
}

// @brief enable_also 関数の取得
LogExpr
CiLatchCell::enable_also() const
{
  return mEnableAlso;
}

END_NAMESPACE_YM_CELL

