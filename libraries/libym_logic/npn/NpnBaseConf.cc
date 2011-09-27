
/// @file libym_npn/NpnBaseConf.cc
/// @brief NpnBaseConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/TvFunc.h"

#include "NpnBaseConf.h"


const int debug_normalize = 0x0001;
const int debug_none      = 0x0000;
const int debug_all       = 0xffff;

#ifdef YM_DEBUG
#define DEBUG_FLAG debug_all
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG debug_none
#endif

int debug = DEBUG_FLAG;


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
// NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// @param[in] func 対象の関数
NpnBaseConf::NpnBaseConf(const TvFunc& func) :
  mFunc(func)
{
  mNi = mFunc.ni();
  mW1 = new ymint32[mNi];
  mW2 = new ymint32[mNi * mNi];
  mW2flag = new ymuint8[mNi * mNi];
  mIpols = new ymuint8[mNi];
  mIcRep = new ymuint32[mNi];
  mIcNum = new ymuint32[mNi];
  mIcLink = new ymuint32[mNi];

  for (ymuint i = 0; i < mNi; ++ i) {
    mIcNum[i] = 0;
    mIcLink[i] = static_cast<ymuint>(-1);
  }
}

// デストラクタ
NpnBaseConf::~NpnBaseConf()
{
  delete [] mW1;
  delete [] mW2;
  delete [] mW2flag;
  delete [] mIpols;
  delete [] mIcRep;
  delete [] mIcNum;
  delete [] mIcLink;
}

// @brief W0/W1 を用いて正規化する．
void
NpnBaseConf::normalize()
{
  // Walsh の0次と1次の係数を計算する．
  // 2次の係数はオンデマンドで計算する．
  mW0 = mFunc.walsh_01(mW1);
  for (ymuint i = 0; i < mNi; ++ i) {
    for (ymuint j = 0; j < mNi; ++ j) {
      mW2flag[i * mNi + j] = 0;
    }
  }

  if ( debug & debug_normalize ) {
    cout << "Before normalize" << endl;
    cout << mFunc << endl;
    dump_walsh(cout);
  }

  // mW0 が非負になるように出力極性の調整を行う．
  if ( mW0 < 0 ) {
    mOpol = 2;
    mW0 = -mW0;
    for (ymuint i = 0; i < mNi; ++ i) {
      mW1[i] = -mW1[i];
    }
  }
  else if ( mW0 == 0 ) {
    mOpol = 0;
  }
  else { // mW0 > 0
    mOpol = 1;
  }

  // Walsh の1次係数を計算し，極性の調整を行う．
  // 同時に等価入力クラスをつくる．
  mNc = 0;
  mIndepNum = 0;
  for (ymuint i = 0; i < mNi; ++ i) {
    // w1 が非負になるように調整する．
    // w1 が 0 の時には実際のサポートかどうかも調べる．
    if ( mW1[i] < 0 ) {
      mW1[i] = -mW1[i];
      mIpols[i] = 2;
    }
    else if ( mW1[i] == 0 ) {
      bool stat = mFunc.check_sup(i);
      if ( !stat ) {
	// この入力はサポートではなかった
	add_indep(i);
	// 意味はないけど極性は決まったものとみなす．
	mIpols[i] = 1;
	continue;
      }
      else {
	mIpols[i] = 0;
      }
    }
    else {
      mIpols[i] = 1;
    }

    // 等価な入力があるかどうか調べる．
    bool found = false;
    for (ymuint cid = 0; cid < mNc; ++ cid) {
      ymuint pos1 = mIcRep[cid];
      if ( mW1[i] != mW1[pos1] ) {
	continue;
      }
      // 1次係数が等しい場合
      // 対称性のチェックを行う．
      tPol poldiff = (mIpols[pos1] * mIpols[i] == 2) ? kPolNega : kPolPosi;
      bool stat = mFunc.check_sym(i, pos1, poldiff);
      if ( stat ) {
	// 対称だった
	found = true;
	if ( mW1[pos1] == 0 && ic_num(pos1) == 1 ) {
	  // bi-symmetry かどうかチェックする
	  bool stat = mFunc.check_sym(i, pos1, ~poldiff);
	  if ( stat ) {
	    set_bisym(pos1);
	  }
	}
	add_elem(pos1, i);
	// bi-symmetry なら2番め以降の入力の極性は固定できる．
	if ( bisym(pos1) ) {
	  mIpols[i] = 1;
	}
	break;
      }
      if ( mW1[i] == 0 ) {
	// w1 == 0 の時には逆相での対称性もチェックする．
	// この場合，最初の要素の極性は常に kPolPosi のはず
	bool stat = mFunc.check_sym(i, pos1, kPolNega);
	if ( stat ) {
	  // 逆相で対称だった．
	  found = true;
	  add_elem(pos1, i);
	  mIpols[i] = 2;
	  break;
	}
      }
    }
    if ( !found ) {
      // 対称な入力が見つからなかった時には新たな入力クラスをつくる．
      new_ic(i);
    }
  }

#if 0
  if ( mOpol == 0 ) {
    // もしも入力の極性がすべて決まっていれば
    // w2 の最大値と最小値の絶対値の大きい方の出力極性を選ぶ．
    // 等しいときには総和を正にする．
    int min = func().walsh_2(0, 0);
    int max = min;
    int sum = 0;
    bool valid = true;
    for (ymuint i = 0; i < mNi; ++ i) {
      if ( mIpols[i] == 0 ) {
	valid = false;
	break;
      }
      for (ymuint j = 0; j <= i; ++ j) {
	int w2 = func().walsh_2(i, j);
	if ( mIpols[i] == 2 ) {
	  w2 = -w2;
	}
	if ( mIpols[j] == 2 ) {
	  w2 = -w2;
	}
	if ( min > w2 ) {
	  min = w2;
	}
	if ( max < w2 ) {
	  max = w2;
	}
	sum += w2;
      }
    }
    if ( valid ) {
      if ( -min > max ) {
	mOpol = 2;
      }
      else if ( -min < max ) {
	mOpol = 1;
      }
      else { // -min == max
	if ( sum < 0 ) {
	  mOpol = 2;
	}
	else if ( sum > 0 ) {
	  mOpol = 1;
	}
      }
    }
  }
#endif

  if ( debug & debug_normalize ) {
    cout << "After normalize" << endl;
    dump_walsh(cout);
    dump_pols(cout);
    cout << "NpnBaseConf::normalize() end" << endl
	 << endl;
  }

#if 0
  if ( mNc > 0 ) {
    for (ymuint i = 0; i < mNc; ++ i) {
      ymuint pos = mIcRep[i];
      int ipol1 = (mW1[pos] == 0) ? 0 : 1;
      conf.add_ic_rep(pos, ipol1);
    }
    // 最初はひとかたまりのグループにしておく．
    conf.add_ig(0);
  }
  if ( mOpol != 0 ) {
    conf.set_opol(1);
  }
  //conf.set_sig(this);
#endif
}

