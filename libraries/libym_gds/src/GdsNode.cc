
/// @file GdsNode.cc
/// @brief GdsNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GdsNode.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elflags ELFLAGS の値
// @param[in] plex PLEX の値
// @param[in] layer LAYER の値
// @param[in] datatype DATATYPE の値
// @param[in] xy XY の値
GdsNode::GdsNode(ymuint16 elflags,
		 ymint32 plex,
		 ymint16 layer,
		 ymint16 nodetype,
		 GdsXY* xy) :
  GdsElement(elflags, plex),
  mLayer(layer),
  mNodeType(nodetype),
  mXY(xy)
{
}

// @briefデストラクタ
GdsNode::~GdsNode()
{
}

// @brief層番号を返す．
int
GdsNode::layer() const
{
  return mLayer;
}

// @brief ノード型を返す．
int
GdsNode::nodetype() const
{
  return mNodeType;
}

// @brief 座標のリストを返す．
GdsXY*
GdsNode::xy() const
{
  return mXY;
}

END_NAMESPACE_YM_GDS
