#ifndef LIBYM_CELL_MISLIB_MISLIBCELL_H
#define LIBYM_CELL_MISLIB_MISLIBCELL_H

/// @file libym_cell/mislib/MislibCell.h
/// @brief MislibCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/Cell.h"
#include "ym_utils/ShString.h"
#include "MislibPin.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class MislibCell MislibCell.h "MislibCell.h"
/// @brief mislib(genlib) 用の Cell の実装クラス
/// @note 出力ピンのピン番号は 0 ，入力ピンは 1 〜 pin_num() - 1 とする．
//////////////////////////////////////////////////////////////////////
class MislibCell :
  public Cell
{
  // このクラスのみが内容を設定できる．
  friend class MislibLibrary;

private:

  /// @brief コンストラクタ
  MislibCell();

  /// @brief デストラクタ
  virtual
  ~MislibCell();


public:
  //////////////////////////////////////////////////////////////////////
  // Cell の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const;

  /// @brief 面積の取得
  virtual
  CellArea
  area() const;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  virtual
  const CellPin*
  pin(ymuint pos) const;

  /// @brief 名前からピンの取得
  /// @param[in] name ピン名
  /// @return name という名前をピンを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellPin*
  pin(const string& name) const;

  /// @brief バス数の取得
  virtual
  ymuint
  bus_num() const;

  /// @brief バスの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
  virtual
  const CellBus*
  bus(ymuint pos) const;

  /// @brief 名前からバスの取得
  /// @param[in] name バス名
  /// @return name という名前のバスを返す．
  /// @note なければ NULL を返す．
  virtual
  const CellBus*
  bus(const string& name) const;

  /// @brief バンドル数の取得
  virtual
  ymuint
  bundle_num() const;

  /// @brief バンドルの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
  virtual
  const CellBundle*
  bundle(ymuint pos) const;

  /// @brief 名前からバンドルの取得
  virtual
  const CellBundle*
  bundle(const string& name) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 面積
  CellArea mArea;

  // 入力ピン数
  ymuint32 mInputPinNum;

  // 入力ピンの配列
  MislibInputPin* mInputPinArray;

  // 出力ピン
  MislibOutputPin mOutputPin;

};

END_NAMESPACE_YM_CELL

#endif // LIBYM_CELL_MISLIB_MISLIBCELL_H
