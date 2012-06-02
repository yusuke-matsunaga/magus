
/// @file genpat2.cc
/// @brief N 入力関数を実現するパタンを生成するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#include "GenPat.h"
#include "GpNode.h"
//#include <ym_npn/TvFunc.h>
//#include <ym_npn/NpnMgr.h>
//#include <ym_npn/NpnMap.h>


#define VERIFY_FF2TABLE 0


BEGIN_NAMESPACE_YM

#if 0
// 3入力のNPN同値類代表関数
ymuint32 npn3rep[] = {
#include "npn3rep.h"
};

// 4入力のNPN同値類代表関数
ymuint32 npn4rep[] = {
#include "npn4rep.h"
};

// 5入力のNPN同値類代表関数
ymuint32 npn5rep[] = {
#if 0
#include "npn5rep.h"
#else
  0xacb00e68U,
  0xFFFFFFFFU
#endif
};

hash_map<ymuint32, size_t> npn4map;

hash_map<ymuint32, size_t> npn5map;

// ni 入力関数のパタンを出力する．
void
dump_pat(ostream& s,
	 ymuint ni,
	 ymuint32 pat)
{
  ymuint np = 1U << ni;
  for (ymuint i = 0; i < np; ++ i) {
    if ( pat & (1U << i) ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

// node もしくは node の部分グラフで fv を実現していたら true を返す．
bool
check_fv(GpNode* node,
	 ymuint32 fv)
{
  ymuint32 npat = node->pat();
  if ( npat & 1U ) {
    npat = ~npat;
  }
  if ( npat == fv ) {
    return true;
  }
  if ( node->is_input() ) {
    return false;
  }
  return check_fv(node->fanin0(), fv) || check_fv(node->fanin1(), fv);
}

bool
check_fv2_sub(GpNode* node1,
	      GpNode* node2,
	      ymuint32 fv,
	      ymuint32 mask)
{
  for (ymuint pp = 0; pp < 4; ++ pp) {
    ymuint32 tmp1 = node1->pat();
    if ( pp & 1U ) {
      tmp1 = ~tmp1;
    }
    ymuint32 tmp2 = node2->pat();
    if ( pp & 2U ) {
      tmp2 = ~tmp2;
    }
    ymuint32 tmp = tmp1 & tmp2 & mask;
    if ( tmp & 1U ) {
      tmp = ~tmp & mask;
    }
    if ( tmp == fv ) {
#if 0
      cout << "check_fv2: " << hex << fv << dec << endl;
      dump_gp(node1);
      cout << endl;
      dump_gp(node2);
      cout << endl;
#endif
      return true;
    }
  }
  if ( node2->is_and() ) {
    if ( check_fv2_sub(node1, node2->fanin0(), fv, mask) ) {
      return true;
    }
    if ( check_fv2_sub(node1, node2->fanin1(), fv, mask) ) {
      return true;
    }
    return false;
  }
  return false;
}

bool
check_fv2(GpNode* node1,
	  GpNode* node2,
	  ymuint32 fv,
	  ymuint32 mask,
	  bool first)
{
  if ( !first ) {
    for (size_t pp = 0; pp < 4; ++ pp) {
      ymuint32 tmp1 = node1->pat();
      if ( pp & 1U ) {
	tmp1 = ~tmp1;
      }
      ymuint32 tmp2 = node2->pat();
      if ( pp & 2U ) {
	tmp2 = ~tmp2;
      }
      ymuint32 tmp = tmp1 & tmp2 & mask;
      if ( tmp & 1U ) {
	tmp = ~tmp & mask;
      }
      if ( tmp == fv ) {
#if 0
	cout << "check_fv2: " << hex << fv << dec << endl;
	dump_gp(node1);
	cout << endl;
	dump_gp(node2);
	cout << endl;
#endif
	return true;
      }
    }
  }

  if ( node1->is_and() ) {
    if ( check_fv2(node1->fanin0(), node2, fv, mask, false) ) {
      return true;
    }
    if ( check_fv2(node1->fanin1(), node2, fv, mask, false) ) {
      return true;
    }
  }
  if ( node2->is_and() ) {
    if ( check_fv2_sub(node1, node2->fanin0(), fv, mask) ) {
      return true;
    }
    if ( check_fv2_sub(node1, node2->fanin1(), fv, mask) ) {
      return true;
    }
  }
  return false;
}

// 2つの関数の合成を行なう．
void
merge_func(ymuint32 func1,
	   ymuint32 func2,
	   ymuint level,
	   ymuint32 mask)
{
  for (ymint pp = 0; pp < 4; ++ pp) {
    ymuint32 fv1 = func1;
    bool inv1 = false;
    if ( pp & 1U ) {
      fv1 = ~fv1;
      inv1 = true;
    }
    ymuint32 fv2 = func2;
    bool inv2 = false;
    if ( pp & 2U ) {
      fv2 = ~fv2;
      inv2 = true;
    }
    ymuint32 fv = fv1 & fv2 & mask;
    // 出力極性の正規化を行う．
    ymuint32 fv0 = fv;
    bool inv = false;
    if ( fv & 1U ) {
      fv0 = ~fv & mask;
      inv = true;
    }
    if ( fv0 == 0U ) {
      // 定数は無視
      continue;
    }
    if ( func_table[fv0].push_back(level, fv1, fv2, pp) ) {
      dst_func_list.push_back(fv0);
    }
  }
}

// 非冗長なパタンを全列挙する．
void
genpat(ymuint ni)
{
  assert_cond( ni <= 5, __FILE__, __LINE__);

  cout << endl;
  cout << "genpat(" << ni << ")" << endl;

  // マスク
  ymuint np = 1U << ni;
  ymuint nf = 1U << np;
  ymuint32 mask = nf - 1U;

  // 関数ベクタをキーとしてパタンリストを保持する配列
  vector<PatList> func_table(nf);

  // レベルごとの関数のリスト
  vector<vector<ymuint32> > func_list_array;

  // レベル0のパタンを作る．
  func_list_array.push_back(vector<ymuint32>());
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint32 fv;
    switch ( i ) {
    case 0: fv = 0xaaaaaaaa; break;
    case 1: fv = 0xcccccccc; break;
    case 2: fv = 0xf0f0f0f0; break;
    case 3: fv = 0xff00ff00; break;
    case 4: fv = 0xffff0000; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
    fv &= mask;
    assert_cond(( fv & 1U) == 0, __FILE__, __LINE__);
    func_table[fv].push_back(0, 0, 0, 0);
    func_list_array[0].push_back(fv);
  }

  for (ymuint level = 1; ; ++ level) {
    func_list_array.push_back(vector<ymuint32>());
    vector<ymuint32>& dst_func_list = func_list_array[level];
    ymuint slevel = level - 1;
    for (ymuint llevel = 0; llevel <= slevel; ++ llevel) {
      ymuint rlevel = slevel - llevel;
      // llevel 左の子供のレベル
      // rlevel 右の子供のレベル
      if ( llevel == rlevel ) {
	const vector<ymuint32>& func_list = func_list_array[llevel];
	ymuint n = func_list.size();
	for (ymuint l = 0; l < n; ++ l) {
	  ymuint32 func1 = func_list[l];
	  for (ymuint r = l + 1; r < n; ++ r) {
	    ymuint32 func2 = func_list[r];
	    merge_func(func1, func2, level, mask);
	  }
	}
      }
      else {
	const vector<ymuint32>& l_func_list = func_list_array[llevel];
	const vector<ymuint32>& r_func_list = func_list_array[rlevel];
	ymuint nl = l_func_list.size();
	ymuint nr = r_func_list.size();
	for (ymuint l = 0; l < nl; ++ l) {
	  ymuint32 func1 = l_func_list[l];
	  for (ymuint r = 0; r < nr; ++ r) {
	    ymuint32 func2 = r_func_list[r];
	    merge_func(func1, func2, level, mask);
	  }
	}
      }
    }
  }

#if 0
  for (ymuint rpos = 0; rpos < func_list.size(); ++ rpos) {
    ymuint32 func1 = func_list[rpos];
    for (ymuint i =  0; i < rpos; ++ i) {
      ymuint32 func2 = func_list[i];
      // func1 と func2 を子供とするパタンを生成する．
      // ただしファンインの極性の組み合わせが4通りある．

      // サイズを計算
      ymuint32 size1 = func_table[func1].mMinSize;
      ymuint32 size2 = func_table[func2].mMinSize;
      ymuint32 size = size1 + size2 + 1;
      for (ymint pp = 0; pp < 4; ++ pp) {
	ymuint32 fv1 = func1;
	bool inv1 = false;
	if ( pp & 1U ) {
	  fv1 = ~fv1;
	  inv1 = true;
	}
	ymuint32 fv2 = func2;
	bool inv2 = false;
	if ( pp & 2U ) {
	  fv2 = ~fv2;
	  inv2 = true;
	}
	ymuint32 fv = fv1 & fv2 & mask;
	// 出力極性の正規化を行う．
	ymuint32 fv0 = fv;
	bool inv = false;
	if ( fv & 1U ) {
	  fv0 = ~fv & mask;
	  inv = true;
	}
	if ( fv0 == 0U ) {
	  // 定数は無視
	  continue;
	}
	if ( func_table[fv0].push_back(size, fv1, fv2, pp) ) {
	  func_list.push_back(fv0);
	  //cout << hex << func1 << " x " << func2 << " = " << fv0 << dec << endl;
	}
      }
    }
  }
#endif

  ymuint total_cost = 0;
  for (ymuint i = 2; i < nf; i += 2) {
    if ( func_table[i].mList.empty() ) {
      cout << "error for function#" << i << endl;
      exit(1);
    }
    total_cost += func_table[i].mMinSize;
  }
  cout << "total cost = " << total_cost << endl
       << endl;
}
#endif
END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  GenPat genpat;

  cout << "genpat(2)" << endl;
  genpat(2, false);

  cout << "genpat(3)" << endl;
  genpat(3, false);

  cout << "genpat(4)" << endl;
  genpat(4, false);

  return 0;
}
