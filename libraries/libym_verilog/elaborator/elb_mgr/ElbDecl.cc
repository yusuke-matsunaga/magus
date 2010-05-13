
/// @file libym_verilog/elb/ElbDecl.cc
/// @brief ElbDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbDecl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "ElbDecl.h"
#include "ElbGenvar.h"
#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbDeclHead
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
ElbDeclHead::ElbDeclHead()
{
}

// @brief デストラクタ
ElbDeclHead::~ElbDeclHead()
{
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// @note このクラスでは false を返す．
bool
ElbDeclHead::is_signed() const
{
  return false;
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
ElbDeclHead::left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
ElbDeclHead::right_range() const
{
  return NULL;
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
// @note このクラスでは kVpiVarNone を返す．
tVpiVarType
ElbDeclHead::data_type() const
{
  return kVpiVarNone;
}
  
// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
// @note このクラスでは kVpiNone を返す．
tVpiNetType
ElbDeclHead::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
// @note このクラスでは kVpiNsNone を返す．
tVpiVsType
ElbDeclHead::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
// @note このクラスでは kVpiNoStrength を返す．
tVpiStrength
ElbDeclHead::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
// @note このクラスでは NULL を返す．
ElbDelay*
ElbDeclHead::delay() const
{
  return NULL;
}

// @brief 遅延式の設定
// @note このクラスではなにもしない．
void
ElbDeclHead::set_delay(ElbDelay* delay)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclBase
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
ElbDeclBase::ElbDeclBase() :
  mNext(NULL)
{
}
  
// @brief デストラクタ
ElbDeclBase::~ElbDeclBase()
{
}
  
// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
ElbDeclBase::value_type() const
{
  return kVpiValueNone;
}
  
// @brief 定数値を持つ型のときに true を返す．
bool
ElbDeclBase::is_consttype() const
{
  return false;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
ElbDeclBase::is_signed() const
{
  return false;
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
ElbDeclBase::left_range() const
{
  return _left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
ElbDeclBase::right_range() const
{
  return _right_range();
}

// @brief データ型の取得
// @retval データ型 パラメータや変数の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
ElbDeclBase::data_type() const
{
  return kVpiVarNone;
}
  
// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
ElbDeclBase::net_type() const
{
  return kVpiNone;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
ElbDeclBase::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbDeclBase::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbDeclBase::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
ElbDeclBase::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
ElbDeclBase::delay() const
{
  return NULL;
}
  
// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
// @note このクラスでは NULL を返す．
const VlExpr*
ElbDeclBase::init_value() const
{
  return NULL;
}

// @brief localparam のときに true 返す．
// @note このクラスでは false を返す．
bool
ElbDeclBase::is_local_param() const
{
  return false;
}

// @brief 配列型オブジェクトの場合の次元数の取得
// @note このクラスでは 0 を返す．
ymuint32
ElbDeclBase::dimension() const
{
  return 0;
}
  
// @brief 範囲の取得
// @param[in] pos 位置 ( 0 <= pos < dimension() )
// @note このクラスでは NULL を返す．
const VlRange*
ElbDeclBase::range(ymuint32 pos) const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDecl
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
ElbDecl::ElbDecl()
{
}
  
// @brief デストラクタ
ElbDecl::~ElbDecl()
{
}

// @brief 初期値の設定
// @param[in] expr 初期値
// @note このクラスではなにもしない．
void
ElbDecl::set_init(ElbExpr* expr)
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbDeclArray
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
ElbDeclArray::ElbDeclArray()
{
}
  
// @brief デストラクタ
ElbDeclArray::~ElbDeclArray()
{
}


//////////////////////////////////////////////////////////////////////
// クラス ElbGenvar
//////////////////////////////////////////////////////////////////////
  
// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
ElbGenvar::value_type() const
{
  return kVpiValueInteger;
}
  
// @brief 定数値を持つ型のときに true を返す．
bool
ElbGenvar::is_consttype() const
{
  return true;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
ElbGenvar::is_signed() const
{
  return true;
}

// @brief ビット幅を返す．
ymuint32
ElbGenvar::bit_size() const
{
  return kVpiSizeInteger;
}

// @brief データ型の取得
// @retval データ型 パラメータや変数の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
ElbGenvar::data_type() const
{
  return kVpiVarInteger;
}

// @brief 符号付きに補正する．
// @note このクラスではなにもしない．
void
ElbGenvar::set_signed()
{
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
ElbGenvar::_left_range() const
{
  return NULL;
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
// @note このクラスでは NULL を返す．
ElbExpr*
ElbGenvar::_right_range() const
{
  return NULL;
}

END_NAMESPACE_YM_VERILOG
