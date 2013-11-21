#ifndef NOTCONV_H
#define NOTCONV_H

/// @file NotConv.h
/// @brief NotConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class NotConv NotConv.h "NotConv.h"
/// @brief NOT 演算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class NotConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  NotConv();

  /// @brief デストラクタ
  virtual
  ~NotConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnNode を BdnMgr に変換する．
  /// @param[in] node ノード
  /// @param[in] bdnetwork 変換結果のサブジェクトグラフ
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvnNode* node,
	     BdnMgr& bdnetwork,
	     MvnBdnMap& nodemap);

};

END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // NOTCONV_H
