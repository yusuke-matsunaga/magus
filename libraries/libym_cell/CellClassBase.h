#ifndef CELLCLASSBASE_H
#define CELLCLASSBASE_H

/// @file CellClassBase.h
/// @brief CellClassBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellClassBase CellClassBase.h "CellClassBase.h"
/// @brief CellClass の実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class CellClassBase :
  public CellClass
{
  friend class CellMgr;

protected:

  /// @brief コンストラクタ
  CellClassBase();

  /// @brief デストラクタ
  virtual
  ~CellClassBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 機能情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 論理セルの場合に true を返す．
  virtual
  bool
  is_logic() const;

  /// @brief トライステートセルの場合に true を返す．
  /// @note もちろん論理セルでもある．
  /// @note 複数出力のうち1つでもトライステートなら true を返す．
  virtual
  bool
  is_tristate() const;

  /// @brief FFセルの場合に true を返す．
  virtual
  bool
  is_ff() const;

  /// @brief ラッチセルの場合に true を返す．
  virtual
  bool
  is_latch() const;

  /// @brief 順序セル(非FF/非ラッチ)の場合に true を返す．
  virtual
  bool
  is_seq() const;

  /// @brief 論理セルの場合に出力ピン数を返す．
  virtual
  ymuint
  output_num() const;

  /// @brief 論理セルの場合に関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 非論理セルの場合の返り値は不定
  virtual
  const TvFunc&
  logic_function(ymuint pos) const;

  /// @brief トライステートセルの場合にトライステート条件関数を返す．
  /// @param[in] pos 出力番号 ( 0 <= pos < output_num() )
  /// @note 通常の論理セルの場合には定数0関数を返す．
  virtual
  const TvFunc&
  tristate_function(ymuint pos) const;

  /// @brief FFセル/ラッチセルの場合にクリア端子を持っていたら true を返す．
  virtual
  bool
  has_clear() const;

  /// @brief FFセル/ラッチセルの場合にプリセット端子を持っていたら true を返す．
  virtual
  bool
  has_preset() const;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルグループの数を返す．
  virtual
  ymuint
  group_num() const;

  /// @brief セルグループを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  virtual
  const CellGroup*
  cell_group(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グループの数
  ymuint32 mGroupNum;

  // グループの配列
  const CellGroup** mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief NPN同値類の数を返す．
inline
ymuint
CellClassBase::group_num() const
{
  return mGroupNum;
}

// @brief CellGroup を返す．
// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
inline
const CellGroup*
CellClassBase::cell_group(ymuint pos) const
{
  return mGroupList[pos];
}

END_NAMESPACE_YM_CELL

#endif // CELLCLASSBASE_H
