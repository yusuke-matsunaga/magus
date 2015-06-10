
/// @file GenVidMap.cc
/// @brief GenVidMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "GenVidMap.h"
#include "TpgNode.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief コンストラクタ
// @param[in] max_id ノード番号の最大値
GenVidMap::GenVidMap(ymuint max_id) :
  mVidArray(max_id)
{
}

// @brief デストラクタ
GenVidMap::~GenVidMap()
{
}

// @brief ノードに関連した変数番号を返す．
// @param[in] node 対象のノード
VarId
GenVidMap::operator()(const TpgNode* node) const
{
  ASSERT_COND( node->id() < mVidArray.size() );
  return mVidArray[node->id()];
}

// @brief 初期化する．
// @param[in] max_id ノード番号の最大値
void
GenVidMap::init(ymuint max_id)
{
  mVidArray.clear();
  mVidArray.resize(max_id);
}

// @brief ノードに関連した変数番号を設定する．
// @param[in] node 対象のノード
// @param[in] vid 変数番号
void
GenVidMap::set_vid(const TpgNode* node,
		   VarId vid)
{
  ASSERT_COND( node->id() < mVidArray.size() );
  mVidArray[node->id()] = vid;
}

END_NAMESPACE_YM_SATPG
