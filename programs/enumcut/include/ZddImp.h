#ifndef ZDDIMP_H
#define ZDDIMP_H

/// @file ZddImp.h
/// @brief ZddImp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "logic/Zdd.h"
#include "logic/ZddMgr.h"
#include "networks/bdn.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ZddImp ZddImp.h "ZddImp.h"
/// @brief ZDD を用いた非明示的列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddImp
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr
  ZddImp(ZddMgr& mgr);

  /// @brief デストラクタ
  ~ZddImp();


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

#endif // ZDDIMP_H
