
/// @file NpnMgrImpl.cc
/// @brief NpnMgrIMpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "NpnMgrImpl.h"

#include "ym_logic/TvFunc.h"
#include "ym_logic/NpnMgr.h"
#include "NpnBaseConf.h"
#include "NpnConf.h"


BEGIN_NONAMESPACE

const int debug_ww0_refine  = 0x0001;
const int debug_w2max_recur = 0x0010;
const int debug_w2refine    = 0x0040;
const int debug_none        = 0x0000;
const int debug_all         = 0xffff;

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
class W1CnumCmp
{
public:

  // コンストラクタ
  W1CnumCmp(const NpnBaseConf& conf) :
    mConf(conf)
  {
  }


  // 大小比較関数
  bool
  gt(ymuint pos1,
     ymuint pos2)
  {
    return mConf.w1gt(pos1, pos2);
  }

  // 等価比較関数
  bool
  eq(ymuint pos1,
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
class W2Cmp
{
public:

  // コンストラクタ
  W2Cmp(const NpnConf& conf,
	ymuint pos0) :
    mConf(conf),
    mPos0(pos0)
  {
  }


  // 大小比較関数
  bool
  gt(ymuint pos1,
     ymuint pos2)
  {
    int v1 = mConf.walsh_2(mPos0, pos1);
    int v2 = mConf.walsh_2(mPos0, pos2);
    return v1 >= v2;
  }

  // 等価比較関数
  bool
  eq(ymuint pos1,
     ymuint pos2)
  {
    int v1 = mConf.walsh_2(mPos0, pos1);
    int v2 = mConf.walsh_2(mPos0, pos2);
    return v1 == v2;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  const NpnConf& mConf;

  ymuint32 mPos0;

};


//////////////////////////////////////////////////////////////////////
// クラス NpnMgrImpl
//////////////////////////////////////////////////////////////////////

// コンストラクタ
NpnMgrImpl::NpnMgrImpl() :
  mAlloc(sizeof(NpnConf), 1024)
{
}

// デストラクタ
NpnMgrImpl::~NpnMgrImpl()
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
      const NpnConf& conf = pollist[i];
      conf.dump(cout);
    }
    cout << endl;
  }

  // 重み別 w0 係数を用いて極性の決定を行う．
  ymuint w = 0;
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
  }

  if ( debug & debug_ww0_refine ) {
    cout << "after ww0_refine()" << endl;
    for (ymuint i = 0; i < pollist.size(); ++ i) {
      const NpnConf& conf = pollist[i];
      conf.dump(cout);
    }
    cout << endl;
  }
}

END_NONAMESPACE


// @brief func の正規化を行う．
void
NpnMgrImpl::cannonical(const TvFunc& func,
		       NpnMap& cmap,
		       int algorithm)
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
    if ( debug ) {
      cout << "Before step1" << endl;
      conf0.dump(cout);
    }

    // W1:cnum:bisym を用いてグループわけを行う．
    conf0.refine(0, W1CnumCmp(base_conf));

    if ( debug ) {
      cout << "After step1" << endl;
      conf0.dump(cout);
    }
  }

  mMaxList.clear();
  mW2max_count = 0;
  mTvmax_count = 0;

  if ( conf0.is_resolved() ) {
    mMaxList.push_back(conf0);
  }
  else {
    mMaxFunc = TvFunc::const_zero(func.ni());

    switch ( algorithm ) {
    case 0:
      algorithm0(conf0);
      break;

    case 1:
      algorithm1(conf0);
      break;

    case 2:
      algorithm2(conf0);
      break;

    case 3:
      algorithm3(conf0);
      break;

    default:
      algorithm0(conf0);
      break;
    }
  }

  if ( debug ) {
    cout << "Final Result" << endl;
    cout << " algorithm:   " << algorithm << endl;
    cout << " w2max_count: " << mW2max_count << endl;
    cout << " tvmax_count: " << mTvmax_count << endl;
    for (vector<NpnConf>::iterator p = mMaxList.begin();
	 p != mMaxList.end(); ++ p) {
      const NpnConf& conf = *p;
      conf.dump(cout);
      NpnMap map;
      conf.set_map(map);
      TvFunc func1 = func.xform(map);
      cout << "map     : " << map << endl
	   << "rep func: " << func1 << endl
	   << endl;
    }
    cout << endl;
  }

  // conf0 から cmap を作る．
  const NpnConf& max_conf = mMaxList.front();
  max_conf.set_map(cmap);
}

