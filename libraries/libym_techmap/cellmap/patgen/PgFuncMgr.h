#ifndef LIBYM_TECHMAP_PATGEN_PGFUNCMGR_H
#define LIBYM_TECHMAP_PATGEN_PGFUNCMGR_H

/// @file libym_techmap/patgen/PgFuncMgr.h
/// @brief PgFuncMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_npn/TvFunc.h"


BEGIN_NAMESPACE_YM_CELLMAP_PATGEN

class PgFunc;
class PgFuncRep;

//////////////////////////////////////////////////////////////////////
/// @class PgFuncMgr PgFuncMgr.h "PgFuncMgr.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class PgFuncMgr
{
public:

  /// @brief コンストラクタ
  PgFuncMgr();

  /// @brief デストラクタ
  ~PgFuncMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 定数関数，リテラル関数を登録しておく
  ///
  /// 常に定数0，定数1，肯定リテラル，否定リテラルの関数番号が
  /// 0, 1, 2, 3 になるようにする．
  void
  init();

  /// @brief 関数と対応するセル番号を登録する．
  /// @param[in] tvfunc 論理関数関数
  /// @param[in] cell_id セル番号
  /// @return 関数情報のオブジェクトを返す．
  PgFunc*
  reg_func(const TvFunc& tvfunc,
	   ymuint cell_id);

  /// @brief f に対応する PgFunc を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  PgFunc*
  find_func(const TvFunc& f);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 論理関数情報のオブジェクトを返す．
  /// @param[in] id 関数番号 ( 0 <= id < func_num() )
  const PgFunc*
  func(ymuint id) const;

  /// @brief 代表関数の数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表関数を返す．
  /// @param[in] id 代表関数番号 ( 0 <= id < rep_num() )
  const PgFuncRep*
  rep(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief f に対応する PgFuncRep を求める．
  /// @param[in] f 関数
  /// @note なければ新規に作る．
  /// @note f は NpnMgr によって正規化されている必要がある．
  PgFuncRep*
  find_repfunc(const TvFunc& f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 論理関数のリスト
  // この配列上の位置と関数番号は一致している．
  vector<PgFunc*> mFuncList;

  // 論理関数のハッシュ表
  hash_map<TvFunc, PgFunc*> mFuncMap;

  // 代表関数のリスト
  // この配列上の位置と代表関数番号は一致している．
  vector<PgFuncRep*> mRepList;

  // 代表関数のハッシュ表
  hash_map<TvFunc, PgFuncRep*> mRepMap;

};


//////////////////////////////////////////////////////////////////////
// 関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates PgFuncMgr
/// @brief グラフ構造全体の内容を表示する．
/// @param[in] s 出力先のストリーム
/// @param[in] pgf_mgr 対象のオブジェクト
void
pg_display(ostream& s,
	   const PgFuncMgr& pgf_mgr);

/// @relates PgFuncMgr
/// @brief グラフ構造全体をダンプする．
/// @param[in] s 出力先のストリーム
/// @param[in] pgf_mgr 対象のオブジェクト
/// @note ダンプされた情報はそのまま PatMgr で読み込むことができる．
void
pg_dump(ostream& s,
	const PgFuncMgr& pat_gen);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 論理関数の数を返す．
inline
ymuint
PgFuncMgr::func_num() const
{
  return mFuncList.size();
}

// @brief 論理関数を返す．
// @param[in] id 関数番号 ( 0 <= id < func_num() )
inline
const PgFunc*
PgFuncMgr::func(ymuint id) const
{
  return mFuncList[id];
}

// @brief 代表関数の数を返す．
inline
ymuint
PgFuncMgr::rep_num() const
{
  return mRepList.size();
}

// @brief 代表関数を返す．
// @param[in] id 代表関数番号 ( 0 <= id < rep_num() )
inline
const PgFuncRep*
PgFuncMgr::rep(ymuint id) const
{
  return mRepList[id];
}

END_NAMESPACE_YM_CELLMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGFUNCMGR_H
