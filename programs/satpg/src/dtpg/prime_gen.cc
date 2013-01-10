
/// @file prime_gen.cc
/// @brief プライムカバーの生成を行う．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "ym_logic/Literal.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

BEGIN_NONAMESPACE

bool debug = false;

//////////////////////////////////////////////////////////////////////
// キューブを表す構造体(常に入力数分の要素を持つポジションマトリクス)
//////////////////////////////////////////////////////////////////////
struct Cube
{
  /// @brief コンストラクタ
  /// @param[in] ni 入力数
  /// @note 全ビットドントケアで初期化される．
  Cube(ymuint ni);

  // 入力数
  ymuint32 mNi;

  // キューブを表す配列
  // - 0: 未使用
  // - 1: '1'
  // - 2: '0'
  // - 3: '-'
  vector<ymuint8> mVector;

  // '1'の数
  ymuint32 mOnesCount;

  // prime マーク
  // マージに用いたら消す．
  bool mPrime;

};

Cube::Cube(ymuint ni) :
  mNi(ni),
  mVector(ni, 3),
  mOnesCount(0),
  mPrime(true)
{
}

// 等価比較関数
bool
operator==(const Cube& a,
	   const Cube& b)
{
  if ( a.mNi != b.mNi ) {
    return false;
  }
  for (ymuint i = 0; i < a.mNi; ++ i) {
    if ( a.mVector[i] != b.mVector[i] ) {
      return false;
    }
  }
  return true;
}


//////////////////////////////////////////////////////////////////////
// Cube::mOnesCount を用いた比較関数オブジェクト
//////////////////////////////////////////////////////////////////////
struct CubeLt
{
  bool
  operator()(Cube* a,
	     Cube* b)
  {
    return a->mOnesCount < b->mOnesCount;
  }
};

// Cube を表示する(デバッグ用)
void
dump_cube(ostream& s,
	  const Cube* cube)
{
  ymuint ni = cube->mNi;
  for (ymuint i = 0; i < ni; ++ i) {
    switch ( cube->mVector[i] ) {
    case 1: s << '1'; break;
    case 2: s << '0'; break;
    case 3: s << '-'; break;
    default: assert_not_reached(__FILE__, __LINE__);
    }
  }
  cout << endl;
}

// Cube のリストを表示する(デバッグ用)
void
dump_cube_list(ostream& s,
	       const vector<Cube*>& cube_list)
{
  ymuint nc = cube_list.size();
  ymuint prev_oc = -1;
  for (ymuint i = 0; i < nc; ++ i) {
    Cube* cube = cube_list[i];
    if ( cube->mOnesCount != prev_oc ) {
      s << endl;
      prev_oc = cube->mOnesCount;
    }
    dump_cube(s, cube);
  }
  s << endl;
}

END_NONAMESPACE

