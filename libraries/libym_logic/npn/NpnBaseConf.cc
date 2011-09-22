
/// @file libym_npn/NpnBaseConf.cc
/// @brief NpnBaseConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/TvFunc.h"

#include "NpnBaseConf.h"
#include "NpnConf.h"


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
  mIpols = new ymint8[mNi];
  mIcRep = new ymuint32[mNi];
  mIcNum = new ymuint32[mNi];
  mIcLink = new ymuint32[mNi];
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
NpnBaseConf::normalize(NpnConf& conf)
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
    mOpol = -1;
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
    init_ic(i);

    // w1 が非負になるように調整する．
    // w1 が 0 の時には実際のサポートかどうかも調べる．
    int w1 = mW1[i];
    if ( w1 < 0 ) {
      w1 = -w1;
      mIpols[i] = -1;
    }
    else if ( w1 == 0 ) {
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
    for (ymuint p = 0; p < mNc; ++ p) {
      ymuint pos1 = mIcRep[p];
      if ( mW1[i] != mW1[pos1] ) {
	continue;
      }
      // 1次係数が等しい場合
      // 対称性のチェックを行う．
      tPol poldiff = (mIpols[pos1] * mIpols[i] == -1) ? kPolNega : kPolPosi;
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
	break;
      }
      if ( w1 == 0 ) {
	// w1 == 0 の時には逆相での対称性もチェックする．
	// この場合，最初の要素の極性は常に kPolPosi のはず
	bool stat = mFunc.check_sym(i, pos1, kPolNega);
	if ( stat ) {
	  // 逆相で対称だった．
	  found = true;
	  add_elem(pos1, i);
	  mIpols[i] = -1;
	  break;
	}
      }
    }
    if ( !found ) {
      // 対称な入力が見つからなかった時には新たな入力クラスをつくる．
      mIcRep[mNc] = i;
      ++ mNc;
    }
  }

  if ( mOpol == 0 ) {
    // もしも入力の極性がすべて決まっていれば
    // w2 の最大値と最小値の絶対値の大きい方の出力極性を選ぶ．
    // 等しいときには総和を正にする．
    int min = walsh_2(0, 0);
    int max = min;
    int sum = 0;
    bool valid = true;
    for (ymuint i = 0; i < mNi; ++ i) {
      if ( mW1[i] == 0 ) {
	valid = false;
	break;
      }
      for (ymuint j = 0; j < mNi; ++ j) {
	int w2 = walsh_2(i, j);
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
	mOpol = -1;
      }
      else if ( -min < max ) {
	mOpol = 1;
      }
      else { // -min == max
	if ( sum < 0 ) {
	  mOpol = -1;
	}
	else if ( sum > 0 ) {
	  mOpol = 1;
	}
      }
    }
  }

  if ( debug & debug_normalize ) {
    cout << "After normalize" << endl;
    dump_walsh(cout);
    dump_pols(cout);
  }

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

}

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
  s << "W2:" << endl;
  for (ymuint i = 0; i < ni(); ++ i) {
    s << "   ";
    for (ymuint j = 0; j < ni(); ++ j) {
      s << " " << setw(4) << walsh_2(i, j);
    }
    s << endl;
  }
  s << endl;
}

// @brief 極性情報を出力する．
void
NpnBaseConf::dump_pols(ostream& s) const
{
  s << "opol: ";
  if ( mOpol == -1 ) {
    s << "N";
  }
  else if ( mOpol == 1 ) {
    s << "P";
  }
  else {
    s << "-";
  }
  s << endl
    << "ipol:";
  for (ymuint i = 0; i < ni(); ++ i) {
    s << " ";
    if ( mIpols[i] == -1 ) {
      s << "N";
    }
    else if ( mIpols[i] == 1 ) {
      s << "P";
    }
    else {
      s << "-";
    }
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
