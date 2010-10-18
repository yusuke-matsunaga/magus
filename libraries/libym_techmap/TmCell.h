#ifndef LIBYM_TECHMAP_TMCELL_H
#define LIBYM_TECHMAP_TMCELL_H

/// @file libym_techmap/TmCell.h
/// @brief TmCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/techmap_nsdef.h"
#include "ym_utils/ShString.h"
#include "ym_npn/NpnMap.h"
#include "PatGraph.h"


BEGIN_NAMESPACE_YM_TECHMAP

class TmCellMgr;
class TmCellRep;
class TmCellGroup;

//////////////////////////////////////////////////////////////////////
/// @class TmCellInput TmCell.h "TmCell.h"
/// @brief セルの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class TmCellInput
{
  friend TmCellMgr;

public:

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 負荷容量を返す．
  double
  capacitance() const;

  /// @brief 立ち上がり時の負荷容量を返す．
  double
  rise_capacitance() const;

  /// @brief 立ち下がり時の負荷容量を返す．
  double
  fall_capacitance() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 負荷容量
  double mCapacitance;

  // 立ち上がり時の負荷容量
  double mRiseCapacitance;

  // 立ち下がり時の負荷容量
  double mFallCapacitance;

};


//////////////////////////////////////////////////////////////////////
/// @class TmCellOutput TmCell.h "TmCell.h"
/// @brief セルの出力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class TmCellOutput
{
  friend TmCellMgr;

public:

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 最大ファンアウト容量を返す．
  double
  max_fanout() const;

  /// @brief 最小ファンアウト容量を返す．
  double
  min_fanout() const;

  /// @brief 最大負荷容量を返す．
  double
  max_capacitance() const;

  /// @brief 最小負荷容量を返す．
  double
  min_capacitance() const;

  /// @brief 最大遷移時間を返す．
  double
  max_transition() const;

  /// @brief 最小遷移時間を返す．
  double
  min_transition() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 最大ファンアウト容量
  double mMaxFanout;

  // 最小ファンアウト容量
  double mMinFanout;

  // 最大負荷容量
  double mMaxCapacitance;

  // 最小負荷容量
  double mMinCapacitance;

  // 最大遷移時間
  double mMaxTransition;

  // 最小遷移時間
  double mMinTransition;

};


//////////////////////////////////////////////////////////////////////
/// @class TmCell TmCell.h "TmCell.h"
/// @brief テクノロジマッピング用のセルを表すクラス
/// @note 1出力の論理ゲートのみを対象とする．
//////////////////////////////////////////////////////////////////////
class TmCell
{
  friend TmCellMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] input_num 入力ピン数
  TmCell(ymuint input_num);

  /// @brief デストラクタ
  ~TmCell();


public:

  /// @brief セル名を返す．
  ShString
  name() const;

  /// @brief 面積を返す．
  double
  area() const;

  /// @brief セルグループを返す．
  const TmCellGroup*
  cell_group() const;

  /// @brief 入力ピン数を返す．
  ymuint
  input_num() const;

  /// @brief 入力ピンを返す．
  /// @param[in] pos 入力ピン番号 ( 0 <= pos < input_num() )
  const TmCellInput&
  input(ymuint pos) const;

  /// @brief 出力ピンを返す．
  const TmCellOutput&
  output() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル名
  ShString mName;

  // 面積
  double mArea;

  // セルグループ
  const TmCellGroup* mCellGreoup;

  // 入力ピン数
  ymuint32 mInputNum;

  // 入力ピンの配列
  TmCellInput* mInputArray;

  // 出力ピン
  TmCellOutput;

};


