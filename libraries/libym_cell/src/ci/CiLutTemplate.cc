
/// @file CiLutTemplate.cc
/// @brief CiLutTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CiLutTemplate.h"


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

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CiLutTemplate::dump(ODO& s) const
{
  ymuint8 d = dimension();
  s << name()
    << d;
  for (ymuint i = 0; i < d; ++ i) {
    s << static_cast<ymuint8>(variable_type(i));
    ymuint8 n = index_num(i);
    s << n;
    for (ymuint j = 0; j < n; ++ j) {
      s << index(i, j);
    }
  }
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
  ASSERT_COND( var == 0 );
  return mVarType;
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate1D::index_num(ymuint32 var) const
{
  ASSERT_COND( var == 0 );
  return mIndexArray.size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate1D::index(ymuint32 var,
		       ymuint32 pos) const
{
  ASSERT_COND( var == 0 );
  ASSERT_COND( pos < index_num(0) );
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
  ASSERT_COND( var < 2  );
  return mVarType[var];
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate2D::index_num(ymuint32 var) const
{
  ASSERT_COND( var < 2  );
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate2D::index(ymuint32 var,
			 ymuint32 pos) const
{
  ASSERT_COND( var < 2  );
  ASSERT_COND( pos < index_num(var)  );
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
  ASSERT_COND( var < 3  );
  return mVarType[var];
}

// @brief インデックス数の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
ymuint32
CiLutTemplate3D::index_num(ymuint32 var) const
{
  ASSERT_COND( var < 3  );
  return mIndexArray[var].size();
}

// @brief デフォルトインデックス値の取得
// @param[in] var 変数番号 ( 0 <= var < dimension() )
// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
double
CiLutTemplate3D::index(ymuint32 var,
			 ymuint32 pos) const
{
  ASSERT_COND( var < 3  );
  ASSERT_COND( pos < index_num(var)  );
  return mIndexArray[var][pos];
}

END_NAMESPACE_YM_CELL
