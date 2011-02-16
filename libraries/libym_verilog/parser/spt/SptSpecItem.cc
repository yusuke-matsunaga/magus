
/// @file libym_verilog/pt_simple/SptSpecItem.cc
/// @brief SptSpecItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SptSpecItem.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "SptItem.h"
#include "SptFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// specify_block の item を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecItem::SptSpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array) :
  SptItem(file_region, kPtItem_SpecItem),
  mId(id),
  mTerminals(terminal_array)
{
}

// デストラクタ
SptSpecItem::~SptSpecItem()
{
}

// specify block item の種類の取得
tVpiSpecItemType
SptSpecItem::specitem_type() const
{
  return mId;
}

// ターミナルリストの要素数を返す．
ymuint
SptSpecItem::size() const
{
  return mTerminals.size();
}

// @brief ターミナルの取得
// @param[in] pos 位置番号 ( 0 <= pos < size() )
const PtExpr*
SptSpecItem::terminal(ymuint pos) const
{
  return mTerminals[pos];
}


//////////////////////////////////////////////////////////////////////
// specify_block の path_declaration を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptSpecPath::SptSpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 const PtExpr* expr,
			 const PtPathDecl* path_decl) :
  SptItem(file_region, kPtItem_SpecPath),
  mId(id),
  mExpr(expr),
  mPathDecl(path_decl)
{
}

// デストラクタ
SptSpecPath::~SptSpecPath()
{
}

// specify block path の種類の取得
tVpiSpecPathType
SptSpecPath::specpath_type() const
{
  return mId;
}

// モジュールパスの式を返す．
const PtExpr*
SptSpecPath::expr() const
{
  return mExpr;
}

// パス記述を返す．
const PtPathDecl*
SptSpecPath::path_decl() const
{
  return mPathDecl;
}


//////////////////////////////////////////////////////////////////////
// path_decl を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDecl::SptPathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExprArray output_array,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay) :
  mFileRegion(file_region),
  mEdge(edge),
  mInputs(input_array),
  mInputPol(input_pol),
  mOp(op),
  mOutputs(output_array),
  mOutputPol(output_pol),
  mExpr(expr),
  mPathDelay(path_delay)
{
}

// デストラクタ
SptPathDecl::~SptPathDecl()
{
}

// ファイル位置を返す．
FileRegion
SptPathDecl::file_region() const
{
  return mFileRegion;
}

// edge_descriptor を取り出す．
// 0の場合もある．
int
SptPathDecl::edge() const
{
  return mEdge;
}

// @brief 入力リストの要素数の取得
// @return 入力リストの要素数
ymuint
SptPathDecl::input_num() const
{
  return mInputs.size();
}

// @brief 入力の取得
// @param[in] pos 位置番号 ( 0 <= pos < input_num() )
const PtExpr*
SptPathDecl::input(ymuint pos) const
{
  return mInputs[pos];
}

// 入力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::input_pol() const
{
  return mInputPol;
}

// パス記述子(?)を得る．vpiParallel か vpiFull
int
SptPathDecl::op() const
{
  return mOp;
}

// @brief 出力リストの要素数の取得
// @return 出力リストの要素数
ymuint
SptPathDecl::output_num() const
{
  return mOutputs.size();
}

// @brief 出力の取得
// @param[in] pos 位置番号 ( 0 <= pos < output_num() )
const PtExpr*
SptPathDecl::output(ymuint pos) const
{
  return mOutputs[pos];
}

// 出力の極性を取り出す．
// 0の場合もありうる．
int
SptPathDecl::output_pol() const
{
  return mOutputPol;
}

// 式を取り出す．
// NULL の場合もありうる．
const PtExpr*
SptPathDecl::expr() const
{
  return mExpr;
}

// path_delay_value を取り出す．
const PtPathDelay*
SptPathDecl::path_delay() const
{
  return mPathDelay;
}


