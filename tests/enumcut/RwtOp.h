
/// @file RwtOp.h
/// @brief RwtOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "EnumCut2.h"

#include "RwtMgr.h"
#include "RwtPat.h"
#include "RwtNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class RwtOp RwtOp.h "RwtOp.h"
/// @brief local rewrting を行うための EnumCutOp
//////////////////////////////////////////////////////////////////////
class RwtOp :
  public EnumCutOp2
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
  all_init(BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(BdnNode* node);

  /// @brief カットを見つけたときに呼ばれる関数
  virtual
  void
  found_cut(BdnNode* root,
	    ymuint ni,
	    BdnNode** inputs);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  calc_func(BdnNode* node);

  void
  clear_mark(BdnNode* node);

  ymuint
  count_node(BdnNode* node);


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
  BdnMgr* mNetwork;

  // 各ノードの作業領域
  vector<NodeInfo> mNodeInfo;

};

END_NAMESPACE_YM_NETWORKS
