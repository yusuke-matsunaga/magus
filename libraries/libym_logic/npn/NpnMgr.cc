
/// @file libym_npn/NpnMgr.cc
/// @brief NpnMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NpnMgr.cc 1510 2008-06-30 05:11:10Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/NpnMgr.h"
#include "NpnBaseConf.h"
#include "NpnConf.h"


BEGIN_NONAMESPACE

const int debug_step1      = 0x0002;
const int debug_ww0_refine = 0x0001;
const int debug_step2      = 0x0004;
const int debug_step3      = 0x0008;
const int debug_step4      = 0x0010;
const int debug_w2max      = 0x0020;
const int debug_final      = 0x1000;
const int debug_final_func = 0x0100;
const int debug_none       = 0x0000;
const int debug_all        = 0xffff;

#ifdef YM_DEBUG
#define DEBUG_FLAG debug_all
#endif

#ifndef DEBUG_FLAG
#define DEBUG_FLAG debug_none
#endif

int debug = DEBUG_FLAG;

END_NONAMESPACE


BEGIN_NAMESPACE_YM_NPN


//////////////////////////////////////////////////////////////////////
// W1:cnum:bisym を用いた比較関数
//////////////////////////////////////////////////////////////////////
class W1CnumGt
{
public:

  // コンストラクタ
  W1CnumGt(const NpnBaseConf& conf) :
    mConf(conf)
  {
  }