// @brief 直前の cannonical の呼び出しにおける NpnMap の全候補を返す．
void
NpnMgrImpl::all_map(list<NpnMap>& map_list) const
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
NpnMgrImpl::w2max_count() const
{
  return mW2max_count;
}

// @brief 直前の cannonical の呼び出しにおける w2max_recur の起動回数を返す．
ymulong
NpnMgrImpl::tvmax_count() const
{
  return mTvmax_count;
}

// @brief DFS
void
NpnMgrImpl::algorithm0(const NpnConf& conf)
{
  // 以降の処理は単純化するために出力極性を固定で考える．
  if ( conf.is_opol_fixed() ) {
    NpnConf conf1(conf);
    w2max_recur(conf1, 0);
  }
  else {
    // 出力極性が決まっていなければ両方の極性で考える．
    NpnConf conf_p(conf, 1);
    w2max_recur(conf_p, 0);

    NpnConf conf_n(conf, 2);
    w2max_recur(conf_n, 0);
  }
}

// @brief BFS
void
NpnMgrImpl::algorithm1(const NpnConf& conf)
{
  vector<NpnConf> conf_list;
  if ( conf.is_opol_fixed() ) {
    conf_list.push_back(conf);
  }
  else {
    NpnConf conf_p(conf, 1);
    conf_list.push_back(conf_p);

    NpnConf conf_n(conf, 2);
    conf_list.push_back(conf_n);
  }
  w2max_recur(conf_list, 0);
}

// @brief 極性を展開した後で DFS
void
NpnMgrImpl::algorithm2(const NpnConf& conf)
{
  // 最初に全ての極性を展開してしまう．
  vector<NpnConf> pollist;
  ymuint ulist[TvFunc::kMaxNi];
  ymuint unum = 0;
  for (ymuint c = 0; c < conf.nc(); ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      ulist[unum] = c;
      ++ unum;
    }
  }
  ymuint unum_pow = 1UL << unum;
  if ( conf.is_opol_fixed() ) {
    pollist.reserve(unum_pow);
    for (ymuint p = 0; p < unum_pow; ++ p) {
      NpnConf conf1(conf);
      for (ymuint i = 0; i < unum; ++ i) {
	ymuint c = ulist[i];
	if ( p & (1 << i) ) {
	  // c のクラスを反転させる．
	  conf1.set_ic_pol(c, 2);
	}
	else {
	  conf1.set_ic_pol(c, 1);
	}
      }
      pollist.push_back(conf1);
    }
  }
  else {
    pollist.reserve(unum_pow * 2);
    for (int opol = 1; opol <= 2; ++ opol) {
      for (ymuint p = 0; p < unum_pow; ++ p) {
	NpnConf conf1(conf, opol);
	for (ymuint i = 0; i < unum; ++ i) {
	  ymuint c = ulist[i];
	  if ( p & (1 << i) ) {
	    // c のクラスを反転させる．
	    conf1.set_ic_pol(c, 2);
	  }
	  else {
	    conf1.set_ic_pol(c, 1);
	  }
	}
	pollist.push_back(conf1);
      }
    }
  }
  // それを WW0 を用いて制限する．
  ww0_refine(pollist);
  for (ymuint i = 0; i < pollist.size(); ++ i) {
    NpnConf& conf = pollist[i];
    w2max_recur(conf, 0);
  }
}

