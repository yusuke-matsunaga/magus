﻿
/// @file lutmap/CrLevelQ.cc
/// @brief CrLevelQ の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "CrLevelQ.h"
#include "CrNode.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
// クラス CrLevelQ
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CrLevelQ::CrLevelQ()
{
  clear();
}

// @brief デストラクタ
CrLevelQ::~CrLevelQ()
{
}

// @brief 作業領域の初期化を行う．
// @param[in] max_level 最大レベル
void
CrLevelQ::init(ymuint max_level)
{
  mLevelQ.clear();
  mLevelQ.resize(max_level + 1);
}

// @brief 内容をクリアする．
void
CrLevelQ::clear()
{
  for (vector<list<CrNode*> >::iterator p = mLevelQ.begin();
       p != mLevelQ.end(); ++ p) {
    p->clear();
  }
  mNum = 0;
  mMaxLevel = 0;
  mMinLevel = 0;
}

// @brief ノードを追加する．
void
CrLevelQ::put(CrNode* node)
{
  ymuint l = node->sbjnode()->level();
  mLevelQ[l].push_back(node);
  if ( mNum == 0 ) {
    mMinLevel = l;
    mMaxLevel = l;
  }
  else {
    if ( mMinLevel > l ) {
      mMinLevel = l;
    }
    if ( mMaxLevel < l ) {
      mMaxLevel = l;
    }
  }
  ++ mNum;
}

// @brief 最小レベルの要素を取り出す．
CrNode*
CrLevelQ::getmin()
{
  if ( mNum > 0 ) {
    for (ymuint l = mMinLevel; l <= mMaxLevel; ++ l ) {
      list<CrNode*>& lq = mLevelQ[l];
      if ( !lq.empty() ) {
	CrNode* node = lq.front();
	lq.pop_front();
	mMinLevel = l;
	-- mNum;
	return node;
      }
    }
    ASSERT_NOT_REACHED;
  }
  return nullptr;
}

// @brief 最大レベルの要素を取り出す．
CrNode*
CrLevelQ::getmax()
{
  if ( mNum > 0 ) {
    for (ymuint l = mMaxLevel; l >= mMinLevel; -- l ) {
      list<CrNode*>& lq = mLevelQ[l];
      if ( !lq.empty() ) {
	CrNode* node = lq.front();
	lq.pop_front();
	mMaxLevel = l;
	-- mNum;
	return node;
      }
    }
    ASSERT_NOT_REACHED;
  }
  return nullptr;
}

END_NAMESPACE_LUTMAP
