
/// @file libym_npn/NpnConf.cc
/// @brief NpnConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnConf.cc 1508 2008-06-30 04:55:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#include "NpnConf.h"
#include "ym_npn/NpnMgr.h"


BEGIN_NAMESPACE_YM_NPN

// コピーコンストラクタ
NpnConf::NpnConf(const NpnConf& src)
{
  copy(src);
}

// 代入演算子
const NpnConf&
NpnConf::operator=(const NpnConf& src)
{
  copy(src);
  return *this;
}

// コピーを行う．
void
NpnConf::copy(const NpnConf& src)
{
  mSig = src.mSig;
  mOpolFixed = src.mOpolFixed;
  mOpol = src.mOpol;
  for (size_t i = 0; i < ni(); ++ i) {
    mIpols[i] = src.mIpols[i];
  }
  mNc = src.mNc;
  for (size_t i = 0; i < mNc; ++ i) {
    mIcList[i] = src.mIcList[i];
  }
  mNg = src.mNg;
  for (size_t i = 0; i <= mNg; ++ i) {
    mIndex[i] = src.mIndex[i];
  }
  mIorderValid = false;
}

// src に出力極性を追加した configuration をつくり出す
NpnConf::NpnConf(const NpnConf& src,
		 int pol)
{
  copy(src);
  mOpolFixed = true;
  mOpol = pol;
  assert_cond(pol == 1 || pol == -1, __FILE__, __LINE__);
  if ( pol == -1 ) {
    for (size_t i = 0; i < ni(); ++ i) {
      if ( mSig->walsh_1(i) != 0 ) {
	mIpols[i] *= -1;
      }
    }
  }
  mIorderValid = false;
}

// src からグループ g 内の c というクラスを切り出した
// configuration をつくり出す
NpnConf::NpnConf(const NpnConf& src,
		 size_t g,
		 size_t c)
{
  mSig = src.mSig;
  mOpolFixed = src.mOpolFixed;
  mOpol = src.mOpol;
  for (size_t i = 0; i < ni(); ++ i) {
    mIpols[i] = src.mIpols[i];
  }
  mIorderValid = false;
  mNc = src.mNc;
  size_t b = src.begin(g);
  size_t e = src.end(g);
  assert_cond(b <= c && c < e, __FILE__, __LINE__);
  for (size_t i = 0; i < b; ++ i) {
    mIcList[i] = src.mIcList[i];
  }
  mIcList[b] = src.mIcList[c];
  size_t k = b + 1;
  for (size_t i = b; i < e; ++ i) {
    if ( i != c ) {
      mIcList[k] = src.mIcList[i];
      ++ k;
    }
  }
  assert_cond(k == e, __FILE__, __LINE__);
  for (size_t i = e; i < mNc; ++ i) {
    mIcList[i] = src.mIcList[i];
  }

  mNg = src.mNg + 1;
  for (size_t i = 0; i <= g; ++ i) {
    mIndex[i] = src.mIndex[i];
  }
  mIndex[g + 1] = b + 1;
  for (size_t i = g + 1; i < mNg; ++ i) {
    mIndex[i + 1] = src.mIndex[i];
  }
}

// @brief 入力順序を正しくする．
void
NpnConf::validate_iorder() const
{
  size_t k = 0;
  for (size_t i = 0; i < nc(); ++ i) {
    size_t rep = ic_rep(i);
    size_t n = ic_num(rep);
    for (size_t j = 0; j < n; ++ j) {
      mIorder[k] = rep;
      ++ k;
      rep = ic_link(rep);
    }
  }
  size_t rep = indep_rep();
  size_t n = indep_num();
  for (size_t j = 0; j < n; ++ j) {
    mIorder[k] = rep;
    ++ k;
    rep = ic_link(rep);
  }
  mIorderValid = true;
}

