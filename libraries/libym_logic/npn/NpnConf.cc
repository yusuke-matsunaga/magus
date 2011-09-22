
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
#include "ym_logic/NpnMgr.h"


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
  mOpol = src.mOpol;
  for (ymuint i = 0; i < mSig->ni(); ++ i) {
    mIpols[i] = src.mIpols[i];
  }
  mNc = src.mNc;
  for (ymuint i = 0; i < mNc; ++ i) {
    mIcList[i] = src.mIcList[i];
  }
  mGroupNum = src.mGroupNum;
  for (ymuint i = 0; i <= mGroupNum; ++ i) {
    mGroupTop[i] = src.mGroupTop[i];
  }
  mIorderValid = false;
}

// src に出力極性を追加した configuration をつくり出す
NpnConf::NpnConf(const NpnConf& src,
		 int pol)
{
  copy(src);
  mOpol = pol;
  assert_cond(pol == 1 || pol == -1, __FILE__, __LINE__);
  if ( pol == -1 ) {
    for (ymuint i = 0; i < mSig->ni(); ++ i) {
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
		 ymuint g,
		 ymuint c)
{
  mSig = src.mSig;
  mOpol = src.mOpol;
  for (ymuint i = 0; i < mSig->ni(); ++ i) {
    mIpols[i] = src.mIpols[i];
  }
  mIorderValid = false;
  mNc = src.mNc;
  ymuint b = src.group_begin(g);
  ymuint e = src.group_end(g);
  assert_cond(b <= c && c < e, __FILE__, __LINE__);
  for (ymuint i = 0; i < b; ++ i) {
    mIcList[i] = src.mIcList[i];
  }
  mIcList[b] = src.mIcList[c];
  ymuint k = b + 1;
  for (ymuint i = b; i < e; ++ i) {
    if ( i != c ) {
      mIcList[k] = src.mIcList[i];
      ++ k;
    }
  }
  assert_cond(k == e, __FILE__, __LINE__);
  for (ymuint i = e; i < mNc; ++ i) {
    mIcList[i] = src.mIcList[i];
  }

  mGroupNum = src.mGroupNum + 1;
  for (ymuint i = 0; i <= g; ++ i) {
    mGroupTop[i] = src.mGroupTop[i];
  }
  mGroupTop[g + 1] = b + 1;
  for (ymuint i = g + 1; i < mGroupNum; ++ i) {
    mGroupTop[i + 1] = src.mGroupTop[i];
  }
}

#if 0
// @brief 入力順序を正しくする．
void
NpnConf::validate_iorder() const
{
  ymuint k = 0;
  for (ymuint i = 0; i < nc(); ++ i) {
    ymuint rep = ic_rep(i);
    ymuint n = ic_num(rep);
    for (ymuint j = 0; j < n; ++ j) {
      mIorder[k] = rep;
      ++ k;
      rep = ic_link(rep);
    }
  }
  ymuint rep = indep_rep();
  ymuint n = indep_num();
  for (ymuint j = 0; j < n; ++ j) {
    mIorder[k] = rep;
    ++ k;
    rep = ic_link(rep);
  }
  mIorderValid = true;
}
#endif

// 完全な正規形になっているとき true を返す．
bool
NpnConf::is_resolved(ymuint g0) const
{
  if ( !is_opol_fixed() ) {
    return false;
  }
  for (ymuint g1 = g0; g1 < mGroupNum; ++ g1) {
    if ( group_size(g1) > 1 ) {
      return false;
    }
    ymuint pos = group_begin(g1);
    if ( ic_pol(pos) == 0 ) {
      return false;
    }
  }
  return true;
}

// @brief 重み別 Walsh の 0次係数を返す．
int
NpnConf::walsh_w0(ymuint w) const
{
  tPol op = (opol() == -1) ? kPolNega : kPolPosi;
  tPol ip[TvFunc::kMaxNi];
  for (ymuint i = 0; i < mSig->ni(); ++ i) {
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
NpnConf::set_ic_pol(ymuint pos,
		    int val)
{
  mIcList[pos] &= ~3;
  mIcList[pos] |= val;
  if ( val == 2 ) {
    ymuint rep = ic_rep(pos);
    ymuint n = mSig->ic_num(rep);
    if ( mSig->bisym(rep) ) {
      n = 1;
    }
    ymuint j = rep;
    for (ymuint i = 0; i < n; ++ i) {
      mIpols[j] = -1;
      j = mSig->ic_link(j);
    }
  }
}

// @brief 内容を NpnMap にセットする．
void
NpnConf::set_map(NpnMap& map) const
{
  ymuint order[TvFunc::kMaxNi];
  ymuint k = 0;
  for (ymuint i = 0; i < nc(); ++ i) {
    ymuint rep = ic_rep(i);
    ymuint n = mSig->ic_num(rep);
    for (ymuint j = 0; j < n; ++ j) {
      order[rep] = k;
      ++ k;
      rep = mSig->ic_link(rep);
    }
  }
  ymuint rep = mSig->indep_rep();
  ymuint n = mSig->indep_num();
  for (ymuint j = 0; j < n; ++ j) {
    order[rep] = k;
    ++ k;
    rep = mSig->ic_link(rep);
  }
  map.resize(mSig->ni());
  tPol opol0 = (mSig->opol() == -1) ? kPolNega : kPolPosi;
  tPol opol1 = (mOpol == -1) ? kPolNega : kPolPosi;
  map.set_opol(opol0 * opol1);
  for (ymuint i = 0; i < mSig->ni(); ++ i) {
    tPol ipol0 = (mSig->ipol(i) == -1) ? kPolNega : kPolPosi;
    tPol ipol1 = (ipol(i) == -1) ? kPolNega : kPolPosi;
    map.set(i, order[i], ipol0 * ipol1);
  }
}


BEGIN_NONAMESPACE
ymuint
fact(ymuint x)
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
  switch ( mOpol ) {
  case 1:  s << "P"; break;
  case -1: s << "N"; break;
  case 0:  s << "-"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;
  s << "ipol:";
  for (ymuint i = 0; i < mSig->ni(); ++ i) {
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
  ymuint cmb = 1;
  for (ymuint g = 0; g < group_num(); ++ g) {
    ymuint b = group_begin(g);
    ymuint e = group_end(g);
    cmb *= fact(group_size(g));
    for (ymuint j = b; j < e; ++ j) {
      ymuint pos1 = ic_rep(j);
      s << " { " << mSig->walsh_1(pos1);
      if ( mSig->bisym(pos1) ) {
	s << "*";
      }
      if ( ic_pol(j) == 0 ) {
	s << "?";
      }
      s << ": " << pos1;
      ymuint n = mSig->ic_num(pos1);
      for (ymuint k = 1; k < n; ++ k) {
	pos1 = mSig->ic_link(pos1);
	s << " " << pos1;
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
