
/// @file libym_verilog/elb_impl/EiPartSelect.cc
/// @brief EiPartSelect の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPartSelect.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiPartSelect.h"
#include "ElbDecl.h"
#include "ElbParameter.h"
#include "ElbPrimitive.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbDecl* obj,
			  ElbExpr* index1,
			  ElbExpr* index2,
			  int index1_val,
			  int index2_val)
{
  void* p = mAlloc.get_memory(sizeof(EiPartSelect));
  EiPartSelect* expr = new (p) EiPartSelect(pt_expr, obj,
					    index1, index2,
					    index1_val, index2_val);

  return expr;
}

// @brief 部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbParameter* obj,
			  ElbExpr* index1,
			  ElbExpr* index2,
			  int index1_val,
			  int index2_val)
{
  void* p = mAlloc.get_memory(sizeof(EiParamPartSelect));
  EiParamPartSelect* expr = new (p) EiParamPartSelect(pt_expr, obj,
						      index1, index2,
						      index1_val, index2_val);

  return expr;
}

// @brief 配列要素の部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] index1, index2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbDeclArray* obj,
			  const vector<ElbExpr*>& index_list,
			  ElbExpr* index1,
			  ElbExpr* index2,
			  int index1_val,
			  int index2_val)
{
  void* p = mAlloc.get_memory(sizeof(EiArrayElemPartSelect));
  EiArrayElemPartSelect* expr = new (p) EiArrayElemPartSelect(pt_expr, obj,
							      index_list,
							      index1, index2,
							      index1_val,
							      index2_val);

  return expr;
}