// 完全な正規形になっているとき true を返す．
bool
NpnConf::is_resolved(size_t g0) const
{
  if ( !mOpolFixed ) {
    return false;
  }
  for (size_t g1 = g0; g1 < mNg; ++ g1) {
    if ( gnum(g1) > 1 ) {
      return false;
    }
    size_t pos = begin(g1);
    if ( ic_pol(pos) == 0 ) {
      return false;
    }
  }
  return true;
}
  
// @brief 重み別 Walsh の 0次係数を返す．
int
NpnConf::walsh_w0(size_t w) const
{
  tPol op = (opol() == -1) ? kPolNega : kPolPosi;
  tPol ip[kNpnMaxNi];
  for (size_t i = 0; i < ni(); ++ i) {
    if ( ipol(i) == -1 ) {
      ip[i] = kPolNega;
    }
    else {
      ip[i] = kPolPosi;
    }
  }
  return mSig->walsh_w0(w, op, ip);
}

// 入力の極性の割り当ての設定
void
NpnConf::set_ic_pol(size_t pos,
		    int val)
{
  mIcList[pos] &= ~3;
  mIcList[pos] |= val;
  if ( val == 2 ) {
    size_t rep = ic_rep(pos);
    size_t n = mSig->ic_num(rep);
    if ( mSig->bisym(rep) ) {
      n = 1;
    }
    size_t j = rep;
    for (size_t i = 0; i < n; ++ i) {
      mIpols[j] = -1;
      j = mSig->ic_link(j);
    }
  }
}
  
// @brief 内容を NpnMap にセットする．
void
NpnConf::set_map(NpnMap& map) const
{
  size_t order[kNpnMaxNi];
  size_t k = 0;
  for (size_t i = 0; i < nc(); ++ i) {
    size_t rep = ic_rep(i);
    size_t n = ic_num(rep);
    for (size_t j = 0; j < n; ++ j) {
      order[rep] = k;
      ++ k;
      rep = ic_link(rep);
    }
  }
  size_t rep = indep_rep();
  size_t n = indep_num();
  for (size_t j = 0; j < n; ++ j) {
    order[rep] = k;
    ++ k;
    rep = ic_link(rep);
  }
  map.resize(ni());
  tPol opol0 = (mSig->opol() == -1) ? kPolNega : kPolPosi;
  tPol opol1 = (mOpol == -1) ? kPolNega : kPolPosi;
  map.set_opol(opol0 * opol1);
  for (size_t i = 0; i < ni(); ++ i) {
    tPol ipol0 = (mSig->ipol(i) == -1) ? kPolNega : kPolPosi;
    tPol ipol1 = (ipol(i) == -1) ? kPolNega : kPolPosi;
    map.set(i, order[i], ipol0 * ipol1);
  }
}


BEGIN_NONAMESPACE
size_t
fact(size_t x)
{
  if ( x <= 2 ) {
    return x;
  }
  return x * fact(x - 1);
}
END_NONAMESPACE

// @brief 内容を出力する．
void
NpnConf::dump(ostream& s) const
{
  s << "opol: ";
  if ( mOpolFixed ) {
    if ( mOpol == 1 ) {
      s << "P";
    }
    else {
      s << "N";
    }
  }
  else {
    s << "-";
  }
  s << endl;
  s << "ipol:";
  for (size_t i = 0; i < mSig->ni(); ++ i) {
    s << " ";
    if ( mIpols[i] == 1 ) {
      s << "P";
    }
    else {
      s << "N";
    }
  }
  s << endl;
  s << "Input groups" << endl;
  size_t cmb = 1;
  for (size_t g = 0; g < ng(); ++ g) {
    size_t b = begin(g);
    size_t e = end(g);
    cmb *= fact(gnum(g));
    for (size_t j = b; j < e; ++ j) {
      size_t pos1 = ic_rep(j);
      s << " { " << mSig->walsh_1(pos1);
      if ( bisym(pos1) ) {
	s << "*";
      }
      if ( ic_pol(j) == 0 ) {
	s << "?";
      }
      s << ": " << pos1;
      size_t n = ic_num(pos1);
      for (size_t k = 1; k < n; ++ k) {
	pos1 = ic_link(pos1);
	s << " " << pos1;
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
