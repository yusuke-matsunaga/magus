
/// @file libym_seal/Dfs.cc
/// @brief Dfs の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dfs.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Dfs.h"


BEGIN_NAMESPACE_YM_SEAL

// @brief コンストラクタ
// @param[in] num ノード数
Dfs::Dfs(ymuint num) :
  mNum(num),
  mNodeArray(new DfsNode[num])
{
  for (ymuint i = 0; i < num; ++ i) {
    DfsNode* node = &mNodeArray[i];
    node->mId = i;
    node->mVisited = false;
    node->mOnStack = false;
    node->mLink = NULL;
  }
  mRepNodes.reserve(num);
  mStack.reserve(num);
}

// @brief デストラクタ
Dfs::~Dfs()
{
  delete [] mNodeArray;
}

// @biref ノードの接続関係を設定する．
// @param[in] id ノード番号
// @param[in] adjlist 遷移先のノード番号のリスト
void
Dfs::set_adjlist(ymuint id,
		 const vector<ymuint>& adjlist)
{
  DfsNode* node = &mNodeArray[id];
  ymuint n = adjlist.size();
  node->mAdjList.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    node->mAdjList[i] = &mNodeArray[adjlist[i]];
  }
}

// @brief dfs を行って強連結成分を求める．
ymuint
Dfs::scc()
{
  mCount = 0;
  mRepNodes.clear();
  mStack.clear();
  for (ymuint i = 0; i < mNum; ++ i) {
    DfsNode* node = &mNodeArray[i];
    node->mVisited = false;
    node->mOnStack = false;
    node->mLink = NULL;
  }
  for (ymuint i = 0; i < mNum; ++ i) {
    DfsNode* node = &mNodeArray[i];
    if ( !node->mVisited ) {
      dfs(node);
    }
  }
  return mRepNodes.size();
}

// @brief 強連結成分の代表ノードを返す．
// @param[in] id 強連結成分の番号
DfsNode*
Dfs::repnode(ymuint id)
{
  return mRepNodes[id];
}

// @brief scc のために dfs を行う関数
void
Dfs::dfs(DfsNode* node)
{
  node->mVisited = true;
  node->mDefnumber = mCount;
  node->mLowlink = mCount;
  ++ mCount;
  mStack.push_back(node);
  node->mOnStack = true;
  for (vector<DfsNode*>::iterator p = node->mAdjList.begin();
       p != node->mAdjList.end(); ++ p) {
    DfsNode* node1 = *p;
    if ( node1->mVisited ) {
      if ( node1->mDefnumber < node->mDefnumber && node1->mOnStack ) {
	if ( node->mLowlink > node1->mLowlink ) {
	  node->mLowlink = node1->mLowlink;
	}
      }
    }
    else {
      dfs(node1);
      if ( node->mLowlink > node1->mLowlink ) {
	node->mLowlink = node1->mLowlink;
      }
    }
  }
  if ( node->mLowlink == node->mDefnumber ) {
    for (DfsNode* last = NULL; last != node; ) {
      DfsNode* node1 = mStack.back();
      mStack.pop_back();
      node1->mOnStack = false;
      node1->mLink = last;
      last = node1;
    }
    mRepNodes.push_back(node);
  }
}

END_NAMESPACE_YM_SEAL