// @brief 極性を展開した後で BFS
void
NpnMgrImpl::algorithm3(const NpnConf& conf)
{
  // 最初に全ての極性を展開してしまう．
  vector<NpnConf> pollist;
  ymuint ulist[TvFunc::kMaxNi];
  ymuint unum = 0;
  for (ymuint c = 0; c < conf.nc(); ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      ulist[unum] = c;
      ++ unum;
    }
  }
  ymuint unum_pow = 1UL << unum;
  if ( conf.is_opol_fixed() ) {
    pollist.reserve(unum_pow);
    for (ymuint p = 0; p < unum_pow; ++ p) {
      NpnConf conf1(conf);
      for (ymuint i = 0; i < unum; ++ i) {
	ymuint c = ulist[i];
	if ( p & (1 << i) ) {
	  // c のクラスを反転させる．
	  conf1.set_ic_pol(c, 2);
	}
	else {
	  conf1.set_ic_pol(c, 1);
	}
      }
      pollist.push_back(conf1);
    }
  }
  else {
    pollist.reserve(unum_pow * 2);
    for (int opol = 1; opol <= 2; ++ opol) {
      for (ymuint p = 0; p < unum_pow; ++ p) {
	NpnConf conf1(conf, opol);
	for (ymuint i = 0; i < unum; ++ i) {
	  ymuint c = ulist[i];
	  if ( p & (1 << i) ) {
	    // c のクラスを反転させる．
	    conf1.set_ic_pol(c, 2);
	  }
	  else {
	    conf1.set_ic_pol(c, 1);
	  }
	}
	pollist.push_back(conf1);
      }
    }
  }
  // それを WW0 を用いて制限する．
  ww0_refine(pollist);
  w2max_recur(pollist, 0);
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
	ymuint d = conf.refine(g1, W2Cmp(conf, pos0));

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
    if ( c == 0 ) {
      break;
    }
  }
}

END_NONAMESPACE


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgrImpl::w2max_recur(NpnConf& conf,
			ymuint g0)
{
  if ( debug & debug_w2max_recur ) {
    cout << "w2max_recur(" << g0 << ")" << endl;
    conf.dump(cout);
  }

  ++ mW2max_count;

  if ( !conf.is_resolved() ) {
    w2refine(conf, g0);
  }
  if ( conf.is_resolved() ) {
#if 0
    TvFunc func1;
    {
      NpnMap map1;
      conf.set_map(map1);
      func1 = conf.func().xform(map1);
    }
    cout << "func1 = " << func1 << endl
	 << "w2 = {";
    ymuint ni = conf.ni();
    for (ymuint i = 1; i < ni; ++ i) {
      for (ymuint j = 0; j < i; ++ j) {
	int w2 = func1.walsh_2(i, j);
	cout << " " << w2;
      }
    }
    cout << "}" << endl;
#endif
    if ( mMaxList.empty() ) {
      mMaxList.push_back(conf);
#if 0
      mMaxFunc = func1;
#else
      {
	NpnMap map1;
	conf.set_map(map1);
	mMaxFunc = conf.func().xform(map1);
      }
#endif
#if 0
      cout << "mMaxFunc = " << mMaxFunc << endl
	   << "w2 = {";
      ymuint ni = conf.ni();
      for (ymuint i = 1; i < ni; ++ i) {
	for (ymuint j = 0; j < i; ++ j) {
	  int w2 = mMaxFunc.walsh_2(i, j);
	  cout << " " << w2;
	}
      }
      cout << "}" << endl;
#endif
    }
    else {
      int diff = 0;
      TvFunc func1;

      ymuint ni = conf.ni();
      for (ymuint i = 1; i < ni; ++ i) {
	for (ymuint j = 0; j < i; ++ j) {
	  int w2_1 = mMaxFunc.walsh_2(i, j);
	  int w2_2 = conf.walsh_2i(i, j);
	  diff = w2_1 - w2_2;
	  if ( diff != 0 ) {
	    if ( diff < 0 ) {
	      NpnMap map1;
	      conf.set_map(map1);
	      func1 = conf.func().xform(map1);
	    }
	    goto loop_exit;
	  }
	}
      }
    loop_exit:
      if ( diff == 0 ) {
	// ここまでで決着が付かなければ真理値表ベクタの辞書式順序で比較する．
	++ mTvmax_count;
	{
	  NpnMap map1;
	  conf.set_map(map1);
	  func1 = conf.func().xform(map1);
	}
	if ( mMaxFunc < func1 ) {
	  diff = -1;
	}
	else if ( mMaxFunc > func1 ) {
	  diff = 1;
	}
      }
      if ( diff <= 0 ) {
	if ( diff < 0 ) {
	  // 最大値の更新
	  mMaxList.clear();
	  mMaxFunc = func1;
#if 0
	  cout << "mMaxFunc = " << mMaxFunc << endl
	       << "w2 = {";
	  ymuint ni = conf.ni();
	  for (ymuint i = 1; i < ni; ++ i) {
	    for (ymuint j = 0; j < i; ++ j) {
	      int w2 = mMaxFunc.walsh_2(i, j);
	      cout << " " << w2;
	    }
	  }
	  cout << "}" << endl;
#endif
	}
	mMaxList.push_back(conf);
      }
    }
    return;
  }

  // 確定している入力グループをスキップする．
  for ( ; g0 < conf.group_num(); ++ g0) {
    if ( conf.group_size(g0) > 1 ) {
      // g0 内に複数のクラスがあって順序は未確定
      ymuint b = conf.group_begin(g0);
      ymuint e = conf.group_end(g0);
      for (ymuint i = b; i < e; ++ i) {
	NpnConf conf_i(conf, g0, i);
	w2max_recur(conf_i, g0);
      }
      break;
    }
    ymuint b = conf.group_begin(g0);
    if ( conf.ic_pol(b) == 0 ) {
      // g0 の唯一のクラスの極性が未定
      NpnConf conf_p(conf);
      ymuint c = conf.group_begin(g0);
      conf_p.set_ic_pol(c, 1);
      w2max_recur(conf_p, g0);

      conf.set_ic_pol(c, 2);
      w2max_recur(conf, g0);
      break;
    }
  }
}


