
/// @file MpInt.cc
/// @brief MpInt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MpInt.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MpInt
//////////////////////////////////////////////////////////////////////

// おぼえ書き
//
// [内部表現]
//  mBlockNum の下位1ビットが符号ビット
//   - 0: 正
//   - 1: 負
//  mBlockNum の2ビット目以降が真のブロック数
//  mBlockArray[] に絶対値表現で数値が入る．
//
// [演算アルゴリズム]
//   - 符号反転:
//     ただ単に符号ビットを反転する．
//
//   - 加算，減算：
//     まず，2つのオペランドの符号で分類する．
//     left right パタン
//      +    +    abs(left) + abs(right)
//      +    -    abs(left) - abs(right)
//      -    +    abs(right) - abs(left)
//      -    -    -(abs(left) + abs(right))
//
//      さらに減算の場合，絶対値で大小比較を行う．
//      left >= right ならそのまま
//      left <  right なら -(abs(right) - abs(left)) とする．
//
//      ピュアな減算では絶対値は小さくなるので桁あふれは生じない．
//      ピュアな加算のみ桁あふれを考慮する必要がある．
//
//      ブロックごとの加算の結果 carry が生じるのは演算後の値が
//      演算前の値よりも小さくなった時
//
//      フロックごとの減算の結果 borrow が生じるのは演算後の値が
//      演算前の値よりも大きくなった時


// @brief 空のコンストラクタ
//
// 値は不定
MpInt::MpInt()
{
  mBlockNum = 0;
  mBlockArray = NULL;
  alloc_block(1);
  mBlockArray[0] = 0UL;
}

// @brief 整数からの変換コンストラクタ
// @param[in] val 値
MpInt::MpInt(int val)
{
  mBlockNum = 0;
  mBlockArray = NULL;
  alloc_block(1);

  if ( val < 0 ) {
    mBlockArray[0] = -val;
    set_sign(true);
  }
  else {
    mBlockArray[0] = val;
    set_sign(false);
  }
}

// @brief コピーコンストラクタ
// @param[in] val 値
MpInt::MpInt(const MpInt& val)
{
  mBlockNum = 0;
  mBlockArray = NULL;
  operator=(val);
}

// @brief 符号反転フラグ付きのコピーコンストラクタ
// @param[in] val 値
// @param[in] inv 符号反転フラグ(true で反転)
MpInt::MpInt(const MpInt& val,
	     bool inv)
{
  mBlockNum = 0;
  mBlockArray = NULL;
  operator=(val);
  if ( inv ) {
    complement();
  }
}

// @brief 代入演算子
// @param[in] val 値
const MpInt&
MpInt::operator=(const MpInt& val)
{
  if ( &val != this ) {
    alloc_block(val.block_num());
    for (ymuint i = 0; i < block_num(); ++ i) {
      mBlockArray[i] = val.mBlockArray[i];
    }
    set_sign(val.is_negative());
  }

  return *this;
}

// @brief デストラクタ
MpInt::~MpInt()
{
  delete [] mBlockArray;
}

// @brief 符号反転を行い自身に代入する．
// @return 結果を返す．
const MpInt&
MpInt::complement()
{
  mBlockNum ^= 1U;

  return *this;
}

// @brief 加算付き代入
// @param[in] right 右辺
// @return 結果を返す．
const MpInt&
MpInt::operator+=(const MpInt& right)
{
  vector<ymuint64> block_list;
  bool inv = false;
  if ( is_positive() ) {
    if ( right.is_positive() ) {
      abs_add(*this, right, block_list);
      inv = false;
    }
    else {
      if ( abs_compare(*this, right) >= 0 ) {
	abs_sub(*this, right, block_list);
	inv = false;
      }
      else {
	abs_sub(right, *this, block_list);
	inv = true;
      }
    }
  }
  else {
    if ( right.is_positive() ) {
      if ( abs_compare(*this, right) >= 0 ) {
	abs_sub(*this, right, block_list);
	inv = true;
      }
      else {
	abs_sub(right, *this, block_list);
	inv = false;
      }
    }
    else {
      abs_add(*this, right, block_list);
      inv = true;
    }
  }
  set(block_list, inv);

  return *this;
}