  // 比較関数
  bool
  operator()(ymuint pos1,
	     ymuint pos2)
  {
    return mConf.w1gt(pos1, pos2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnBaseConf& mConf;

};


//////////////////////////////////////////////////////////////////////
// W1:cnum:bisym を用いた等価比較関数
//////////////////////////////////////////////////////////////////////
class W1CnumEq
{
public:

  // コンストラクタ
  W1CnumEq(const NpnBaseConf& conf) :
    mConf(conf)
  {
  }


  // 比較関数
  bool
  operator()(ymuint pos1,
	     ymuint pos2)
  {
    return mConf.w1eq(pos1, pos2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnBaseConf& mConf;

};


//////////////////////////////////////////////////////////////////////
// W2 を用いた大小比較関数
//////////////////////////////////////////////////////////////////////
class W2Gt
{
public:

  // コンストラクタ
  W2Gt(const NpnConf& conf,
       ymuint pos0) :
    mConf(conf),
    mPos0(pos0)
  {
  }


  // 比較関数
  bool
  operator()(ymuint pos1,
	     ymuint pos2)
  {
    return mConf.w2gt(mPos0, pos1, pos2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnConf& mConf;

  ymuint32 mPos0;

};


//////////////////////////////////////////////////////////////////////
// W2 を用いた等価比較関数
//////////////////////////////////////////////////////////////////////
class W2Eq
{
public:

  // コンストラクタ
  W2Eq(const NpnConf& conf,
       ymuint pos0) :
    mConf(conf),
    mPos0(pos0)
  {
  }


  // 比較関数
  bool
  operator()(ymuint pos1,
	     ymuint pos2)
  {
    return mConf.w2eq(mPos0, pos1, pos2);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnConf& mConf;

  ymuint32 mPos0;

};


//////////////////////////////////////////////////////////////////////
// NPN同値類を区別するためのシグネチャを表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnMgr::NpnMgr()
{
}

// デストラクタ
NpnMgr::~NpnMgr()
{
}


BEGIN_NONAMESPACE

// @brief 重み別 w0 を用いて極性を確定させる．
void
ww0_refine(vector<NpnConf>& pollist)
{
  const NpnConf& conf0 = pollist.front();
  ymuint ni = conf0.ni();

  if ( debug & debug_ww0_refine ) {
    cout << "before ww0_refine()" << endl;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      NpnConf& conf = pollist[i];
      conf.dump(cout);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
  int max_d0array[TvFunc::kMaxNi + 1];
  for (w = 0; w <= ni && pollist.size() > 1; ++ w) {
    bool first = true;
    int max_d0 = 0;
    ymuint wpos = 0;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      const NpnConf& conf = pollist[i];
      int d0 = conf.walsh_w0(w);

      int stat = -1;
      if ( first ) {
	first = false;
      }
      else {
	stat = max_d0 - d0;
      }
      if ( stat <= 0 ) {
	if ( stat < 0 ) {
	  wpos = 0;
	  max_d0 = d0;
	}
	pollist[wpos] = conf;
	++ wpos;
      }
    }
    if ( wpos < pollist.size() ) {
      pollist.erase(pollist.begin() + wpos, pollist.end());
    }
    max_d0array[w] = max_d0;
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after ww0_refine()" << endl;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      NpnConf& conf = pollist[i];
      conf.dump(cout);
    }
    cout << endl;
  }
}

END_NONAMESPACE


// @brief func の正規化を行う．
void
NpnMgr::cannonical(const TvFunc& func,
		   NpnMap& cmap)
{
  // 特例
  if ( func.ni() == 0 ) {
    if ( func.value(0) == 0 ) {
      // 定数0関数
      cmap.set_identity(0);
      return;
    }
    else {
      // 定数1関数
      cmap.set_identity(0);
      cmap.set_opol(kPolNega);
      return;
    }
  }
  if ( func.ni() == 1 ) {
    if ( func.value(0) == 0 ) {
      if ( func.value(1) == 0 ) {
	// 1入力の定数0関数
	cmap.set_identity(1);
	return;
      }
      else {
	// 肯定のリテラル関数
	cmap.set_identity(1);
	return;
      }
    }
    else {
      if ( func.value(1) == 0 ) {
	// 否定のリテラル関数
	cmap.set_identity(1);
	cmap.set_opol(kPolNega);
	return;
      }
      else {
	// 1入力の定数1関数
	cmap.set_identity(1);
	cmap.set_opol(kPolNega);
	return;
      }
    }
  }

  NpnBaseConf base_conf(func);

  // W0 と W1 が非負になるように極性の調節を行う．
  // W0 と W1 および対称変数の情報を用いて正規化を行う．
  // 以降は対称入力グループ単位で考えればよい．
  // また W0 と W1 シグネチャは最大化されているので考える必要はない．
  base_conf.normalize();

  NpnConf conf0(base_conf);

  if ( !conf0.is_resolved() && conf0.nc() > 1 ) {
    if ( debug & debug_step1) {
      cout << "Before step1" << endl;
      conf0.dump(cout);
    }

    // W1:cnum:bisym を用いてグループわけを行う．
    conf0.refine(0, W1CnumGt(base_conf), W1CnumEq(base_conf));

    if ( debug & debug_step1) {
      cout << "After step1" << endl;
      conf0.dump(cout);
    }
  }

  mMaxList.clear();
  mW2max_count = 0;

  if ( conf0.is_resolved() ) {
    mMaxList.push_back(conf0);
  }
  else {
#if 1
    mMaxFunc = TvFunc::const_zero(func.ni());

#if 1
    // 以降の処理は単純化するために出力極性を固定で考える．
    if ( !conf0.is_opol_fixed() ) {
      // 出力極性が決まっていなければ両方の極性で考える．
      NpnConf conf_p(conf0, 1);
      w2max_recur(conf_p, 0);

      NpnConf conf_n(conf0, 2);
      w2max_recur(conf_n, 0);
    }
    else {
      w2max_recur(conf0, 0);
    }
#else
    // 以降の処理は単純化するために出力極性を固定で考える．
    if ( !conf0.is_opol_fixed() ) {
      // 出力極性が決まっていなければ両方の極性で考える．
      NpnConf conf_p(conf0, 1);
      tvmax_recur(conf_p, 0);

      NpnConf conf_n(conf0, 2);
      tvmax_recur(conf_n, 0);
    }
    else {
      tvmax_recur(conf0, 0);
    }
#endif
#else
    // 最初に全ての極性を展開してしまう．
    vector<NpnConf> pollist;
    ymuint ulist[TvFunc::kMaxNi];
    ymuint unum = 0;
    for (ymuint c = 0; c < conf0.nc(); ++ c) {
      if ( conf0.ic_pol(c) == 0 ) {
	ulist[unum] = c;
	++ unum;
      }
    }
    ymuint unum_pow = 1UL << unum;
    pollist.reserve(unum_pow);
    if ( conf0.is_opol_fixed() ) {
      for (ymuint p = 0; p < unum_pow; ++ p) {
	NpnConf conf(conf0);
	for (ymuint i = 0; i < unum; ++ i) {
	  ymuint c = ulist[i];
	  if ( p & (1 << i) ) {
	    // c のクラスを反転させる．
	    conf.set_ic_pol(c, 2);
	  }
	  else {
	    conf.set_ic_pol(c, 1);
	  }
	}
	pollist.push_back(conf);
      }
    }
    else {
      for (int opol = 1; opol <= 2; ++ opol) {
	for (ymuint p = 0; p < unum_pow; ++ p) {
	  NpnConf conf(conf0, opol);
	  for (ymuint i = 0; i < unum; ++ i) {
	    ymuint c = ulist[i];
	    if ( p & (1 << i) ) {
	      // c のクラスを反転させる．
	      conf.set_ic_pol(c, 2);
	    }
	    else {
	      conf.set_ic_pol(c, 1);
	    }
	  }
	  pollist.push_back(conf);
	}
      }
    }
    // それを WW0 を用いて制限する．
    ww0_refine(pollist);
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      NpnConf& conf = pollist[i];
      w2max_recur(conf, 0);
    }
#endif
  }

  if ( debug & debug_final ) {
    cout << "Final Result" << endl;
    cout << " w2max_count: " << mW2max_count << endl;
    for (vector<NpnConf>::iterator p = mMaxList.begin();
	 p != mMaxList.end(); ++ p) {
      const NpnConf& conf = *p;
      conf.dump(cout);
      if ( debug & debug_final_func ) {
	NpnMap map;
	conf.set_map(map);
	TvFunc func1 = func.xform(map);
	cout << "map     : " << map << endl
	     << "rep func: " << func1 << endl
	     << endl;
      }
    }
    cout << endl;
  }

  // conf0 から cmap を作る．
  const NpnConf& max_conf = mMaxList.front();
  max_conf.set_map(cmap);
}

// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
void
NpnMgr::all_map(list<NpnMap>& map_list) const
{
  map_list.clear();
  for (vector<NpnConf>::const_iterator p = mMaxList.begin();
       p != mMaxList.end(); ++ p) {
    const NpnConf& conf = *p;
    NpnMap map;
    conf.set_map(map);
    map_list.push_back(map);
  }
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgr::w2max_count() const
{
#if 1
  return mW2max_count;
#else
  return mTvmax_count;
#endif
}


BEGIN_NONAMESPACE

// @brief 確定した入力との w2 を用いて細分化する．
void
w2refine(NpnConf& conf,
	 ymuint g0)
{
  // Walsh の 2次の係数を用いた細分化

  // 確定した入力をキューに積む．
  ymuint queue[TvFunc::kMaxNi];
  ymuint rp = 0;
  ymuint wp = 0;
  for (ymuint g = g0; g < conf.group_num(); ++ g) {
    ymuint s = conf.group_begin(g);
    ymuint e = conf.group_end(g);
    if ( e - s == 1 && conf.ic_pol(s) != 0 ) {
      queue[wp] = s;
      ++ wp;
    }
  }

  while ( wp > rp ) {
    ymuint s = queue[rp];
    ymuint pos0 = conf.ic_rep(s);
    ++ rp;

    ymuint c = 0;
    for (ymuint g1 = g0; g1 < conf.group_num(); ++ g1) {
      ymuint b = conf.group_begin(g1);
      ymuint e = conf.group_end(g1);
      if ( e - b == 1 ) {
	// 順序が確定している場合

	if ( conf.ic_pol(b) == 0 ) {
	  // ても極性は確定していない場合
	  ymuint pos1 = conf.ic_rep(b);
	  int w2_1 = conf.walsh_2(pos0, pos1);
	  if ( w2_1 < 0 ) {
	    conf.set_ic_pol(b, 2);
	    queue[wp] = b;
	    ++ wp;
	  }
	  else if ( w2_1 > 0 ) {
	    conf.set_ic_pol(b, 1);
	    queue[wp] = b;
	    ++ wp;
	  }
	  else {
	    // ここでは確定しなかった．
	    ++ c;
	  }
	}
      }
      else {
	// 順序が確定していない場合

	// 極性が確定していない場合には2次係数で極性を制限する．
	for (ymuint i = b; i < e; ++ i) {
	  if ( conf.ic_pol(i) == 0 ) {
	    ymuint pos1 = conf.ic_rep(i);
	    int w2 = conf.walsh_2(pos0, pos1);
	    if ( w2 > 0 ) {
	      conf.set_ic_pol(i, 1);
	    }
	    else if ( w2 < 0 ) {
	      conf.set_ic_pol(i, 2);
	    }
	  }
	}

	// 2次係数の降べきの順に並べ替える．
	ymuint d = conf.refine(g1, W2Gt(conf, pos0), W2Eq(conf, pos0));

	// 新しく増えたグループで確定したものがあるか調べる．
	for (ymuint j = g1; j < g1 + d; ++ j) {
	  ymuint s = conf.group_begin(j);
	  if ( conf.group_size(j) == 1 && conf.ic_pol(s) != 0 ) {
	    queue[wp] = s;
	    ++ wp;
	  }
	  else {
	    ++ c;
	  }
	}
	g1 += d;
      }
    }
    if ( debug & debug_step3 ) {
      cout << "  pivot = " << pos0 << endl;
      conf.dump(cout);
    }
    if ( c == 0 ) {
      break;
    }
  }
}

END_NONAMESPACE


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgr::w2max_recur(NpnConf& conf,
		    ymuint g0)
{
  for ( ; ; ) {
    if ( debug & debug_w2max ) {
      cout << "w2max_recur(" << g0 << ")" << endl;
      conf.dump(cout);
    }

    ++ mW2max_count;

    if ( !conf.is_resolved() ) {
      w2refine(conf, g0);
    }
    if ( conf.is_resolved() ) {
      if ( mMaxList.empty() ) {
	mMaxList.push_back(conf);
	NpnMap map1;
	conf.set_map(map1);
	mMaxFunc = conf.func().xform(map1);
      }
      else {
	int diff = 0;
#if 1
	const NpnConf& max_conf = mMaxList.front();
	ymuint ni = max_conf.ni();
	for (ymuint i = 0; i < ni - 1; ++ i) {
	  for (ymuint j = i + 1; j < ni; ++ j) {
	    int w2_1 = max_conf.walsh_2i(i, j);
	    int w2_2 = conf.walsh_2i(i, j);
	    diff = w2_1 - w2_2;
	    if ( diff != 0 ) {
	      goto loop_exit;
	    }
	  }
	}
      loop_exit:
#endif
	if ( diff == 0 ) {
	  // ここまでで決着が付かなければ真理値表ベクタの辞書式順序で比較する．
	  NpnMap map1;
	  conf.set_map(map1);
	  TvFunc func1 = conf.func().xform(map1);
	  if ( mMaxFunc < func1 ) {
	    diff = -1;
	    mMaxFunc = func1;
	  }
	  else if ( mMaxFunc > func1 ) {
	    diff = 1;
	  }
	}
	if ( diff < 0 ) {
	  // 最大値の更新
	  mMaxList.clear();
	  mMaxList.push_back(conf);
	}
	else if ( diff == 0 ) {
	  mMaxList.push_back(conf);
	}
      }
      return;
    }

    // 確定している入力グループをスキップする．
    for ( ; g0 < conf.group_num(); ++ g0) {
      if ( conf.group_size(g0) > 1 ) {
	break;
      }
      ymuint b = conf.group_begin(g0);
      if ( conf.ic_pol(b) == 0 ) {
	break;
      }
    }

    if ( conf.group_size(g0) > 1 ) {
      // g0 内に複数のクラスがあって順序は未確定
      ymuint b = conf.group_begin(g0);
      ymuint e = conf.group_end(g0);
      for (ymuint i = b; i < e; ++ i) {
	NpnConf conf_i(conf, g0, i);
	w2max_recur(conf_i, g0);
      }
      return;
    }
    // g0 の唯一のクラスの極性が未定
    NpnConf conf_p(conf);
    ymuint c = conf.group_begin(g0);
    conf_p.set_ic_pol(c, 1);
    w2max_recur(conf_p, g0);

    conf.set_ic_pol(c, 2);
    // わざとループする．
  }
}


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgr::tvmax_recur(NpnConf& conf,
		    ymuint g0)
{
  for ( ; ; ) {
    if ( debug & debug_w2max ) {
      cout << "tvmax_recur(" << g0 << ")" << endl;
      conf.dump(cout);
    }

    ++ mTvmax_count;

    if ( conf.is_resolved() ) {
      NpnMap map1;
      conf.set_map(map1);
      TvFunc func1 = conf.func().xform(map1);
      if ( mMaxList.empty() ) {
	mMaxList.push_back(conf);
	mMaxFunc = func1;
      }
      else {
	// 真理値表ベクタの辞書式順序で比較する．
	if ( mMaxFunc < func1 ) {
	  // 最大値の更新
	  mMaxList.clear();
	  mMaxList.push_back(conf);
	  mMaxFunc = func1;
	}
	else if ( mMaxFunc == func1 ) {
	  mMaxList.push_back(conf);
	}
      }
      return;
    }

    // 確定している入力グループをスキップする．
    for ( ; g0 < conf.group_num(); ++ g0) {
      if ( conf.group_size(g0) > 1 ) {
	break;
      }
      ymuint b = conf.group_begin(g0);
      if ( conf.ic_pol(b) == 0 ) {
	break;
      }
    }

    if ( conf.group_size(g0) > 1 ) {
      // g0 内に複数のクラスがあって順序は未確定
      ymuint b = conf.group_begin(g0);
      ymuint e = conf.group_end(g0);
      for (ymuint i = b; i < e; ++ i) {
	NpnConf conf_i(conf, g0, i);
	tvmax_recur(conf_i, g0);
      }
      return;
    }
    // g0 の唯一のクラスの極性が未定
    NpnConf conf_p(conf);
    ymuint c = conf.group_begin(g0);
    conf_p.set_ic_pol(c, 1);
    tvmax_recur(conf_p, g0);

    conf.set_ic_pol(c, 2);
    // わざとループする．
  }
}

END_NAMESPACE_YM_NPN
