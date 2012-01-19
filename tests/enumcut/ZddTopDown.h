#ifndef ZDDTOPDOWN_H
#define ZDDTOPDOWN_H

/// @file ZddTopDown.h
/// @brief ZddTopDown のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ym_logic/ZddMgr.h"
#include "ym_networks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class ZddTopDown ZddTopDown.h "ZddTopDown.h"
/// @brief ZDD を用いた非明示的列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddTopDown
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr
  ZddTopDown(ZddMgr& mgr);

  /// @brief デストラクタ
  ~ZddTopDown();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  void
  operator()(BdnMgr& network,
	     ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeTemp
  {
    // カット集合
    Zdd mCut;

    // footprint ノード
    vector<ymuint> mFpNodeList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgr& mMgr;

  // ノードの情報
  vector<NodeTemp> mNodeTemp;

};

END_NAMESPACE_YM_NETWORKS

#endif // ZDDTOPDOWN_H
