
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

#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] index1, index2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbExpr* parent_expr,
			  const PtExpr* index1,
			  const PtExpr* index2,
			  int index1_val,
			  int index2_val)
{
  void* p = mAlloc.get_memory(sizeof(EiConstPartSelect));
  ElbExpr* expr = new (p) EiConstPartSelect(pt_expr, parent_expr,
					    index1, index2,
					    index1_val, index2_val);

  return expr;
}

// @brief 固定部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] index1, inde2 パート選択式
ElbExpr*
EiFactory::new_PartSelect(const PtBase* pt_expr,
			  ElbExpr* parent_expr,
			  int index1,
			  int index2)
{
  void* p = mAlloc.get_memory(sizeof(EiConstPartSelect));
  ElbExpr* expr = new (p) EiConstPartSelect(pt_expr, parent_expr,
					    NULL, NULL,
					    index1, index2);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_PlusPartSelect(const PtBase* pt_expr,
			      ElbExpr* parent_expr,
			      ElbExpr* base,
			      const PtExpr* range,
			      int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiPlusPartSelect));
  ElbExpr* expr = new (p) EiPlusPartSelect(pt_expr, parent_expr,
					   base, range, range_val);

  return expr;
}

// @brief 可変部分選択式を生成する．
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
ElbExpr*
EiFactory::new_MinusPartSelect(const PtBase* pt_expr,
			       ElbExpr* parent_expr,
			       ElbExpr* base,
			       const PtExpr* range,
			       int range_val)
{
  void* p = mAlloc.get_memory(sizeof(EiMinusPartSelect));
  ElbExpr* expr = new (p) EiMinusPartSelect(pt_expr, parent_expr,
					    base, range, range_val);

  return expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 対象の式
EiPartSelect::EiPartSelect(const PtBase* pt_expr,
			   ElbExpr* parent_expr) :
  EiExprBase1(pt_expr),
  mParentExpr(parent_expr)
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

// @brief 定数の時 true を返す．
// @note 参照している要素の型によって決まる．
bool
EiPartSelect::is_const() const
{
  return parent_expr()->is_const() && is_constant_select();
}

// @brief 範囲指定の時に true を返す．
bool
EiPartSelect::is_partselect() const
{
  return true;
}

// @brief 親の式を返す．
// @note 式に対するビット選択/範囲選択の時，意味を持つ．
const VlExpr*
EiPartSelect::parent_expr() const
{
  return mParentExpr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiConstPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 対象の式
// @param[in] index1, index2 パート選択式
// @param[in] index1_val, index2_val パート選択式の値
EiConstPartSelect::EiConstPartSelect(const PtBase* pt_expr,
				     ElbExpr* parent_expr,
				     const PtExpr* index1,
				     const PtExpr* index2,
				     int index1_val,
				     int index2_val) :
  EiPartSelect(pt_expr, parent_expr),
  mLeftRange(index1),
  mRightRange(index2),
  mLeftVal(index1_val),
  mRightVal(index2_val)
{
}

// @brief デストラクタ
EiConstPartSelect::~EiConstPartSelect()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiConstPartSelect::value_type() const
{
  int w = 0;
  if ( mLeftVal > mRightVal ) {
    w = mLeftVal - mRightVal + 1;
  }
  else {
    w = mRightVal - mLeftVal + 1;
  }
  return pack(kVpiValueUS, w);
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiConstPartSelect::is_constant_select() const
{
  return true;
}

// @brief 範囲指定のモードを返す．
tVpiRangeMode
EiConstPartSelect::range_mode() const
{
  return kVpiConstRange;
}

// @brief 範囲の MSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiConstPartSelect::left_range() const
{
  return NULL;
}

// @brief 範囲の LSB の式を返す．
// @note 通常の範囲選択の時，意味を持つ．
const VlExpr*
EiConstPartSelect::right_range() const
{
  return NULL;
}

// @brief 範囲の MSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiConstPartSelect::left_range_val() const
{
  return mLeftVal;
}

// @brief 範囲の LSB の値を返す．
// @note 式に対する範囲選択の時，意味を持つ．
int
EiConstPartSelect::right_range_val() const
{
  return mRightVal;
}

#if 0
// @brief スカラー値を返す．
tVpiScalarVal
EiConstPartSelect::eval_scalar() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.bit_select(0);
}

// @brief 論理値を返す．
tVpiScalarVal
EiConstPartSelect::eval_logic() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_logic();
}

// @brief real 型の値を返す．
double
EiConstPartSelect::eval_real() const
{
  BitVector tmp;
  eval_bitvector(tmp);
  return tmp.to_real();
}
#endif

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiConstPartSelect::decompile_impl(int ppri) const
{
  string ans = parent_expr()->decompile();
  string left_str;
  string right_str;
  if ( mLeftRange && mRightRange ) {
    left_str = mLeftRange->decompile();
    right_str = mRightRange->decompile();
  }
  else {
    ostringstream buf;
    buf << mLeftVal;
    left_str = buf.str();
    buf.clear();
    buf << mRightVal;
    right_str = buf.str();
  }
  ans += "[" + left_str + ":" + right_str + "]";

  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiConstPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiVarPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiVarPartSelect::EiVarPartSelect(const PtBase* pt_expr,
				 ElbExpr* parent_expr,
				 ElbExpr* base,
				 const PtExpr* range,
				 int range_val) :
  EiPartSelect(pt_expr, parent_expr),
  mBaseExpr(base),
  mRangeExpr(range),
  mRangeVal(range_val)
{
}

// @brief デストラクタ
EiVarPartSelect::~EiVarPartSelect()
{
}

// @brief 式のタイプを返す．
tVpiValueType
EiVarPartSelect::value_type() const
{
  return pack(kVpiValueUS, mRangeVal);
}

// @brief 固定選択子の時 true を返す．
// @note ビット選択，部分選択の時，意味を持つ．
bool
EiVarPartSelect::is_constant_select() const
{
  return mBaseExpr->is_const();
}

// @brief 範囲のベースを表す式を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では NULL を返す．
const VlExpr*
EiVarPartSelect::base() const
{
  return mBaseExpr;
}

// @brief 範囲のビット幅を返す．
// @note 可変範囲選択の時，意味を持つ．
// @note それ以外では 0 を返す．
int
EiVarPartSelect::range_width() const
{
  return mRangeVal;
}

#if 0
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
#endif

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiVarPartSelect::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}


//////////////////////////////////////////////////////////////////////
// クラス EiPlusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiPlusPartSelect::EiPlusPartSelect(const PtBase* pt_expr,
				   ElbExpr* parent_expr,
				   ElbExpr* base,
				   const PtExpr* range,
				   int range_val) :
  EiVarPartSelect(pt_expr, parent_expr, base, range, range_val)
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

// @brief decompile() の実装関数
// @param[in] ppri 親の演算子の優先順位
string
EiPlusPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << parent_expr()->decompile() << "[" << base()->decompile()
      << "+:" << range_width() << "]";
  return buf.str();
}


//////////////////////////////////////////////////////////////////////
// クラス EiMinusPartSelect
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_expr 本体の式
// @param[in] base 範囲のベースアドレスを表す式
// @param[in] range 範囲を表す式
// @param[in] range_val 範囲の値
EiMinusPartSelect::EiMinusPartSelect(const PtBase* pt_expr,
				     ElbExpr* parent_expr,
				     ElbExpr* base,
				     const PtExpr* range,
				     int range_val) :
  EiVarPartSelect(pt_expr, parent_expr, base, range, range_val)
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

// @brief decompile() の実装関数
// @param[in] ppri 親の演算子の優先順位
string
EiMinusPartSelect::decompile_impl(int ppri) const
{
  ostringstream buf;
  buf << parent_expr()->decompile() << "[" << base()->decompile()
      << "-:" << range_width() << "]";
  return buf.str();
}

END_NAMESPACE_YM_VERILOG
