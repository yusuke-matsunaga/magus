#ifndef VERILOG_VL_VLDECLARRAY_H
#define VERILOG_VL_VLDECLARRAY_H

/// @file YmVerilog/vl/VlDeclArray.h
/// @brief VlDeclArray のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/vl/VlDeclBase.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlDeclArray VlDeclArray.h "ym_YmVerilog/vl/VlDeclArray.h"
/// @brief エラボレーション中の名前付きオブジェクトを表す基底クラス
//////////////////////////////////////////////////////////////////////
class VlDeclArray :
  public VlDeclBase
{
protected:

  /// @brief デストラクタ
  virtual
  ~VlDeclArray() { }


public:
  //////////////////////////////////////////////////////////////////////
  // VlDeclArray の派生クラスに共通な仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 多次元の配列型オブジェクトの時に true を返す．
  virtual
  bool
  is_multi_array() const = 0;

  /// @brief 次元数の取得
  virtual
  ymuint
  dimension() const = 0;

  /// @brief 範囲の取得
  /// @param[in] pos 位置 ( 0 <= pos < dimension() )
  virtual
  const VlRange*
  range(ymuint pos) const = 0;

  /// @brief 配列の要素数の取得
  virtual
  ymuint
  array_size() const = 0;

  /// @brief 1次元配列の場合にインデックスからオフセットを計算する．
  /// @param[in] index インデックス
  /// @param[out] offset index に対するオフセット値
  /// @retval true index が範囲内だった．
  /// @retval false index が範囲外だった．
  virtual
  bool
  calc_array_offset(int index,
		    ymuint& offset) const = 0;

  /// @brief 他次元配列の場合にインデックスのリストからオフセットを計算する．
  /// @param[in] index_list インデックスのリスト
  /// @param[out] offset index_list に対するオフセット値
  /// @retval true オフセットが正しく計算できた．
  /// @retval false index_list のいずれかの値が範囲外だった．
  virtual
  bool
  calc_array_offset(const vector<int>& index_list,
		    ymuint& offset) const = 0;

};

END_NAMESPACE_YM_VERILOG

#endif // VERILOG_VL_VLDECLARRAY_H
