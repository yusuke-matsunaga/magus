
/// @file TvFuncM.cc
/// @brief TvFuncM の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/TvFuncM.h"
#include "ym_logic/NpnMapM.h"


#if SIZEOF_SIZE_T == 8
// 1 ワード 64 ビットの時 1 となるマクロ
#define WORD64 1
// 1 ワード当たりの入力数
#define NIPW 6
#else
// 1 ワード 64 ビットの時 1 となるマクロ
#define WORD64 0
// 1 ワード当たりの入力数
#define NIPW 5
#endif

//#define DEBUG

BEGIN_NONAMESPACE

#if WORD64

// コファクターマスク
ymulong c_masks[] = {
  0xAAAAAAAAAAAAAAAA,
  0xCCCCCCCCCCCCCCCC,
  0xF0F0F0F0F0F0F0F0,
  0xFF00FF00FF00FF00,
  0xFFFF0000FFFF0000,
  0xFFFFFFFF00000000
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
ymulong sym_masks2[] = {
  0x2222222222222222, // (1, 0)
  0x0A0A0A0A0A0A0A0A, // (2, 0)
  0x0C0C0C0C0C0C0C0C, // (2, 1)
  0x00AA00AA00AA00AA, // (3, 0)
  0x00CC00CC00CC00CC, // (3, 1)
  0x00F000F000F000F0, // (3, 2)
  0x0000AAAA0000AAAA, // (4, 0)
  0x0000CCCC0000CCCC, // (4, 1)
  0x0000F0F00000F0F0, // (4, 2)
  0x0000FF000000FF00, // (4, 3)
  0x00000000AAAAAAAA, // (5, 0)
  0x00000000CCCCCCCC, // (5, 1)
  0x00000000F0F0F0F0, // (5, 2)
  0x00000000FF00FF00, // (5, 3)
  0x00000000FFFF0000  // (5, 4)
};

// 対称性を調べるためのテーブルその2
// 逆位相の時に用いる．
ymulong sym_masks3[] = {
  0x1111111111111111, // (1, 0)
  0x0505050505050505, // (2, 0)
  0x0303030303030303, // (2, 1)
  0x0055005500550055, // (3, 0)
  0x0033003300330033, // (3, 1)
  0x000F000F000F000F, // (3, 2)
  0x0000555500005555, // (4, 0)
  0x0000333300003333, // (4, 1)
  0x00000F0F00000F0F, // (4, 2)
  0x000000FF000000FF, // (4, 3)
  0x0000000055555555, // (5, 0)
  0x0000000033333333, // (5, 1)
  0x000000000F0F0F0F, // (5, 2)
  0x0000000000FF00FF, // (5, 3)
  0x000000000000FFFF  // (5, 4)
};

#else

// コファクターマスク
ymulong c_masks[] = {
  0xAAAAAAAA, // 10101010101010101010101010101010
  0xCCCCCCCC, // 11001100110011001100110011001100
  0xF0F0F0F0, // 11110000111100001111000011110000
  0xFF00FF00, // 11111111000000001111111100000000
  0xFFFF0000  // 11111111111111110000000000000000
};

// 対称性を調べるためのテーブルその1
// 同位相の時に用いる．
ymulong sym_masks2[] = {
  0x22222222, // (1, 0)
  0x0A0A0A0A, // (2, 0)
  0x0C0C0C0C, // (2, 1)
  0x00AA00AA, // (3, 0)
  0x00CC00CC, // (3, 1)
  0x00F000F0, // (3, 2)
  0x0000AAAA, // (4, 0)
  0x0000CCCC, // (4, 1)
  0x0000F0F0, // (4, 2)
  0x0000FF00  // (4, 3)
};

// 対称性を調べるためのテーブルその2
// 逆位相の時に用いる．
ymulong sym_masks3[] = {
  0x11111111, // (1, 0)
  0x05050505, // (2, 0)
  0x03030303, // (2, 1)
  0x00550055, // (3, 0)
  0x00330033, // (3, 1)
  0x000F000F, // (3, 2)
  0x00005555, // (4, 0)
  0x00003333, // (4, 1)
  0x00000F0F, // (4, 2)
  0x000000FF  // (4, 3)
};

#endif

END_NONAMESPACE

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////

// 入力数と出力数のみ指定したコンストラクタ
// 中身は恒偽関数
TvFuncM::TvFuncM(ymuint ni,
		 ymuint no) :
  mNi(ni),
  mNo(no),
  mNblk1(nblock(ni)),
  mNblk(mNblk1 * no),
  mVector(new ymulong[mNblk])
{
  for (ymuint i = 0; i < mNblk; ++ i) {
    mVector[i] = 0UL;
  }
}

// @brief TvFunc を用いたコンストラクタ
// @param[in] src_list 各出力の論理関数
// @note src_list の関数の入力数は等しくなければならない．
TvFuncM::TvFuncM(const vector<TvFunc>& src_list)
{
  assert_cond( !src_list.empty(), __FILE__, __LINE__);
  const TvFunc& first = src_list.front();
  ymuint ni = first.ni();
  ymuint no = src_list.size();
  for (ymuint i = 1; i < no; ++ i) {
    assert_cond( src_list[i].ni() == ni, __FILE__, __LINE__);
  }

  mNi = ni;
  mNo = no;
  mNblk1 = nblock(ni);
  mNblk = mNblk1 * no;
  mVector = new ymulong[mNblk];
  for (ymuint i = 0; i < no; ++ i) {
    ymuint offset = i * mNblk1;
    const TvFunc& src1 = src_list[i];
    for (ymuint j = 0; j < mNblk1; ++ j) {
      mVector[offset + j] = src1.raw_data(j);
    }
  }
}

// コピーコンストラクタ
TvFuncM::TvFuncM(const TvFuncM& src) :
  mNi(src.mNi),
  mNo(src.mNo),
  mNblk1(src.mNblk1),
  mNblk(src.mNblk),
  mVector(new ymulong[mNblk])
{
  ymulong* endp = mVector + mNblk;
  ymulong* src_bp = src.mVector;
  for (ymulong* bp = mVector; bp != endp; ++ bp, ++ src_bp) {
    *bp = *src_bp;
  }
}

// 代入演算子
const TvFuncM&
TvFuncM::operator=(const TvFuncM& src)
{
  if ( mNblk != src.mNblk ) {
    delete [] mVector;
    mNblk1 = src.mNblk1;
    mNblk = src.mNblk;
    mVector = new ymulong[mNblk];
  }
  mNi = src.mNi;
  mNo = src.mNo;

  ymulong* endp = mVector + mNblk;
  ymulong* src_bp = src.mVector;
  for (ymulong* bp = mVector; bp != endp; ++ bp, ++ src_bp) {
    *bp = *src_bp;
  }

  return *this;
}

// デストラクタ
TvFuncM::~TvFuncM()
{
  delete [] mVector;
}

// 自分自身を否定する．
const TvFuncM&
TvFuncM::negate()
{
  switch ( mNi ) {
  case 0:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0x1;
    }
    break;

  case 1:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0x3;
    }
    break;

  case 2:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0xF;
    }
    break;

  case 3:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0xFF;
    }
    break;

  case 4:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0xFFFF;
    }
    break;

  case 5:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0xFFFFFFFF;
    }
    break;

