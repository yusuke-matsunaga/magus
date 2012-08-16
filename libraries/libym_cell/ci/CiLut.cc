
/// @file CiLut.cc
/// @brief CiLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CiLut.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLutTemplate::CiLutTemplate(ShString name) :
  mName(name)
{
  mLink = NULL;
}

// @brief デストラクタ
CiLutTemplate::~CiLutTemplate()
{
}

// @brief 名前の取得
const char*
CiLutTemplate::name() const
{
  return mName;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate1D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLutTemplate1D::CiLutTemplate1D(ShString name,
				 tCellVarType var_type,
				 const vector<double>& index_array) :
  CiLutTemplate(name),
  mVarType(var_type),
  mIndexArray(index_array.size())
{
  ymuint32 n = index_array.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    mIndexArray[i] = index_array[i];
  }
}

// @brief デストラクタ
CiLutTemplate1D::~CiLutTemplate1D()
{
}

// @brief 次元数の取得
ymuint32
CiLutTemplate1D::dimension() const
{
  return 1;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CiLutTemplate1D::variable_type(ymuint32 var) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  return mVarType;
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate1D::index_num(ymuint32 var) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  return mIndexArray.size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate1D::index(ymuint32 var,
		       ymuint32 pos) const
{
  assert_cond( var == 0, __FILE__, __LINE__);
  assert_cond( pos < index_num(0), __FILE__, __LINE__);
  return mIndexArray[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate2D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLutTemplate2D::CiLutTemplate2D(ShString name,
				 tCellVarType var1,
				 const vector<double>& index_array1,
				 tCellVarType var2,
				 const vector<double>& index_array2) :
  CiLutTemplate(name)
{
  mVarType[0] = var1;
  ymuint32 n1 = index_array1.size();
  mIndexArray[0].resize(n1);
  for (ymuint32 i = 0; i < n1; ++ i) {
    mIndexArray[0][i] = index_array1[i];
  }

  mVarType[1] = var2;
  ymuint32 n2 = index_array2.size();
  mIndexArray[1].resize(n2);
  for (ymuint32 i = 0; i < n2; ++ i) {
    mIndexArray[1][i] = index_array2[i];
  }
}

// @brief デストラクタ
CiLutTemplate2D::~CiLutTemplate2D()
{
}

// @brief 次元数の取得
ymuint32
CiLutTemplate2D::dimension() const
{
  return 2;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CiLutTemplate2D::variable_type(ymuint32 var) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  return mVarType[var];
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate2D::index_num(ymuint32 var) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate2D::index(ymuint32 var,
			 ymuint32 pos) const
{
  assert_cond( var < 2 , __FILE__, __LINE__);
  assert_cond( pos < index_num(var) , __FILE__, __LINE__);
  return mIndexArray[var][pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplate3D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLutTemplate3D::CiLutTemplate3D(ShString name,
				 tCellVarType var1,
				 const vector<double>& index_array1,
				 tCellVarType var2,
				 const vector<double>& index_array2,
				 tCellVarType var3,
				 const vector<double>& index_array3) :
  CiLutTemplate(name)
{
  mVarType[0] = var1;
  ymuint32 n1 = index_array1.size();
  mIndexArray[0].resize(n1);
  for (ymuint32 i = 0; i < n1; ++ i) {
    mIndexArray[0][i] = index_array1[i];
  }

  mVarType[1] = var2;
  ymuint32 n2 = index_array2.size();
  mIndexArray[1].resize(n2);
  for (ymuint32 i = 0; i < n2; ++ i) {
    mIndexArray[1][i] = index_array2[i];
  }

  mVarType[2] = var3;
  ymuint32 n3 = index_array3.size();
  mIndexArray[2].resize(n3);
  for (ymuint32 i = 0; i < n3; ++ i) {
    mIndexArray[2][i] = index_array3[i];
  }
}

// @brief デストラクタ
CiLutTemplate3D::~CiLutTemplate3D()
{
}

// @brief 次元数の取得
ymuint32
CiLutTemplate3D::dimension() const
{
  return 3;
}

// @brief 変数型の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
tCellVarType
CiLutTemplate3D::variable_type(ymuint32 var) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  return mVarType[var];
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate3D::index_num(ymuint32 var) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate3D::index(ymuint32 var,
			 ymuint32 pos) const
{
  assert_cond( var < 3 , __FILE__, __LINE__);
  assert_cond( pos < index_num(var) , __FILE__, __LINE__);
  return mIndexArray[var][pos];
}


//////////////////////////////////////////////////////////////////////
// クラス CiLut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut::CiLut(const CellLutTemplate* lut_template) :
  mTemplate(lut_template)
{
}

// @brief デストラクタ
CiLut::~CiLut()
{
}

// @brief テンプレートの取得
const CellLutTemplate*
CiLut::lut_template() const
{
  return mTemplate;
}

// @brief val に対応する区間を求める．
ymuint
CiLut::search(double val,
	      const vector<double>& index_array)
{
  ymuint n = index_array.size();

  if ( val <= index_array[0] ) {
    // 値が小さすぎる時は [0, 1] を返す．
    return 0;
  }
  if ( val >= index_array[n - 1] ) {
    // 値が大きすぎる時は [n - 2, n - 1] を返す．
    return n - 2;
  }
  // 単純な線形探索を行う．
  for (ymuint i = 0; i < n - 1; ++ i) {
    if ( val < index_array[i + 1] ) {
      return i;
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLut1D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut1D::CiLut1D(const CellLutTemplate* lut_template,
		 const vector<double>& value_array,
		 const vector<double>& index_array) :
  CiLut(lut_template)
{
  ymuint n = 0;
  if ( index_array.empty() ) {
    n = index_num(0);
    mIndexArray.resize(n);
    for (ymuint32 i = 0; i < n; ++ i) {
      mIndexArray[i] = lut_template->index(0, i);
    }
  }
  else {
    n = index_array.size();
    mIndexArray.resize(n);
    for (ymuint32 i = 0; i < n; ++ i) {
      mIndexArray[i] = index_array[i];
    }
  }
  assert_cond( value_array.size() == n, __FILE__, __LINE__);
  mValueArray.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mValueArray[i] = value_array[i];
  }

  mDataArray.resize(n - 1);
  for (ymuint i = 0; i < n - 1; ++ i) {
    Data& data = mDataArray[i];
    data.mIndexBase = mIndexArray[i];
    data.mIndexWidth = mIndexArray[i + 1] - mIndexArray[i];
    data.mBase = mValueArray[i];
    data.mCoef = mValueArray[i + 1] - mValueArray[i];
  }
}

// @brief デストラクタ
CiLut1D::~CiLut1D()
{
}

// @brief 次元数の取得
ymuint32
CiLut1D::dimension() const
{
  return 1;
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLut1D::index_num(ymuint32 var) const
{
  assert_cond( var < 1 , __FILE__, __LINE__);
  return mIndexArray.size();
}

// @brief インデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLut1D::index(ymuint32 var,
	       ymuint32 pos) const
{
  assert_cond( var < 1 , __FILE__, __LINE__);
  assert_cond( pos < index_num(var) , __FILE__, __LINE__);
  return mIndexArray[pos];
}

// @brief 格子点の値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut1D::grid_value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 1, __FILE__, __LINE__);
  ymuint pos1 = pos_array[0];
  assert_cond( pos1 < index_num(0), __FILE__, __LINE__);
  return mValueArray[pos1];
}

// @brief 値の取得
// @param[in] val_array 入力の値の配列
// @note val_array のサイズは dimension() と同じ
double
CiLut1D::value(const vector<double>& val_array) const
{
  assert_cond( val_array.size() == 1, __FILE__, __LINE__);

  double val = val_array[0];

#if 0
  ymuint idx_a = search(val, mIndexArray);
  ymuint idx_b = idx_a + 1;
  double x0 = mIndexArray[idx_a];
  double x1 = mIndexArray[idx_b];

  double dx = (val - x0) / (x1 - x0);
  double val_0 = mValueArray[idx_a];
  double val_x = mValueArray[idx_b];
  double val_dx = val_x - val_0;
  return val_0 + val_dx * dx;
#else
  ymuint idx = search(val, mIndexArray);
  return mDataArray[idx].calc(val);
#endif
}

// @brief 値を計算する．
double
CiLut1D::Data::calc(double val) const
{
  double dx = (val - mIndexBase) / mIndexWidth;
  return mBase + mCoef * dx;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLut2D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut2D::CiLut2D(const CellLutTemplate* lut_template,
		 const vector<double>& value_array,
		 const vector<double>& index_array1,
		 const vector<double>& index_array2) :
  CiLut(lut_template)
{
  ymuint n1 = 0;
  if ( index_array1.empty() ) {
    n1 = index_num(0);
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = lut_template->index(0, i);
    }
  }
  else {
    n1 = index_array1.size();
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = index_array1[i];
    }
  }

  ymuint n2 = 0;
  if ( index_array2.empty() ) {
    n2 = index_num(1);
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = lut_template->index(1, i);
    }
  }
  else {
    n2 = index_array2.size();
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = index_array2[i];
    }
  }

  ymuint n = n1 * n2;
  assert_cond( value_array.size() == n, __FILE__, __LINE__);
  mValueArray.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mValueArray[i] = value_array[i];
  }

  ymuint nd = (n1 - 1) * (n2 - 1);
  mDataArray.resize(nd);
  for (ymuint i1 = 0; i1 < n1 - 1; ++ i1) {
    for (ymuint i2 = 0; i2 < n2 - 1; ++ i2) {
      Data& data = mDataArray[i1 * (n2 - 1) + i2];
      data.mIndexBase[0] = mIndexArray[0][i1];
      data.mIndexBase[1] = mIndexArray[1][i2];
      data.mIndexWidth[0] = mIndexArray[0][i1 + 1] - mIndexArray[0][i1];
      data.mIndexWidth[1] = mIndexArray[1][i2 + 1] - mIndexArray[1][i2];
      double val00 = mValueArray[idx(i1, i2)];
      double val10 = mValueArray[idx(i1 + 1, i2)];
      double val01 = mValueArray[idx(i1, i2 + 1)];
      double val11 = mValueArray[idx(i1 + 1, i2 + 2)];
      data.mBase = val00;
      data.mCoef1[0] = val10 - val00;
      data.mCoef1[1] = val01 - val00;
      data.mCoef2 = val11 - val10 - val01 + val00;
    }
  }
}

// @brief デストラクタ
CiLut2D::~CiLut2D()
{
}

// @brief 次元数の取得
ymuint32
CiLut2D::dimension() const
{
  return 2;
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLut2D::index_num(ymuint32 var) const
{
  assert_cond( var < 2, __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief インデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLut2D::index(ymuint32 var,
	       ymuint32 pos) const
{
  assert_cond( var < 2, __FILE__, __LINE__);
  assert_cond( pos < index_num(var), __FILE__, __LINE__);
  return mIndexArray[var][pos];
}

// @brief 格子点の値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut2D::grid_value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 2, __FILE__, __LINE__);
  ymuint pos1 = pos_array[0];
  ymuint pos2 = pos_array[1];
  assert_cond( pos1 < index_num(0), __FILE__, __LINE__);
  assert_cond( pos2 < index_num(1), __FILE__, __LINE__);
  return mValueArray[idx(pos1, pos2)];
}

// @brief 値の取得
// @param[in] val_array 入力の値の配列
// @note val_array のサイズは dimension() と同じ
double
CiLut2D::value(const vector<double>& val_array) const
{
  assert_cond( val_array.size() == 2, __FILE__, __LINE__);

  double val1 = val_array[0];
  ymuint idx1_a = search(val1, mIndexArray[0]);
  ymuint idx1_b = idx1_a + 1;
  double x0 = mIndexArray[0][idx1_a];
  double x1 = mIndexArray[0][idx1_b];

  double val2 = val_array[1];
  ymuint idx2_a = search(val2, mIndexArray[1]);
  ymuint idx2_b = idx2_a + 1;
  double y0 = mIndexArray[1][idx2_a];
  double y1 = mIndexArray[1][idx2_b];

  // 単純な線形補間
  double dx = (val1 - x0) / (x1 - x0);
  double dy = (val2 - y0) / (y1 - y0);
  double val_0 = mValueArray[idx(idx1_a, idx2_a)];
  double val_x = mValueArray[idx(idx1_b, idx2_a)];
  double val_y = mValueArray[idx(idx1_a, idx2_b)];
  double val_xy = mValueArray[idx(idx1_b, idx2_b)];
  double val_dx = val_x - val_0;
  double val_dy = val_y - val_0;
  double val_dxy = val_xy - val_x - val_y + val_0;

  double tmp = val_0 + val_dx * dx + val_dy * dy + val_dxy * dx * dy;

  ymuint idx = idx1_a * (index_num(1) - 1) + idx2_b;
  double tmp2 = mDataArray[idx].calc(val1, val2);

  if ( tmp != tmp2 ) {
    cout << "Error " << endl;
    cout << "tmp  = " << tmp << endl
	 << "tmp2 = " << tmp2 << endl;
    cout << "val_0 = " << val_0 << endl
	 << "mBase = " << mDataArray[idx].mBase << endl
	 << "val_dx    = " << val_dx << endl
	 << "mCoef1[0] = " << mDataArray[idx].mCoef1[0] << endl
	 << "val_dy    = " << val_dy << endl
	 << "mCoef1[1] = " << mDataArray[idx].mCoef1[1] << endl
	 << "val_dxy   = " << val_dxy << endl
	 << "mCoef2    = " << mDataArray[idx].mCoef2 << endl;
  }
  return tmp2;
}

// @brief 値を計算する．
double
CiLut2D::Data::calc(double val1,
		    double val2) const
{
  double dx = (val1 - mIndexBase[0]) / mIndexWidth[0];
  double dy = (val2 - mIndexBase[1]) / mIndexWidth[1];
  return mBase + mCoef1[0] * dx + mCoef1[1] * dy + mCoef2 * dx * dy;
}


//////////////////////////////////////////////////////////////////////
// クラス CiLut3D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut3D::CiLut3D(const CellLutTemplate* lut_template,
		 const vector<double>& value_array,
		 const vector<double>& index_array1,
		 const vector<double>& index_array2,
		 const vector<double>& index_array3) :
  CiLut(lut_template)
{
  ymuint n1 = 0;
  if ( index_array1.empty() ) {
    n1 = index_num(0);
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = lut_template->index(0, i);
    }
  }
  else {
    n1 = index_array1.size();
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = index_array1[i];
    }
  }

  ymuint n2 = 0;
  if ( index_array2.empty() ) {
    n2 = index_num(1);
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = lut_template->index(1, i);
    }
  }
  else {
    n2 = index_array2.size();
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = index_array2[i];
    }
  }

  ymuint n3 = 0;
  if ( index_array3.empty() ) {
    n3 = index_num(2);
    mIndexArray[2].resize(n3);
    for (ymuint32 i = 0; i < n3; ++ i) {
      mIndexArray[2][i] = lut_template->index(2, i);
    }
  }
  else {
    n3 = index_array3.size();
    mIndexArray[2].resize(n3);
    for (ymuint32 i = 0; i < n3; ++ i) {
      mIndexArray[2][i] = index_array3[i];
    }
  }

  ymuint n = n1 * n2 * n3;
  assert_cond( value_array.size() == n, __FILE__, __LINE__);
  mValueArray.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mValueArray[i] = value_array[i];
  }
}

