
/// @file libym_verilog/elb_impl/EiParameter.cc
/// @brief EiParameter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParameter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiParameter.h"
#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"

#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief parameter 宣言のヘッダを生成する(範囲指定なし)．
// @param[in] parent 親のスコープ
ElbParamHead*
EiFactory::new_ParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head)
{
  void* p = mAlloc.get_memory(sizeof(EiParamHead));
  EiParamHead* head = new (p) EiParamHead(parent, pt_head);
  return head;
}

// @brief parameter 宣言のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
ElbParamHead*
EiFactory::new_ParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head,
			 const PtExpr* left,
			 const PtExpr* right,
			 int left_val,
			 int right_val)
{
  assert_cond( left != NULL, __FILE__, __LINE__);
  assert_cond( right != NULL, __FILE__, __LINE__);

  void* p = mAlloc.get_memory(sizeof(EiParamHeadV));
  EiParamHead* head = new (p) EiParamHeadV(parent, pt_head,
					   left, right,
					   left_val, right_val);
  return head;
}

// @brief parameter 宣言を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期割り当て式
// @param[in] is_local localparam の時 true
ElbParameter*
EiFactory::new_Parameter(ElbParamHead* head,
			 const PtNamedBase* pt_item,
			 bool is_local)
{
  EiParameter* param = NULL;

  switch ( head->type() ) {
  case kVpiParameter:
  case kVpiSpecParam:
    if ( is_local ) {
      void* p = mAlloc.get_memory(sizeof(EiLocalParam));
      param = new (p) EiLocalParam(head, pt_item);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(EiParameter));
      param = new (p) EiParameter(head, pt_item);
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return param;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
EiParamHead::EiParamHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head) :
  mParent(parent),
  mPtHead(pt_head)
{
}

// @brief デストラクタ
EiParamHead::~EiParamHead()
{
}

// @brief 型の取得
tVpiObjType
EiParamHead::type() const
{
  switch ( mPtHead->type() ) {
  case kPtDecl_Param:
  case kPtDecl_LocalParam:
    return kVpiParameter;

  case kPtDecl_SpecParam:
    return kVpiSpecParam;

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }
  return kVpiParameter;
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParamHead::parent() const
{
  return mParent;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiParamHead::is_signed() const
{
  return mPtHead->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParamHead::has_range() const
{
  return false;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHead::left_range_val() const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarInteger:
    return kVpiSizeInteger - 1;

  case kVpiVarReal:
    return 0;

  case kVpiVarTime:
    return kVpiSizeTime - 1;

  default:
    break;
  }
  // int とみなす．
  return kVpiSizeInteger - 1;
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHead::right_range_val() const
{
  return 0;
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHead::left_range_string() const
{
  return string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHead::right_range_string() const
{
  return string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParamHead::is_big_endian() const
{
  return true;
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParamHead::is_little_endian() const
{
  return true;
}

// @brief ビット幅を返す．
ymuint
EiParamHead::bit_size() const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarInteger:
    return kVpiSizeInteger;

  case kVpiVarReal:
    return kVpiSizeReal;

  case kVpiVarTime:
    return kVpiSizeTime;

  default:
    break;
  }
  // int とみなす．
  return kVpiSizeInteger;
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiParamHead::calc_bit_offset(int index,
			     ymuint& offset) const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarReal:
    return false;

  case kVpiVarTime:
    if ( index >= 0 && index < static_cast<int>(kVpiSizeTime) ) {
      offset = index;
      return true;
    }
    return false;

  case kVpiVarInteger:
  default:
    // int とみなす．
    if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
      offset = index;
      return true;
    }
    return false;
  }
  return false;
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
EiParamHead::value_type() const
{
  switch ( mPtHead->data_type() ) {
  case kVpiVarReal:
  case kVpiVarRealtime:
    return kVpiValueReal;

  case kVpiVarTime:
    return kVpiValueTime;

  case kVpiVarInteger:
    return kVpiValueInteger;

  case kVpiVarNone:
    // この場合，式の値で決まる．
    return kVpiValueNone;
  }
  assert_not_reached(__FILE__, __LINE__);
  return kVpiValueNone;
}

// @brief データ型の取得
tVpiVarType
EiParamHead::data_type() const
{
  return mPtHead->data_type();
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamHeadV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] left 範囲の左側の式
// @param[in] right 範囲の右側の式
// @param[in] left_val 範囲の左側の値
// @param[in] right_val 範囲の右側の値
EiParamHeadV::EiParamHeadV(const VlNamedObj* parent,
			   const PtDeclHead* pt_head,
			   const PtExpr* left,
			   const PtExpr* right,
			   int left_val,
			   int right_val) :
  EiParamHead(parent, pt_head)
{
  mRange.set(left, right, left_val, right_val);
}

// @brief デストラクタ
EiParamHeadV::~EiParamHeadV()
{
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParamHeadV::has_range() const
{
  return true;
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHeadV::left_range_val() const
{
  return mRange.left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParamHeadV::right_range_val() const
{
  return mRange.right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHeadV::left_range_string() const
{
  return mRange.left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParamHeadV::right_range_string() const
{
  return mRange.right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParamHeadV::is_big_endian() const
{
  return mRange.is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParamHeadV::is_little_endian() const
{
  return mRange.is_little_endian();
}

// @brief ビット幅を返す．
ymuint
EiParamHeadV::bit_size() const
{
  return mRange.size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiParamHeadV::calc_bit_offset(int index,
			      ymuint& offset) const
{
  return mRange.calc_offset(index, offset);
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
EiParamHeadV::value_type() const
{
  tVpiValueType type = is_signed() ? kVpiValueSS : kVpiValueUS;
  return pack(type, bit_size());
}


//////////////////////////////////////////////////////////////////////
// クラス EiParameter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiParameter::EiParameter(ElbParamHead* head,
			 const PtNamedBase* pt_item) :
  mHead(head),
  mPtItem(pt_item),
  mExpr(NULL)
{
}

// @brief デストラクタ
EiParameter::~EiParameter()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
tVpiObjType
EiParameter::type() const
{
  return mHead->type();
}

// @brief ファイル位置の取得
FileRegion
EiParameter::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiParameter::parent() const
{
  return mHead->parent();
}

// @brief 名前の取得
const char*
EiParameter::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
tVpiValueType
EiParameter::value_type() const
{
  // (1) with no type or range specification の場合，そのパラメータに
  //     割り当てられる最終的な値の type と range を持つ．
  //
  // (2) with a range, but with no type の場合，その range を持つ
  //     unsigned 型と定義される．
  //     初期値および override された値はこの型に変換される．
  //
  // (3) with a type specification, but with no range specification の場合，
  //     その型に定義される．range は最終的な値の range をとる．
  //
  // (4) a signed parameter (signed but with no range のこと？) は最終的な値の
  //     範囲を持つ．
  //
  // (5) with a signed type specification and with a range specification
  //     の場合，そのままの型を持つ．値はこの型に変換される．
  //
  // (6) with no range specification, and with either a signed type
  //     specification or no type specification は lsb = 0, msb = size -1
  //     という implied range を持つ．
  //     ただし，最終的な値も unsized の場合，lsb = 0, msb は最低31以上の
  //     実装依存の値をとる．

  tVpiValueType vt = mHead->value_type();
  if ( vt == kVpiValueNone ) {
    // (1)
    if ( mValue.is_int_type() ) {
      return kVpiValueInteger;
    }
    if ( mValue.is_scalar_type() ) {
      return pack(kVpiValueUS, 1);
    }
    if ( mValue.is_bitvector_type() ) {
      const BitVector& bv = mValue.bitvector_value();
      ymuint size = bv.size();
      if ( bv.is_signed() ) {
	if ( bv.is_sized() ) {
	  return pack(kVpiValueSS, size);
	}
	else {
	  return pack(kVpiValueSU, size);
	}
      }
      else {
	if ( bv.is_sized() ) {
	  return pack(kVpiValueUS, size);
	}
	else {
	  return pack(kVpiValueUU, size);
	}
      }
    }
    if ( mValue.is_real_type() ) {
      return kVpiValueReal;
    }
    return kVpiValueNone;
  }
  if ( is_bitvector_type(vt) ) {
    if ( is_sized_type(vt) ) {
      // (2)
      return vt;
    }
    else {
      // (3), (4) ?
      const BitVector& bv = mValue.bitvector_value();
      ymuint size = bv.size();
      if ( is_signed_type(vt) ) {
	if ( bv.is_sized() ) {
	  return pack(kVpiValueSS, size);
	}
	else {
	  return pack(kVpiValueSU, size);
	}
      }
      else {
	if ( bv.is_sized() ) {
	  return pack(kVpiValueUS, size);
	}
	else {
	  return pack(kVpiValueUU, size);
	}
      }
    }
  }
  // (5)
  return vt;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiParameter::is_signed() const
{
  return mHead->is_signed();
}

// @brief 範囲指定を持つとき true を返す．
bool
EiParameter::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParameter::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiParameter::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParameter::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiParameter::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiParameter::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiParameter::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
ymuint
EiParameter::bit_size() const
{
#if 1
  return mHead->bit_size();
#else
  return unpack_size(value_type());
#endif
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiParameter::calc_bit_offset(int index,
			     ymuint& offset) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiParameter::data_type() const
{
  return mHead->data_type();
}

// @brief localparam のときに true 返す．
// @note このクラスでは false を返す．
bool
EiParameter::is_local_param() const
{
  return false;
}

// @brief 値の取得
VlValue
EiParameter::get_value() const
{
  return mValue;
}

// @brief 値の設定
// @param[in] expr 値を表す式
// @param[in] value 値
void
EiParameter::set_expr(const PtExpr* expr,
		      const VlValue& value)
{
  mExpr = expr;
  mValue = VlValue(value, mHead->value_type());
}


//////////////////////////////////////////////////////////////////////
// クラス EiLocalParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiLocalParam::EiLocalParam(ElbParamHead* head,
			   const PtNamedBase* pt_item) :
  EiParameter(head, pt_item)
{
}

// @brief デストラクタ
EiLocalParam::~EiLocalParam()
{
}

// @brief localparam のときに true 返す．
// @note このクラスでは true を返す．
bool
EiLocalParam::is_local_param() const
{
  return true;
}

END_NAMESPACE_YM_VERILOG
