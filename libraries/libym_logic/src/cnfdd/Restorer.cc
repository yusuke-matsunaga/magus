
/// @file Restorer.cc
/// @brief Restorer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Restorer.h"
#include "YmLogic/CNFdd.h"
//#include "YmLogic/CNFddVector.h"
//#include "YmLogic/CNFddList.h"
#include "YmLogic/CNFddMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

// @brief ダンプされた情報を ZDD を読み込む．
// @param[in] s 入力ストリーム
void
CNFdd::restore(IDO& s)
{
  Restorer restorer(mMgr, s);
  ymuint n = restorer.read();
  CNFddEdge ans;
  if ( n != 1 ) {
    // エラーもしくは複数の ZDD データだった．
    ans = CNFddEdge::make_error();
  }
  else {
    ans = restorer.root(0);
  }
  assign(ans);
}

#if 0
// @brief ダンプされた情報を ZDD ベクタに読み込む．
// @param[in] s 入力ストリーム
// @param[in] array 読み込み先の ZDD ベクタ
void
CNFddVector::restore(IDO& s)
{
  Restorer restorer(mMgr.mImpl, s);
  ymuint n = restorer.read();

  resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    CNFddEdge e = restorer.root(i);
    at(i).assign(e);
  }
}

// @brief ダンプされた情報を ZDD リストに読み込む．
// @param[in] s 入力ストリーム
void
CNFddList::restore(IDO& s)
{
  Restorer restorer(mMgr.mImpl, s);
  ymuint n = restorer.read();
  for (ymuint i = 0; i < n; ++ i) {
    CNFddEdge e = restorer.root(i);
    push_back(CNFdd(mMgr.mImpl, e));
  }
}
#endif

//////////////////////////////////////////////////////////////////////
// クラス Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(CNFddMgrImpl* mgr,
		   IDO& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Restorer::~Restorer()
{
}

// 読み込む．
// 読み込んだZDDの根の枝の数を返す．
// エラーが起きたら 0 を返す．
ymuint
Restorer::read()
{
#if 0
  mRootVector.clear();
  mEdgeVector.clear();
  char buff[1024];
  while ( mStream.getline(buff, sizeof(buff), '\n') ) {
    if ( '0' <= buff[0] && buff[0] <= '9' ) {
      // 内部のノードの記述
      char* p = strtok(buff, ": \t");
      ymuint vid = atoi(p);
      p = strtok(NULL, ": \t");
      CNFddEdge e0 = find_edge(p);
      p = strtok(NULL, ": \t");
      CNFddEdge e1 = find_edge(p);
      CNFddEdge ans = mMgr->make_zdd(vid, e0, e1);
      mEdgeVector.push_back(ans);
    }
    else {
      // 根のノードの記述
      mRootVector.push_back(find_edge(buff));
    }
  }
#endif
  return mRootVector.size();
}

// pos 番目の枝を返す．
CNFddEdge
Restorer::root(ymuint pos)
{
  if ( pos < mRootVector.size() ) {
    return mRootVector[pos];
  }
  return CNFddEdge::make_error();
}

// 内部の枝を指す文字列から枝を得る．
// 見つからなければ kEdgeError を返す．
CNFddEdge
Restorer::find_edge(const char* str) const
{
#if 0
  bool negated = false;
  switch ( str[0] ) {
  case 'T': return CNFddEdge::make_one();
  case 'F': return CNFddEdge::make_zero();
  case 'E': return CNFddEdge::make_error();
  case 'O': return CNFddEdge::make_overflow();
  case 'P': break;
  case 'N': negated = true; break;
  default: return CNFddEdge::make_error();
  }
  ymuint pos = atoi(str + 1);
  if ( pos < mEdgeVector.size() ) {
    CNFddEdge ans = mEdgeVector[pos];
    if ( negated ) {
      ans.negate();
    }
    return ans;
  }
#endif
  return CNFddEdge::make_error();
}

END_NAMESPACE_YM_CNFDD
