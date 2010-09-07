
/// @file libym_verilog/elb_impl/EiVarPartSelect.cc
/// @brief EiVarPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiVarPartSelect.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiVarPartSelect.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_PlusPartSelect(const PtBase* pt_expr,
			      ElbDecl* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiPlusPartSelect));
  EiVarPartSelect* expr = new (p) EiPlusPartSelect(pt_expr, obj,
						   base, range, range_val);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_MinusPartSelect(const PtBase* pt_expr,
			      ElbDecl* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiMinusPartSelect));
  EiVarPartSelect* expr = new (p) EiMinusPartSelect(pt_expr, obj,
						    base, range, range_val);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_PlusPartSelect(const PtBase* pt_expr,
			      ElbParameter* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiParamPlusPartSelect));
  EiParamVarPartSelect* expr = new (p) EiParamPlusPartSelect(pt_expr, obj,
							     base, range,
							     range_val);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_MinusPartSelect(const PtBase* pt_expr,
			      ElbParameter* obj,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiParamMinusPartSelect));
  EiParamVarPartSelect* expr = new (p) EiParamMinusPartSelect(pt_expr, obj,
							     base, range,
							     range_val);

  return expr;
}

// @brief 配列要素の可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_PlusPartSelect(const PtBase* pt_expr,
			      ElbDeclArray* obj,
			      const vector<ElbExpr*>& index_list,
			      ElbExpr* base,
			      ElbExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiArrayElemPlusPartSelect));
  EiArrayElemVarPartSelect* expr = new (p) EiArrayElemPlusPartSelect(pt_expr, obj,
								     index_list,
								     base, range,
								     range_val);

  return expr;
}

// @brief 配列要素の可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_MinusPartSelect(const PtBase* pt_expr,
			       ElbDeclArray* obj,
			       const vector<ElbExpr*>& index_list,
			       ElbExpr* base,
			       ElbExpr* range,
			       int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiArrayElemMinusPartSelect));
  EiArrayElemVarPartSelect* expr = new (p) EiArrayElemMinusPartSelect(pt_expr, obj,
								     index_list,
								     base, range,
								     range_val);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiVarPartSelect::EiVarPartSelect(const PtBase* pt_expr,
				 ElbDecl* obj,
				 ElbExpr* base,
				 ElbExpr* range,
				 int range_val) :
  EiExprBase1(pt_expr),
  mObj(obj),
  mBase(base),
  mRange(range),
  mRangeVal(range_val)
{
}

// @brief デストラクタ
EiVarPartSelect::~EiVarPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiVarPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiVarPartSelect::value_type() const
{
  return pack(kVpiValueUS, mRangeVal);
}

