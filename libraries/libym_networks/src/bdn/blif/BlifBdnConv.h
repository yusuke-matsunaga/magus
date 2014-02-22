#ifndef BLIFBDNCONV_H
#define BLIFBDNCONV_H

/// @file BlifBdnConv.h
/// @brief BlifBdnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"
#include "networks/blif_nsdef.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
/// @class BlifBdnConv BlifBdnConv.h "BlifBdnConv.h"
/// @ingroup BdnGroup
/// @brief BlifNetwork を BdnMgr に変換するクラス
/// @sa BdnMgr BlifNetwork
//////////////////////////////////////////////////////////////////////
class BlifBdnConv
{
public:

  /// @brief コンストラクタ
  BlifBdnConv();

  /// @brief デストラクタ
  ~BlifBdnConv();


public:

  /// @brief 変換する
  /// @param[in] blif_network 変換元のネットワーク
  /// @param[in] network 変換先のネットワーク
  /// @param[in] clock_name クロック信号のポート名
  /// @param[in] reset_name リセット信号のポート名
  /// @note clock_name と reset_name はオプション
  bool
  operator()(const BlifNetwork& blif_network,
	     BdnMgr& network,
	     const string& clock_name = "clock",
	     const string& reset_name = "reset");


private:

  /// @brief node_id に対応した BdnNode を生成する．
  BdnNodeHandle
  make_node(const BlifNetwork& blif_network,
	    ymuint node_id);

  /// @brief blif_node に対応した BdnNode を取り出す．
  bool
  get_node(const BlifNode* blif_node,
	   BdnNodeHandle& node_handle);

  /// @brief blif_node に対応した BdnNode を登録する．
  void
  put_node(const BlifNode* blif_node,
	   BdnNodeHandle node_handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換先のネットワーク
  BdnMgr* mNetwork;

  // BlifNode の ID をキーとして対応する BdnNodeHandle を収める配列
  vector<BdnNodeHandle> mNodeMap;

  // BlifNode の ID をキーとして対応する BdnNodeHandle が登録されて
  // いるかどうかを記録する配列
  vector<bool> mNodeFlag;

};

END_NAMESPACE_YM_NETWORKS_BLIF

#endif // BLIFBDNCONV_H