// @brief デストラクタ
CiLut3D::~CiLut3D()
{
}

// @brief 次元数の取得
ymuint32
CiLut3D::dimension() const
{
  return 3;
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLut3D::index_num(ymuint32 var) const
{
  assert_cond( var < 3, __FILE__, __LINE__);
  return mIndexArray[var].size();
}

// @brief インデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLut3D::index(ymuint32 var,
	       ymuint32 pos) const
{
  assert_cond( var < 3, __FILE__, __LINE__);
  assert_cond( pos < index_num(var), __FILE__, __LINE__);
  return mIndexArray[var][pos];
}

// @brief 格子点の値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut3D::grid_value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 3, __FILE__, __LINE__);
  ymuint pos1 = pos_array[0];
  ymuint pos2 = pos_array[1];
  ymuint pos3 = pos_array[2];
  assert_cond( pos1 < index_num(0), __FILE__, __LINE__);
  assert_cond( pos2 < index_num(1), __FILE__, __LINE__);
  assert_cond( pos3 < index_num(2), __FILE__, __LINE__);
  return mValueArray[idx(pos1, pos2, pos3)];
}

// @brief 値の取得
// @param[in] val_array 入力の値の配列
// @note val_array のサイズは dimension() と同じ
double
CiLut3D::value(const vector<double>& val_array) const
{
  assert_cond( val_array.size() == 3, __FILE__, __LINE__);
  double val1 = val_array[0];
  ymuint idx1_a = search(val1, mIndexArray[0]);
  ymuint idx1_b = idx1_a + 1;
  double x0 = mIndexArray[0][idx1_a];
  double x1 = mIndexArray[0][idx1_b];

  double val2 = val_array[1];
  ymuint idx2_a = search(val2, mIndexArray[1]);
  ymuint idx2_b = idx2_a + 1;
  double y0 = mIndexArray[1][idx2_a];
  double y1 = mIndexArray[1][idx2_b];

  double val3 = val_array[2];
  ymuint idx3_a = search(val3, mIndexArray[2]);
  ymuint idx3_b = idx3_a + 1;
  double z0 = mIndexArray[2][idx3_a];
  double z1 = mIndexArray[3][idx3_b];

  // 単純な線形補間
  double dx = (val1 - x0) / (x1 - x0);
  double dy = (val2 - y0) / (y1 - y0);
  double dz = (val3 - z0) / (z1 - z0);
  double val_0 = mValueArray[idx(idx1_a, idx2_a, idx3_a)];
  double val_x = mValueArray[idx(idx1_b, idx2_a, idx3_a)];
  double val_y = mValueArray[idx(idx1_a, idx2_b, idx3_a)];
  double val_z = mValueArray[idx(idx1_a, idx2_a, idx3_b)];
  double val_xy = mValueArray[idx(idx1_b, idx2_b, idx3_a)];
  double val_yz = mValueArray[idx(idx1_a, idx2_b, idx3_b)];
  double val_zx = mValueArray[idx(idx1_b, idx2_a, idx3_b)];
  double val_xyz = mValueArray[idx(idx1_b, idx2_b, idx3_b)];
  double val_dx = val_x - val_0;
  double val_dy = val_y - val_0;
  double val_dz = val_z - val_0;
  double val_dxy = val_xy - val_dx - val_dy;
  double val_dyz = val_yz - val_dy - val_dz;
  double val_dzx = val_zx - val_dz - val_dx;
  double val_dxyz = val_xyz - val_xy - val_yz - val_zx + val_x + val_y + val_z - val_0;
  return val_0 + val_dx * dx + val_dy * dy + val_dz * dz
    + val_dxy * dx * dy + val_dyz * dy * dz + val_dzx * dz * dx
    + val_dxyz * dx * dy * dz;
}

END_NAMESPACE_YM_CELL