// @brief 減算付き代入
// @param[in] right 右辺
// @return 結果を返す．
const MpInt&
MpInt::operator-=(const MpInt& right)
{
  vector<ymuint64> block_list;
  bool inv = false;
  if ( is_positive() ) {
    if ( right.is_positive() ) {
      if ( abs_compare(*this, right) >= 0 ) {
	abs_sub(*this, right, block_list);
	inv = false;
      }
      else {
	abs_sub(right, *this, block_list);
	inv = true;
      }
    }
    else {
      abs_add(*this, right, block_list);
      inv = false;
    }
  }
  else {
    if ( right.is_positive() ) {
      abs_add(*this, right, block_list);
      inv = true;
    }
    else {
      if ( abs_compare(*this, right) >= 0 ) {
	abs_sub(*this, right, block_list);
	inv = true;
      }
      else {
	abs_sub(right, *this, block_list);
	inv = false;
      }
    }
  }
  set(block_list, inv);

  return *this;
}

// @brief 加算を行う．
// @param[in] left, right オペランド
MpInt
operator+(const MpInt& left,
	  const MpInt& right)
{
  vector<ymuint64> block_list;
  bool inv = false;
  if ( left.is_positive() ) {
    if ( right.is_positive() ) {
      MpInt::abs_add(left, right, block_list);
      inv = false;
    }
    else {
      if ( MpInt::abs_compare(left, right) >= 0 ) {
	MpInt::abs_sub(left, right, block_list);
	inv = false;
      }
      else {
	MpInt::abs_sub(right, left, block_list);
	inv = true;
      }
    }
  }
  else {
    if ( right.is_positive() ) {
      if ( MpInt::abs_compare(left, right) >= 0 ) {
	MpInt::abs_sub(left, right, block_list);
	inv = true;
      }
      else {
	MpInt::abs_sub(right, left, block_list);
	inv = false;
      }
    }
    else {
      MpInt::abs_add(left, right, block_list);
      inv = true;
    }
  }

  return MpInt(block_list, inv);
}

// @brief 減算を行う．
// @param[in] left, right オペランド
MpInt
operator-(const MpInt& left,
	  const MpInt& right)
{
  vector<ymuint64> block_list;
  bool inv = false;
  if ( left.is_positive() ) {
    if ( right.is_positive() ) {
      if ( MpInt::abs_compare(left, right) >= 0 ) {
	MpInt::abs_sub(left, right, block_list);
	inv = false;
      }
      else {
	MpInt::abs_sub(right, left, block_list);
	inv = true;
      }
    }
    else {
      MpInt::abs_add(left, right, block_list);
      inv = false;
    }
  }
  else {
    if ( right.is_positive() ) {
      MpInt::abs_add(left, right, block_list);
      inv = true;
    }
    else {
      if ( MpInt::abs_compare(left, right) >= 0 ) {
	MpInt::abs_sub(left, right, block_list);
	inv = true;
      }
      else {
	MpInt::abs_sub(right, left, block_list);
	inv = false;
      }
    }
  }
  return MpInt(block_list, inv);
}

// @brief 乗算付き代入
// @param[in] right 右辺
// @return 結果を返す．
const MpInt&
MpInt::operator*=(const MpInt& right)
{
}

// @brief 乗算を行う．
// @param[in] left, right オペランド
MpInt
operator*(const MpInt& left,
	  const MpInt& right)
{
}

// @brief 左シフト付き代入
// @param[in] shamt シフト量
// @return 結果を返す．
const MpInt&
MpInt::operator<<=(ymuint shamt)
{
}

// @brief 左シフト演算を行う．
// @param[in] left オペランド
// @param[in] shamt シフト量
MpInt
operator<<(const MpInt& left,
	   ymuint shamt)
{
}

// @brief 右シフト付き代入
// @param[in] shamt シフト量
// @return 結果を返す．
const MpInt&
MpInt::operator>>=(ymuint shamt)
{
}

// @brief 右シフト演算を行う．
// @param[in] left オペランド
// @param[in] shamt シフト量
MpInt
operator>>(const MpInt& left,
	   ymuint shamt)
{
}

