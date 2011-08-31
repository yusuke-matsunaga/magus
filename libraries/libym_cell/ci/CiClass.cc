
/// @file CiClass.cc
/// @brief CiClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiClass::CiClass() :
  mGroupNum(0),
  mGroupList(NULL)
{
}

// @brief デストラクタ
CiClass::~CiClass()
{
  // mGroupList は CellMgr が管理している．
}

// @brief 入力ピン数の取得
ymuint
CiClass::input_num() const
{
  return mInputNum;
}

// @brief 出力ピン数の取得
ymuint
CiClass::output_num() const
{
  return mOutputNum;
}

// @brief 入出力ピン数の取得
ymuint
CiClass::inout_num() const
{
  return mInoutNum;
}

// @brief 論理セルの場合に true を返す．
bool
CiClass::is_logic() const
{
  return false;
}

// @brief トライステートセルの場合に true を返す．
// @note もちろん論理セルでもある．
// @note 複数出力のうち1つでもトライステートなら true を返す．
bool
CiClass::is_tristate() const
{
  return false;
}

// @brief FFセルの場合に true を返す．
bool
CiClass::is_ff() const
{
  return false;
}

// @brief ラッチセルの場合に true を返す．
bool
CiClass::is_latch() const
{
  return false;
}

// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
bool
CiClass::is_seq() const
{
  return false;
}

// @brief 論理セルの場合に関数を返す．
// @param[in] pos 出力番号
// @note 非論理セルの場合の返り値は不定
TvFunc
CiClass::logic_function(ymuint pos) const
{
  return TvFunc(0);
}

// @brief トライステートセルの場合にトライステート条件関数を返す．
// @param[in] pos 出力番号
// @note 通常の論理セルの場合には定数0関数を返す．
TvFunc
CiClass::tristate_function(ymuint pos) const
{
  return TvFunc(0);
}

// @brief FFセルの場合に次状態関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiClass::next_state_function() const
{
}

// @brief FFセルの場合にクロックのアクティブエッジを表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiClass::clock_function() const
{
}

// @brief ラッチセルの場合にデータ入力関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiClass::data_in_function() const
{
}

// @brief ラッチセルの場合にイネーブル条件を表す関数を返す．
// @note それ以外の型の場合の返り値は不定
TvFunc
CiClass::enable_function() const
{
}

// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
bool
CiClass::has_clear() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にクリア条件を表す関数を返す．
// @note クリア端子がない場合の返り値は不定
TvFunc
CiClass::clear_function() const
{
}

// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
bool
CiClass::has_preset() const
{
  return false;
}

// @brief FFセル/ラッチセルの場合にプリセット条件を表す関数を返す．
// @note プリセット端子がない場合の返り値は不定
TvFunc
CiClass::preset_function() const
{
}

END_NAMESPACE_YM_CELL
