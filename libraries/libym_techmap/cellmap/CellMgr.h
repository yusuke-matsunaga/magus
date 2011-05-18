#ifndef LIBYM_TECHMAP_CELLMAP_CELLMGR_H
#define LIBYM_TECHMAP_CELLMAP_CELLMGR_H

/// @file libym_techmap/cellmap/CellMgr.h
/// @brief CellMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"
#include "ym_networks/bdn_nsdef.h"
#include "ym_cell/cell_nsdef.h"
#include "PatMgr.h"


BEGIN_NAMESPACE_YM_CELLMAP

class FuncGroup;
class RepFunc;
class FFGroup;
class LatchGroup;

//////////////////////////////////////////////////////////////////////
/// @class CellMgr CellMgr.h "CellMgr.h"
/// @brief セルを管理するクラス
///
/// このクラスが持っている情報は
/// - 各々のセルに対応する論理関数
/// - その論理関数とNPN同値な関数の集合
/// - FF セルの集合
/// - ラッチセルの集合
///
/// 情報の設定は専用形式のバイナリファイルを読み込むことでのみ行える．
/// バイナリファイルの生成は patgen/PatGen, pg_dump を参照のこと．
//////////////////////////////////////////////////////////////////////
class CellMgr
{
public:

  /// @brief コンストラクタ
  CellMgr();

  /// @brief デストラクタ
  ~CellMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // CellMap 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load_library(istream& s);


public:
  //////////////////////////////////////////////////////////////////////
  // 論理関数グループに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを返す．
  const CellLibrary&
  library() const;

  /// @brief 論理関数の個数を返す．
  ymuint
  func_num() const;

  /// @brief 関数グループを返す．
  /// @param[in] id 関数番号　( 0 <= id < func_num() )
  const FuncGroup&
  func_group(ymuint id) const;

  /// @brief 定数0の関数グループを返す．
  const FuncGroup&
  const0_func() const;

  /// @brief 定数1の関数グループを返す．
  const FuncGroup&
  const1_func() const;

  /// @brief バッファセルの関数グループを返す．
  const FuncGroup&
  buf_func() const;

  /// @brief インバータセルの関数グループを返す．
  const FuncGroup&
  inv_func() const;

  /// @brief 代表関数の個数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表関数を返す．
  /// @param[in] id 代表関数番号
  const RepFunc&
  rep(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタンに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンを管理するオブジェクトを得る．
  const PatMgr&
  pat_mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @note 以前確保されたメモリは開放される．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリ
  const CellLibrary* mLibrary;

  // 関数の数
  ymuint32 mFuncNum;

  // 関数グループの配列
  // サイズは mFuncNum
  FuncGroup* mFuncArray;

  // 代表関数の数
  ymuint32 mRepNum;

  // 代表関数の配列
  // サイズは mRepNum
  RepFunc* mRepArray;

  // パタンを管理するオブジェクト
  PatMgr mPatMgr;

  // FFグループの数
  ymuint32 mFFNum;

  // FFグループの配列
  FFGroup* mFFArray;

  // ラッチグループの数
  ymuint32 mLatchNum;

  // ラッチグループの配列
  LatchGroup* mLatchGroup;

};


/// @relates CellMgr
/// @brief CellMgr の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cell_mgr セルライブラリの情報を持つオブジェクト
void
dump(ostream& s,
     const CellMgr& cell_mgr);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @bireif 論理関数の個数を返す．
inline
ymuint
CellMgr::func_num() const
{
  return mFuncNum;
}

// @brief 定数0の関数グループを返す．
inline
const FuncGroup&
CellMgr::const0_func() const
{
  // 決め打ち
  return func_group(0);
}

// @brief 定数1の関数グループを返す．
inline
const FuncGroup&
CellMgr::const1_func() const
{
  // 決め打ち
  return func_group(1);
}

// @brief バッファセルの関数グループを返す．
inline
const FuncGroup&
CellMgr::buf_func() const
{
  // 決め打ち
  return func_group(2);
}

// @brief インバータセルの関数グループを返す．
inline
const FuncGroup&
CellMgr::inv_func() const
{
  // 決め打ち
  return func_group(3);
}

// @brief 代表関数の個数を返す．
inline
ymuint
CellMgr::rep_num() const
{
  return mRepNum;
}

// @brief パタンを管理するオブジェクトを得る．
inline
const PatMgr&
CellMgr::pat_mgr() const
{
  return mPatMgr;
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_CELLMGR_H
