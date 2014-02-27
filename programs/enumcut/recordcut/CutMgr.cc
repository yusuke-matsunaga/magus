
/// @file CutMgr.cc
/// @brief CutMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "CutMgr.h"
#include "Cut.h"
#include "networks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス CutMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CutMgr::CutMgr()
{
}

// @brief デストラクタ
CutMgr::~CutMgr()
{
  clear();
}

// @brief 初期化する．
void
CutMgr::clear()
{
  for (list<Cut*>::iterator p = mCutList.begin();
       p != mCutList.end(); ++ p) {
    delete *p;
  }
  mAllFunc.clear();
  mRepFunc.clear();
  mCutList.clear();
}

BEGIN_NONAMESPACE

TvFunc
make_func(const BdnNode* node,
	  hash_map<ymuint, TvFunc>& f_map)
{
  hash_map<ymuint, TvFunc>::const_iterator p = f_map.find(node->id());
  if ( p != f_map.end() ) {
    return p->second;
  }

  assert_cond( node->type() == BdnNode::kLOGIC, __FILE__, __LINE__);

  TvFunc func0 = make_func(node->fanin0(), f_map);
  if ( node->fanin0_inv() ) {
    func0 = ~func0;
  }

  TvFunc func1 = make_func(node->fanin1(), f_map);
  if ( node->fanin1_inv() ) {
    func1 = ~func1;
  }

  TvFunc func;
  if ( node->is_and() ) {
    func = func0 & func1;
  }
  else {
    func = func0 ^ func1;
  }

  f_map.insert(make_pair(node->id(), func));

  return func;
}

Aig
make_aig(const BdnNode* node,
	 AigMgr& aigmgr,
	 hash_map<ymuint, Aig>& aig_map)
{
  {
    hash_map<ymuint, Aig>::iterator p = aig_map.find(node->id());
    if ( p != aig_map.end() ) {
      return p->second;
    }
  }

  assert_cond( node->type() == BdnNode::kLOGIC, __FILE__, __LINE__);

  Aig aig0 = make_aig(node->fanin0(), aigmgr, aig_map);
  if ( node->fanin0_inv() ) {
    aig0 = ~aig0;
  }

  Aig aig1 = make_aig(node->fanin1(), aigmgr, aig_map);
  if ( node->fanin1_inv() ) {
    aig1 = ~aig1;
  }

  Aig aig;
  if ( node->is_and() ) {
    aig = aigmgr.make_and(aig0, aig1);
  }
  else {
    aig = aigmgr.make_xor(aig0, aig1);
  }

  aig_map.insert(make_pair(node->id(), aig));

  return aig;
}

END_NONAMESPACE

// @brief カットを登録する．
// @note すでの同じ関数のカットがあればそれを返す．
Cut*
CutMgr::new_cut(const BdnNode* root,
		ymuint ni,
		const BdnNode** inputs)
{
  // 関数を作る．
  hash_map<ymuint, TvFunc> f_map;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = inputs[i];
    TvFunc f = TvFunc::posi_literal(ni, VarId(i));
    f_map.insert(make_pair(node->id(), f));
  }
  TvFunc f = make_func(root, f_map);

  if ( mAllFunc.count(f) > 0 ) {
    return NULL;
  }
  mAllFunc.insert(f);

#if 0
  NpnMap cmap;
  mNpnMgr.cannonical(f, cmap);

  TvFunc frep = f.xform(cmap);

  if ( mRepFunc.count(frep) > 0 ) {
    return NULL;
  }
  mRepFunc.insert(frep);
#endif
#if 0
  // AIG を作る．
  hash_map<ymuint, Aig> aig_map;
  for (ymuint i = 0; i < ni; ++ i) {
    const BdnNode* node = inputs[i];
    Aig aig = mAigMgr.make_input(VarId(i));
    aig_map.insert(make_pair(node->id(), aig));
  }
  Aig aig = make_aig(root, mAigMgr, aig_map);
#else
  Aig aig;
#endif

  Cut* cut = new Cut(f, aig);

  mCutList.push_back(cut);

  return cut;
}

// @brief 全カットのリストを返す．
const list<Cut*>&
CutMgr::cut_list() const
{
  return mCutList;
}

BEGIN_NONAMESPACE

void
dump_aig(ODO& s,
	 Aig aig)
{
  if ( aig.is_zero() ) {
    s << static_cast<ymuint8>(0);
  }
  else if ( aig.is_one() ) {
    s << static_cast<ymuint8>(1);
  }
  else if ( aig.is_input() ) {
    ymuint8 type = 2;
    if ( aig.inv() ) {
      type |= 1;
    }
    s << type
      << aig.input_id();
  }
  else {
    ymuint8 type = 4;
    if ( aig.inv() ) {
      type |= 1;
    }
    s << type;
    dump_aig(s, aig.fanin0());
    dump_aig(s, aig.fanin1());
  }
}

END_NONAMESPACE

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
CutMgr::dump(ODO& s) const
{
  // カット数をダンプする．
  ymuint64 n = mCutList.size();
  s << n;

  // カットの内容をダンプする．
  for (list<Cut*>::const_iterator p = mCutList.begin();
       p != mCutList.end(); ++ p) {
    Cut* cut = *p;
    cut->function().dump(s);
    dump_aig(s, cut->aig());
  }
}

BEGIN_NONAMESPACE

Aig
restore_aig(IDO& s,
	    AigMgr& aig_mgr)
{
  ymuint8 type;
  s >> type;
  switch ( type ) {
  case 0: return aig_mgr.make_zero();
  case 1: return aig_mgr.make_one();
  case 2:
    {
      VarId id;
      s >> id;
      return aig_mgr.make_input(id);
    }

  case 3:
    {
      VarId id;
      s >> id;
      return ~aig_mgr.make_input(id);
    }
  }

  Aig aig0 = restore_aig(s, aig_mgr);
  Aig aig1 = restore_aig(s, aig_mgr);

  Aig aig = aig_mgr.make_and(aig0, aig1);

  if ( type & 1U ) {
    aig = ~aig;
  }

  return aig;
}

END_NONAMESPACE

// @brief バイナリダンプされたファイルを読み込む．
void
CutMgr::restore(IDO& s)
{
  // 現在の内容をクリアする．
  clear();

  ymuint64 n;
  s >> n;

  for (ymuint i = 0; i < n; ++ i) {
    TvFunc f;
    f.restore(s);
    Aig aig = restore_aig(s, mAigMgr);
    Cut* cut = new Cut(f, aig);

    NpnMap cmap;
    mNpnMgr.cannonical(f, cmap);

    TvFunc frep = f.xform(cmap);

    mAllFunc.insert(f);
    mRepFunc.insert(frep);
    mCutList.push_back(cut);
  }
}

END_NAMESPACE_YM
