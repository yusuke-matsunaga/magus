
/// @file lutmap/CrLevelQ.cc
/// @brief CrLevelQ の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
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
void
CrLevelQ::init(
  SizeType max_level
)
{
  mLevelQ.clear();
  mLevelQ.resize(max_level + 1);
}

// @brief 内容をクリアする．
void
CrLevelQ::clear()
{
  for ( auto p: mLevelQ ) {
    p.clear();
  }
  mNum = 0;
  mMaxLevel = 0;
  mMinLevel = 0;
}

// @brief ノードを追加する．
void
CrLevelQ::put(
  CrNode* node
)
{
  SizeType l = node->sbjnode()->level();
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
    for ( SizeType l = mMinLevel; l <= mMaxLevel; ++ l ) {
      auto& lq = mLevelQ[l];
      if ( !lq.empty() ) {
	auto node = lq.front();
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
    for ( SizeType l = mMaxLevel; l >= mMinLevel; -- l ) {
      auto& lq = mLevelQ[l];
      if ( !lq.empty() ) {
	auto node = lq.front();
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