#if WORD64
  case 6:
    for (ymuint i = 0; i < mNo; ++ i) {
      mVector[i] ^= 0xFFFFFFFFFFFFFFFF;
    }
    break;
#endif

  default:
    {
      ymulong* endp = mVector + mNblk;
      for (ymulong* bp = mVector; bp != endp; ++ bp) {
	*bp ^= ~(0UL);
      }
    }
  }
  return *this;
}

// src1 との論理積を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator&=(const TvFuncM& src1)
{
  ymulong* endp = mVector + mNblk;
  ymulong* src_bp = src1.mVector;
  for (ymulong* bp = mVector; bp != endp; ++ bp) {
    *bp &= *src_bp;
  }
  return *this;
}

// src1 との論理和を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator|=(const TvFuncM& src1)
{
  ymulong* endp = mVector + mNblk;
  ymulong* src_bp = src1.mVector;
  for (ymulong* bp = mVector; bp != endp; ++ bp) {
    *bp |= *src_bp;
  }
  return *this;
}

// src1 との排他的論理和を計算し自分に代入する．
const TvFuncM&
TvFuncM::operator^=(const TvFuncM& src1)
{
  ymulong* endp = mVector + mNblk;
  ymulong* src_bp = src1.mVector;
  for (ymulong* bp = mVector; bp != endp; ++ bp) {
    *bp ^= *src_bp;
  }
  return *this;
}

// @brief コファクターを計算し自分に代入する．
// @param[in] pos 変数番号
// @param[in] pol 極性
// @return 自身への参照を返す．
const TvFuncM&
TvFuncM::set_cofactor(ymuint pos,
		      tPol pol)
{
  if ( pos < NIPW ) {
    ymulong mask = c_masks[pos];
    if ( pol == kPolNega ) {
      mask = ~mask;
    }
    ymulong* endp = mVector + mNblk;
    int shift = 1 << pos;
    for (ymulong* bp = mVector; bp != endp; ++ bp) {
      ymulong pat = *bp & mask;
      if ( pol == kPolPosi ) {
	pat |= (pat >> shift);
      }
      else {
	pat |= (pat << shift);
      }
      *bp = pat;
    }
  }
  else {
    pos -= NIPW;
    ymuint bit = 1U << pos;
    for (ymuint j = 0; j < mNo; ++ j) {
      ymuint offset = j * mNblk1;
      for (ymuint i = 0; i < mNblk1; ++ i) {
	if ( pol == kPolPosi ) {
	  if ( (i & bit) == 0U ) {
	    mVector[i + offset] = mVector[(i ^ bit) + offset];
	  }
	}
	else {
	  if ( (i & bit) == bit ) {
	    mVector[i + offset] = mVector[(i ^ bit) + offset];
	  }
	}
      }
    }
  }
  return *this;
}

