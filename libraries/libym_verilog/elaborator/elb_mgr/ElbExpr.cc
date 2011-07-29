
/// @file libym_verilog/elb/ElbExpr.cc
/// @brief ElbExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbExpr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ElbExpr::ElbExpr()
{
}

// @brief デストラクタ
ElbExpr::~ElbExpr()
{
}

// @brief 要求された値のタイプを返す．
VlValueType
ElbExpr::req_type() const
{
  if ( mReqType.is_no_type() ) {
    return value_type();
  }
  else {
    return mReqType;
  }
}

// @brief 式のビット幅を返す．
ymuint
ElbExpr::bit_size() const
{
  return value_type().size();
}

// 二項演算のタイプとサイズを決める．
VlValueType
ElbExpr::calc_type(const VlValueType& type0,
		   const VlValueType& type1)
{
  // どちらか一方でも real なら答は real
  if ( type0.is_real_type() || type1.is_real_type() ) {
    return VlValueType::real_type();
  }

  bool is_sized = false;
  bool is_signed = false;

  // 両方が signed の場合にのみ signed
  if ( type0.is_signed() && type1.is_signed() ) {
    is_signed = true;
  }

  // どちらか一方が sized なら結果も sized
  ymuint ans_size = 0;
  if ( type0.is_sized() ) {
    is_sized = true;
    ans_size = type0.size();
  }
  if ( type1.is_sized() ) {
    is_sized = true;
    if ( ans_size < type1.size() ) {
      ans_size = type1.size();
    }
  }

  return VlValueType(is_signed, is_sized, ans_size);
}

// 巾乗演算のタイプとサイズを決める．
VlValueType
ElbExpr::calc_type2(const VlValueType& type0,
		    const VlValueType& type1)
{
  // 両方が unsigned でなければ結果は real
  // どちらかが signed なら結果は real
  if ( type0.is_signed() || type1.is_signed() ) {
    return VlValueType::real_type();
  }

  // どちらか一方が sized なら結果も sized
  bool is_sized = false;
  ymuint ans_size = 0;
  if ( type0.is_sized() ) {
    is_sized = true;
    ans_size = type0.size();
  }
  if ( type1.is_sized() ) {
    is_sized = true;
    if ( ans_size < type1.size() ) {
      ans_size = type1.size();
    }
  }

  return VlValueType(false, is_sized, ans_size);
}

// 出力に要求されているサイズから自分のサイズを決める．
VlValueType
ElbExpr::update_size(const VlValueType& type,
		     const VlValueType& req_type)
{
  if ( req_type.is_real_type() ) {
    // real は伝播しない．
    return type;
  }
  if ( type.is_real_type() ) {
    // 自分が real でも伝播しない．
    return type;
  }

  ymuint req_size = req_type.size();
  ymuint size = type.size();
  if ( req_size <= size ) {
    return type;
  }
  return VlValueType(type.is_signed(), type.is_sized(), req_size);
}

END_NAMESPACE_YM_VERILOG
