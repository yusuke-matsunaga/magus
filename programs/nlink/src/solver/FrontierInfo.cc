
/// @file FrontierInfo.cc
/// @brief FrontierInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FrontierInfo.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス FrontierInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FrontierInfo::FrontierInfo()
{
}

// @brief コンストラクタ
// @param[in] src コピー元のソース
FrontierInfo::FrontierInfo(const FrontierInfo& src) :
  mNodeList(src.mNodeList)
{
}

// @brief デストラクタ
FrontierInfo::~FrontierInfo()
{
}

// @brief ノード数を返す．
ymuint
FrontierInfo::node_num() const
{
  return mNodeList.size();
}

// @brief ノード番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
ymuint
FrontierInfo::node_id(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mId;
}

// @brief 次数を返す．
// @param[in] pos ノードの位置番号
//
// pos はこのフロンティア内の相対的な位置
// ノード番号ではない．
ymuint
FrontierInfo::deg(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mDeg;
}

// @brief 連結成分番号を返す．
// @param[in] pos ノードの位置番号
//
// pos はこのフロンティア内の相対的な位置
// ノード番号ではない．
// 端子につながっている場合は - 端子番号を
// を返す．
int
FrontierInfo::comp(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mComp;
}

// @brief ノードを削除する．
// @param[in] pos ノードの位置番号
void
FrontierInfo::delete_node(ymuint pos)
{
  ASSERT_COND( pos < node_num() );
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < node_num(); ++ rpos) {
    if ( pos != rpos ) {
      const Node& node = mNodeList[rpos];
      if ( wpos != rpos ) {
	mNodeList[wpos] = node;
      }
      ++ wpos;
    }
  }
  mNodeList.erase(mNodeList.begin() + wpos, mNodeList.end());
}

// @brief ノードを追加する．
// @param[in] node_id ノード番号
// @param[in] deg 次数
// @param[in] comp_id 連結成分番号
//
// deg は 1, comp は自分自身となる．
void
FrontierInfo::add_node(ymuint node_id,
		       ymuint deg,
		       int comp_id)
{
  mNodeList.push_back(Node(node_id, deg, comp_id));
}

END_NAMESPACE_YM_NLINK
