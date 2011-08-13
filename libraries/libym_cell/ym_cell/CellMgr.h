#ifndef YM_CELL_CELLMGR_H
#define YM_CELL_CELLMGR_H

/// @file ym_cell/CellMgr.h
/// @brief CellMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_cell/CellPatMgr.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellMgr CellMgr.h "ym_cell/CellMgr.h"
/// @brief セルを管理するクラス
///
/// このクラスが持っている情報は
/// - 各々のセルに対応する論理関数
/// - その論理関数とNPN同値な関数の集合
/// - FF セルの集合
/// - ラッチセルの集合
///
/// 情報の設定は専用形式のバイナリファイルを読み込むことでのみ行える．
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
  // ライブラリの読み込み/取得
  //////////////////////////////////////////////////////////////////////

  /// @brief ライブラリの内容をバイナリファイルに書き出す．
  /// @param[in] s 出力先のストリーム
  /// @param[in] library ダンプ対象のライブラリ
  static
  void
  dump_library(ostream& s,
	       const CellLibrary& library);

  /// @brief データを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load_library(istream& s);

  /// @brief セルライブラリを返す．
  const CellLibrary&
  library() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 論理関数グループに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理関数の個数を返す．
  ymuint
  func_num() const;

  /// @brief 関数グループを返す．
  /// @param[in] id 関数番号　( 0 <= id < func_num() )
  const CellFuncGroup&
  func_group(ymuint id) const;

  /// @brief 定数0の関数グループを返す．
  const CellFuncGroup&
  const0_func() const;

  /// @brief 定数1の関数グループを返す．
  const CellFuncGroup&
  const1_func() const;

  /// @brief バッファセルの関数グループを返す．
  const CellFuncGroup&
  buf_func() const;

  /// @brief インバータセルの関数グループを返す．
  const CellFuncGroup&
  inv_func() const;

  /// @brief 代表関数の個数を返す．
  ymuint
  rep_num() const;

  /// @brief 代表関数を返す．
  /// @param[in] id 代表関数番号
  const CellFuncClass&
  rep(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FFクラス数を返す．
  ymuint
  ff_class_num() const;

  /// @brief FFクラスを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < ff_class_num() )
  const CellFFClass&
  ff_class(ymuint pos) const;

  /// @brief ラッチクラス数を返す．
  ymuint
  latch_class_num() const;

  /// @brief ラッチクラスを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < latch_class_num() )
  const CellLatchClass&
  latch_class(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタンに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンを管理するオブジェクトを得る．
  const CellPatMgr&
  pat_mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を初期化する．
  /// @note 以前確保されたメモリは開放される．
  void
  init();

  /// @brief CellGroup の内容をロードする．
  /// @param[in] s 入力ストリーム
  /// @param[in] cell_group ロード対象の CellGroup
  void
  load_cellgroup(istream& s,
		 CellGroup& cell_group);

  /// @brief RepFunc の内容をロードする．
  /// @param[in] s 入力ストリーム
  /// @param[in] id 代表番号
  void
  load_repfunc(istream& s,
	       ymuint id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // セルライブラリ
  const CellLibrary* mLibrary;

  // 関数の数
  ymuint32 mFuncNum;

  // 関数グループの配列
  // サイズは mFuncNum
  CellFuncGroup* mFuncArray;

  // 代表関数の数
  ymuint32 mRepNum;

  // 代表関数の配列
  // サイズは mRepNum
  CellFuncClass* mRepArray;

  // FFグループ数
  ymuint32 mFFGroupNum;

  // FFグループの(実体の)配列
  CellFFGroup* mFFGroupArray;

  // FFクラスの数
  ymuint32 mFFClassNum;

  // FFクラスの配列
  CellFFClass* mFFClassArray;

  // ラッチクラスの数
  ymuint32 mLatchClassNum;

  // ラッチクラスの(実体の)配列
  CellLatchClass* mLatchArray;

  // パタンを管理するオブジェクト
  CellPatMgr mPatMgr;

};


/// @relates CellMgr
/// @brief CellMgr の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] cell_mgr セルライブラリの情報を持つオブジェクト
void
dump(ostream& s,
     const CellMgr& cell_mgr);

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLMGR_H
