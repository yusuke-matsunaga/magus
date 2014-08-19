#ifndef ZDDIMP2_H
#define ZDDIMP2_H

/// @file ZddImp2.h
/// @brief ZddImp2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Zdd.h"
#include "YmLogic/ZddMgr.h"
#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ZddImp2 ZddImp2.h "ZddImp2.h"
/// @brief ZDD を用いた非明示的列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddImp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr
  ZddImp2(ZddMgr& mgr);

  /// @brief デストラクタ
  ~ZddImp2();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  void
  operator()(BdnMgr& network,
	     ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マネージャ
  ZddMgr& mMgr;

  // ノードの情報
  vector<Zdd> mCuts;

};

END_NAMESPACE_YM

#endif // ZDDIMP2_H
