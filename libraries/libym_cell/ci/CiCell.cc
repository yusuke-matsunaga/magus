
/// @file CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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
  mInputNum(0),
  mInputArray(NULL),
  mOutputNum(0),
  mOutputArray(NULL),
  mInoutNum(0),
  mInoutArray(NULL),
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

// @brief 入力ピン数の取得
ymuint
CiCell::input_num() const
{
  return mInputNum;
}

// @brief 入力ピン+入出力ピン数の取得
// @note input_num() + inout_num() に等しい．
ymuint
CiCell::input_num2() const
{
  return mInputNum + mInoutNum;
}

// @brief 入力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
const CellPin*
CiCell::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ピン数の取得
ymuint
CiCell::output_num() const
{
  return mOutputNum;
}

// @brief 出力ピン+入出力ピン数の取得
// @note output_num() + inout_num() に等しい．
ymuint
CiCell::output_num2() const
{
  return mOutputNum + mInoutNum;
}

// @brief 出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
const CellPin*
CiCell::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 入出力ピン数の取得
ymuint
CiCell::inout_num() const
{
  return mInoutNum;
}

// @brief 入出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < inout_num() )
const CellPin*
CiCell::inout(ymuint pos) const
{
  return mInoutArray[pos];
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

// @brief 属している CellGroup を返す．
const CellGroup*
CiCell::cell_group() const
{
  return mCellGroup;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @return 条件に合致するタイミング情報を返す．
// @note なければ NULL を返す．
const CellTiming*
CiCell::timing(ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense) const
{
  ymuint base = opos * input_num2() + ipos;
  switch ( sense ) {
  case kCellPosiUnate:
    return mTimingArray[base + 0];

  case kCellNegaUnate:
    return mTimingArray[base + 1];

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return NULL;
}

#if 0
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
#endif

// @brief ピン数，バス数，バンドル数の設定をする．
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
void
CiCell::set_pinnum(ymuint ni,
		   ymuint no,
		   ymuint nio,
		   ymuint nb,
		   ymuint nc,
		   AllocBase& alloc)
{
  mInputNum = ni;
  void* p = alloc.get_memory(sizeof(CiPin*) * ni);
  mInputArray = new (p) CiPin*[ni];

  mOutputNum = no;
  void* q = alloc.get_memory(sizeof(CiPin*) * no);
  mOutputArray = new (q) CiPin*[no];

  mInoutNum = nio;
  void* r = alloc.get_memory(sizeof(CiPin*) * nio);
  mInoutArray = new (r) CiPin*[nio];

  ymuint n = (ni + nio) * (no + nio) * 2;
  void* s = alloc.get_memory(sizeof(const CiTiming*) * n);
  mTimingArray = new (s) CiTiming*[n];
  for (ymuint i = 0; i < n; ++ i) {
    mTimingArray[i] = NULL;
  }

  // バス，バンドル関係は未完

  mBusNum = nb;

  mBundleNum = nc;
}

// @brief 出力ピンの機能を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] function 機能を表す論理式
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
void
CiCell::set_logic_function(ymuint opin_id,
			   const LogExpr& function)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 出力ピンの three_state 条件を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] expr three_state 条件を表す論理式
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
void
CiCell::set_tristate_function(ymuint opin_id,
			      const LogExpr& expr)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief タイミング情報を設定する．
// @param[in] opin_id 出力(入出力)ピン番号 ( *1 )
// @param[in] ipin_id 関連する入力(入出力)ピン番号 ( *2 )
// @param[in] sense タイミング条件
// @param[in] timing 設定するタイミング情報
// @note ( *1 ) opin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->output_num() を使う．
// @note ( *2 ) ipin_id で入出力ピンを表す時には入出力ピン番号
//  + cell->input_num() を使う．
void
CiCell::set_timing(ymuint opin_id,
		   ymuint ipin_id,
		   tCellTimingSense sense,
		   CiTiming* timing)
{
  ymuint base = opin_id * input_num2() + ipin_id;
  switch ( sense ) {
  case kCellPosiUnate:
    mTimingArray[base + 0] = timing;
    break;

  case kCellNegaUnate:
    mTimingArray[base + 1] = timing;
    break;

  case kCellNonUnate:
    mTimingArray[base + 0] = timing;
    mTimingArray[base + 1] = timing;
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
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

