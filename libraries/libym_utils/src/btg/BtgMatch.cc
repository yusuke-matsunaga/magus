
/// @file BtgMatch.cc
/// @brief BtgMatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/BtgMatch.h"
#include "BtgMatchImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BtgMatch
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
BtgMatch::BtgMatch()
{
  mImpl = new nsBtg::BtgMatchImpl();
}

// @brief デストラクタ
BtgMatch::~BtgMatch()
{
  delete mImpl;
}

// @brief 節点数を設定する．
// @param[in] node1_num 節点グループ1の要素数
// @param[in] node2_num 節点グループ2の要素数
// @note 以前の内容はクリアされる．
void
BtgMatch::set_size(ymuint node_num1,
		   ymuint node_num2)
{
  mImpl->set_size(node_num1, node_num2);
}

// @brief 節点グループ1 の要素数を返す．
ymuint
BtgMatch::node1_num() const
{
  return mImpl->node1_num();
}

// @brief 節点グループ2 の要素数を返す．
ymuint
BtgMatch::node2_num() const
{
  return mImpl->node2_num();
}

// @brief 枝をすべて取り除く
void
BtgMatch::clear()
{
  mImpl->clear();
}

// @brief 枝を追加する．
// @param[in] node1_id 節点グループ1のノード番号 ( 0 <= node1_id < node1_num() )
// @param[in] node2_id 節点グループ2のノード番号 ( 0 <= node2_id < node2_num() )
// @param[in] weight 枝の重み (省略されたらデフォルトは1)
// @note 既に同じ枝があったら上書きされる．
void
BtgMatch::add_edge(ymuint node1_id,
		   ymuint node2_id,
		   ymuint weight)
{
  mImpl->add_edge(node1_id, node2_id, weight);
}

// @brief 枝数を返す．
ymuint
BtgMatch::edge_num() const
{
  return mImpl->edge_num();
}

// @brief 枝の情報を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
// @param[out] node1_id 節点グループ1のノード番号
// @param[out] node2_id 節点グループ2のノード番号
// @param[out] weight 枝の重み
void
BtgMatch::edge_info(ymuint pos,
		    ymuint& node1_id,
		    ymuint& node2_id,
		    ymuint& weight) const
{
  mImpl->edge_info(pos, node1_id, node2_id, weight);
}

// @brief 最大マッチングを求める．
// @param[in] edge_list マッチング結果の枝を格納するリスト
// @return マッチング結果の重みの総和を返す．
ymuint
BtgMatch::calc_match(vector<ymuint>& edge_list)
{
  return mImpl->calc_match(edge_list);
}

END_NAMESPACE_YM
