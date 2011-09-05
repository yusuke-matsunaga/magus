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

class CellGroupBase;
class CellFuncClass;
class CellFuncGroup;

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

  /// @brief 論理グループの個数を返す．
  ymuint
  logic_group_num() const;

  /// @brief 論理グループを返す．
  /// @param[in] id 論理グループ番号　( 0 <= id < logic_group_num() )
  const CellGroup&
  logic_group(ymuint id) const;

  /// @brief 定数0の論理グループを返す．
  const CellGroup&
  const0_func() const;

  /// @brief 定数1の論理グループを返す．
  const CellGroup&
  const1_func() const;

  /// @brief バッファセルの論理グループを返す．
  const CellGroup&
  buf_func() const;

  /// @brief インバータセルの論理グループを返す．
  const CellGroup&
  inv_func() const;

  /// @brief 論理クラスの個数を返す．
  ymuint
  logic_class_num() const;

  /// @brief 論理クラスを返す．
  /// @param[in] id 論理クラス番号 ( 0 <= id < logic_class_num() )
  const CellClass&
  logic_class(ymuint id) const;


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチに関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FFクラス数を返す．
  ymuint
  ff_class_num() const;

  /// @brief FFクラスを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < ff_class_num() )
  const CellClass&
  ff_class(ymuint pos) const;

  /// @brief ラッチクラス数を返す．
  ymuint
  latch_class_num() const;

  /// @brief ラッチクラスを得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < latch_class_num() )
  const CellClass&
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
		 CellGroupBase& cell_group);

  /// @brief CellClass の内容をロードする．
  /// @param[in] s 入力ストリーム
  /// @param[in] funcclass 論理セルクラス
  void
  load_funcclass(istream& s,
		 CellFuncClass& funcclass);


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
  CellGroup* mFuncArray;

  // 代表関数の数
  ymuint32 mRepNum;

  // 代表関数の配列
  // サイズは mRepNum
  CellClass* mRepArray;

  // FFグループ数
  ymuint32 mFFGroupNum;

  // FFグループの(実体の)配列
  CellGroup* mFFGroupArray;

  // FFクラスの数
  ymuint32 mFFClassNum;

  // FFクラスの配列
  CellClass* mFFClassArray;

  // ラッチクラスの数
  ymuint32 mLatchClassNum;

  // ラッチクラスの(実体の)配列
  CellClass* mLatchArray;

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
