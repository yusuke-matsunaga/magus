
/// @file libym_verilog/elaborator/ei/EiGenvar.cc
/// @brief EiGenvar の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiGenvar.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiGenvar.h"
#include "ym_verilog/pt/PtDecl.h"
#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief genvar を生成する．
// @param[in] parent 親のスコープ環境
// @param[in] pt_item 対応するパース木の要素
// @param[in] val 初期値
ElbGenvar*
EiFactory::new_Genvar(const VlNamedObj* parent,
		      const PtDeclItem* pt_item,
		      int val)
{
  void* p = mAlloc.get_memory(sizeof(EiGenvar));
  EiGenvar* genvar = new (p) EiGenvar(parent, pt_item, val);

  return genvar;
}


//////////////////////////////////////////////////////////////////////
// genvar を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義要素
// @param[in] name 名前
// @param[in] val 初期値
EiGenvar::EiGenvar(const VlNamedObj* parent,
		   const PtDeclItem* pt_item,
		   int val) :
  mParent(parent),
  mPtItem(pt_item),
  mInUse(false),
  mValue(val)
{
}

// デストラクタ
EiGenvar::~EiGenvar()
{
}

// @brief 型の取得
tVpiObjType
EiGenvar::type() const
{
  return kVpiConstant;
}

// @brief ファイル位置の取得
FileRegion
EiGenvar::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiGenvar::parent() const
{
  return mParent;
}

// @brief 名前の取得
const char*
EiGenvar::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
tVpiValueType
EiGenvar::value_type() const
{
  return pack(kVpiValueSS, kVpiSizeInteger);
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiGenvar::is_consttype() const
{
  return true;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiGenvar::get_scalar() const
{
  return conv_to_scalar(mValue);
}

// @brief スカラー値を設定する．
void
EiGenvar::set_scalar(tVpiScalarVal val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 論理値を返す．
tVpiScalarVal
EiGenvar::get_logic() const
{
#warning "TODO: 仕様確認"
  // int なら 0 かどうか
  // ビットベクタなら LSB
  return conv_to_scalar(mValue & 1);
}

// @brief real 型の値を返す．
double
EiGenvar::get_real() const
{
  return static_cast<double>(mValue);
}

// @brief real 型の値を設定する．
void
EiGenvar::set_real(double val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief bitvector 型の値を返す．
void
EiGenvar::get_bitvector(BitVector& bitvector,
			tVpiValueType req_type) const
{
  bitvector = BitVector(mValue);
  bitvector.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiGenvar::set_bitvector(const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
tVpiScalarVal
EiGenvar::get_bitselect(int index) const
{
  if ( index >= 0 && index < static_cast<int>(kVpiSizeInteger) ) {
    return conv_to_scalar((mValue >> index) & 1U);
  }
  else {
    return kVpiScalarX;
  }
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiGenvar::set_bitselect(int index,
			tVpiScalarVal val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiGenvar::get_partselect(int left,
			 int right,
			 BitVector& val) const
{
  if ( left >= 0 && left < static_cast<int>(kVpiSizeInteger) &&
       right >= 0 && right < static_cast<int>(kVpiSizeInteger) ) {
    BitVector tmp(mValue);
    val = tmp.part_select(left, right);
  }
  else {
    ymuint32 w = 0;
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
EiGenvar::set_partselect(int left,
			 int right,
			 const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// 使用中の時 true を返す．
bool
EiGenvar::is_inuse() const
{
  return mInUse;
}

// 使用中にする．
void
EiGenvar::set_inuse()
{
  mInUse = true;
}

// 使用を終える．
void
EiGenvar::reset_inuse()
{
  mInUse = false;
}

// 現在の値を返す．
int
EiGenvar::value() const
{
  return mValue;
}

// 値を設定する．
void
EiGenvar::set_value(int value)
{
  mValue = value;
}

// @brief 元となったパース木の定義要素を返す．
const PtDeclItem*
EiGenvar::pt_item() const
{
  return mPtItem;
}

END_NAMESPACE_YM_VERILOG
