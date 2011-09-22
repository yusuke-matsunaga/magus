
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

// コンストラクタ
NpnConf::NpnConf() :
  mBaseConf(NULL),
  mOpol(0),
  mGroupNum(0),
  mIpolsValid(false),
  mIorderValid(false)
{
  mGroupTop[0] = 0;
}

// @brief NpnBaseConf を指定したコンストラクタ
NpnConf::NpnConf(const NpnBaseConf& base_conf)
{
  mBaseConf = &base_conf;
  mOpol = base_conf.opol();
  mGroupNum = 0;
  mIpolsValid = false;
  mIorderValid = false;

  if ( nc() > 0 ) {
    for (ymuint c = 0; c < nc(); ++ c) {
      ymuint pos = base_conf.ic_rep(c);
      ymuint pol = base_conf.ipol(pos);
      ymuint tmp = (pos << 2) | pol;
      mIcList[c] = tmp;
    }
    add_ig(0);
  }
}

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

// src に出力極性を追加した configuration をつくり出す
NpnConf::NpnConf(const NpnConf& src,
		 int pol)
{
  copy(src);
  mOpol = pol;
  assert_cond(pol == 1 || pol == 2, __FILE__, __LINE__);
}

// src からグループ g 内の c というクラスを切り出した
// configuration をつくり出す
NpnConf::NpnConf(const NpnConf& src,
		 ymuint g,
		 ymuint c)
{
  mBaseConf = src.mBaseConf;
  mOpol = src.mOpol;

  ymuint b = src.group_begin(g);
  ymuint e = src.group_end(g);
  assert_cond( b <= c && c < e, __FILE__, __LINE__);
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
  for (ymuint i = e; i < nc(); ++ i) {
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

// デストラクタ
NpnConf::~NpnConf()
{
}

// コピーを行う．
void
NpnConf::copy(const NpnConf& src)
{
  mBaseConf = src.mBaseConf;
  mOpol = src.mOpol;
  for (ymuint i = 0; i < nc(); ++ i) {
    mIcList[i] = src.mIcList[i];
  }
  mGroupNum = src.mGroupNum;
  for (ymuint i = 0; i <= mGroupNum; ++ i) {
    mGroupTop[i] = src.mGroupTop[i];
  }
  mIpolsValid = false;
  mIorderValid = false;
}

// @brief 入力の極性を正しくする．
void
NpnConf::validate_ipols() const
{
  for (ymuint i = 0; i < ni(); ++ i) {
    mIpols[i] = mBaseConf->ipol(i);
  }
  for (ymuint c = 0; c < nc(); ++ c) {
    ymuint ip = ic_pol(c);
    if ( ip == 2 ) {
      ymuint pos = ic_rep(c);
      ymuint n;
      if ( mBaseConf->bisym(pos) ) {
	n = 1;
      }
      else {
	n = mBaseConf->ic_num(pos);
      }
      for (ymuint j = 0; j < n; ++ j) {
	mIpols[pos] = 2;
	pos = mBaseConf->ic_link(pos);
      }
    }
  }
  mIpolsValid = true;
}

// @brief 入力順序を正しくする．
void
NpnConf::validate_iorder() const
{
  ymuint k = 0;
  for (ymuint i = 0; i < nc(); ++ i) {
    ymuint rep = ic_rep(i);
    ymuint n = mBaseConf->ic_num(rep);
    for (ymuint j = 0; j < n; ++ j) {
      mIorder[k] = rep;
      ++ k;
      rep = mBaseConf->ic_link(rep);
    }
  }
  ymuint rep = mBaseConf->indep_rep();
  ymuint n = mBaseConf->indep_num();
  for (ymuint j = 0; j < n; ++ j) {
    mIorder[k] = rep;
    ++ k;
    rep = mBaseConf->ic_link(rep);
  }
  assert_cond( k == ni(), __FILE__, __LINE__);
  mIorderValid = true;
}

// 完全な正規形になっているとき true を返す．
bool
NpnConf::is_resolved(ymuint g0) const
{
  if ( !is_opol_fixed() ) {
    return false;
  }
  for (ymuint g1 = g0; g1 < group_num(); ++ g1) {
    if ( group_size(g1) > 1 ) {
      // 複数のクラスが分離されていない．
      return false;
    }
    ymuint pos = group_begin(g1);
    if ( ic_pol(pos) == 0 ) {
      // 極性が確定していない．
      return false;
    }
  }
  return true;
}

// @brief 重み別 Walsh の 0次係数を返す．
int
NpnConf::walsh_w0(ymuint w) const
{
  tPol op = (opol() == 2) ? kPolNega : kPolPosi;
  ymuint32 ibits = 0UL;
  for (ymuint i = 0; i < ni(); ++ i) {
    if ( ipol(i) == 2 ) {
      ibits |= (1UL << i);
    }
  }
  return func().walsh_w0(w, op, ibits);
}

// 入力の極性の割り当ての設定
void
NpnConf::set_ic_pol(ymuint pos,
		    int val)
{
  mIcList[pos] &= ~3;
  mIcList[pos] |= val;
  mIpolsValid = false;
}

// @brief 内容を NpnMap にセットする．
void
NpnConf::set_map(NpnMap& map) const
{
#if 0
  ymuint order[TvFunc::kMaxNi];
  ymuint k = 0;
  for (ymuint i = 0; i < nc(); ++ i) {
    ymuint rep = ic_rep(i);
    ymuint n = mBaseConf->ic_num(rep);
    for (ymuint j = 0; j < n; ++ j) {
      order[rep] = k;
      ++ k;
      rep = mBaseConf->ic_link(rep);
    }
  }
  ymuint rep = mBaseConf->indep_rep();
  ymuint n = mBaseConf->indep_num();
  for (ymuint j = 0; j < n; ++ j) {
    order[rep] = k;
    ++ k;
    rep = mBaseConf->ic_link(rep);
  }
#else
  if ( !mIorderValid ) {
    validate_iorder();
  }
#endif
  if ( !mIpolsValid ) {
    validate_ipols();
  }

  map.resize(mBaseConf->ni());
  tPol opol = (mOpol == 2) ? kPolNega : kPolPosi;
  map.set_opol(opol);
  for (ymuint i = 0; i < mBaseConf->ni(); ++ i) {
    tPol ipol1 = (ipol(i) == 2) ? kPolNega : kPolPosi;
    map.set(mIorder[i], i, ipol1 * opol);
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
  case 0: s << "-"; break;
  case 1: s << "P"; break;
  case 2: s << "N"; break;
  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  s << endl;
  s << "ipol:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " ";
    switch ( ipol(i) ) {
    case 0: s << "-"; break;
    case 1: s << "P"; break;
    case 2: s << "N"; break;
    default:
      assert_not_reached(__FILE__, __LINE__);
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
      ymint w1 = func().walsh_1(pos1);
      if ( ipol(pos1) == 2 ) {
	w1 = -w1;
      }
      if ( opol() == 2 ) {
	w1 = -w1;
      }
      s << " { " << w1;
      if ( mBaseConf->bisym(pos1) ) {
	s << "*";
      }
      if ( ic_pol(j) == 0 ) {
	s << "?";
      }
      s << ": " << pos1;
      ymuint n = mBaseConf->ic_num(pos1);
      for (ymuint k = 1; k < n; ++ k) {
	pos1 = mBaseConf->ic_link(pos1);
	s << " " << pos1;
      }
      s << "}";
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