void
prime_gen(ymuint ni,
	  const vector<ymuint32>& minterm_list,
	  vector<vector<Literal> >& cover)
{
  if ( debug ) {
    cout << "prime_gen" << endl;
    for (vector<ymuint32>::const_iterator p = minterm_list.begin();
	 p != minterm_list.end(); ++ p) {
      ymuint32 pat = *p;
      for (ymuint i = 0; i < ni; ++ i) {
	if ( pat & (1U << i) ) {
	  cout << '1';
	}
	else {
	  cout << '0';
	}
      }
      cout << endl;
    }
    cout << endl;
  }

  // 生成した全てのキューブを格納するリスト
  vector<Cube*> all_cube_list;

  // マージ用に用いるキューブのリスト x 2
  vector<Cube*> cube_list[2];

  // 最小項のリストを作る．
  for (vector<ymuint>::const_iterator p = minterm_list.begin();
       p != minterm_list.end(); ++ p) {
    ymuint32 pat = *p;
    Cube* cube = new Cube(ni);
    ymuint n = 0;
    for (ymuint i = 0; i < ni; ++ i) {
      ymuint32 mask = 1U << i;
      if ( pat & mask ) {
	cube->mVector[i] = 1;
	++ n;
      }
      else {
	cube->mVector[i] = 2;
      }
    }
    cube->mOnesCount = n;
    cube_list[0].push_back(cube);
  }

  // '1' の数で整列させる．
  sort(cube_list[0].begin(), cube_list[0].end(), CubeLt());

  ymuint step = 0;
  ymuint src_pos = 0;
  ymuint dst_pos = 1;
  while ( !cube_list[src_pos].empty() ) {
    if ( debug ) {
      cout << "STEP#" << step << endl;
      dump_cube_list(cout, cube_list[src_pos]);
      ++ step;
    }
    // '1' の数が1違いなもの同士でマージを試みる．
    cube_list[dst_pos].clear();
    ymuint nc = cube_list[src_pos].size();
    for (ymuint i1 = 0; i1 < nc; ++ i1) {
      Cube* cube1 = cube_list[src_pos][i1];
      ymuint oc = cube1->mOnesCount;
      for (ymuint i2 = i1 + 1; i2 < nc; ++ i2) {
	Cube* cube2 = cube_list[src_pos][i2];
	if ( cube2->mOnesCount <= oc ) {
	  continue;
	}
	if ( cube2->mOnesCount > oc + 1 ) {
	  break;
	}
	ymuint nd = 0;
	ymuint dpos = 0;
	for (ymuint b = 0; b < ni; ++ b) {
	  ymuint p1 = cube1->mVector[b];
	  ymuint p2 = cube2->mVector[b];
	  if ( p1 == p2 ) {
	    continue;
	  }
	  ++ nd;
	  if ( nd > 1 ) {
	    break;
	  }
	  if ( (p1 ^ p2) == 3 ) {
	    dpos = b;
	  }
	}
	if ( nd == 1 ) {
	  Cube* new_cube = new Cube(ni);
	  for (ymuint b = 0; b < ni; ++ b) {
	    if ( b == dpos ) {
	      new_cube->mVector[b] = 3;
	    }
	    else {
	      new_cube->mVector[b] = cube1->mVector[b];
	    }
	  }
	  new_cube->mOnesCount = cube1->mOnesCount;
	  bool found = false;
	  for (vector<Cube*>::iterator p = cube_list[dst_pos].begin();
	       p != cube_list[dst_pos].end(); ++ p) {
	    Cube* cube = *p;
	    if ( *cube == *new_cube ) {
	      found = true;
	      break;
	    }
	  }
	  if ( !found ) {
	    cube_list[dst_pos].push_back(new_cube);
	    all_cube_list.push_back(new_cube);
	  }
	  cube1->mPrime = false;
	  cube2->mPrime = false;
	}
      }
    }
    src_pos ^= 1;
    dst_pos ^= 1;
  }

  for (vector<Cube*>::iterator p = all_cube_list.begin();
       p != all_cube_list.end(); ++ p) {
    Cube* cube = *p;
    if ( cube->mPrime ) {
      vector<Literal> tmp_cube;
      for (ymuint i = 0; i < ni; ++ i) {
	switch ( cube->mVector[i] ) {
	case 1: tmp_cube.push_back(Literal(VarId(i), kPolPosi)); break;
	case 2: tmp_cube.push_back(Literal(VarId(i), kPolNega)); break;
	case 3: break;
	default: assert_not_reached(__FILE__, __LINE__);
	}
      }
      cover.push_back(tmp_cube);
    }
  }

  if ( debug ) {
    cout << "FINAL RESULT" << endl;
    for (vector<Cube*>::iterator p = all_cube_list.begin();
	 p != all_cube_list.end(); ++ p) {
      Cube* cube = *p;
      if ( cube->mPrime ) {
	dump_cube(cout, cube);
      }
    }
    cout << endl;
  }
}

END_NAMESPACE_YM_SATPG_DTPG
