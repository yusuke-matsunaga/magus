
/// @file EiDecl.cc
/// @brief EiDecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiDecl.h"
#include "ElbDelay.h"
#include "ElbExpr.h"

#include "verilog/pt/PtDecl.h"
#include "verilog/pt/PtItem.h"
#include "verilog/pt/PtMisc.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 宣言要素を生成する．
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期割り当て式
ElbDecl*
EiFactory::new_Decl(ElbDeclHead* head,
		    const PtNamedBase* pt_item,
		    ElbExpr* init)
{
  EiDecl* decl = NULL;

  switch ( head->type() ) {
  case kVpiReg:
  case kVpiNet:
    if ( head->bit_size() == 1 ) {
      if ( init ) {
	void* p = mAlloc.get_memory(sizeof(EiDeclIS));
	decl = new (p) EiDeclIS(head, pt_item, init);
      }
      else {
	void* p = mAlloc.get_memory(sizeof(EiDeclS));
	decl = new (p) EiDeclS(head, pt_item);
      }
      break;
    }
    // わざと次に続く

  case kVpiIntegerVar:
  case kVpiTimeVar:
    if ( init ) {
      void* p = mAlloc.get_memory(sizeof(EiDeclIV));
      decl = new (p) EiDeclIV(head, pt_item, init);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(EiDeclV));
      decl = new (p) EiDeclV(head, pt_item);
    }
    break;

  case kVpiRealVar:
    if ( init ) {
      void* p = mAlloc.get_memory(sizeof(EiDeclIR));
      decl = new (p) EiDeclIR(head, pt_item, init);
    }
    else {
      void* p = mAlloc.get_memory(sizeof(EiDeclR));
      decl = new (p) EiDeclR(head, pt_item);
    }
    break;

  case kVpiNamedEvent:
    assert_cond(init == NULL, __FILE__, __LINE__);
    {
      void* p = mAlloc.get_memory(sizeof(EiDeclN));
      decl = new (p) EiDeclN(head, pt_item);
    }
    break;

  case kVpiParameter:
  case kVpiSpecParam:
  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  return decl;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDecl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDecl::EiDecl(ElbDeclHead* head,
	       const PtNamedBase* pt_item) :
  mHead(head),
  mPtItem(pt_item),
  mAuxSign(false)
{
}

// @brief デストラクタ
EiDecl::~EiDecl()
{
}

// @brief 型の取得
// @return vpi_user.h で定義された型 (vpiModule など)
tVpiObjType
EiDecl::type() const
{
  return mHead->type();
}

// @brief ファイル位置の取得
FileRegion
EiDecl::file_region() const
{
  return mPtItem->file_region();
}

// @brief このオブジェクトの属しているスコープを返す．
const VlNamedObj*
EiDecl::parent() const
{
  return mHead->parent();
}

// @brief 名前の取得
const char*
EiDecl::name() const
{
  return mPtItem->name();
}

// @breif 値の型を返す．
// @note 値を持たないオブジェクトの場合には kVpiValueNone を返す．
VlValueType
EiDecl::value_type() const
{
  switch ( type() ) {
  case kVpiNet:
  case kVpiReg:
    return VlValueType(is_signed(), true, bit_size());

  case kVpiIntegerVar:
    return VlValueType::int_type();

  case kVpiRealVar:
    return VlValueType::real_type();

  case kVpiTimeVar:
    return VlValueType::time_type();

  case kVpiParameter:
  case kVpiSpecParam:
  case kVpiConstant:
    // ここにはこない
    assert_not_reached(__FILE__, __LINE__);
    break;

  default:
    // 上記以外は形無し
    break;
  }

  return VlValueType();
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiDecl::is_signed() const
{
  return mHead->is_signed() || mAuxSign;
}

// @brief 範囲指定を持つとき true を返す．
bool
EiDecl::has_range() const
{
  return mHead->has_range();
}

// @brief 範囲の MSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDecl::left_range_val() const
{
  return mHead->left_range_val();
}

// @brief 範囲の LSB の値を返す．
// @note 範囲を持たないときの値は不定
int
EiDecl::right_range_val() const
{
  return mHead->right_range_val();
}

// @brief 範囲のMSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDecl::left_range_string() const
{
  return mHead->left_range_string();
}

// @brief 範囲のLSBを表す文字列の取得
// @note 範囲を持たない時の値は不定
string
EiDecl::right_range_string() const
{
  return mHead->right_range_string();
}

// @brief left_range >= right_range の時に true を返す．
bool
EiDecl::is_big_endian() const
{
  return mHead->is_big_endian();
}

// @brief left_range <= right_range の時に true を返す．
bool
EiDecl::is_little_endian() const
{
  return mHead->is_little_endian();
}

// @brief ビット幅を返す．
ymuint
EiDecl::bit_size() const
{
  return mHead->bit_size();
}

// @brief オフセット値の取得
// @param[in] index インデックス
// @param[out] offset インデックスに対するオフセット値
// @retval true インデックスが範囲内に入っている時
// @retval false インデックスが範囲外の時
bool
EiDecl::calc_bit_offset(int index,
			ymuint& offset) const
{
  return mHead->calc_bit_offset(index, offset);
}

// @brief データ型の取得
// @retval データ型 kParam, kLocalParam, kVar の場合
// @retval kVpiVarNone 上記以外
tVpiVarType
EiDecl::data_type() const
{
  return mHead->data_type();
}

// @brief net 型の取得
// @retval net 型 net 型の要素の場合
// @retval kVpiNone net 型の要素でない場合
tVpiNetType
EiDecl::net_type() const
{
  return mHead->net_type();
}

// @brief vectored|scalared 属性の取得
// @retval kVpiVsNone vectored|scalared 指定なし
// @retval kVpiVectored vectored 指定あり
// @retval kVpiScalared scalared 指定あり
tVpiVsType
EiDecl::vs_type() const
{
  return mHead->vs_type();
}

// @brief drive0 strength の取得
// @retval 0 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDecl::drive0() const
{
  return mHead->drive0();
}

// @brief drive1 strength の取得
// @retval 1 の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDecl::drive1() const
{
  return mHead->drive1();
}

// @brief charge strength の取得
// @retval 電荷の強度
// @retval kVpiNoStrength strength の指定なし
tVpiStrength
EiDecl::charge() const
{
  return mHead->charge();
}

// @brief delay の取得
// @retval delay
// @retval NULL delay の指定なし
const VlDelay*
EiDecl::delay() const
{
  return mHead->delay();
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiDecl::init_value() const
{
  return NULL;
}

// @brief 符号付きに補正する．
void
EiDecl::set_signed()
{
  mAuxSign = true;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclN
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDeclN::EiDeclN(ElbDeclHead* head,
		 const PtNamedBase* pt_item) :
  EiDecl(head, pt_item)
{
}

// @brief デストラクタ
EiDeclN::~EiDeclN()
{
}

// @brief スカラー値を返す．
VlScalarVal
EiDeclN::get_scalar() const
{
  assert_not_reached(__FILE__, __LINE__);
  return VlScalarVal::x();
}

// @brief スカラー値を設定する．
void
EiDeclN::set_scalar(const VlScalarVal& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 論理値を返す．
VlScalarVal
EiDeclN::get_logic() const
{
  assert_not_reached(__FILE__, __LINE__);
  return VlScalarVal::x();
}

// @brief real 型の値を返す．
double
EiDeclN::get_real() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0.0;
}

// @brief real 型の値を設定する．
void
EiDeclN::set_real(double val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief bitvector 型の値を返す．
void
EiDeclN::get_bitvector(BitVector& bitvector,
		       const VlValueType& req_type) const
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief bitvector 型の値を設定する．
void
EiDeclN::set_bitvector(const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
VlScalarVal
EiDeclN::get_bitselect(int index) const
{
  assert_not_reached(__FILE__, __LINE__);
  return VlScalarVal::x();
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiDeclN::set_bitselect(int index,
		       const VlScalarVal& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiDeclN::get_partselect(int left,
			int right,
			BitVector& val) const
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiDeclN::set_partselect(int left,
			int right,
			const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclS
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDeclS::EiDeclS(ElbDeclHead* head,
		   const PtNamedBase* pt_item) :
  EiDecl(head, pt_item),
  mVal(VlScalarVal::x())
{
}

// @brief デストラクタ
EiDeclS::~EiDeclS()
{
}

// @brief スカラー値を返す．
VlScalarVal
EiDeclS::get_scalar() const
{
  return mVal;
}

// @brief スカラー値を設定する．
void
EiDeclS::set_scalar(const VlScalarVal& val)
{
  mVal = val;
}

// @brief 論理値を返す．
VlScalarVal
EiDeclS::get_logic() const
{
  return mVal.to_logic();
}

// @brief real 型の値を返す．
double
EiDeclS::get_real() const
{
  return mVal.to_real();
}

// @brief real 型の値を設定する．
void
EiDeclS::set_real(double val)
{
  mVal = VlScalarVal(val);
}

// @brief bitvector 型の値を返す．
void
EiDeclS::get_bitvector(BitVector& val,
		       const VlValueType& req_type) const
{
  val = mVal;
  val.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiDeclS::set_bitvector(const BitVector& val)
{
  mVal = val.to_scalar();
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
VlScalarVal
EiDeclS::get_bitselect(int index) const
{
  ymuint offset;
  if ( calc_bit_offset(index, offset) ) {
    // offset == 0 のはず．
    return mVal;
  }
  else {
    // 範囲外は X
    return VlScalarVal::x();
  }
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiDeclS::set_bitselect(int index,
		       const VlScalarVal& val)
{
  ymuint offset;
  if ( calc_bit_offset(index, offset) ) {
    // offset == 0 のはず．
    mVal = val;
  }
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiDeclS::get_partselect(int left,
			int right,
			BitVector& val) const
{
  ymuint bpos1;
  ymuint bpos2;
  if ( calc_bit_offset(left, bpos1) &&
       calc_bit_offset(right, bpos2) ) {
    val = mVal;
  }
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiDeclS::set_partselect(int left,
			int right,
			const BitVector& val)
{
  ymuint bpos1;
  ymuint bpos2;
  if ( calc_bit_offset(left, bpos1) &&
       calc_bit_offset(right, bpos2) ) {
    mVal = val.to_scalar();
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclR
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDeclR::EiDeclR(ElbDeclHead* head,
		 const PtNamedBase* pt_item) :
  EiDecl(head, pt_item),
  mVal(0.0)
{
}

// @brief デストラクタ
EiDeclR::~EiDeclR()
{
}

// @brief スカラー値を返す．
VlScalarVal
EiDeclR::get_scalar() const
{
  return VlScalarVal(mVal);
}

// @brief スカラー値を設定する．
void
EiDeclR::set_scalar(const VlScalarVal& val)
{
  mVal = val.to_real();
}

// @brief 論理値を返す．
VlScalarVal
EiDeclR::get_logic() const
{
  return VlScalarVal(get_real());
}

// @brief real 型の値を返す．
double
EiDeclR::get_real() const
{
  return mVal;
}

// @brief real 型の値を設定する．
void
EiDeclR::set_real(double val)
{
  mVal = val;
}

// @brief bitvector 型の値を返す．
void
EiDeclR::get_bitvector(BitVector& bitvector,
		       const VlValueType& req_type) const
{
  bitvector = mVal;
  bitvector.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiDeclR::set_bitvector(const BitVector& val)
{
  mVal = val.to_real();
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
VlScalarVal
EiDeclR::get_bitselect(int index) const
{
  assert_not_reached(__FILE__, __LINE__);
  return VlScalarVal::x();
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiDeclR::set_bitselect(int index,
		       const VlScalarVal& val)
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiDeclR::get_partselect(int left,
			int right,
			BitVector& val) const
{
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiDeclR::set_partselect(int left,
			int right,
			const BitVector& val)
{
  assert_not_reached(__FILE__, __LINE__);
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
EiDeclV::EiDeclV(ElbDeclHead* head,
		   const PtNamedBase* pt_item) :
  EiDecl(head, pt_item)
{
}

// @brief デストラクタ
EiDeclV::~EiDeclV()
{
}

// @brief スカラー値を返す．
VlScalarVal
EiDeclV::get_scalar() const
{
  return mVal.to_scalar();
}

// @brief スカラー値を設定する．
void
EiDeclV::set_scalar(const VlScalarVal& val)
{
  mVal = val;
}

// @brief 論理値を返す．
VlScalarVal
EiDeclV::get_logic() const
{
  return mVal.to_logic();
}

// @brief real 型の値を返す．
double
EiDeclV::get_real() const
{
  return mVal.to_real();
}

// @brief real 型の値を設定する．
void
EiDeclV::set_real(double val)
{
  mVal = val;
}

// @brief bitvector 型の値を返す．
void
EiDeclV::get_bitvector(BitVector& bitvector,
		       const VlValueType& req_type) const
{
  bitvector = mVal;
  bitvector.coerce(req_type);
}

// @brief bitvector 型の値を設定する．
void
EiDeclV::set_bitvector(const BitVector& val)
{
  mVal = val;
}

// @brief ビット選択値を返す．
// @param[in] index ビット位置
VlScalarVal
EiDeclV::get_bitselect(int index) const
{
  ymuint bpos;
  if ( calc_bit_offset(index, bpos) ) {
    return mVal.bit_select(bpos);
  }
  else {
    return VlScalarVal::x();
  }
}

// @brief ビット値を設定する．
// @param[in] index ビット位置
// @param[in] val 値
void
EiDeclV::set_bitselect(int index,
		       const VlScalarVal& val)
{
  ymuint bpos;
  if ( calc_bit_offset(index, bpos) ) {
    mVal.bit_select(bpos, val);
  }
}

// @brief 範囲選択値を返す．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[out] val 値
void
EiDeclV::get_partselect(int left,
			int right,
			BitVector& val) const
{
  ymuint bpos1;
  ymuint bpos2;
  if ( calc_bit_offset(left, bpos1) &&
       calc_bit_offset(right, bpos2) ) {
    val = mVal.part_select(bpos1, bpos2);
  }
  else {
    int w;
    if ( left > right ) {
      w = left - right + 1;
    }
    else {
      w = right - left + 1;
    }
    val = BitVector(VlScalarVal::x(), w);
  }
}

// @brief 範囲値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
// @param[in] val 値
void
EiDeclV::set_partselect(int left,
			int right,
			const BitVector& val)
{
  ymuint bpos1;
  ymuint bpos2;
  if ( calc_bit_offset(left, bpos1) &&
       calc_bit_offset(right, bpos2) ) {
    mVal.part_select(bpos1, bpos2, val);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclIS
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiDeclIS::EiDeclIS(ElbDeclHead* head,
		   const PtNamedBase* pt_item,
		   ElbExpr* init) :
  EiDeclS(head, pt_item),
  mInit(init)
{
}

// @brief デストラクタ
EiDeclIS::~EiDeclIS()
{
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiDeclIS::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
// @param[in] expr 初期値
void
EiDeclIS::set_init(ElbExpr* expr)
{
  mInit = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclIR
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiDeclIR::EiDeclIR(ElbDeclHead* head,
		   const PtNamedBase* pt_item,
		   ElbExpr* init) :
  EiDeclR(head, pt_item),
  mInit(init)
{
}

// @brief デストラクタ
EiDeclIR::~EiDeclIR()
{
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiDeclIR::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
// @param[in] expr 初期値
void
EiDeclIR::set_init(ElbExpr* expr)
{
  mInit = expr;
}


//////////////////////////////////////////////////////////////////////
// クラス EiDeclIV
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head ヘッダ
// @param[in] pt_item パース木の宣言要素
// @param[in] init 初期値
EiDeclIV::EiDeclIV(ElbDeclHead* head,
		   const PtNamedBase* pt_item,
		   ElbExpr* init) :
  EiDeclV(head, pt_item),
  mInit(init)
{
}

// @brief デストラクタ
EiDeclIV::~EiDeclIV()
{
}

// @brief 初期値の取得
// @retval 初期値
// @retval NULL 設定がない場合
const VlExpr*
EiDeclIV::init_value() const
{
  return mInit;
}

// @brief 初期値の設定
// @param[in] expr 初期値
void
EiDeclIV::set_init(ElbExpr* expr)
{
  mInit = expr;
}

END_NAMESPACE_YM_VERILOG
