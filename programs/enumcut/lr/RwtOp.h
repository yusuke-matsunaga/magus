
/// @file RwtOp.h
/// @brief RwtOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "EnumCut.h"

#include "RwtMgr.h"
#include "RwtPat.h"
#include "RwtNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class RwtOp RwtOp.h "RwtOp.h"
/// @brief local rewrting を行うための EnumCutOp
//////////////////////////////////////////////////////////////////////
class RwtOp :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  RwtOp(RwtMgr& mgr);

  /// @brief デストラクタ
  virtual
  ~RwtOp();


public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(const BdnNode* node);

  /// @brief カットを見つけたときに呼ばれる関数
  virtual
  void
  found_cut(const BdnNode* root,
	    ymuint ni,
	    const BdnNode** inputs);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(const BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  calc_func(const BdnNode* node);

  void
  clear_mark(const BdnNode* node);

  ymuint
  count_node(const BdnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeInfo
  {
    ymuint32 mMark;

    TvFunc mFunc;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // RwtMgr
  RwtMgr& mMgr;

  // 対象のネットワーク
  const BdnMgr* mNetwork;

  // 各ノードの作業領域
  vector<NodeInfo> mNodeInfo;

};

END_NAMESPACE_YM_NETWORKS
