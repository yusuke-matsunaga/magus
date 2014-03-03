#ifndef CILUT_H
#define CILUT_H

/// @file　CiLut.h
/// @brief CiLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "cell/CellLut.h"
#include "utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLut CiLut.h "CiLut.h"
/// @brief ルックアップテーブルの実装クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class CiLut :
  public CellLut
{
  friend class CiLibrary;

protected:

  /// @brief コンストラクタ
  CiLut(const CellLutTemplate* lut_template);

  /// @brief デストラクタ
  virtual
  ~CiLut();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief テンプレート名の取得
  virtual
  const char*
  template_name() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  tCellVarType
  variable_type(ymuint32 var) const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;


public:
  //////////////////////////////////////////////////////////////////////
  // dump/restore 関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  dump(ODO& s) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief val に対応する区間を求める．
  static
  ymuint
  search(double val,
	 const vector<double>& index_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate* mTemplate;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut1D CiLut.h "CiLut.h"
/// @brief 1次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut1D :
  public CiLut
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiLut1D(const CellLutTemplate* lut_template,
	  const vector<double>& value_array,
	  const vector<double>& index_array = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CiLut1D();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 格子点の値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  grid_value(const vector<ymuint32>& pos_array) const;

  /// @brief 値の取得
  /// @param[in] val_array 入力の値の配列
  /// @note val_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<double>& val_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // インデックスの配列
  vector<double> mIndexArray;

  // インデックスの間隔の配列
  vector<double> mIndexWidthArray;

  // 格子点の値の配列
  vector<double> mValueArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut2D CiLut.h "CiLut.h"
/// @brief 2次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut2D :
  public CiLut
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiLut2D(const CellLutTemplate* lut_template,
	  const vector<double>& value_array,
	  const vector<double>& index_array1 = vector<double>(),
	  const vector<double>& index_array2 = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CiLut2D();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 格子点の値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  grid_value(const vector<ymuint32>& pos_array) const;

  /// @brief 値の取得
  /// @param[in] val_array 入力の値の配列
  /// @note val_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<double>& val_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mValueArray のインデックスを計算する．
  /// @param[in] idx1 1番めのインデックス
  /// @param[in] idx2 2番めのインデックス
  ymuint
  idx(ymuint idx1,
      ymuint idx2) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[2];

  // インデックスの間隔の配列
  vector<double> mIndexWidthArray[2];

  // 格子点の値の配列
  vector<double> mValueArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut3D CiLut.h "CiLut.h"
/// @brief 3次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut3D :
  public CiLut
{
  friend class CiLibrary;

private:

  /// @brief コンストラクタ
  CiLut3D(const CellLutTemplate* lut_template,
	  const vector<double>& value_array,
	  const vector<double>& index_array1 = vector<double>(),
	  const vector<double>& index_array2 = vector<double>(),
	  const vector<double>& index_array3 = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CiLut3D();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  virtual
  ymuint32
  index_num(ymuint32 var) const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 格子点の値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  grid_value(const vector<ymuint32>& pos_array) const;

  /// @brief 値の取得
  /// @param[in] val_array 入力の値の配列
  /// @note val_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<double>& val_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mValueArray のインデックスを計算する．
  /// @param[in] idx1 1番めのインデックス
  /// @param[in] idx2 2番めのインデックス
  /// @param[in] idx3 3番めのインデックス
  ymuint
  idx(ymuint idx1,
      ymuint idx2,
      ymuint idx3) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CellLutTemplate* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[3];

  // インデックスの間隔の配列
  vector<double> mIndexWidthArray[3];

  // 格子点の値の配列
  vector<double> mValueArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief mValueArray のインデックスを計算する．
// @param[in] idx1 1番めのインデックス
// @param[in] idx2 2番めのインデックス
inline
ymuint
CiLut2D::idx(ymuint idx1,
	     ymuint idx2) const
{
  return idx1 * index_num(1) + idx2;
}

// @brief mValueArray のインデックスを計算する．
// @param[in] idx1 1番めのインデックス
// @param[in] idx2 2番めのインデックス
// @param[in] idx3 3番めのインデックス
inline
ymuint
CiLut3D::idx(ymuint idx1,
	     ymuint idx2,
	     ymuint idx3) const
{
  return ((idx1 * index_num(1) + idx2) * index_num(2)) + idx3;
}

END_NAMESPACE_YM_CELL

#endif // CILUT_H
