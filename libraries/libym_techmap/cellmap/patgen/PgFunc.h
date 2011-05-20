#ifndef LIBYM_TECHMAP_PATGEN_PGFUNC_H
#define LIBYM_TECHMAP_PATGEN_PGFUNC_H

/// @file libym_techmap/patgen/PgFunc.h
/// @brief PgFunc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_npn/npn_nsdef.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

class PgFuncRep;

//////////////////////////////////////////////////////////////////////
/// @class PgFunc PgFunc.h "PgFunc.h"
/// @brief 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class PgFunc
{
  friend class PgFuncMgr;
  friend class PgDumper;
private:

  /// @brief コンストラクタ
  PgFunc();

  /// @brief デストラクタ
  ~PgFunc();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 属しているセルの番号のリストを返す．
  const vector<ymuint>&
  cell_list() const;

  /// @brief 親の代表関数を返す．
  const PgFuncRep*
  rep() const;

  /// @brief 親に対する変換マップを返す．
  const NpnMap&
  map() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // セル番号のリスト
  vector<ymuint> mCellList;

  // 親の代表関数
  PgFuncRep* mRep;

  // 代表関数から自分自身へ変換するマップ
  NpnMap mMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
PgFunc::PgFunc()
{
}

// @brief デストラクタ
inline
PgFunc::~PgFunc()
{
}

// @brief ID番号を返す．
inline
ymuint
PgFunc::id() const
{
  return mId;
}

// @brief 属しているセルの番号のリストを返す．
inline
const vector<ymuint>&
PgFunc::cell_list() const
{
  return mCellList;
}

// @brief 親の代表関数を返す．
inline
const PgFuncRep*
PgFunc::rep() const
{
  return mRep;
}

// @brief 親に対する変換マップを返す．
inline
const NpnMap&
PgFunc::map() const
{
  return mMap;
}

END_NAMESPACE_YM_CELLMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGFUNC_H
