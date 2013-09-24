
/// @file Restorer.cc
/// @brief Restorer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Restorer.h"

#include "ym_logic/Bdd.h"
#include "ym_logic/BddVector.h"
#include "ym_logic/BddList.h"


BEGIN_NAMESPACE_YM_BDD

// @brief ダンプされた情報を BDD を読み込む．
// @param[in] s 入力ストリーム
void
Bdd::restore(IDO& s)
{
  Restorer restorer(mMgr, s);
  ymuint n = restorer.read();
  BddEdge ans;
  if ( n != 1 ) {
    // エラーもしくは複数の BDD データだった．
    ans = BddEdge::make_error();
  }
  else {
    ans = restorer.root(0);
  }
  assign(ans);
}

// @brief ダンプされた情報を読み込む．
// @param[in] s 入力ストリーム
void
BddVector::restore(IDO& s)
{
  Restorer restorer(mMgr.mImpl, s);
  ymuint n = restorer.read();
  clear();
  reserve(n);
  for (ymuint i = 0; i < n; ++ i) {
    BddEdge root = restorer.root(i);
    push_back(Bdd(mMgr.mImpl, root));
  }
}

// @brief ダンプされた情報を読み込む．
// @param[in] s 入力ストリーム
void
BddList::restore(IDO& s)
{
  Restorer restorer(mMgr.mImpl, s);
  ymuint n = restorer.read();
  clear();
  for (ymuint i = 0; i < n; ++ i) {
    BddEdge root = restorer.root(i);
    push_back(Bdd(mMgr.mImpl, root));
  }
}

//////////////////////////////////////////////////////////////////////
// Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(BddMgrImpl* mgr,
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
    BddEdge idx = mMgr->make_posiliteral(varid);
    BddEdge e0 = read_edge();
    BddEdge e1 = read_edge();
    BddEdge ans = mMgr->ite_op(idx, e1, e0);
    mEdgeVector.push_back(ans);
  }
  ymuint32 n;
  mStream >> n;
  for (ymuint32 i = 0; i < n; ++ i) {
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
