
/// @file libym_verilog/elb/ElbParameter.cc
/// @brief ElbParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParameter.h"
#include "ElbParamAssign.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbParameter
//////////////////////////////////////////////////////////////////////
  
// @brief 定数値を持つ型のときに true を返す．
bool
ElbParameter::is_consttype() const
{
  return true;
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
ElbParameter::left_range() const
{
  return _left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
ElbParameter::right_range() const
{
  return _right_range();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
ElbParameter::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
ElbParameter::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbParameter::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
ElbParameter::delay() const
{
  return NULL;
}

// @brief 配列型オブジェクトの場合の次元数の取得
// @note このクラスでは 0 を返す．
ymuint32
ElbParameter::dimension() const
{
  return 0;
}
  
// @brief 範囲の取得
// @param[in] pos 位置 ( 0 <= pos < dimension() )
// @note このクラスでは NULL を返す．
const VlRange*
ElbParameter::range(ymuint32 pos) const
{
  return NULL;
}

END_NAMESPACE_YM_VERILOG
