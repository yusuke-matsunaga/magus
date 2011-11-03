#ifndef ENUMCUT_H
#define ENUMCUT_H

/// @file EnumCut.h
/// @brief EnumCut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class EnumCut EnumCut.h "EnumCut.h"
/// @brief カット列挙を行う基底クラス
//////////////////////////////////////////////////////////////////////
class EnumCut
{
public:

  /// @brief コンストラクタ
  EnumCut();

  /// @brief デストラクタ
  virtual
  ~EnumCut();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(const BdnMgr& network,
	     ymuint limit) = 0;


protected:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  void
  node_init(const BdnNode* node);

  /// @brief cut が一つ見つかったときに呼ばれる関数(singlton cut)
  /// @param[in] root 根のノード
  void
  found_cut(const BdnNode* root);

  /// @brief cut が一つ見つかったときに呼ばれる関数(non-trivial cut)
  /// @param[in] root 根のノード
  /// @param[in] ni 入力数
  /// @param[in] inputs 入力ノードの配列
  void
  found_cut(const BdnNode* root,
	    ymuint32 ni,
	    ymuint32 inputs[]);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  void
  node_end(const BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在処理中のノード
  const BdnNode* mCurNode;

  // 現在処理中のノード番号
  ymuint32 mCurPos;

  // 現在のカット数
  ymuint32 mNcCur;

  // 全カット数
  ymuint32 mNcAll;

};

END_NAMESPACE_YM_NETWORKS

#endif // ENUMCUT_H