BEGIN_NONAMESPACE

void
w2refine2_sub(NpnConf& conf,
	      ymuint g0)
{
  if ( debug & debug_w2refine) {
    cout << "conf" << endl;
    conf.dump(cout);
  }

  // 極性の確定を行う．
  ymuint c0 = conf.group_begin(g0);
  ymuint pos0 = conf.ic_rep(c0);
  ymuint cend = conf.group_end(conf.group_num() - 1);
  for (ymuint c = c0 + 1; c < cend; ++ c) {
    if ( conf.ic_pol(c) == 0 ) {
      int w2 = conf.walsh_2(pos0, conf.ic_rep(c));
      if ( w2 < 0 ) {
	conf.set_ic_pol(c, 2);
      }
      else if ( w2 > 0 ) {
	conf.set_ic_pol(c, 1);
      }
    }
  }
  // 並べ替え＆グループ化を行う．
  for (ymuint g = g0 + 1; g < conf.group_num(); ++ g) {
    ymuint delta = conf.refine(g, W2Cmp(conf, pos0));
    g += delta;
  }
  if ( debug & debug_w2refine) {
    cout << " =>" << endl;
    conf.dump(cout);
    cout << endl;
  }
}

// @brief 確定した入力との w2 を用いて細分化する．
void
w2refine2(const NpnConf& conf,
	  ymuint g0,
	  list<NpnConf>& conf_list)
{
  // Walsh の 2次の係数を用いた細分化
  if ( debug & debug_w2refine ) {
    cout << endl
	 << "w2refine2(g0 = " << g0 << ")" << endl;
  }

  assert_cond( conf.group_size(g0) == 1, __FILE__, __LINE__);
  ymuint c0 = conf.group_begin(g0);
  if ( conf.ic_pol(c0) == 0 ) {
    // 極性が確定していない．
    NpnConf conf_p(conf);
    conf_p.set_ic_pol(c0, 1);
    w2refine2_sub(conf_p, g0);
    conf_list.push_back(conf_p);

    NpnConf conf_n(conf);
    conf_n.set_ic_pol(c0, 2);
    w2refine2_sub(conf_n, g0);
    conf_list.push_back(conf_n);
  }
  else {
    // 2次係数の降べきの順に並べ替える．
    NpnConf conf1(conf);
    w2refine2_sub(conf1, g0);
    conf_list.push_back(conf1);
  }
}

END_NONAMESPACE