// @brief 定数の時 true を返す．
bool
EiVarPartSelect::is_const() const
{
  return mObj->is_consttype() && is_constant_select();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiVarPartSelect::is_constant_select() const
{
  return mBase->is_const();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiVarPartSelect::decl_obj() const
{
  return mObj;
}

// @brief 範囲のベースアドレスの式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiVarPartSelect::range_base() const
{
  return mBase;
}

// @brief 範囲のビット幅を表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiVarPartSelect::range_expr() const
{
  return mRange;
}

// @brief 範囲のベースアドレスの値を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiVarPartSelect::range_base_val() const
{
  int ans;
  if ( mBase->eval_int(ans) ) {
    return ans;
  }
  return 0;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiVarPartSelect::range_val() const
{
  return mRangeVal;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiVarPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiVarPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiVarPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiVarPartSelect::eval_bitvector(BitVector& bitvector,
				tVpiValueType req_type) const
{
  int lsb = range_base_val();
  int msb = lsb + range_val() - 1;
  mObj->get_partselect(msb, lsb, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] ppri 親の演算子の優先順位
string
EiVarPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << mObj->name() << "[" << mBase->decompile();
  if ( range_mode() == kVpiPlusRange ) {
    buf << "+: " << mRangeVal;
  }
  else {
    buf << "-: " << - mRangeVal;
  }
  buf << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiVarPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiVarPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_cond( mRangeVal == 1, __FILE__, __LINE__);
  mObj->set_bitselect(range_base_val(), v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiVarPartSelect::set_bitvector(const BitVector& v)
{
  int lsb = range_base_val();
  int msb = lsb + range_val() - 1;
  mObj->set_partselect(msb, lsb, v);
}  


//////////////////////////////////////////////////////////////////////
// クラス EiPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiPlusPartSelect::EiPlusPartSelect(const PtBase* pt_expr,
				   ElbDecl* obj,
				   ElbExpr* base,
				   ElbExpr* range,
				   int range_val) :
  EiVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiPlusPartSelect::~EiPlusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiPlusPartSelect::range_mode() const
{
  return kVpiPlusRange;
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiMinusPartSelect::EiMinusPartSelect(const PtBase* pt_expr,
				     ElbDecl* obj,
				     ElbExpr* base,
				     ElbExpr* range,
				     int range_val) :
  EiVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiMinusPartSelect::~EiMinusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiMinusPartSelect::range_mode() const
{
  return kVpiMinusRange;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiParamVarPartSelect::EiParamVarPartSelect(const PtBase* pt_expr,
					   ElbParameter* obj,
					   ElbExpr* base,
					   ElbExpr* range,
					   int range_val) :
  EiExprBase1(pt_expr),
  mObj(obj),
  mBase(base),
  mRange(range),
  mRangeVal(range_val)
{
}

// @brief デストラクタ
EiParamVarPartSelect::~EiParamVarPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiParamVarPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiParamVarPartSelect::value_type() const
{
  return pack(kVpiValueUS, mRangeVal);
}

// @brief 定数の時 true を返す．
bool
EiParamVarPartSelect::is_const() const
{
  return is_constant_select();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiParamVarPartSelect::is_constant_select() const
{
  return mBase->is_const();
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiParamVarPartSelect::decl_obj() const
{
  return mObj;
}

// @brief 範囲のベースアドレスの式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiParamVarPartSelect::range_base() const
{
  return mBase;
}

// @brief 範囲のビット幅を表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiParamVarPartSelect::range_expr() const
{
  return mRange;
}

// @brief 範囲のベースアドレスの値を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiParamVarPartSelect::range_base_val() const
{
  int ans;
  if ( mBase->eval_int(ans) ) {
    return ans;
  }
  return 0;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiParamVarPartSelect::range_val() const
{
  return mRangeVal;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiParamVarPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiParamVarPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiParamVarPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiParamVarPartSelect::eval_bitvector(BitVector& bitvector,
				     tVpiValueType req_type) const
{
  BitVector bv;
  mObj->eval_bitvector(bv);
  int lsb = range_base_val();
  int msb = lsb + range_val() - 1;
  bv.part_select(msb, lsb, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiParamVarPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << mObj->name() << "[" << mBase->decompile();
  if ( range_mode() == kVpiPlusRange ) {
    buf << "+: " << mRangeVal;
  }
  else {
    buf << "-: " << - mRangeVal;
  }
  buf << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiParamVarPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiParamVarPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiParamVarPartSelect::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiParamPlusPartSelect::EiParamPlusPartSelect(const PtBase* pt_expr,
					     ElbParameter* obj,
					     ElbExpr* base,
					     ElbExpr* range,
					     int range_val) :
  EiParamVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiParamPlusPartSelect::~EiParamPlusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiParamPlusPartSelect::range_mode() const
{
  return kVpiPlusRange;
}


//////////////////////////////////////////////////////////////////////
// クラス EiParamMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiParamMinusPartSelect::EiParamMinusPartSelect(const PtBase* pt_expr,
					       ElbParameter* obj,
					       ElbExpr* base,
					       ElbExpr* range,
					       int range_val) :
  EiParamVarPartSelect(pt_expr, obj, base, range, range_val)
{
}

// @brief デストラクタ
EiParamMinusPartSelect::~EiParamMinusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiParamMinusPartSelect::range_mode() const
{
  return kVpiMinusRange;
}


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiArrayElemVarPartSelect::EiArrayElemVarPartSelect(const PtBase* pt_expr,
						   ElbDeclArray* obj,
						   const vector<ElbExpr*>& index_list,
						   ElbExpr* base,
						   ElbExpr* range,
						   int range_val) :
  EiArrayElemPrimary(pt_expr, obj, index_list),
  mBase(base),
  mRange(range),
  mRangeVal(range_val)
{
}

// @brief デストラクタ
EiArrayElemVarPartSelect::~EiArrayElemVarPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiArrayElemVarPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiArrayElemVarPartSelect::value_type() const
{
  return pack(kVpiValueUS, mRangeVal);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiArrayElemVarPartSelect::is_const() const
{
  return decl_array()->is_consttype() && is_constant_select();
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiArrayElemVarPartSelect::is_constant_select() const
{
  return mBase->is_const();
}

// @brief 範囲のベースアドレスの式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiArrayElemVarPartSelect::range_base() const
{
  return mBase;
}

// @brief 範囲のビット幅を表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiArrayElemVarPartSelect::range_expr() const
{
  return mRange;
}

// @brief 範囲のベースアドレスの値を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiArrayElemVarPartSelect::range_base_val() const
{
  int ans;
  if ( mBase->eval_int(ans) ) {
    return ans;
  }
  return 0;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiArrayElemVarPartSelect::range_val() const
{
  return mRangeVal;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiArrayElemVarPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiArrayElemVarPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiArrayElemVarPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiArrayElemVarPartSelect::eval_bitvector(BitVector& bitvector,
					 tVpiValueType req_type) const
{
  if ( eval_index() ) {
    int lsb = range_base_val();
    int msb = lsb + mRangeVal - 1;
    decl_array()->get_partselect(mIndexValList, msb, lsb, bitvector);
  }
  else {
    bitvector = kVpiScalarX;
  }
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiArrayElemVarPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << decl_array()->name() << decompile_index()
      << "[" << mBase->decompile();
  if ( mRangeVal > 0 ) {
    buf << "+: " << mRangeVal;
  }
  else {
    buf << "-: " << - mRangeVal;
  }
  buf << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiArrayElemVarPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiArrayElemVarPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_cond(mRangeVal == 1, __FILE__, __LINE__);
  if ( eval_index() ) {
    decl_array()->set_bitselect(mIndexValList, range_base_val(), v);
  }
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiArrayElemVarPartSelect::set_bitvector(const BitVector& v)
{
  if ( eval_index() ) {
    int lsb = range_base_val();
    int msb = lsb + mRangeVal - 1;
    decl_array()->set_partselect(mIndexValList, msb, lsb, v);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiArrayElemPlusPartSelect::EiArrayElemPlusPartSelect(const PtBase* pt_expr,
						     ElbDeclArray* obj,
						     const vector<ElbExpr*>& index_list,
						     ElbExpr* base,
						     ElbExpr* range,
						     int range_val) :
  EiArrayElemVarPartSelect(pt_expr, obj, index_list, base, range, range_val)
{
}

// @brief デストラクタ
EiArrayElemPlusPartSelect::~EiArrayElemPlusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiArrayElemPlusPartSelect::range_mode() const
{
  return kVpiPlusRange;
}


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiArrayElemMinusPartSelect::EiArrayElemMinusPartSelect(const PtBase* pt_expr,
						       ElbDeclArray* obj,
						       const vector<ElbExpr*>& index_list,
						       ElbExpr* base,
						       ElbExpr* range,
						       int range_val) :
  EiArrayElemVarPartSelect(pt_expr, obj, index_list, base, range, range_val)
{
}

// @brief デストラクタ
EiArrayElemMinusPartSelect::~EiArrayElemMinusPartSelect()
{
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiArrayElemMinusPartSelect::range_mode() const
{
  return kVpiMinusRange;
}


END_NAMESPACE_YM_VERILOG
