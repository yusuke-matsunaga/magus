
/// @file libym_verilog/elb_impl/EiImpNet.cc
/// @brief EiNet の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiImpNet.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiImpNet.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 暗黙のネットを生成する．
// @param[in] pt_expr パース木のプライマリ式
ElbDecl*
EiFactory::new_ImpNet(const VlNamedObj* parent,
		      const PtExpr* pt_expr,
		      tVpiNetType net_type)
{
  void* p = mAlloc.get_memory(sizeof(EiImpNet));
  EiImpNet* decl = new (p) EiImpNet(parent, pt_expr, net_type);

  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiImpNet
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木のプライマリ式
EiImpNet::EiImpNet(const VlNamedObj* parent,
		   const PtExpr* pt_expr,
		   tVpiNetType net_type) :
  mParent(parent),
  mPtExpr(pt_expr),
  mNetType(net_type)
{
}

// @brief デストラクタ
EiImpNet::~EiImpNet()
{
}

// @brief 型の取得
tVpiObjType
EiImpNet::type() const
{
  return kVpiNet;
}

// @brief ファイル位置の取得
FileRegion
EiImpNet::file_region() const
{
  return mPtExpr->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiImpNet::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiImpNet::name() const
{
  return mPtExpr->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
VlValueType
EiImpNet::value_type() const
{
  return VlValueType(false, true, 1);
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
// @note このクラスでは false を返す．
bool
EiImpNet::is_signed() const
{
  return false;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiImpNet::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiImpNet::left_range_val() const
{
  return 0;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiImpNet::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiImpNet::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiImpNet::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiImpNet::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiImpNet::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
ymuint
EiImpNet::bit_size() const
{
  return 1;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiImpNet::calc_bit_offset(int index,
			  ymuint& offset) const
{
  if ( index == 0 ) {
    offset = 0;
    return true;
  }
  else {
    return false;
  }
}

// @brief データ型の取得
// @retval データ型 パラメータや変数の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiImpNet::data_type() const
{
  return kVpiVarNone;
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiImpNet::net_type() const
{
  return mNetType;
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
EiImpNet::vs_type() const
{
  return kVpiVsNone;
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiImpNet::drive0() const
{
  return kVpiNoStrength;
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiImpNet::drive1() const
{
  return kVpiNoStrength;
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiImpNet::charge() const
{
  return kVpiNoStrength;
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
EiImpNet::delay() const
{
  return NULL;
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiImpNet::init_value() const
{
  return NULL;
}

// @brief 符号付きに補正する．
void
EiImpNet::set_signed()
{
  // なにもしない．
}

// @brief スカラー値を返す．
VlScalarVal
EiImpNet::get_scalar() const
{
  return mVal;
}

// @brief スカラー値を設定する．
void
EiImpNet::set_scalar(const VlScalarVal& val)
{
  mVal = val;
}

// @brief 論理値を返す．
VlScalarVal
EiImpNet::get_logic() const
{
  return mVal.to_logic();
}

// @brief real 型の値を返す．
double
EiImpNet::get_real() const
{
  return mVal.to_real();
}

// @brief real 型の値を設定する．
void
EiImpNet::set_real(double val)
{
  mVal = VlScalarVal(val);
}

// @brief bitvector 型の値を返す．
void
EiImpNet::get_bitvector(BitVector& bitvector,
			const VlValueType& req_type) const
{
  bitvector = mVal;
  bitvector.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiImpNet::set_bitvector(const BitVector& val)
{
  mVal = val.to_scalar();
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
VlScalarVal
EiImpNet::get_bitselect(int index) const
{
  if ( index == 0 ) {
    return mVal;
  }
  else {
    return VlScalarVal::x();
  }
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiImpNet::set_bitselect(int index,
			const VlScalarVal& val)
{
  if ( index == 0 ) {
    mVal = val;
  }
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiImpNet::get_partselect(int left,
			 int right,
			 BitVector& val) const
{
  if ( left == 0 && right == 0 ) {
    val = mVal;
  }
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiImpNet::set_partselect(int left,
			 int right,
			 const BitVector& val)
{
  if ( left == 0 && right == 0 ) {
    mVal = val.to_scalar();
  }
}

END_NAMESPACE_YM_VERILOG