// @brief 部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] index1, inde2 パート選択式
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbExpr* expr,
			  int index1,
			  int index2)
{
  void* p = mAlloc.get_memory(sizeof(EiExprPartSelect));
  EiExprPartSelect* expr1 = new (p) EiExprPartSelect(pt_expr, expr,
						     index1, index2);

  return expr1;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
EiPartSelect::EiPartSelect(const PtBase* pt_expr,
			   ElbDecl* obj,
			   ElbExpr* index1,
			   ElbExpr* index2,
			   int index1_val,
			   int index2_val) :
  EiExprBase1(pt_expr),
  mObj(obj),
  mIndex1(index1),
  mIndex2(index2),
  mIndex1Val(index1_val),
  mIndex2Val(index2_val)
{
}

// @brief デストラクタ
EiPartSelect::~EiPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1Val > mIndex2Val ) {
    w = mIndex1Val - mIndex2Val + 1;
  }
  else {
    w = mIndex2Val - mIndex1Val + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPartSelect::is_const() const
{
  return mObj->is_consttype();
}

// @brief 範囲指定の時に true を返す．
bool
EiPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiPartSelect::decl_obj() const
{
  return mObj;
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiPartSelect::is_constant_select() const
{
  // mIndex2 は常に定数のはず
  return mIndex1->is_const();
}

// @brief 範囲の MSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiPartSelect::left_range() const
{
  return mIndex1;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiPartSelect::right_range() const
{
  return mIndex2;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiPartSelect::left_range_val() const
{
  int ans;
  if ( !mIndex1->eval_int(ans) ) {
    return 0;
  }
  return ans;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiPartSelect::right_range_val() const
{
  int ans;
  if ( !mIndex2->eval_int(ans) ) {
    return 0;
  }
  return ans;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiPartSelect::eval_bitvector(BitVector& bitvector,
			     tVpiValueType req_type) const
{
  mObj->get_partselect(mIndex1Val, mIndex2Val, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiPartSelect::decompile_impl(int ppri) const
{
  string ans = mObj->name();
  ans += "[" + mIndex1->decompile() + ":"
    + mIndex2->decompile() + "]";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_cond(mIndex1Val == mIndex2Val, __FILE__, __LINE__);
  mObj->set_bitselect(mIndex1Val, v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiPartSelect::set_bitvector(const BitVector& v)
{
  mObj->set_partselect(mIndex1Val, mIndex2Val, v);
}  


//////////////////////////////////////////////////////////////////////
// クラス EiParamPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index1, inde2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
EiParamPartSelect::EiParamPartSelect(const PtBase* pt_expr,
			   ElbParameter* obj,
			   ElbExpr* index1,
			   ElbExpr* index2,
			   int index1_val,
			   int index2_val) :
  EiExprBase1(pt_expr),
  mObj(obj),
  mIndex1(index1),
  mIndex2(index2),
  mIndex1Val(index1_val),
  mIndex2Val(index2_val)
{
}

// @brief デストラクタ
EiParamPartSelect::~EiParamPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiParamPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiParamPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1Val > mIndex2Val ) {
    w = mIndex1Val - mIndex2Val + 1;
  }
  else {
    w = mIndex2Val - mIndex1Val + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiParamPartSelect::is_const() const
{
  return true;
}

// @brief 範囲指定の時に true を返す．
bool
EiParamPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiParamPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 宣言要素への参照の場合，対象のオブジェクトを返す．
const VlDecl*
EiParamPartSelect::decl_obj() const
{
  return mObj;
}

// @brief 範囲の MSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiParamPartSelect::left_range() const
{
  return mIndex1;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiParamPartSelect::right_range() const
{
  return mIndex2;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiParamPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiParamPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiParamPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiParamPartSelect::eval_bitvector(BitVector& bitvector,
			     tVpiValueType req_type) const
{
  BitVector bv;
  mObj->eval_bitvector(bv);
  bv.part_select(mIndex1Val, mIndex2Val, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiParamPartSelect::decompile_impl(int ppri) const
{
  string ans = mObj->name();
  ans += "[" + mIndex1->decompile() + ":"
    + mIndex2->decompile() + "]";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiParamPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiParamPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiParamPartSelect::set_bitvector(const BitVector& v)
{
  assert_not_reached(__FILE__, __LINE__);
}  


//////////////////////////////////////////////////////////////////////
// クラス EiArrayElemPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] obj 本体のオブジェクト
// @param[in] index_list インデックスのリスト
// @param[in] index1, index2 範囲選択式
// @param[in] index1_val, index2_val 範囲選択式の値
EiArrayElemPartSelect::EiArrayElemPartSelect(const PtBase* pt_expr,
					     ElbDeclArray* obj,
					     const vector<ElbExpr*>& index_list,
					     ElbExpr* index1,
					     ElbExpr* index2,
					     int index1_val,
					     int index2_val) :
  EiArrayElemPrimary(pt_expr, obj, index_list),
  mIndex1(index1),
  mIndex2(index2),
  mIndex1Val(index1_val),
  mIndex2Val(index2_val)
{
}

// @brief デストラクタ
EiArrayElemPartSelect::~EiArrayElemPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiArrayElemPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiArrayElemPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1Val > mIndex2Val ) {
    w = mIndex1Val - mIndex2Val + 1;
  }
  else {
    w = mIndex2Val - mIndex1Val + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiArrayElemPartSelect::is_const() const
{
  return decl_array()->is_consttype();
}

// @brief 範囲指定の時に true を返す．
bool
EiArrayElemPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiArrayElemPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 範囲の MSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiArrayElemPartSelect::left_range() const
{
  return mIndex1;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiArrayElemPartSelect::right_range() const
{
  return mIndex2;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiArrayElemPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiArrayElemPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiArrayElemPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiArrayElemPartSelect::eval_bitvector(BitVector& bitvector,
				      tVpiValueType req_type) const
{
  if ( eval_index() ) {
    mObj->get_partselect(mIndexValList, mIndex1Val, mIndex2Val, bitvector);
  }
  else {
    bitvector = kVpiScalarX;
  }
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiArrayElemPartSelect::decompile_impl(int ppri) const
{
  string ans = decl_array()->name();
  ans += decompile_index();
  ans += "[" + mIndex1->decompile() + ":"
    + mIndex2->decompile() + "]";
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiArrayElemPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiArrayElemPartSelect::set_scalar(tVpiScalarVal v)
{
  assert_cond(mIndex1Val == mIndex2Val, __FILE__, __LINE__);
  if ( eval_index() ) {
    mObj->set_bitselect(mIndexValList, mIndex1Val, v);
  }
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiArrayElemPartSelect::set_bitvector(const BitVector& v)
{
  if ( eval_index() ) {
    mObj->set_partselect(mIndexValList, mIndex1Val, mIndex2Val, v);
  }
}  


//////////////////////////////////////////////////////////////////////
// クラス EiExprPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] expr 本体の式
// @param[in] index1, index2 パート選択式
EiExprPartSelect::EiExprPartSelect(const PtBase* pt_expr,
				   ElbExpr* expr,
				   int index1,
				   int index2) :
  EiExprBase1(pt_expr),
  mExpr(expr),
  mIndex1(index1),
  mIndex2(index2)
{
}

// @brief デストラクタ
EiExprPartSelect::~EiExprPartSelect()
{
}

// @brief 型の取得
tVpiObjType
EiExprPartSelect::type() const
{
  return kVpiPartSelect;
}

// @brief 式のタイプを返す．
tVpiValueType
EiExprPartSelect::value_type() const
{
  int w = 0;
  if ( mIndex1 > mIndex2 ) {
    w = mIndex1 - mIndex2 + 1;
  }
  else {
    w = mIndex2 - mIndex1 + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiExprPartSelect::is_const() const
{
  return mExpr->is_const();
}

// @brief 範囲指定の時に true を返す．
bool
EiExprPartSelect::is_partselect() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiExprPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
const VlExpr*
EiExprPartSelect::parent_expr() const
{
  return mExpr;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiExprPartSelect::left_range_val() const
{
  return mIndex1;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiExprPartSelect::right_range_val() const
{
  return mIndex2;
}

// @brief スカラー値を返す．
tVpiScalarVal
EiExprPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiExprPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiExprPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}

// @brief bitvector 型の値を返す．
void
EiExprPartSelect::eval_bitvector(BitVector& bitvector,
				 tVpiValueType req_type) const
{
  BitVector tmp;
  mExpr->eval_bitvector(tmp);
  tmp.part_select(mIndex1, mIndex2, bitvector);
  bitvector.coerce(req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiExprPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << "(" << mExpr->decompile() << ")"
      << "[" << mIndex1 << ":" << mIndex2 << "]";
  return buf.str();
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiExprPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

END_NAMESPACE_YM_VERILOG
