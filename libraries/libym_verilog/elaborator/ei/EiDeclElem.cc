
/// @file libym_verilog/elaborator/ei/EiDeclElem.cc
/// @brief EiDeclElem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiPrimary.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDeclElem.h"

#include "ym_verilog/BitVector.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// クラス EiDeclElem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_expr パース木の定義要素
// @param[in] parent_array 親の配列
// @param[in] index_list インデックスのリスト
EiDeclElem::EiDeclElem(const PtBase* pt_expr,
		       ElbDeclArray* parent_array,
		       const vector<ElbExpr*>& index_list) :
  mPtExpr(pt_expr),
  mArray(parent_array),
  mIndexList(index_list),
  mIndexValList(index_list.size())
{
}

// @brief デストラクタ
EiDeclElem::~EiDeclElem()
{
}

// @brief 型の取得
tVpiObjType
EiDeclElem::type() const
{
  switch( mObj->type() ) {
  case kVpiNetArray: return kVpiNet;
  case kVpiRegArray: return kVpiReg;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return kVpiNet;
}

// @brief ファイル位置を返す．
FileRegion
EiDeclElem::file_region() const
{
  return mPtExpr->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiDeclElem::parent() const
{
  return mArray->parent();
}

// @brief 名前の取得
const char*
EiDeclElem::name() const
{
  return mArray->name();
}

// @brief 式のタイプを返す．
tVpiValueType
EiDeclElem::value_type() const
{
  return mArray->value_type();
}

// @brief 定数値を持つ型のときに true を返す．
bool
EiDeclElem::is_consttype() const
{
  return false;
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDeclElem::is_signed() const
{
  return mArray->is_signed();
}

// @brief MSB の値を返す．
// @retval 範囲のMSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiDeclElem::left_range_const() const
{
  return mArray->left_range_const();
}

// @brief LSB の値を返す．
// @retval 範囲のLSBの値 範囲指定を持つとき
// @retval -1 範囲指定を持たないとき
int
EiDeclElem::right_range_const() const
{
  return mArray->right_range_const();
}

// @brief ビット幅を返す．
ymuint32
EiDeclElem::bit_size() const
{
  return mArray->bit_size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @retval index に対するオフセット値 index が範囲内に入っている時．
// @retval -1 index が範囲外の時
int
EiDeclElem::bit_offset(int index) const
{
  return mArray->bit_offset(index);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDeclElem::data_type() const
{
  return mArray->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDeclElem::net_type() const
{
  return mArray->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
EiDeclElem::vs_type() const
{
  return mArray->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::drive0() const
{
  return mArray->drive0();
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::drive1() const
{
  return mArray->drive1();
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDeclElem::charge() const;
{
  return mArray->charge();
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
EiDeclElem::delay() const
{
  return mArray->delay();
}

// @brief 符号付きに補正する．
// @note このクラスで呼ばれることはない．
void
EiDeclElem::set_signed()
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiDeclElem::_left_range() const
{
  return mArray->_left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
ElbExpr*
EiDeclElem::_right_range() const
{
  return mArray->_right_range();
}

// @brief 配列型宣言要素への参照の場合，配列の次元を返す．
ymuint32
EiDeclElem::declarray_dimension() const
{
  return mIndexList.size();
}

// @brief 配列型宣言要素への参照の場合，配列のインデックスを返す．
// @param[in] pos 位置番号 ( 0 <= pos < declarray_dimension() )
const VlExpr*
EiDeclElem::declarray_index(ymuint32 pos) const
{
  return mIndexList[pos];
}

// @brief スカラー値を返す．
tVpiScalarVal
EiDeclElem::eval_scalar() const
{
  eval_index();
  return mObj->get_scalar(mIndexValList);
}

// @brief 論理値を返す．
tVpiScalarVal
EiDeclElem::eval_logic() const
{
  eval_index();
  return mObj->get_scalar(mIndexValList);
}

// @brief real 型の値を返す．
double
EiDeclElem::eval_real() const
{
  eval_index();
  return mObj->get_real(mIndexValList);
}

// @brief bitvector 型の値を返す．
void
EiDeclElem::eval_bitvector(BitVector& bitvector,
				   tVpiValueType req_type) const
{
  eval_index();
  mObj->get_bitvector(mIndexValList, bitvector, req_type);
}

// @brief decompile() の実装関数
// @param[in] pprim 親の演算子の優先順位
string
EiDeclElem::decompile_impl(int ppri) const
{
  string ans = mObj->name();
  ans += decompile_index();
  return ans;
}

// @brief 要求される式の型を計算してセットする．
// @param[in] type 要求される式の型
// @note 必要であればオペランドに対して再帰的に処理を行なう．
void
EiDeclElem::set_reqsize(tVpiValueType type)
{
  // なにもしない．
}

// @brief インデックス部分を decompile する．
string
EiDeclElem::decompile_index() const
{
  ymuint32 n = mIndexList.size();
  string ans;
  for (ymuint32 i = 0; i < n; ++ i) {
    ans += "[" + mIndexList[i]->decompile() + "]";
  }
  return ans;
}

// @brief スカラー値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclElem::set_scalar(tVpiScalarVal v)
{
  eval_index();
  mObj->set_scalar(mIndexValList, v);
}

// @brief 実数値を書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclElem::set_real(double v)
{
  eval_index();
  mObj->set_real(mIndexValList, v);
}

// @brief ビットベクタを書き込む．
// @param[in] v 書き込む値
// @note 左辺式の時のみ意味を持つ．
void
EiDeclElem::set_bitvector(const BitVector& v)
{
  eval_index();
  mObj->set_bitvector(mIndexValList, v);
}

// @brief インデックスの値を計算する．
bool
EiDeclElem::eval_index() const
{
  ymuint32 n = mIndexList.size();
  for (ymuint32 i = 0; i < n; ++ i) {
    if ( !mIndexList[i]->eval_int(mIndexValList[i]) ) {
      return false;
    }
  }
  return true;
}

END_NAMESPACE_YM_VERILOG
