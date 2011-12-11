
/// @file Restorer.cc
/// @brief Restorer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Restorer.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(BddMgrImpl* mgr,
		   BinI& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Restorer::~Restorer()
{
}

// 読み込む．
// 読み込んだBDDの根の枝の数を返す．
// エラーが起きたら 0 を返す．
ymuint64
Restorer::read()
{
  mRootVector.clear();
  mEdgeVector.clear();
  for ( ; ; ) {
    VarId varid;
    mStream >> varid;
    if ( varid == kVarIdIllegal ) {
      // エンドマーク
      break;
    }
    BddEdge e0 = read_edge();
    BddEdge e1 = read_edge();
    BddEdge ans = mMgr->make_bdd(varid, e0, e1);
    mEdgeVector.push_back(ans);
  }
  ymuint64 n;
  mStream >> n;
  for (ymuint64 i = 0; i < n; ++ i) {
    // 根のノードの記述
    BddEdge root = read_edge();
    mRootVector.push_back(root);
  }
  return mRootVector.size();
}

// pos 番目の枝を返す．
BddEdge
Restorer::root(ymuint pos)
{
  if ( pos < mRootVector.size() ) {
    return mRootVector[pos];
  }
  return BddEdge::make_error();
}

// 内部の枝を指す文字列から枝を得る．
// 見つからなければ kEdgeError を返す．
BddEdge
Restorer::read_edge()
{
  ymuint8 type;
  mStream >> type;
  bool negated = false;
  switch ( type ) {
  case 0: return BddEdge::make_zero();
  case 1: return BddEdge::make_one();
  case 2: return BddEdge::make_error();
  case 3: return BddEdge::make_overflow();
  case 4: break;
  case 5: negated = true; break;
  default: return BddEdge::make_error();
  }
  ymuint64 id;
  mStream >> id;
  if ( id < mEdgeVector.size() ) {
    BddEdge ans = mEdgeVector[id];
    if ( negated ) {
      ans.negate();
    }
    return ans;
  }
  return BddEdge::make_error();
}

END_NAMESPACE_YM_BDD
