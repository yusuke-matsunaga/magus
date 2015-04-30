
/// @file NpnBaseConf.cc
/// @brief NpnBaseConf の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmLogic/TvFunc.h"

#include "NpnBaseConf.h"


const int debug_normalize = 0x0001;
const int debug_none      = 0x0000;
const int debug_all       = 0xffff;


BEGIN_NAMESPACE_YM_NPN

extern
int debug;

//////////////////////////////////////////////////////////////////////
// NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnBaseConf::NpnBaseConf()
{
}

// デストラクタ
NpnBaseConf::~NpnBaseConf()
{
#if USE_MALLOC
  delete [] mW1;
  delete [] mW2;
  delete [] mW2flag;
  delete [] mIpols;
  delete [] mIcRep;
  delete [] mIcNum;
  delete [] mIcLink;
#endif
}

// @brief W0/W1 を用いて正規化する．
// @param[in] func 対象の関数
void
NpnBaseConf::normalize(const TvFunc& func)
{
  mFunc = func;
  mInputNum = mFunc.input_num();

#if USE_MALLOC
  mW1 = new ymint32[mInputNum];
  mW2 = new ymint32[mInputNum * mInputNum];
  mW2flag = new ymuint8[mInputNum * mInputNum];
  mIpols = new ymuint8[mInputNum];
  mIcRep = new ymuint32[mInputNum];
  mIcNum = new ymuint32[mInputNum];
  mIcLink = new ymuint32[mInputNum];
#endif
  for (ymuint i = 0; i < mInputNum; ++ i) {
    ymuint base = i * mInputNum;
    for (ymuint j = 0; j < mInputNum; ++ j) {
      mW2flag[base + j] = 0;
    }
  }
  for (ymuint i = 0; i < mInputNum; ++ i) {
    mIcNum[i] = 0;
    mIcLink[i] = static_cast<ymuint>(-1);
  }
  // Walsh の0次と1次の係数を計算する．
  // 2次の係数はオンデマンドで計算する．
  mW0 = mFunc.walsh_01(mW1);

  if ( debug & debug_normalize ) {
    cout << "Before normalize" << endl;
    cout << mFunc << endl;
    print_walsh(cout);
  }

  // mW0 が非負になるように出力極性の調整を行う．
  if ( mW0 < 0 ) {
    mOpol = 2;
    mW0 = -mW0;
    for (ymuint i = 0; i < mInputNum; ++ i) {
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
  for (ymuint i = 0; i < mInputNum; ++ i) {
    // w1 が非負になるように調整する．
    // w1 が 0 の時には実際のサポートかどうかも調べる．
    if ( mW1[i] < 0 ) {
      mW1[i] = -mW1[i];
      mIpols[i] = 2;
    }
    else if ( mW1[i] == 0 ) {
      bool stat = mFunc.check_sup(VarId(i));
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
      VarId var(i);
      VarId var1(pos1);
      bool inv = (mIpols[pos1] * mIpols[i] == 2);
      bool stat = mFunc.check_sym(var, var1, inv);
      if ( stat ) {
	// 対称だった
	found = true;
	if ( mW1[pos1] == 0 && ic_num(pos1) == 1 ) {
	  // bi-symmetry かどうかチェックする
	  bool stat = mFunc.check_sym(var, var1, !inv);
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
	// この場合，最初の要素の極性は常に false のはず
	bool stat = mFunc.check_sym(var, var1, true);
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

  if ( debug & debug_normalize ) {
    cout << "After normalize" << endl;
    print_walsh(cout);
    print_pols(cout);
    cout << "NpnBaseConf::normalize() end" << endl
	 << endl;
  }
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
  for (ymuint i = 0; i < input_num(); ++ i) {
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
NpnBaseConf::print_walsh(ostream& s) const
{
  s << "W0: " << mW0 << endl
    << "w1:";
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << " " << mW1[i];
  }
  s << endl;

  s << "W2:" << endl;
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << "   ";
    for (ymuint j = 0; j < input_num(); ++ j) {
      int w2 = func().walsh_2(VarId(i), VarId(j));
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
}

// @brief 極性情報を出力する．
void
NpnBaseConf::print_pols(ostream& s) const
{
  s << "opol: ";
  switch ( opol() ) {
  case 0: s << "-"; break;
  case 1: s << "P"; break;
  case 2: s << "N"; break;
  default: ASSERT_NOT_REACHED;
  }
  s << endl
    << "ipol:";
  for (ymuint i = 0; i < input_num(); ++ i) {
    s << " ";
    switch ( ipol(i) ) {
    case 0: s << "-"; break;
    case 1: s << "P"; break;
    case 2: s << "N"; break;
    default: ASSERT_NOT_REACHED;
    }
  }
  s << endl;
}

END_NAMESPACE_YM_NPN
