
/// @file CiGroup.cc
/// @brief CiGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiGroup.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiGroup::CiGroup() :
  mCellClass(NULL),
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
CiGroup::~CiGroup()
{
  // mCellList は CellMgr が管理する．
}

// @brief 属している CellClass を返す．
const CellClass*
CiGroup::cell_class() const
{
  return mCellClass;
}

// @brief 入力ピン数の取得
ymuint
CiGroup::input_num() const
{
  return cell_class()->input_num();
}

// @brief 出力ピン数の取得
ymuint
CiGroup::output_num() const
{
  return cell_class()->output_num();
}

// @brief 入出力ピン数の取得
ymuint
CiGroup::inout_num() const
{
  return cell_class()->inout_num();
}

// @brief 入力ピンの割り当てを得る．
// @param[in] ipos このグループのセルの入力ピン番号 ( 0 <= ipos < input_num() )
// @return ipos に対応するクラスの代表セル(グループ)のピン番号を返す．
ymuint
CiGroup::input_map(ymuint ipos) const
{
}

// @brief 入力ピンの極性を得る．
// @param[in] ipos このグループのセルの入力ピン番号 ( 0 <= ipos < input_num() )
// @return ipos に対応するクラスの代表セル(グループ)のピンとの極性差を返す．
// @note false で同位相，true で逆位相という意味
bool
CiGroup::input_inv(ymuint ipos) const
{
}

// @brief 出力ピンの割り当てを得る．
// @param[in] opos このグループのセルの出力ピン番号 ( 0 <= opos < output_num() )
// @return opos に対応するクラスの代表セル(グループ)のピン番号を返す．
ymuint
CiGroup::output_map(ymuint opos) const
{
}

// @brief 出力ピンの極性を得る．
// @param[in] opos このグループのセルの出力ピン番号 ( 0 <= opos < output_num() )
// @return opos に対応するクラスの代表セル(グループ)のピンとの極性差を返す．
// @note false で同位相，true で逆位相という意味
bool
CiGroup::output_inv(ymuint ipos) const
{
}

// @brief 論理セルの場合に true を返す．
bool
CiGroup::is_logic() const
{
  return cell_class()->is_logic();
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
bool
CiGroup::is_tristate() const
{
  return cell_class()->is_tristate();
}

// @brief FFセルの場合に true を返す．
bool
CiGroup::is_ff() const
{
  return cell_class()->is_ff();
}

// @brief ラッチセルの場合に true を返す．
bool
CiGroup::is_latch() const
{
  return cell_class()->is_latch();
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiGroup::is_seq() const
{
  return cell_class()->is_seq();
}

// @brief 出力の論理関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note FF/ラッチの場合は状態変数の変数(Q, XQ)が2つ入力に加わる．
TvFunc
CiGroup::logic_function(ymuint pos) const
{
  return mLogicFunction[pos];
}

// @brief 出力のトライステート条件関数を返す．
// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
// @note トライステートでない場合には定数0関数を返す．
TvFunc
CiGroup::tristate_function(ymuint pos) const
{
  return mTristateFunction[pos];
}

// @brief FFセルの場合に次状態関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::next_state_function() const
{
#warning "TODO: not implemented"
}

// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::clock_function() const
{
#warning "TODO: not implemented"
}

// @brief FFセルの場合にスレーブクロックのアクティブエッジを表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::clock2_function() const
{
#warning "TODO: not implemented"
}

// @brief ラッチセルの場合にデータ入力関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::data_in_function() const
{
#warning "TODO: not implemented"
}

// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::enable_function() const
{
#warning "TODO: not implemented"
}

// @brief ラッチセルの場合に2つめのイネーブル条件を表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiGroup::enable2_function() const
{
#warning "TODO: not implemented"
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiGroup::has_clear() const
{
  return cell_class()->has_clear();
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
// @note クリア端子がない場合の返り値は不定
TvFunc
CiGroup::clear_function() const
{
#warning "TODO: not implemented"
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiGroup::has_preset() const
{
  return cell_class()->has_preset();
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
// @note プリセット端子がない場合の返り値は不定
TvFunc
CiGroup::preset_function() const
{
#warning "TODO: not implemented"
}

// @brief セル数を返す．
ymuint
CiGroup::cell_num() const
{
  return mCellNum;
}

// @brief セルを返す．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
const Cell*
CiGroup::cell(ymuint pos) const
{
  assert_cond( pos < mCellNum, __FILE__, __LINE__);
  return mCellList[pos];
}

END_NAMESPACE_YM_CELL
