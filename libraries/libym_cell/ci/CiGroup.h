#ifndef CIGROUP_H
#define CIGROUP_H

/// @file CiGroup.h
/// @brief CiGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellGroup.h"
#include "ym_logic/NpnMapM.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiGroup CiGroup.h "CiGroup.h"
/// @brief CellGroup の実装クラス
//////////////////////////////////////////////////////////////////////
class CiGroup :
  public CellGroup
{
  friend class CiLibrary;

public:

  /// @brief コンストラクタ
  CiGroup();

  /// @brief デストラクタ
  virtual
  ~CiGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 一般的な情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  /// @note CellLibrary::group(id) で返されるオブジェクトの id() は id となる．
  virtual
  ymuint
  id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属している CellClass を返す．
  virtual
  const CellClass*
  cell_class() const;

  /// @brief 代表クラスに対する変換マップを返す．
  virtual
  const NpnMapM&
  map() const;


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチセルの場合にピンの情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセルの場合のピン情報を返す．
  /// @note FFセル以外の場合には返り値は不定
  CellFFInfo
  ff_info() const;

  /// @brief ラッチセルの場合のピン情報を返す．
  /// @note ラッチセル以外の場合には返り値は不定
  CellLatchInfo
  latch_info() const;

  /// @brief データ入力を持つとき true を返す．
  virtual
  bool
  has_data() const;

  /// @brief データ入力のピン番号を返す．
  virtual
  ymuint
  data_pos() const;

  /// @brief クロック入力のタイプを返す．
  /// @retval 0 該当しない
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  virtual
  ymuint
  clock_sense() const;

  /// @brief クロック入力のピン番号を返す．
  virtual
  ymuint
  clock_pos() const;

  /// @brief イネーブル入力を持つとき true を返す．
  virtual
  bool
  has_enable() const;

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  virtual
  ymuint
  enable_sense() const;

  /// @brief イネーブル入力のピン番号を返す．
  virtual
  ymuint
  enable_pos() const;

  /// @brief クリア入力を持つタイプの時に true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  virtual
  ymuint
  clear_sense() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  virtual
  ymuint
  clear_pos() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  virtual
  bool
  has_preset() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  virtual
  ymuint
  preset_sense() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  virtual
  ymuint
  preset_pos() const;

  /// @brief 肯定出力のピン番号を返す．
  virtual
  ymuint
  q_pos() const;

  /// @brief 否定出力のピン番号を返す．
  virtual
  ymuint
  xq_pos() const;


public:
  //////////////////////////////////////////////////////////////////////
  // このグループに属しているセルの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セル数を返す．
  virtual
  ymuint
  cell_num() const;

  /// @brief セルを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
  virtual
  const Cell*
  cell(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] cell_class 代表クラス
  /// @param[in] map 変換マップ
  /// @param[in] cell_list セルのリスト
  /// @param[in] alloc メモリアロケータ
  void
  init(const CellClass* cell_class,
       const NpnMapM& map,
       const vector<const Cell*>& cell_list,
       Alloc& alloc);

  /// @brief FFのピン情報を設定する．
  /// @param[in] pos_array ピン位置と極性情報の配列
  /// @note pos_array の意味は以下の通り
  ///  - pos_array[0] : データ入力のピン番号     (3bit)
  ///  - pos_array[1] : クロック入力のピン番号   (3bit) | 極性情報 (1bit)
  ///  - pos_array[2] : クリア入力のピン番号     (3bit) | 極性情報 (2bit)
  ///  - pos_array[3] : プリセット入力のピン番号 (3bit) | 極性情報 (2bit)
  ///  - pos_array[4] : 肯定出力のピン番号       (3bit)
  ///  - pos_array[5] : 否定出力のピン番号       (3bit)
  void
  set_ff_info(ymuint pos_array[]);

  /// @brief ラッチのピン情報を設定する．
  /// @param[in] pos_array ピン位置と極性情報の配列
  /// @note pos_array の意味は以下の通り
  ///  - pos_array[0] : データ入力のピン番号     (3bit)
  ///  - pos_array[1] : イネーブル入力のピン番号 (3bit) | 極性情報 (2bit)
  ///  - pos_array[2] : クリア入力のピン番号     (3bit) | 極性情報 (2bit)
  ///  - pos_array[3] : プリセット入力のピン番号 (3bit) | 極性情報 (2bit)
  ///  - pos_array[4] : 肯定出力のピン番号       (3bit)
   void
  set_latch_info(ymuint pos_array[]);


public:
  //////////////////////////////////////////////////////////////////////
  // バイナリダンプ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バイナリダンプを行う．
  /// @param[in] bos 出力先のストリーム
  void
  dump(BinO& bos) const;

  /// @brief バイナリファイルを読み込む．
  /// @param[in] bis 入力元のストリーム
  /// @param[in] library セルライブラリ
  /// @param[in] alloc メモリアロケータ
  void
  restore(BinI& bis,
	  const CellLibrary& library,
	  Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メモリ領域の確保を行う．
  /// @param[in] alloc メモリアロケータ
  void
  alloc_array(Alloc& alloc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 属している CellClass
  const CellClass* mCellClass;

  // CellClass に対する入出力の変換関数
  NpnMapM mMap;

  // FF/ラッチのピン情報
  ymuint32 mPinInfo;

  // セル数
  ymuint32 mCellNum;

  // セルのポインタの配列
  const Cell** mCellList;

};

END_NAMESPACE_YM_CELL

#endif // CIGROUP_H