#if 0
// @brief 重み別 Walsh の 0次係数を得る．
int
NpnBaseConf::walsh_w0(ymuint w,
		    tPol opol,
		    tPol ipol[]) const
{
  if ( mOpol == -1 ) {
    opol = ~opol;
  }
  ymuint32 ibits = 0UL;
  for (ymuint i = 0; i < ni(); ++ i) {
    tPol ip = ipol[i];
    if ( mIpols[i] == -1 ) {
      ip = ~ip;
    }
    if ( ip == kPolNega ) {
      ibits |= (1UL << i);
    }
  }
  return mFunc.walsh_w0(w, opol, ibits);
}
#endif

// @brief Walsh 係数を出力する．
void
NpnBaseConf::dump_walsh(ostream& s) const
{
  s << "W0: " << mW0 << endl
    << "w1:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " " << mW1[i];
  }
  s << endl;
#if 1
  s << "W2:" << endl;
  for (ymuint i = 0; i < ni(); ++ i) {
    s << "   ";
    for (ymuint j = 0; j < ni(); ++ j) {
      int w2 = func().walsh_2(i, j);
      if ( ipol(i) == 2 ) {
	w2 = -w2;
      }
      if ( ipol(j) == 2 ) {
	w2 = -w2;
      }
      if ( opol() == 2 ) {
	w2 = -w2;
      }
      s << " " << setw(4) << w2;
    }
    s << endl;
  }
  s << endl;
#endif
}

// @brief 極性情報を出力する．
void
NpnBaseConf::dump_pols(ostream& s) const
{
  s << "opol: ";
  switch ( opol() ) {
  case 0: s << "-"; break;
  case 1: s << "P"; break;
  case 2: s << "N"; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  s << endl
    << "ipol:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " ";
    switch ( ipol(i) ) {
    case 0: s << "-"; break;
    case 1: s << "P"; break;
    case 2: s << "N"; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