// @brief 絶対値の大小比較を行う関数
// @param[in] left, right オペランド
// @retval -1 left < right
// @retval  0 left = right
// @retval  1 left > right
int
MpInt::abs_compare(const MpInt& left,
		   const MpInt& right)
{
  ymuint nb1 = left.block_num();
  ymuint nb2 = right.block_num();
  ymuint nb = nb1 > nb2 ? nb1 : nb2;
  for (ymuint i = nb; i > 0; ) {
    -- i;
    ymuint64 val1 = (i < nb1) ? left.mBlockArray[i] : 0UL;
    ymuint64 val2 = (i < nb2) ? right.mBlockArray[i] : 0UL;
    if ( val1 > val2 ) {
      return 1;
    }
    if ( val1 < val2 ) {
      return -1;
    }
  }
  return 0;
}

// @brief 絶対値の加算を行う関数
// @param[in] left, right オペランド
// @param[out] block_list 結果を格納するリスト
void
MpInt::abs_add(const MpInt& left,
	       const MpInt& right,
	       vector<ymuint64>& block_list)

{
  ymuint nb1 = left.block_num();
  ymuint nb2 = right.block_num();
  ymuint nb = nb1 > nb2 ? nb1 : nb2;

  block_list.clear();
  block_list.reserve(nb + 1);

  ymuint64 carry = 0UL;
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 val1 = (i < nb1) ? left.mBlockArray[i] : 0UL;
    ymuint64 val2 = (i < nb2) ? right.mBlockArray[i] : 0UL;
    ymuint64 val3 = val1 + val2 + carry;
    block_list.push_back(val3);
    if ( val3 < val1 ) {
      carry = 1UL;
    }
    else {
      carry = 0UL;
    }
  }
  if ( carry == 1UL ) {
    // 桁あふれしたので拡張する．
    block_list.push_back(1UL);
  }
}

// @brief 絶対値の減算を行うサブルーティン
// @param[in] left, right オペランド
// @param[out] block_list 結果を格納するリスト
void
MpInt::abs_sub(const MpInt& left,
	       const MpInt& right,
	       vector<ymuint64>& block_list)
{
  ymuint nb1 = left.block_num();
  ymuint nb2 = right.block_num();
  ymuint nb = nb1 > nb2 ? nb1 : nb2;

  block_list.clear();
  block_list.reserve(nb);

  ymuint64 borrow = 0UL;
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint64 val1 = (i < nb1) ? left.mBlockArray[i] : 0UL;
    ymuint64 val2 = (i < nb2) ? right.mBlockArray[i] : 0UL;
    ymuint64 val3 = val1 - val2 - borrow;
    block_list.push_back(val3);
    if ( val3 > val1 ) {
      borrow = 1UL;
    }
    else {
      borrow = 0UL;
    }
  }
  ASSERT_COND( borrow == 0UL );
}

// @brief 内容を設定する関数
// @param[in] block_list ブロックリスト
// @param[in] inv 結果に対する符号反転フラグ
void
MpInt::set(const vector<ymuint64>& block_list,
	   bool inv)
{
  // 真のブロック数を数える．
  ymuint nb = 0;
  for (ymuint i = 0; i < block_list.size(); ++ i) {
    if ( block_list[i] != 0UL ) {
      nb = i;
    }
  }
  ++ nb;

  alloc_block(nb);
  for (ymuint i = 0; i < nb; ++ i) {
    mBlockArray[i] = block_list[i];
  }
  set_sign(inv);
}

// @brief ブロックを確保する．
// @param[in] nb ブロック数
//
// 現在のブロック数が nb と等しい時はなにもしない．
// いかなる時も内容は初期化されない．
void
MpInt::alloc_block(ymuint nb)
{
  if ( block_num() != nb ) {
    delete [] mBlockArray;
    mBlockNum &= 1U;
    mBlockNum |= nb << 1;
    mBlockArray = new ymuint64[nb];
  }
}

// @brief 符号ビットをセットする．
void
MpInt::set_sign(bool sign)
{
  if ( sign ) {
    mBlockNum |= 1U;
  }
  else {
    mBlockNum &= ~1U;
  }
}


END_NAMESPACE_YM