// @brief シグネチャが最大になるように極性と順序を決める．
void
NpnMgrImpl::w2max_recur(vector<NpnConf>& conf_list,
			ymuint g0)
{
  ymuint cend;
  {
    assert_cond( !conf_list.empty(), __FILE__, __LINE__);
    const NpnConf& conf = conf_list.front();
    cend = conf.group_end(conf.group_num() - 1);
  }

  while ( !conf_list.empty() ) {
    if ( debug & debug_w2max_recur ) {
      cout << endl
	   << "w2max_recur(g0 = " << g0 << ")" << endl;
      for (vector<NpnConf>::const_iterator p = conf_list.begin();
	   p != conf_list.end(); ++ p) {
	const NpnConf& conf = *p;
	conf.dump(cout);
      }
      cout << endl;
    }

    ++ mW2max_count;

#if 0
    for (ymuint c1 = 0; c1 < cend; ++ c1) {
      mMaxW2[c1] = -INT_MAX;
    }
#endif
    bool first = true;
    list<NpnConf> conf_list1;
    for (vector<NpnConf>::const_iterator p = conf_list.begin();
	 p != conf_list.end(); ++ p) {
      const NpnConf& conf = *p;
      if ( conf.is_resolved() ) {
	++ mTvmax_count;

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
	continue;
      }

      list<NpnConf> tmp_list;
      if ( conf.group_size(g0) > 1 ) {
	ymuint b = conf.group_begin(g0);
	ymuint e = conf.group_end(g0);
	for (ymuint c0 = b; c0 < e; ++ c0) {
	  NpnConf conf1(conf, g0, c0);
	  w2refine2(conf1, g0, tmp_list);
	}
      }
      else {
	w2refine2(conf, g0, tmp_list);
      }

      for (list<NpnConf>::iterator p = tmp_list.begin();
	   p != tmp_list.end(); ++ p) {
	const NpnConf& conf = *p;
	ymuint c0 = conf.group_begin(g0);
	ymuint pos0 = conf.ic_rep(c0);
	ymuint pol0 = conf.ic_pol(c0);
	assert_cond( pol0 != 0, __FILE__, __LINE__);
	int tmp_w2[TvFunc::kMaxNi];
	for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	  ymuint pos1 = conf.ic_rep(c1);
	  tmp_w2[c1] = conf.walsh_2(pos0, pos1);
	}
	if ( debug & debug_w2max_recur ) {
	  cout << "tmp_w2 = ";
	  for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	    cout << " " << tmp_w2[c1];
	  }
	  cout << endl;
	}
	int diff = 0;
	if ( first ) {
	  first = false;
	  diff = -1;
	}
	else {
	  for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	    int diff1 = mMaxW2[c1] - tmp_w2[c1];
	    if ( diff1 != 0 ) {
	      diff = diff1;
	      break;
	    }
	  }
	}
	if ( diff <= 0 ) {
	  if ( diff < 0 ) {
	    conf_list1.clear();
	    for (ymuint c1 = c0 + 1; c1 < cend; ++ c1) {
	      mMaxW2[c1] = tmp_w2[c1];
	    }
	  }
	  conf_list1.push_back(conf);
	}
      }
    }

    if ( debug & debug_w2max_recur ) {
      cout << "conf_list1 = " << endl;
      for (list<NpnConf>::iterator p = conf_list1.begin();
	   p != conf_list1.end(); ++ p) {
	const NpnConf& conf = *p;
	conf.dump(cout);
      }
      cout << endl;
    }

    conf_list.clear();
    for (list<NpnConf>::iterator p = conf_list1.begin();
	 p != conf_list1.end(); ++ p) {
      const NpnConf& conf = *p;
      conf_list.push_back(conf);
    }
    ++ g0;
  }
}

// @brief NpnConf を取り出す．
NpnConf*
NpnMgrImpl::new_npnconf()
{
  void* p = mAlloc.get_memory(sizeof(NpnConf));
  return new (p) NpnConf;
}

// @brief NpnConf を使用可能リストに戻す．
void
NpnMgrImpl::free_npnconf(NpnConf* conf)
{
  mAlloc.put_memory(sizeof(NpnConf), conf);
}

END_NAMESPACE_YM_NPN
