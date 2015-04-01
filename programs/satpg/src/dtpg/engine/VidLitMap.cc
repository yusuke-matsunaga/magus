
/// @file VidLitMap.cc
/// @brief VidLitMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "VidLitMap.h"
#include "VidMap.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// クラス VidLitMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VidLitMap::VidLitMap(const TpgNode* node,
		     const VidMap& vid_map) :
  mNode(node),
  mVidMap(vid_map)
{
}

// @brief デストラクタ
VidLitMap::~VidLitMap()
{
}

// @brief 入力数を返す．
ymuint
VidLitMap::input_size() const
{
  return mNode->fanin_num();
}

// @brief 入力のリテラルを返す．
Literal
VidLitMap::input(ymuint pos) const
{
  const TpgNode* inode = mNode->fanin(pos);
  return Literal(mVidMap(inode), false);
}

// @brief 出力のリテラルを返す．
Literal
VidLitMap::output() const
{
  return Literal(mVidMap(mNode), false);
}

END_NAMESPACE_YM_SATPG
