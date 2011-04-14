#ifndef YM_BDN_BLIFBDNCONV_H
#define YM_BDN_BLIFBDNCONV_H

/// @file ym_bnet/BlifBdnConv.h
/// @brief BlifBdnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BlifBdnConv.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/bdn_nsdef.h"
#include "ym_blif/blif_nsdef.h"


BEGIN_NAMESPACE_YM_BDN

//////////////////////////////////////////////////////////////////////
/// @class BlifBdnConv BlifBdnConv.h "ym_bdn/BlifBdnConv.h"
/// @ingroup BdnGroup
/// @brief BlifNetwork を BdnMgr に変換するクラス
/// @sa BdnMgr nsYm::nsBlif::BlifNetwork
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

  /// @brief blif_node に対応した BdnNode を生成する．
  BdnNodeHandle
  make_node(const BlifNode* blif_node);

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

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BLIFBDNCONV_H