//////////////////////////////////////////////////////////////////////
// path_delay_value を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value) :
  mFileRegion(file_region)
{
  mValues[0] = value;
  for (ymuint i = 1; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  for (ymuint i = 2; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2,
			   const PtExpr* value3) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  for (ymuint i = 3; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2,
			   const PtExpr* value3,
			   const PtExpr* value4,
			   const PtExpr* value5,
			   const PtExpr* value6) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  mValues[3] = value4;
  mValues[4] = value5;
  mValues[5] = value6;
  for (ymuint i = 6; i < 12; ++ i) {
    mValues[i] = NULL;
  }
}
SptPathDelay::SptPathDelay(const FileRegion& file_region,
			   const PtExpr* value1,
			   const PtExpr* value2,
			   const PtExpr* value3,
			   const PtExpr* value4,
			   const PtExpr* value5,
			   const PtExpr* value6,
			   const PtExpr* value7,
			   const PtExpr* value8,
			   const PtExpr* value9,
			   const PtExpr* value10,
			   const PtExpr* value11,
			   const PtExpr* value12) :
  mFileRegion(file_region)
{
  mValues[0] = value1;
  mValues[1] = value2;
  mValues[2] = value3;
  mValues[3] = value4;
  mValues[4] = value5;
  mValues[5] = value6;
  mValues[6] = value7;
  mValues[7] = value8;
  mValues[8] = value9;
  mValues[9] = value10;
  mValues[10] = value11;
  mValues[11] = value12;
}

// デストラクタ
SptPathDelay::~SptPathDelay()
{
}

// ファイル位置を返す．
FileRegion
SptPathDelay::file_region() const
{
  return mFileRegion;
}

// 値を取り出す．
// 0の場合もある．
const PtExpr*
SptPathDelay::value(ymuint pos) const
{
  return mValues[pos];
}


//////////////////////////////////////////////////////////////////////
// item 関係の生成
//////////////////////////////////////////////////////////////////////

// @brief specify block item の生成
// @param[in] file_region ファイル位置の情報
// @param[in] id specify block item の種類
// @param[in] terminal_list 端子のリスト
// @return 生成された specify block item
const PtItem*
SptFactory::new_SpecItem(const FileRegion& file_region,
			 tVpiSpecItemType id,
			 PtExprArray terminal_array)
{
  void* p = alloc().get_memory(sizeof(SptSpecItem));
  return new (p) SptSpecItem(file_region, id, terminal_array);
}

// @brief path 仕様を生成する．
// @param[in] file_region ファイル位置の情報
// @param[in] id spec path の種類
// @param[in] expr 条件式
// @param[in] path_decl パス記述
// @return 生成された spec path
const PtItem*
SptFactory::new_SpecPath(const FileRegion& file_region,
			 tVpiSpecPathType id,
			 const PtExpr* expr,
			 const PtPathDecl* path_decl)
{
  void* p = alloc().get_memory(sizeof(SptSpecPath));
  return new (p) SptSpecPath(file_region, id, expr, path_decl);
}

// @brief パス記述の生成
// @param[in] file_region ファイル位置の情報
// @param[in] edge
// @param[in] input_array
// @param[in] input_pol
// @param[in] op
// @param[in] output_array
// @param[in] output_pol
// @param[in] expr
// @param[in] path_delay
// @return 生成されたパス記述
const PtPathDecl*
SptFactory::new_PathDecl(const FileRegion& file_region,
			 int edge,
			 PtExprArray input_array,
			 int input_pol,
			 int op,
			 PtExprArray output_array,
			 int output_pol,
			 const PtExpr* expr,
			 const PtPathDelay* path_delay)
{
  void* p = alloc().get_memory(sizeof(SptPathDecl));
  return new (p) SptPathDecl(file_region, edge, input_array, input_pol,
			     op, output_array, output_pol,
			     expr, path_delay);
}

// @brief path delay value の生成 (値が1つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value 値
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value);
}

// @brief path delay value の生成 (値が2つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region, value1, value2);
}

// @brief path delay value の生成 (値が3つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2,
			  const PtExpr* value3)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region,
			      value1, value2, value3);
}

// @brief path delay value の生成 (値が6つ)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2,
			  const PtExpr* value3,
			  const PtExpr* value4,
			  const PtExpr* value5,
			  const PtExpr* value6)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region,
			      value1, value2, value3,
			      value4, value5, value6);
}

// @brief path delay value の生成 (値が12個)
// @param[in] file_region ファイル位置の情報
// @param[in] value1 値1
// @param[in] value2 値2
// @param[in] value3 値3
// @param[in] value4 値4
// @param[in] value5 値5
// @param[in] value6 値6
// @param[in] value7 値7
// @param[in] value8 値8
// @param[in] value9 値9
// @param[in] value10 値10
// @param[in] value11 値11
// @param[in] value12 値12
// @return 生成された path delay value
const PtPathDelay*
SptFactory::new_PathDelay(const FileRegion& file_region,
			  const PtExpr* value1,
			  const PtExpr* value2,
			  const PtExpr* value3,
			  const PtExpr* value4,
			  const PtExpr* value5,
			  const PtExpr* value6,
			  const PtExpr* value7,
			  const PtExpr* value8,
			  const PtExpr* value9,
			  const PtExpr* value10,
			  const PtExpr* value11,
			  const PtExpr* value12)
{
  void* p = alloc().get_memory(sizeof(SptPathDelay));
  return new (p) SptPathDelay(file_region,
			      value1, value2, value3,
			      value4, value5, value6,
			      value7, value8, value9,
			      value10, value11, value12);
}

END_NAMESPACE_YM_VERILOG
