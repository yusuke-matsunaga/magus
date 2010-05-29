
/// @file libym_verilog/elb_impl/EiParam.cc
/// @brief EiParam の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiParam.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiParam.h"
#include "ElbDelay.h"
#include "ElbExpr.h"

#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/pt/PtItem.h"
#include "ym_verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// クラス EiParam EiDecl.h "EiDecl.h"
//////////////////////////////////////////////////////////////////////

// parameter の型についてのルール(3.11.1)(12.2)
//
// case A:
// with no type or range specification の場合，そのパラメータに割り当てられ
// る最終的な値の type と range を持つ．
//
// case B:
// with a range, but with no type の場合，その range を持つ unsigned 型と定
// 義される．初期値および override された値はこの型に変換される．
//
// case C:
// with a type specification, but with no range specification の場合，その
// 型に定義される．range は最終的な値の range をとる．
// a signed parameter (signed but with no range のこと？) は最終的な値の
// 範囲を持つ．
//
// case D:
// with a signed type specification and with a range specification の場合，
// そのままの型を持つ．値はこの型に変換される．
//
// case E:
// with no range specification, and with either a signed type
// specification or no type specification は lsb = 0, msb = size -1 とい
// う implied range を持つ．
// ただし，最終的な値も unsized の場合，lsb = 0, msb は最低31以上の実装依
// 存の値をとる．

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiParam::EiParam(ElbDeclHead* head,
		 const PtNamedBase* pt_item,
		 ElbExpr* init) :
  EiDecl(head, pt_item),
  mInit(init)
{
}

// @brief デストラクタ
EiParam::~EiParam()
{
}

// @breif 値の型を返す．
tVpiValueType
EiParam::value_type() const
{
  // ちょっと複雑
  switch ( data_type() ) {
  case kVpiVarInteger:
    return kVpiValueInteger;

  case kVpiVarReal:
  case kVpiVarRealtime:
    return kVpiValueReal;
    
  case kVpiVarTime:
    return kVpiValueTime;

  case kVpiVarNone:
    // 型の指定はなかった．
    break;
  }
  
  if ( left_range() && right_range() ) {
    if ( is_signed() ) {
      // case D
      return pack(kVpiValueSS, bit_size());
    }
    else {
      // case B
      return pack(kVpiValueUS, bit_size());
    }
  }
  else {
    // ここまで来たら基本的に右辺の型を用いる．
    tVpiValueType rtype = init_value()->value_type();
    if ( is_signed() ) {
      // case C, E
      return merge(rtype, kVpiValueSigned);
    }
    if ( unpack_size(rtype) == 0 ) {
      // 右辺もサイズがなかった．
      return pack(rtype, kVpiSizeInteger);
    }
    // case A
    return rtype;
  }
  
  return kVpiValueNone;
}
  
// @brief 定数値を持つ型のときに true を返す．
bool
EiParam::is_consttype() const
{
  return true;
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiParam::init_value() const
{
  return mInit;
}
  
// @brief 初期値の設定
// @param[in] expr 初期値
void
EiParam::set_init(ElbExpr* expr)
{
  mInit = expr;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiParam::get_scalar() const
{
  // どんな型でもこれでいいはず．
  return init_value()->eval_scalar();
}
      
// @brief スカラー値を設定する．
void
EiParam::set_scalar(tVpiScalarVal val)
{
  assert_not_reached(__FILE__, __LINE__);
}
  
// @brief 論理値を返す．
tVpiScalarVal
EiParam::get_logic() const
{
  tVpiValueType req_type = value_type();
  if ( req_type == kVpiValueRealType ) {
    return conv_from_real(init_value()->eval_real());
  }
  else if ( req_type == kVpiValueInteger ) {
    // 一旦 int にしてから logic に変換する．
    int tmp;
    if ( init_value()->eval_int(tmp) ) {
      return conv_from_int(tmp & 1);
    }
    return kVpiScalarX;
  }
  else {
    // 残りは bitvector 型
    BitVector tmp;
    init_value()->eval_bitvector(tmp, req_type);
    return tmp.to_logic();
  }
}

// @brief real 型の値を返す．
double
EiParam::get_real() const
{
  tVpiValueType req_type = value_type();
  if ( req_type == kVpiValueRealType ) {
    return init_value()->eval_real();
  }
  else if ( req_type == kVpiValueInteger ) {
    // 一旦 int にしてから double に変換する．
    int tmp;
    if ( init_value()->eval_int(tmp) ) {
      return static_cast<double>(tmp);
    }
    // 不定値は表わし様がないので 0.0 を返す．
#warning "TODO: 正しい処理かどうか調べる．"
    return 0.0;
  }
  else {
    // 残りは bitvector 型
    BitVector tmp;
    init_value()->eval_bitvector(tmp, req_type);
    return tmp.to_real();
  }
}
  
// @brief real 型の値を設定する．
void
EiParam::set_real(double val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief bitvector 型の値を返す．
void
EiParam::get_bitvector(BitVector& bitvector,
		       tVpiValueType req_type) const
{
  tVpiValueType req_type1 = value_type();
  if ( req_type1 == kVpiValueRealType ) {
    // 一旦 double で評価してから bitvector に変換する．
    double tmp = init_value()->eval_real();
    bitvector = tmp;
  }
  else if ( req_type1 == kVpiValueInteger ) {
    // 一旦 int にしてから bitvector に変換する．
    int tmp;
    if ( init_value()->eval_int(tmp) ) {
      bitvector = tmp;
    }
    else {
      bitvector = kVpiScalarX;
    }
  }
  else {
    // 残りは bitvector 型
    init_value()->eval_bitvector(bitvector, req_type1);
  }
  bitvector.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiParam::set_bitvector(const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}
  
// @brief ビット選択値を返す．
// @param[in] index ビット位置
tVpiScalarVal
EiParam::get_bitselect(int index) const
{
  int bpos = bit_offset(index);
  if ( bpos >= 0 ) {
    BitVector tmp;
    init_value()->eval_bitvector(tmp);
    return tmp.bit_select(bpos);
  }
  else {
    return kVpiScalarX;
  }
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiParam::set_bitselect(int index,
		       tVpiScalarVal val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiParam::get_partselect(int left,
			int right,
			BitVector& val) const
{
  int bpos1 = bit_offset(left);
  int bpos2 = bit_offset(right);
  if ( bpos1 >= 0 && bpos2 >= 0 ) {
    BitVector tmp;
    init_value()->eval_bitvector(tmp);
    val = tmp.part_select(bpos1, bpos2);
  }
  else {
    int w;
    if ( left > right ) {
      w = left - right + 1;
    }
    else {
      w = right - left + 1;
    }
    val = BitVector(kVpiScalarX, w);
  }
} 

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiParam::set_partselect(int left,
			int right,
			const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiLocalParam
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiLocalParam::EiLocalParam(ElbDeclHead* head,
			   const PtNamedBase* pt_item,
			   ElbExpr* init) :
  EiParam(head, pt_item, init)
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