//////////////////////////////////////////////////////////////////////
/// @class TmCellGroup TmCell.h "TmCell.h"
/// @brief 論理的に等価なセルのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class TmCellGroup
{
  friend TmCellMgr;

public:

  /// @brief 代表グループを返す．
  const TmCellRep:
  rep() const;

  /// @brief NPN同値類の代表グループに対する変換を返す．
  const NpnMap&
  npn_map() const;

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 要素数を返す．
  ymuint
  cell_num() const;

  /// @brief 要素のセルを返す．
  /// @param[in] pos 位置 ( 0 <= pos < cell_num() )
  const TmCell*
  cell(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表グループ
  const TmCellRep* mRep;

  // NPN同値類の代表に対する変換
  NpnMap mNpnMap;

  // ID番号
  ymuint32 mId;

  // 要素のセルのリスト
  vector<TmCell*> mCellList;

};


//////////////////////////////////////////////////////////////////////
/// @class TmCellRep TmCell.h "TmCell.h"
/// @brief セルグループの代表を表すクラス
//////////////////////////////////////////////////////////////////////
class TmCellRep
{
  friend TmCellMgr;

public:

  /// @brief パタングラフリストの要素数を返す．
  ymuint
  patgraph_num() const;

  /// @brief パタングラフを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < patgraph_num() )
  const PatGraph*
  patgraph(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パタングラフのリスト
  vector<PatGraph*> mPatGraphList;

};


//////////////////////////////////////////////////////////////////////
/// @class TmCellMgr TmCell.h "TmCell.h"
/// @brief セルライブラリを管理するクラス
//////////////////////////////////////////////////////////////////////
class TmCellMgr
{
public:

  /// @brief コンストラクタ
  TmCellMgr();

  /// @brief デストラクタ
  ~TmCellMgr();


public:

  /// @brief データを読み込む．
  /// @brief s 入力ストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load(istream& s);

  /// @brief 代表グループ数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表グループを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < rep_num() )
  const TmCellRep*
  rep(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 代表グループのリスト
  vector<TmCellRep*> mRepList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 名前を返す．
inline
ShString
TmCellPin::name() const
{
  return mName;
}

// @brief 負荷容量を返す．
inline
double
TmCellInput::capacitance() const
{
  return mCapacitance;
}

// @brief 立ち上がり時の負荷容量を返す．
inline
double
TmCellInput::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief 立ち下がり時の負荷容量を返す．
inline
double
TmCellInput::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief 名前を返す．
inline
ShString
TmCellOutput::name() const
{
  return mName;
}

// @brief 最大ファンアウト容量を返す．
inline
double
TmCellOutput::max_fanout() const
{
  return mMaxFanout;
}

// @brief 最小ファンアウト容量を返す．
inline
double
TmCellOutput::min_fanout() const
{
  return mMinFanout;
}

// @brief 最大負荷容量を返す．
inline
double
TmCellOutput::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief 最小負荷容量を返す．
inline
double
TmCellOutput::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief 最大遷移時間を返す．
inline
double
TmCellOutput::max_transition() const
{
  return mMaxTransition;
}

// @brief 最小遷移時間を返す．
inline
double
TmCellOutput::min_transition() const
{
  return mMinTransition;
}

// @brief セル名を返す．
inline
ShString
TmCell::name() const
{
  return mName;
}

// @brief 面積を返す．
inline
double
TmCell::area() const
{
  return mArea;
}

// @brief セルグループを返す．
inline
const TmCellGroup*
TmCell::cell_group() const
{
  return mCellGroup;
}

// @brief 入力ピン数を返す．
inline
ymuint
TmCell::input_num() const
{
  return mInputNum;
}

// @brief 入力ピンを返す．
// @param[in] pos 入力ピン番号 ( 0 <= pos < input_num() )
inline
const TmCellInput&
TmCell::input(ymuint pos) const
{
  return mInputArray[pos];
}

// @brief 出力ピンを返す．
inline
const TmCellOutput&
TmCell::output() const
{
  return mOutput;
}

// @brief 代表グループを返す．
inline
const TmCellRep*
TmCellGroup::rep() const
{
  return mRep;
}

// @brief NPN同値類の代表グループに対する変換を返す．
inline
const NpnMap&
TmCellGroup::npn_map() const
{
  return mNpnMap;
}

// @brief 要素数を返す．
inline
ymuint
TmCellGroup::cell_num() const
{
  return mCellList.size();
}

// @brief 要素のセルを返す．
// @param[in] pos 位置 ( 0 <= pos < cell_num() )
inline
const TmCell*
TmCellGroup::cell(ymuint pos) const
{
  return mCellList[pos];
}

// @brief パタングラフリストの要素数を返す．
inline
ymuint
TmCellRep::patgraph_num() const
{
  return mPatGraphList.size();
}

// @brief パタングラフを返す．
// @param[in] pos 位置番号 ( 0 <= pos < patgraph_num() )
inline
const PatGraph*
TmCellRep::patgraph(ymuint pos) const
{
  return mPatGraphList[pos];
}

// @brief 代表グループ数を返す．
inline
ymuint
TmCellMgr::rep_num() const
{
  return mRepList.size();
}

// @brief 代表グループを返す．
// @param[in] pos 位置番号 ( 0 <= pos < rep_num() )
inline
const TmCellRep*
TmCellMgr::rep(ymuint pos) const
{
  return mRepList[pos];
}

END_NAMESPACE_YM_TECHMAP

#endif // LIBYM_TECHMAP_TMCELL_H
