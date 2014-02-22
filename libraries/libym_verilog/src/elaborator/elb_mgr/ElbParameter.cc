
/// @file ElbParameter.cc
/// @brief ElbParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ElbParameter.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbParamHead
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
ElbParamHead::ElbParamHead()
{
}

// @brief デストラクタ
ElbParamHead::~ElbParamHead()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbParameter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbParameter::ElbParameter() :
  mNext(NULL)
{
}

// @brief デストラクタ
ElbParameter::~ElbParameter()
{
}

// @brief 定数値を持つ型のときに true を返す．
bool
ElbParameter::is_consttype() const
{
  return true;
}

// @brief ビット要素を返す．
// @param[in] index インデックス
// @retval ビット要素 index が範囲内の場合
// @retval NULL index が範囲外の場合．またはビット展開されていない場合
// @note このクラスでは NULL を返す．
const VlDecl*
ElbParameter::elem_by_index(int index) const
{
  return NULL;
}

// @brief ビット要素を返す．
// @param[in] offset オフセット
// @retval ビット要素 offset が範囲内の場合
// @retval NULL offset が範囲外の場合．またはビット展開されていない場合
// @note このクラスでは NULL を返す．
const VlDecl*
ElbParameter::elem_by_offset(ymuint offset) const
{
  return NULL;
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

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
ElbParameter::init_value() const
{
  return NULL;
}

END_NAMESPACE_YM_VERILOG