// i 番目の変数がサポートの時 true を返す．
bool
TvFuncM::check_sup(tVarId i) const
{
  if ( i < NIPW ) {
    // ブロックごとにチェック
    ymuint dist = 1U << i;
    ymulong mask = c_masks[i];
    ymulong* endp = mVector + mNblk;
    for (ymulong* bp = mVector; bp != endp; ++ bp) {
      ymulong word = *bp;
      if ( (word ^ (word << dist)) & mask ) {
	return true;
      }
    }
  }
  else {
    // ブロック単位でチェック
    ymuint i5 = i - NIPW;
    ymuint check = 1U << i5;
    for (ymuint i = 0; i < mNo; ++ i) {
      ymuint offset = i * mNblk1;
      for (ymuint b = 0; b < mNblk1; ++ b) {
	if ( (b & check) && (mVector[b + offset] != mVector[(b ^ check) + offset]) ) {
	  return true;
	}
      }
    }
  }
  return false;
}

// i 番目と j 番目の変数が対称のとき true を返す．
bool
TvFuncM::check_sym(tVarId i,
		   tVarId j,
		   tPol pol) const
{
  // i と j を正規化する．
  if ( i < j ) {
    tVarId tmp = i;
    i = j;
    j = tmp;
  }
  // ここ以降では必ず i > j が成り立つ．

  bool ans = true;
  if ( j >= NIPW ) {
    // i >= NIPW (実際には i > NIPW)
    // j >= NIPW
    // ブロック単位で比較する．
    ymuint mask_i = (1U << (i - NIPW));
    ymuint mask_j = (1U << (j - NIPW));
    ymuint mask_all = mask_i | mask_j;
    ymuint cond;
    if ( pol == kPolPosi ) {
      cond = mask_j;
    }
    else {
      cond = 0UL;
    }
    for (ymuint i = 0; i < mNo; ++ i) {
      ymuint offset = i * mNblk1;
      for (ymuint v = 0; v < mNblk1; ++ v) {
	if ( (v & mask_all) == cond &&
	     mVector[v + offset] != mVector[(v ^ mask_all) + offset] ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else if ( i >= NIPW ) {
    // i >= NIPW
    // j < NIPW
    ymuint mask_i = (1U << (i - NIPW));
    ymuint cond;
    if ( pol == kPolPosi ) {
      cond = mask_i;
    }
    else {
      cond = 0UL;
    }
    ymuint mask2 = ~c_masks[j];
    ymuint s = 1U << j;
    for (ymuint i = 0; i < mNo; ++ i) {
      ymuint offset = i * mNblk1;
      for (ymuint v = 0; v < mNblk1; ++ v) {
	if ( (v & mask_i) == cond &&
	     (mVector[v + offset] ^ (mVector[(v ^ mask_i) + offset] >> s)) & mask2 ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  else {
    // i < NIPW
    // j < NIPW
    if ( pol == kPolPosi ) {
      ymulong mask = sym_masks2[(i * (i - 1)) / 2 + j];
      ymuint s = (1U << i) - (1U << j);
      ymulong* endp = mVector + mNblk;
      for (ymulong* bp = mVector; bp != endp; ++ bp) {
	ymulong word = *bp;
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
    else {
      ymulong mask = sym_masks3[(i * (i - 1)) / 2 + j];
      ymuint s = (1U << i) + (1U << j);
      ymulong* endp = mVector + mNblk;
      for (ymulong* bp = mVector; bp != endp; ++ bp) {
	ymuint word = *bp;
	if ( ((word >> s) ^ word) & mask ) {
	  ans = false;
	  break;
	}
      }
    }
  }
  return ans;
}

// npnmap に従った変換を行う．
TvFuncM
TvFuncM::xform(const NpnMapM& npnmap) const
{
  ymuint ni_pow = 1UL << mNi;

#if defined(DEBUG)
  cout << "xform" << endl
       << *this << endl
       << npnmap << endl;
#endif

  ymuint imask = 0UL;
  ymuint ipat[kMaxNi];
  for (ymuint i = 0; i < mNi; ++ i) {
    NpnVmap imap = npnmap.imap(i);
    if ( imap.pol() == kPolNega ) {
      imask |= (1UL << i);
    }
    ymuint j = imap.pos();
    ipat[i] = 1UL << j;
  }

  TvFuncM ans(mNi, mNo);

  for (ymuint o = 0; o < mNo; ++ o) {
    NpnVmap omap = npnmap.omap(o);
    ymuint dst_pos = omap.pos();
    ymuint omask = omap.pol() == kPolPosi ? 0UL : 1UL;

    for (ymuint i = 0; i < ni_pow; ++ i) {
      ymuint new_i = 0;
      ymuint tmp = i;
      for (ymuint b = 0; b < mNi; ++ b, tmp >>= 1) {
	if ( tmp & 1 ) {
	  new_i |= ipat[b];
	}
      }
      ymulong pat = (value(i ^ imask, o) ^ omask) << shift(new_i);
      ans.mVector[block(new_i) + dst_pos * mNblk1] |= pat;
    }
  }

#if defined(DEBUG)
  cout << ans << endl;
#endif

  return ans;
}

// ハッシュ値を返す．
ymuint
TvFuncM::hash() const
{
  ymulong ans = 0;
  for (ymuint i = 0; i < mNblk; ++ i) {
    ans ^= mVector[i];
  }
  return ans + mNi + (mNo << 8);
}

// 等価比較
bool
operator==(const TvFuncM& func1,
	   const TvFuncM& func2)
{
  if ( !TvFuncM::check_nio(func1, func2) ) {
    return false;
  }
  ymulong* endp = func1.mVector + func1.mNblk;
  ymulong* bp1 = func1.mVector;
  ymulong* bp2 = func2.mVector;
  for ( ; bp1 != endp; ++ bp1, ++ bp2) {
    if ( *bp1 != *bp2 ) {
      return false;
    }
  }
  return true;
}


// @brief 大小比較を実際に行う関数
bool
TvFuncM::lt(const TvFuncM& right) const
{
  ymulong* endp = mVector;
  ymulong* bp1 = mVector + mNblk;
  ymulong* bp2 = right.mVector + mNblk;
  while ( bp1 != endp ) {
    -- bp1;
    -- bp2;
    ymulong w1 = *bp1;
    ymulong w2 = *bp2;
    if ( w1 > w2 ) {
      return false;
    }
    if ( w1 < w2 ) {
      return true;
    }
  }
  return false;
}

// @relates TvFuncM
// @brief 交差チェック
bool
operator&&(const TvFuncM& func1,
	   const TvFuncM& func2)
{
  if ( !TvFuncM::check_nio(func1, func2) ) {
    return false;
  }
  ymulong* endp = func1.mVector;
  ymulong* bp1 = func1.mVector + func1.mNblk;
  ymulong* bp2 = func2.mVector + func1.mNblk;
  while ( bp1 != endp ) {
    -- bp1;
    -- bp2;
    ymulong w1 = *bp1;
    ymulong w2 = *bp2;
    if ( (w1 & w2) != 0U ) {
      return true;
    }
  }
  return false;
}

// 内容の出力
// mode は 2 か 16
void
TvFuncM::dump(ostream& s,
	      int mode) const
{
  ymuint ni_pow = 1UL << mNi;
  const ymuint wordsize = SIZEOF_SIZE_T * 8;
  if ( mode == 2 ) {
    ymulong* bp = mVector;
    ymuint offset = 0;
    ymulong tmp = *bp;
    const char* del = "";
    for (ymuint j = 0; j < mNo; ++ j) {
      s << del;
      del = "|";
      for (ymuint i = 0; i < ni_pow; ++ i) {
	s << (tmp & 1L);
	tmp >>= 1;
	++ offset;
	if ( offset == wordsize ) {
	  offset = 0;
	  ++ bp;
	  tmp = *bp;
	}
      }
    }
  }
  else if ( mode == 16 ) {
    ymuint ni_pow4 = ni_pow / 4;
    ymulong* bp = mVector;
    ymuint offset = 0;
    ymulong tmp = *bp;
    for (ymuint j = 0; j < mNo; ++ j) {
      const char* del = "";
      for (ymuint i = 0; i < ni_pow4; ++ i) {
	s << del;
	del = "|";
	ymulong tmp1 = (tmp & 0xF);
	if ( tmp1 < 10 ) {
	  s << static_cast<char>('0' + tmp1);
	}
	else {
	  s << static_cast<char>('A' + tmp1 - 10);
	}
	tmp >>= 4;
	offset += 4;
	if ( offset == wordsize ) {
	  offset = 0;
	  ++ bp;
	  tmp = *bp;
	}
      }
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

END_NAMESPACE_YM
