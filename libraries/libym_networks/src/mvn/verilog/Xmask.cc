
/// @file Xmask.cc
/// @brief Xmask の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Xmask.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス Xmask
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note ビット幅は0で初期化される．
Xmask::Xmask() :
  mBitWidth(0),
  mBits(NULL)
{
}

// @brief ビット幅を指定したコンストラクタ
// @note この時点では mBits はアロケートされない．
Xmask::Xmask(ymuint bit_width) :
  mBitWidth(bit_width),
  mBits(NULL)
{
}

// @brief コピーコンストラクタ
Xmask::Xmask(const Xmask& src) :
  mBitWidth(src.mBitWidth),
  mBits(NULL)
{
  if ( src.mBits ) {
    alloc();
    for (ymuint i = 0; i < blksize(); ++ i) {
      mBits[i] = src.mBits[i];
    }
  }
}

// @brief 代入演算子
const Xmask&
Xmask::operator=(const Xmask& src)
{
  if ( &src != this ) {
    set_bit_width(src.bit_width());
    if ( src.mBits ) {
      alloc();
      for (ymuint i = 0; i < blksize(); ++ i) {
	mBits[i] = src.mBits[i];
      }
    }
  }
  return *this;
}

// @brief デストラクタ
Xmask::~Xmask()
{
  free();
}


// @brief ビット幅を再設定する．
// @param[in] bit_width 設定するビット幅
// @note 以前の内容はクリアされる．
void
Xmask::set_bit_width(ymuint bit_width)
{
  ymuint new_blksize = (bit_width + 31) / 32;
  if ( blksize() != new_blksize ) {
    free();
  }
  else {
    clear_all();
  }
  mBitWidth = bit_width;
}

// @brief 全ビットをセットする．
void
Xmask::set_all()
{
  alloc();
  // 手抜きで全てのビットを1にする．
  ymuint32* end = mBits + blksize();
  for (ymuint32* bp = mBits; bp != end; ++ bp) {
    *bp = 0xFFFFFFFFU;
  }
}

// @brief 全ビットをクリアする．
void
Xmask::clear_all()
{
  if ( mBits ) {
    ymuint32* end = mBits + blksize();
    for (ymuint32* bp = mBits; bp != end; ++ bp) {
      *bp = 0U;
    }
  }
}

// @brief 論理和を取る．
// @param[in] right オペランド
// @return 自分自身の参照を返す．
const Xmask&
Xmask::operator|=(const Xmask& right)
{
  ASSERT_COND( bit_width() == right.bit_width() );
  if ( right.has_x() ) {
    alloc();
    ymuint32* dst_end = mBits + blksize();
    ymuint32* src = right.mBits;
    for (ymuint32* dst = mBits; dst != dst_end; ++ dst, ++ src) {
      *dst |= *src;
    }
  }
  return *this;
}

// @brief ドントケアビットを含むときに true を返す．
bool
Xmask::has_x() const
{
  if ( mBits ) {
    ymuint32* end = mBits + blksize();
    for (ymuint32* bp = mBits; bp != end; ++ bp) {
      if ( *bp ) {
	return true;
      }
    }
  }
  return false;
}

// @brief ymuint32 のベクタに変換する．
void
Xmask::to_vector(vector<ymuint32>& vect) const
{
  ymuint n = blksize();
  vect.resize(n, 0U);
  if ( mBits ) {
    for (ymuint i = 0; i < n; ++ i) {
      vect[i] = mBits[i];
    }
  }
}

// @brief メモリ領域を確保する．
void
Xmask::alloc()
{
  if ( mBits == NULL ) {
    mBits = new ymuint32[blksize()];
  }
}

// @brief メモリ領域を解放する．
void
Xmask::free()
{
  delete mBits;
  mBits = NULL;
}

END_NAMESPACE_YM_NETWORKS_MVN_VERILOG
