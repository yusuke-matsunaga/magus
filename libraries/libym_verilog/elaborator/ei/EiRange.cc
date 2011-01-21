
/// @file libym_verilog/elb_impl/EiRange.cc
/// @brief EiRange の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiRange.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "EiFactory.h"
#include "EiRange.h"

#include "ym_verilog/pt/PtExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief 範囲の配列を生成する．
// @param[in] dim_size 要素数
ElbRange*
EiFactory::new_RangeArray(ymuint dim_size)
{
  void* p = mAlloc.get_memory(sizeof(EiRange) * dim_size);
  EiRange* range_array = new (p) EiRange[dim_size];

  return range_array;
}


//////////////////////////////////////////////////////////////////////
// クラス EiRange
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EiRange::EiRange()
{
}

// @brief デストラクタ
EiRange::~EiRange()
{
}

// @brief 値を設定する．
// @param[in] src 元となる情報
void
EiRange::set(const ElbRangeSrc& src)
{
  mPtRange = src.pt_range();
  mLeftRange = src.left_range();
  mRightRange = src.right_range();
  mLeftVal = src.left_range_val();
  mRightVal = src.right_range_val();
}

// @brief 型の取得
tVpiObjType
EiRange::type() const
{
  return kVpiRange;
}

// @brief ファイル位置を返す．
FileRegion
EiRange::file_region() const
{
  return mPtRange->file_region();
}

// @brief 要素数(ビット幅)を返す．
ymuint
EiRange::size() const
{
  return calc_size(mLeftVal, mRightVal);
}

// @brief MSB の値を返す．
// @retval MSB の値 値が確定しているとき
// @retval -1 値が確定していない
int
EiRange::left_range_val() const
{
  return mLeftVal;
}

// @brief LSB の値を返す．
// @retval LSB の値 値が確定しているとき
// @retval -1 値が確定していない
int
EiRange::right_range_val() const
{
  return mRightVal;
}

// @brief MSB を表す文字列を返す．
string
EiRange::left_range_string() const
{
  return mLeftRange->decompile();
}

// @brief LSB を表す文字列を返す．
string
EiRange::right_range_string() const
{
  return mRightRange->decompile();
}

// @brief 範囲のチェック
// @param[in] index インデックス
// @retval true index が範囲内に入っている．
// @retval false index が範囲外
bool
EiRange::is_in(int index) const
{
  return is_in(mLeftVal, mRightVal, index);
}

// @brief LSB からのオフセット値の取得
// @param[in] index インデックス
// @retval index の LSB からのオフセット index が範囲内に入っている．
// @retval -1 index が範囲外
int
EiRange::offset(int index) const
{
  return offset(mLeftVal, mRightVal, index);
}

// @brief MSB からのオフセット値の取得
// @param[in] index インデックス
// @retval index の MSB からのオフセット index が範囲内に入っている．
// @retval -1 index が範囲外
int
EiRange::roffset(int index) const
{
  return roffset(mLeftVal, mRightVal, index);
}

// @brief offset の逆関数
// @param[in] offset LSB からのオフセット値
// @return offset に対応したインデックスを返す．
int
EiRange::index(int offset) const
{
  return index(mLeftVal, mRightVal, offset);
}

// @brief roffset の逆関数
// @param[in] roffset MSB からのオフセット値
// @return roffset に対応したインデックスを返す．
int
EiRange::rindex(int roffset) const
{
  return rindex(mLeftVal, mRightVal, roffset);
}


//////////////////////////////////////////////////////////////////////
// クラス EiRangeImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
EiRangeImpl::EiRangeImpl() :
  mLeftRange(NULL),
  mRightRange(NULL)
{
}

// デストラクタ
EiRangeImpl::~EiRangeImpl()
{
}

// @brief 値を設定する．
// @param[in] left 範囲の MSB
// @param[in] right 範囲の LSB
void
EiRangeImpl::set(const PtExpr* left,
		 const PtExpr* right,
		 int left_val,
		 int right_val)
{
  mLeftRange = left;
  mRightRange = right;
  mLeftVal = left_val;
  mRightVal = right_val;
}

// @brief サイズを返す．
ymuint
EiRangeImpl::size() const
{
  return EiRange::calc_size(mLeftVal, mRightVal);
}

// @brief MSB の値を返す．
int
EiRangeImpl::left_range_val() const
{
  return mLeftVal;
}

// @brief LSB の値を返す．
int
EiRangeImpl::right_range_val() const
{
  return mRightVal;
}

// @brief MSB を表す文字列を返す．
string
EiRangeImpl::left_range_string() const
{
  return mLeftRange->decompile();
}

// @brief LSB を表す文字列を返す．
string
EiRangeImpl::right_range_string() const
{
  return mRightRange->decompile();
}

// index が範囲内に入っていたら true を返す．
// 範囲外の時には false を返す．
bool
EiRangeImpl::is_in(int index) const
{
  return EiRange::is_in(mLeftVal, mRightVal, index);
}

// index のLSBからのオフセットを返す．
// 範囲外の時は -1 を返す。
int
EiRangeImpl::offset(int index) const
{
  return EiRange::offset(mLeftVal, mRightVal, index);
}

// index のMSBからのオフセットを返す．
// 範囲外の時は -1 を返す。
int
EiRangeImpl::roffset(int index) const
{
  return EiRange::roffset(mLeftVal, mRightVal, index);
}

// offset の逆関数
int
EiRangeImpl::index(int offset) const
{
  return EiRange::index(mLeftVal, mRightVal, offset);
}

// roffset の逆関数
int
EiRangeImpl::rindex(int roffset) const
{
  return EiRange::rindex(mLeftVal, mRightVal, roffset);
}


//////////////////////////////////////////////////////////////////////
// EiRange の配列
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @brief dim_size 次元数
EiRangeArray::EiRangeArray(ymuint dim_size,
			   EiRange* array) :
  mDimSize(dim_size),
  mArray(array)
{
  // 要素数を計算する．
  mElemSize = 1;
  for (ymuint i = 0; i < dim_size; ++ i) {
    mElemSize *= array[i].size();
  }
}

// デストラクタ
EiRangeArray::~EiRangeArray()
{
}

// @brief アドレス(オフセット)からインデックスの配列を作る．
// @param[in] offset オフセット
// @param[out] index_list
void
EiRangeArray::index(ymuint offset,
		    vector<int>& index_list) const
{
  ymuint n = size();
  index_list.resize(n);
  for (ymuint i = n; i -- > 0; ) {
    const EiRange* r = range(i);
    int k = r->size();
    int offset1 = offset % k;
    offset /= k;
    index_list[i] = r->rindex(offset1);
  }
}

// @brief インデックスのリストからオフセットを得る．
// @param[in] index_list インデックスのリスト
// @return index_list の値に対応したオフセット値
// @note index_list のいずれかの値が範囲外の場合には -1 を返す．
int
EiRangeArray::offset(const vector<int>& index_list) const
{
  ymuint n = size();
  if ( index_list.size() != n ) {
    return -1;
  }

  int offset = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const EiRange* r = range(i);
    int k = r->size();
    offset *= k;
    int offset1 = r->roffset(index_list[i]);
    offset += offset1;
  }
  return offset;
}

END_NAMESPACE_YM_VERILOG
