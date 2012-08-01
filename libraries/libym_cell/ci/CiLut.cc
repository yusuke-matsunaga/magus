
/// @file CiLut.cc
/// @brief CiLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CiLut.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// クラス CiLutTemplateBase
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLutTemplateBase::CiLutTemplateBase(ShString name) :
  mName(name)
{
}

// @brief デストラクタ
CiLutTemplateBase::~CiLutTemplateBase()
{
}

// @brief 名前の取得
const char*
CiLutTemplateBase::name() const
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
  CiLutTemplateBase(name),
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
  CiLutTemplateBase(name)
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
  CiLutTemplateBase(name)
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
// クラス CiLut1D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut1D::CiLut1D(const CellLutTemplate* lut_template,
		 const vector<double>& value_array,
		 const vector<double>& index_array) :
  mTemplate(lut_template)
{
  ymuint n = 0;
  if ( index_array.empty() ) {
    n = mTemplate->index_num(0);
    mIndexArray.resize(n);
    for (ymuint32 i = 0; i < n; ++ i) {
      mIndexArray[i] = mTemplate->index(0, i);
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
  for (ymuint i = 0; i < n; ++ i) {
    mValueArray[i] = value_array[i];
  }
}

// @brief デストラクタ
CiLut1D::~CiLut1D()
{
}

// @brief テンプレートの取得
const CellLutTemplate*
CiLut1D::lut_template() const
{
  return mTemplate;
}

// @brief インデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLut1D::index(ymuint32 var,
	       ymuint32 pos) const
{
  assert_cond( var < 1 , __FILE__, __LINE__);
  assert_cond( pos < mIndexArray.size() , __FILE__, __LINE__);
  return mIndexArray[pos];
}

// @brief 値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut1D::value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 1, __FILE__, __LINE__);
  return mValueArray[pos_array[0]];
}


//////////////////////////////////////////////////////////////////////
// クラス CiLut2D
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CiLut2D::CiLut2D(const CellLutTemplate* lut_template,
		 const vector<double>& value_array,
		 const vector<double>& index_array1,
		 const vector<double>& index_array2) :
  mTemplate(lut_template)
{
  ymuint n1 = 0;
  if ( index_array1.empty() ) {
    n1 = mTemplate->index_num(0);
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = mTemplate->index(0, i);
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
    n2 = mTemplate->index_num(1);
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = mTemplate->index(1, i);
    }
  }
  else {
    n2 = index_array2.size();
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = index_array2[i];
    }
  }

  assert_cond( value_array.size() == n1 * n2, __FILE__, __LINE__);
  for (ymuint i1 = 0; i1 < n1; ++ i1) {
    for (ymuint i2 = 0; i2 < n2; ++ i2) {
      ymuint i = i1 * n2 + i2;
      mValueArray[i] = value_array[i];
    }
  }
}

// @brief デストラクタ
CiLut2D::~CiLut2D()
{
}

// @brief テンプレートの取得
const CellLutTemplate*
CiLut2D::lut_template() const
{
  return mTemplate;
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

// @brief 値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut2D::value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 2, __FILE__, __LINE__);
  ymuint i = pos_array[0] * index_num(1) + pos_array[1];
  return mValueArray[i];
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
  mTemplate(lut_template)
{
  ymuint n1 = 0;
  if ( index_array1.empty() ) {
    n1 = mTemplate->index_num(0);
    mIndexArray[0].resize(n1);
    for (ymuint32 i = 0; i < n1; ++ i) {
      mIndexArray[0][i] = mTemplate->index(0, i);
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
    n2 = mTemplate->index_num(1);
    mIndexArray[1].resize(n2);
    for (ymuint32 i = 0; i < n2; ++ i) {
      mIndexArray[1][i] = mTemplate->index(1, i);
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
    n3 = mTemplate->index_num(2);
    mIndexArray[2].resize(n3);
    for (ymuint32 i = 0; i < n3; ++ i) {
      mIndexArray[2][i] = mTemplate->index(2, i);
    }
  }
  else {
    n3 = index_array3.size();
    mIndexArray[2].resize(n3);
    for (ymuint32 i = 0; i < n3; ++ i) {
      mIndexArray[2][i] = index_array3[i];
    }
  }

  assert_cond( value_array.size() == n1 * n2 * n3, __FILE__, __LINE__);
  for (ymuint i1 = 0; i1 < n1; ++ i1) {
    for (ymuint i2 = 0; i2 < n2; ++ i2) {
      for (ymuint i3 = 0; i3 < n3; ++ i3) {
	ymuint i = ((i1 * n2) + i2) * n3 + i3;
	mValueArray[i] = value_array[i];
      }
    }
  }
}

// @brief デストラクタ
CiLut3D::~CiLut3D()
{
}

// @brief テンプレートの取得
const CellLutTemplate*
CiLut3D::lut_template() const
{
  return mTemplate;
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

// @brief 値の取得
// @param[in] pos_array 格子点座標
// @note pos_array のサイズは dimension() と同じ
double
CiLut3D::value(const vector<ymuint32>& pos_array) const
{
  assert_cond( pos_array.size() == 3, __FILE__, __LINE__);
  ymuint i = ((pos_array[0] * index_num(1)) + pos_array[1]) * index_num(2) + pos_array[2];
  return mValueArray[i];
}

END_NAMESPACE_YM_CELL
