
/// @file libym_verilog/elb/ElbExpr.cc
/// @brief ElbExpr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbExpr.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbExpr.h"

#include "ym_verilog/BitVector.h"
#include "ym_verilog/VlTime.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ElbExpr
//////////////////////////////////////////////////////////////////////

// @brief Verilog-HDL の文字列を得る．
string
ElbExpr::decompile() const
{
  return decompile_impl(0);
}

// @brief オペランドを返す．
// @param[in] pos 位置番号
// @note 演算子の時，意味を持つ．
// @note このクラスでは NULL を返す．
ElbExpr*
ElbExpr::_operand(ymuint pos) const
{
  return NULL;
}

// 二項演算のタイプとサイズを決める．
tVpiValueType
ElbExpr::calc_type(tVpiValueType type0,
		   tVpiValueType type1)
{
  // どちらか一方でも real なら答は real
  if ( type0 == kVpiValueReal || type1 == kVpiValueReal ) {
    return kVpiValueReal;
  }

  bool is_sized = false;
  bool is_signed = false;

  // 両方が signed の場合にのみ signed
  if ( is_signed_type(type0) && is_signed_type(type1) ) {
    is_signed = true;
  }

  // どちらか一方が sized なら結果も sized
  if ( is_sized_type(type0) || is_sized_type(type1) ) {
    is_sized = true;
  }

  if ( is_sized ) {
    ymuint size0 = unpack_size(type0);
    ymuint size1 = unpack_size(type1);
    ymuint ans_size = ( size0 > size1 ) ? size0 : size1;
    if ( is_signed ) {
      return pack(kVpiValueSS, ans_size);
    }
    else {
      return pack(kVpiValueUS, ans_size);
    }
  }
  else {
    if ( is_signed ) {
      return pack(kVpiValueSU, 0);
    }
    else {
      return pack(kVpiValueUU, 0);
    }
  }
}

// 巾乗演算のタイプとサイズを決める．
tVpiValueType
ElbExpr::calc_type2(tVpiValueType type0,
		    tVpiValueType type1)
{
  // 両方が unsigned でなければ結果は real
  if ( is_signed_type(type0) || is_signed_type(type1) ) {
    return kVpiValueReal;
  }

  // どちらか一方が sized なら結果も sized
  if ( is_sized_type(type0) || is_sized_type(type1) ) {
    ymuint size0 = unpack_size(type0);
    ymuint size1 = unpack_size(type1);
    ymuint ans_size = ( size0 > size1 ) ? size0 : size1;
    return pack(kVpiValueUS, ans_size);
  }
  else {
    return pack(kVpiValueUU, 0);
  }
}

// 出力に要求されているサイズから自分のサイズを決める．
tVpiValueType
ElbExpr::update_size(tVpiValueType type,
		     tVpiValueType req_type)
{
  if ( req_type == kVpiValueReal ) {
    // real は伝播しない．
    return type;
  }
  if ( type == kVpiValueReal ) {
    // 自分が real でも伝播しない．
    return type;
  }
  ymuint req_size = unpack_size(req_type);
  ymuint size = unpack_size(type);
  if ( req_size <= size ) {
    return type;
  }
  return pack(unpack_type(type), req_size);
}

END_NAMESPACE_YM_VERILOG
