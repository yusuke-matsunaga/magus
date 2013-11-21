
/// @file CiCell.cc
/// @brief CiCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiCell.h"
#include "CiLibrary.h"
#include "CiPin.h"
#include "CiTiming.h"
#include "CiBus.h"
#include "CiBundle.h"
#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] library 親のセルライブラリ
// @param[in] id ID番号
// @param[in] name 名前
// @param[in] area 面積
// @param[in] ni 入力ピン数
// @param[in] no 出力ピン数
// @param[in] nio 入出力ピン数
// @param[in] nit 内部ピン数
// @param[in] nb バス数
// @param[in] nc バンドル数
// @param[in] output_array 出力の情報の配列(*1)
// @param[in] logic_array 出力の論理式の配列
// @param[in] tristated_array トライステート条件の論理式の配列
// @param[in] alloc メモリアロケータ
// *1: - false 論理式なし
//     - true 論理式あり
CiCell::CiCell(CiLibrary* library,
	       ymuint id,
	       const ShString& name,
	       CellArea area,
	       ymuint ni,
	       ymuint no,
	       ymuint nio,
	       ymuint nit,
	       ymuint nb,
	       ymuint nc,
	       const vector<bool>& output_array,
	       const vector<LogExpr>& logic_array,
	       const vector<LogExpr>& tristate_array,
	       Alloc& alloc)
{
  mLibrary = library;

  mId = id;
  mName = name;
  mArea = area;

  ymuint ni2 = ni + nio;
  ymuint no2 = no + nio;

  mPinNum = ni + no + nio + nit;
  {
    void* p = alloc.get_memory(sizeof(CiPin*) * mPinNum);
    mPinArray = new (p) CiPin*[mPinNum];
  }

  mInputNum = ni;
  {
    void* p = alloc.get_memory(sizeof(CiPin*) * ni2);
    mInputArray = new (p) CiPin*[ni2];
  }

  mOutputNum = no;
  {
    void* q = alloc.get_memory(sizeof(CiPin*) * no2);
    mOutputArray = new (q) CiPin*[no2];
  }

  mInternalNum = nit;
  if ( nit > 0 ) {
    void* p = alloc.get_memory(sizeof(CiPin*) * nit);
    mInternalArray = new (p) CiPin*[nit];
  }
  else {
    mInternalArray = NULL;
  }

  {
    ymuint n = ni2 * no2 * 2;
    void* s = alloc.get_memory(sizeof(const CiTimingArray*) * n);
    mTimingMap = new (s) CiTimingArray*[n];
    for (ymuint i = 0; i < n; ++ i) {
      mTimingMap[i] = NULL;
    }
  }

  // バス，バンドル関係は未完

  mBusNum = nb;

  mBundleNum = nc;

  void* x = alloc.get_memory(sizeof(ymuint8) * no2);
  mLTArray = new (x) ymuint8[no2];

  void* t = alloc.get_memory(sizeof(LogExpr) * no2);
  mLogicArray = new (t) LogExpr[no2];

  void* u = alloc.get_memory(sizeof(LogExpr) * no2);
  mTristateArray = new (u) LogExpr[no2];

  for (ymuint i = 0; i < no2; ++ i) {
    ymuint8 val = 0U;
    if ( output_array[i] ) {
      if ( !tristate_array[i].is_zero() ) {
	val = 2U;
      }
      else {
	val = 1U;
      }
    }
    mLTArray[i] = val;
    mLogicArray[i] = logic_array[i];
    mTristateArray[i] = tristate_array[i];
  }
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

// @brief ピン数の取得
ymuint
CiCell::pin_num() const
{
  return mPinNum;
}

// @brief ピンの取得
// @param[in] id ピン番号 ( 0 <= id < pin_num() )
const CellPin*
CiCell::pin(ymuint id) const
{
  return mPinArray[id];
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
CiCell::pin(const string& name) const
{
  return mLibrary->get_pin(this, ShString(name));
}

// @brief 名前からピンの取得
// @param[in] name ピン名
// @return name という名前をピンを返す．
// @note なければ NULL を返す．
const CellPin*
CiCell::pin(const char* name) const
{
  return mLibrary->get_pin(this, ShString(name));
}

// @brief 入力ピン数の取得
ymuint
CiCell::input_num() const
{
  return mInputNum;
}

// @brief 出力ピン数の取得
ymuint
CiCell::output_num() const
{
  return mOutputNum;
}

// @brief 入出力ピン数の取得
ymuint
CiCell::inout_num() const
{
  return mPinNum - mInputNum - mOutputNum - mInternalNum;
}

// @brief 内部ピン数の取得
ymuint
CiCell::internal_num() const
{
  return mInternalNum;
}

// @brief 入力ピン+入出力ピン数の取得
// @note input_num() + inout_num() に等しい．
ymuint
CiCell::input_num2() const
{
  return mInputNum + inout_num();
}

// @brief 入力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num2() )
const CellPin*
CiCell::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ピン+入出力ピン数の取得
// @note output_num() + inout_num() に等しい．
ymuint
CiCell::output_num2() const
{
  return mOutputNum + inout_num();
}

// @brief 出力ピンの取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num2() )
const CellPin*
CiCell::output(ymuint pos) const
{
  return mOutputArray[pos];
}

// @brief 内部ピンの取得
// @param[in] internal_id 内部ピン番号 ( 0 <= internal_id < internal_num() )
const CellPin*
CiCell::internal(ymuint internal_id) const
{
  return mInternalArray[internal_id];
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

// @brief タイミング情報の数の取得
ymuint
CiCell::timing_num() const
{
  return mTimingNum;
}

// @brief タイミング情報の取得
// @param[in] pos 位置番号 ( 0 <= pos < timing_num() )
const CellTiming*
CiCell::timing(ymuint pos) const
{
  assert_cond( pos < timing_num(), __FILE__, __LINE__);
  return mTimingArray[pos];
}

// @brief 条件に合致するタイミング情報の数の取得
// @param[in] ipos 開始ピン番号 ( 0 <= ipos < input_num2() )
// @param[in] opos 終了ピン番号 ( 0 <= opos < output_num2() )
// @param[in] timing_sense タイミング情報の摘要条件
ymuint
CiCell::timing_num(ymuint ipos,
		   ymuint opos,
		   tCellTimingSense sense) const
{
  ymuint base = (opos * input_num2() + ipos) * 2;
  switch ( sense ) {
  case kCellPosiUnate: base += 0; break;
  case kCellNegaUnate: base += 1; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( mTimingMap[base] == NULL ) {
    return 0;
  }
  return mTimingMap[base]->mNum;
}

// @brief タイミング情報の取得
// @param[in] ipos 開始ピン番号
// @param[in] opos 終了ピン番号
// @param[in] timing_sense タイミング情報の摘要条件
// @param[in] pos 位置番号 ( 0 <= pos < timing_num(ipos, opos, timing_sense) )
// @return 条件に合致するタイミング情報を返す．
const CellTiming*
CiCell::timing(ymuint ipos,
	       ymuint opos,
	       tCellTimingSense sense,
	       ymuint pos) const
{
  assert_cond( pos < timing_num(ipos, opos, sense), __FILE__, __LINE__);
  ymuint base = (opos * input_num2() + ipos) * 2;
  switch ( sense ) {
  case kCellPosiUnate: base += 0; break;
  case kCellNegaUnate: base += 1; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return mTimingMap[base]->mArray[pos];
}

// @brief 属している CellGroup を返す．
const CellGroup*
CiCell::cell_group() const
{
  return mCellGroup;
}

// @brief 組み合わせ論理セルの時に true を返す．
bool
CiCell::is_logic() const
{
  return false;
}

// @brief FFセルの時に true を返す．
bool
CiCell::is_ff() const
{
  return false;
}

// @brief ラッチセルの時に true を返す．
bool
CiCell::is_latch() const
{
  return false;
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiCell::is_fsm() const
{
  return false;
}

// @brief 出力の論理式を持っている時に true を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
bool
CiCell::has_logic(ymuint pin_id) const
{
  return mLTArray[pin_id] != 0U;
}

// @brief 全ての出力が論理式を持っているときに true を返す．
bool
CiCell::has_logic() const
{
  for (ymuint i = 0; i < output_num2(); ++ i) {
    if ( !has_logic(i) ) {
      return false;
    }
  }
  return true;
}

// @brief 論理セルの場合に出力の論理式を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
// @note 論理式中の変数番号は入力ピン番号に対応する．
LogExpr
CiCell::logic_expr(ymuint pin_id) const
{
  return mLogicArray[pin_id];
}

// @brief 出力がトライステート条件を持っている時に true を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
bool
CiCell::has_tristate(ymuint pin_id) const
{
  return mLTArray[pin_id] == 2U;
}

// @brief トライステートセルの場合にトライステート条件式を返す．
// @param[in] pin_id 出力ピン番号 ( 0 <= pin_id < output_num2() )
// @note 論理式中の変数番号は入力ピン番号に対応する．
// @note 通常の論理セルの場合には定数0を返す．
LogExpr
CiCell::tristate_expr(ymuint pin_id) const
{
  return mTristateArray[pin_id];
}

// @brief FFセルの場合に次状態関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::next_state_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセルの場合にクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::clock_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::clock2_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合にデータ入力関数を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::data_in_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合にイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::enable_expr() const
{
  return LogExpr::make_zero();
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す論理式を返す．
// @note それ以外の型の場合の返り値は不定
LogExpr
CiCell::enable2_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiCell::has_clear() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す論理式を返す．
// @note クリア端子がない場合の返り値は不定
LogExpr
CiCell::clear_expr() const
{
  return LogExpr::make_zero();
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiCell::has_preset() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す論理式を返す．
// @note プリセット端子がない場合の返り値は不定
LogExpr
CiCell::preset_expr() const
{
  return LogExpr::make_zero();
}

// @brief clear_preset_var1 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiCell::clear_preset_var1() const
{
  return 0;
}

// @brief clear_preset_var2 の取得
// @retval 0 "L"
// @retval 1 "H"
// @note FFセルとラッチセルの時に意味を持つ．
ymuint
CiCell::clear_preset_var2() const
{
  return 0;
}

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiCell::dump(ODO& s) const
{
  ymuint8 tid = 0;
  if ( is_logic() ) {
    tid = 0;
  }
  else if ( is_ff() ) {
    tid = 1;
  }
  else if ( is_latch() ) {
    tid = 2;
  }
  else if ( is_fsm() ) {
    tid = 3;
  }
  else {
    // 無視？
    assert_not_reached(__FILE__, __LINE__);
  }
  ymuint32 ni = input_num();
  ymuint32 no = output_num();
  ymuint32 nio = inout_num();
  ymuint32 nit = internal_num();
  ymuint32 nbus = bus_num();
  ymuint32 nbundle = bundle_num();

  s << tid
    << name()
    << area()
    << ni
    << no
    << nio
    << nit
    << nbus
    << nbundle;

  ymuint no2 = no + nio;
  for (ymuint opos = 0; opos < no2; ++ opos) {
    s << has_logic()
      << logic_expr(opos)
      << tristate_expr(opos);
  }

  if ( is_ff() ) {
    s << next_state_expr()
      << clock_expr()
      << clock2_expr()
      << clear_expr()
      << preset_expr()
      << static_cast<ymuint8>(clear_preset_var1())
      << static_cast<ymuint8>(clear_preset_var2());
  }
  else if ( is_latch() ) {
    s << data_in_expr()
      << enable_expr()
      << enable2_expr()
      << clear_expr()
      << preset_expr()
      << static_cast<ymuint8>(clear_preset_var1())
      << static_cast<ymuint8>(clear_preset_var2());
  }

  // 入力ピンのダンプ
  for (ymuint32 ipin = 0; ipin < ni; ++ ipin) {
    input(ipin)->dump(s);
  }

  // 出力ピンのダンプ
  for (ymuint32 opin = 0; opin < no; ++ opin) {
    output(opin)->dump(s);
  }

  // 入出力ピンのダンプ
  for (ymuint32 iopin = 0; iopin < nio; ++ iopin) {
    output(iopin)->dump(s);
  }

  // 内部ピンのダンプ
  for (ymuint32 itpin = 0; itpin < nit; ++ itpin) {
    internal(itpin)->dump(s);
  }

  // タイミング情報のダンプ
  ymuint32 nt = timing_num();
  s << nt;
  for (ymuint i = 0; i < nt; ++ i) {
    timing(i)->dump(s);
  }

  // ちょっと効率が悪いけど線形探索を用いている．
  // 効率が気になるならハッシュ関数を用いればよい．
  for (ymuint32 ipos = 0; ipos < ni + nio; ++ ipos) {
    for (ymuint32 opos = 0; opos < no + nio; ++ opos) {
      ymuint32 np = timing_num(ipos, opos, kCellPosiUnate);
      s << np;
      for (ymuint i = 0; i < np; ++ i) {
	const CellTiming* timing1 = timing(ipos, opos, kCellPosiUnate, i);
	for (ymuint32 j = 0; j < nt; ++ j) {
	  if ( timing(j) == timing1 ) {
	    s << j;
	  }
	}
      }
      ymuint32 nn = timing_num(ipos, opos, kCellNegaUnate);
      s << nn;
      for (ymuint i = 0; i < nn; ++ i) {
	const CellTiming* timing1 = timing(ipos, opos, kCellNegaUnate, i);
	for (ymuint32 j = 0; j < nt; ++ j) {
	  if ( timing(j) == timing1 ) {
	    s << j;
	  }
	}
      }
    }
  }
}

// @brief セルグループを設定する．
void
CiCell::set_group(const CellGroup* group)
{
  mCellGroup = group;
}

END_NAMESPACE_YM_CELL
