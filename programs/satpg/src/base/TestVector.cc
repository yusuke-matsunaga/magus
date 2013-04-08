
/// @file src/base/TestVector.cc
/// @brief TestVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TestVector.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2009, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "TestVector.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
// @param[in] 入力数を指定する．
TestVector::TestVector(ymuint input_num) :
  mInputNum(input_num)
{
}

// @brief デストラクタ
TestVector::~TestVector()
{
}

// @brief X の個数を得る．
ymuint
TestVector::x_num() const
{
  ymuint ni = input_num();
  ymuint nb = block_num(ni);
  ymint n = 0;
  for (ymuint i = 0; i < nb; i += 2) {
    PackedVal v = mPat[i] | mPat[i + 1];
    // v 中の1の数を数える．
    n += count_ones(v);
  }
  return ni - n;
}

// @brief 2つのベクタが両立しないとき true を返す．
bool
TestVector::is_conflict(const TestVector& tv1,
			const TestVector& tv2)
{
  assert_cond( tv1.input_num() == tv2.input_num(), __FILE__, __LINE__);
  ymuint nb = block_num(tv1.input_num());
  for (ymuint i = 0; i < nb; i += 2) {
    ymuint i0 = i;
    ymuint i1 = i + 1;
    PackedVal diff0 = (tv1.mPat[i0] ^ tv2.mPat[i0]);
    PackedVal diff1 = (tv1.mPat[i1] ^ tv2.mPat[i1]);
    if ( (diff0 & diff1) != kPvAll0 ) {
      return true;
    }
  }
  return false;
}

// @brief すべて未定(X) で初期化する．
void
TestVector::init()
{
  ymuint nb = block_num(input_num());
  for (ymuint i = 0; i < nb; i += 2) {
    ymuint i0 = i;
    ymuint i1 = i + 1;
    mPat[i0] = kPvAll0;
    mPat[i1] = kPvAll0;
  }
}

// @brief HEX文字列から内容を設定する．
// @param[in] hex_string HEX 文字列
// @return hex_string に不適切な文字が含まれていたら false を返す．
// @note hex_string が短い時には残りは0で初期化される．
// @note hex_string が長い時には余りは捨てられる．
bool
TestVector::set_from_hex(const string& hex_string)
{
  // よく問題になるが，ここでは最下位ビット側から入力する．
  ymuint nl = hex_length(input_num());
  ymuint sft = 0;
  ymuint blk = 0;
  PackedVal pat = kPvAll0;
  for (ymuint i = 0; i < nl; ++ i) {
    char c = (i < hex_string.size()) ? hex_string[i] : '0';
    PackedVal pat1 = kPvAll0;
    if ( '0' <= c && c <= '9' ) {
      pat1 = static_cast<PackedVal>(c - '0');
    }
    else if ( 'a' <= c && c <= 'f' ) {
      pat1 = static_cast<PackedVal>(c - 'a' + 10);
    }
    else if ( 'A' <= c && c <= 'F' ) {
      pat1 = static_cast<PackedVal>(c - 'A' + 10);
    }
    else {
      return false;
    }
    pat |= (pat1 << sft);
    sft += 4;
    if ( sft == kPvBitLen ) {
      mPat[blk] = ~pat;
      mPat[blk + 1] = pat;
      sft = 0;
      blk += 2;
      pat = kPvAll0;
    }
  }
  if ( sft != 0 ) {
    mPat[blk] = ~pat;
    mPat[blk + 1] = pat;
  }

  return true;
}

// @brief 乱数パタンを設定する．
// @param[in] randgen 乱数生成器
void
TestVector::set_from_random(RandGen& randgen)
{
  ymuint nb = block_num(input_num());
  for (ymuint i = 0; i < nb; i += 2) {
    PackedVal v = randgen.ulong();
    mPat[i] = ~v;
    mPat[i + 1] = v;
  }
}

// @brief テストベクタをコピーする．
// @param[in] src コピー元のテストベクタ
// @note X の部分はコピーしない．
void
TestVector::copy(const TestVector& src)
{
  ymuint nb = block_num(input_num());
  for (ymuint i = 0; i < nb; i += 2) {
    ymuint i0 = i;
    ymuint i1 = i + 1;
    PackedVal mask = src.mPat[i0] | src.mPat[i1];
    mPat[i0] &= ~mask;
    mPat[i0] |= src.mPat[i0];
    mPat[i1] &= ~mask;
    mPat[i1] |= src.mPat[i1];
  }
}

// @breif テストベクタをマージする．
// @note X 以外で相異なるビットがあったら false を返す．
bool
TestVector::merge(const TestVector& src)
{
  ymuint nb = block_num(input_num());

  // コンフリクトチェック
  for (ymuint i = 0; i < nb; i += 2) {
    ymuint i0 = i;
    ymuint i1 = i + 1;
    PackedVal diff0 = (mPat[i0] ^ src.mPat[i0]);
    PackedVal diff1 = (mPat[i1] ^ src.mPat[i1]);
    if ( (diff0 & diff1) != kPvAll0 ) {
      return false;
    }
  }

  // 実際のマージ
  for (ymuint i = 0; i < nb; i += 2) {
    ymuint i0 = i;
    ymuint i1 = i + 1;
    mPat[i0] |= src.mPat[i0];
    mPat[i1] |= src.mPat[i1];
  }
  return true;
}

// @brief 内容を BIN 形式で出力する．
void
TestVector::dump_bin(ostream& s) const
{
  // よく問題になるが，ここでは最下位ビット側から出力する．
  for (ymuint i = 0; i < input_num(); ++ i) {
    switch ( val3(i) ) {
    case kVal0: s << '0'; break;
    case kVal1: s << '1'; break;
    case kValX: s << 'X'; break;
    default:    s << '-'; break;
    }
  }
}

// @brief 内容を HEX 形式で出力する．
void
TestVector::dump_hex(ostream& s) const
{
  // よく問題になるが，ここでは最下位ビット側から出力する．
  ymuint tmp = 0U;
  ymuint bit = 1U;
  for (ymuint i = 0; ; ++ i) {
    if ( i < input_num() ) {
      if ( val3(i) == kVal1 ) {
	// 面倒くさいので kValX は kVal0 と同じとみなす．
	tmp += bit;
      }
      bit <<= 1;
      if ( bit != 16U ) {
	continue;
      }
    }
    else if ( bit == 1U ) {
      break;
    }

    if ( tmp <= 9 ) {
      s << static_cast<char>('0' + tmp);
    }
    else {
      s << static_cast<char>('A' + tmp - 10);
    }
    bit = 1U;
    tmp = 0U;
  }
}

// 暫定的に用意する関数
void
TestVector::dump(FILE* fp) const
{
  for (ymuint i = 0; i < input_num(); ++ i) {
    putc((val3(i) == kVal1 ? '1' : '0'), fp);
  }
  putc('\n', fp);
}

END_NAMESPACE_YM_SATPG
