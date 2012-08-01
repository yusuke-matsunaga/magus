#ifndef CILUT_H
#define CILUT_H

/// @file　CiLut.h
/// @brief CiLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellLut.h"
#include "ym_utils/ShString.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplateBase CiLut.h "CiLut.h"
/// @brief CiLutTemplate の(擬似)基底クラス
//////////////////////////////////////////////////////////////////////
class CiLutTemplateBase :
  public CellLutTemplate
{
protected:

  /// @brief コンストラクタ
  CiLutTemplateBase(ShString name);

  /// @brief デストラクタ
  virtual
  ~CiLutTemplateBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  const char*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate1D CiLut.h "CiLut.h"
/// @brief 1次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate1D :
  public CiLutTemplateBase
{
private:

  /// @brief コンストラクタ
  CiLutTemplate1D(ShString name,
		  tCellVarType var_type,
		  const vector<double>& index_array);

  /// @brief デストラクタ
  virtual
  ~CiLutTemplate1D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

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

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType;

  // デフォルトインデックス値の配列
  vector<double> mIndexArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate2D CiLut.h "CiLut.h"
/// @brief 2次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate2D :
  public CiLutTemplateBase
{
  friend class CellManip;

private:

  /// @brief コンストラクタ
  CiLutTemplate2D(ShString name,
		  tCellVarType var1,
		  const vector<double>& index_array1,
		  tCellVarType var2,
		  const vector<double>& index_array2);

  /// @brief デストラクタ
  virtual
  ~CiLutTemplate2D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

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

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType[2];

  // デフォルトインデックス値の配列
  vector<double> mIndexArray[2];

};


//////////////////////////////////////////////////////////////////////
/// @class CiLutTemplate3D CiLut.h "CiLut.h"
/// @brief 3次元のルックアップテーブルテンプレート
//////////////////////////////////////////////////////////////////////
class CiLutTemplate3D :
  public CiLutTemplateBase
{
  friend class CellManip;

private:

  /// @brief コンストラクタ
  CiLutTemplate3D(ShString name,
		  tCellVarType var1,
		  const vector<double>& index_array1,
		  tCellVarType var2,
		  const vector<double>& index_array2,
		  tCellVarType var3,
		  const vector<double>& index_array3);

  /// @brief デストラクタ
  virtual
  ~CiLutTemplate3D();


public:

  /// @brief 次元数の取得
  virtual
  ymuint32
  dimension() const;

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

  /// @brief デフォルトインデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数の型
  tCellVarType mVarType[3];

  // デフォルトインデックス値の配列
  vector<double> mIndexArray[3];

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut1D CiLut.h "CiLut.h"
/// @brief 1次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut1D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CiLut1D(const CiLutTemplate1D* lut_template,
	  const vector<double>& index_array = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CiLut1D();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<ymuint32>& pos_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CiLutTemplate1D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray;

  // 格子点の値の配列
  vector<double> mValueArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut2D CiLut.h "CiLut.h"
/// @brief 2次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut2D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CiLut2D(const CiLutTemplate2D* lut_template,
	    const vector<double>& index_array1 = vector<double>(),
	    const vector<double>& index_array2 = vector<double>());

  /// @brief デストラクタ
  virtual
  ~CiLut2D();


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<ymuint32>& pos_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CiLutTemplate2D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[2];

  // 格子点の値の配列
  vector<double> mValueArray;

};


//////////////////////////////////////////////////////////////////////
/// @class CiLut3D CiLut.h "CiLut.h"
/// @brief 3次元のルックアップテーブルを表すクラス
//////////////////////////////////////////////////////////////////////
class CiLut3D :
  public CellLut
{
public:

  /// @brief コンストラクタ
  CiLut3D(const CiLutTemplate3D* lut_template,
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

  /// @brief テンプレートの取得
  virtual
  const CellLutTemplate*
  lut_template() const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  virtual
  double
  index(ymuint32 var,
	ymuint32 pos) const;

  /// @brief 値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  virtual
  double
  value(const vector<ymuint32>& pos_array) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート
  const CiLutTemplate3D* mTemplate;

  // インデックスの配列の配列
  vector<double> mIndexArray[3];

  // 格子点の値の配列
  vector<double> mValueArray;

};

END_NAMESPACE_YM_CELL

#endif // CELLLUTIMPL_H
