#ifndef NETWORKS_BNETBDNCONV_H
#define NETWORKS_BNETBDNCONV_H

/// @file YmNetworks/BNetBdnConv.h
/// @brief BNetBdnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/bnet.h"
#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class BNetBdnConv BNetBdnConv.h "YmNetworks/BNetBdnConv.h"
/// @brief BNetwork を BdnMgr に変換するクラス
//////////////////////////////////////////////////////////////////////
class BNetBdnConv
{
public:

  /// @brief コンストラクタ
  BNetBdnConv();

  /// @brief デストラクタ
  ~BNetBdnConv();


public:

  /// @brief 変換する
  /// @param[in] bnetwork 変換元のネットワーク
  /// @param[in] network 変換先のネットワーク
  /// @param[in] clock_name クロック信号のポート名
  /// @param[in] reset_name リセット信号のポート名
  /// @note clock_name と reset_name はオプション
  bool
  operator()(const BNetwork& bnetwork,
	     BdnMgr& network,
	     const string& clock_name = "clock",
	     const string& reset_name = "reset");


private:

  /// @brief bnode に対応した BdnNode を生成する．
  BdnNodeHandle
  make_node(const BNode* bnode);

  /// @brief bnode に対応した BdnNode を取り出す．
  bool
  get_node(const BNode* bnode,
	   BdnNodeHandle& node_handle);

  /// @brief bnode に対応した BdnNode を登録する．
  void
  put_node(const BNode* bnode,
	   BdnNodeHandle node_handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換先のネットワーク
  BdnMgr* mNetwork;

  // BNode の ID をキーとして対応する BdnNodeHandle を収める配列
  vector<BdnNodeHandle> mNodeMap;

  // BNode の ID をキーとして対応する BdnNodeHandle が登録されて
  // いるかどうかを記録する配列
  vector<bool> mNodeFlag;

};

END_NAMESPACE_YM

#endif // NETWORKS_BNETBDNCONV_H
