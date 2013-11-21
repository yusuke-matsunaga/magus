#ifndef ISCAS89BDNCONV_H
#define ISCAS89BDNCONV_H

/// @file Iscas89BdnConv.h
/// @brief Iscas89BdnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "iscas89_nsdef.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
/// @class Iscas89BdnConv Iscas89BdnConv.h
/// @brief Iscas89Network を BdnMgr に変換するクラス
/// @sa BdnMgr Iscas89Network
//////////////////////////////////////////////////////////////////////
class Iscas89BdnConv
{
public:

  /// @brief コンストラクタ
  Iscas89BdnConv();

  /// @brief デストラクタ
  ~Iscas89BdnConv();


public:

  /// @brief 変換する．
  /// @param[in] iscas89_network 変換元のネットワーク
  /// @param[in] network 変換先のネットワーク
  /// @param[in] clock_name クロック信号のポート名
  bool
  operator()(const Iscas89Network& iscas89_network,
	     BdnMgr& network,
	     const string& clock_name = "clock");


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief iscas89_node に対応した BdnNode を生成する．
  BdnNodeHandle
  make_node(const Iscas89Node* iscas89_node);

  /// @brief iscas89_node に対応した BdnNode を取り出す．
  bool
  get_node(const Iscas89Node* iscas89_node,
	   BdnNodeHandle& node_handle);

  /// @brief iscas89_node に対応した BdnNode を登録する．
  void
  put_node(const Iscas89Node* iscas89_node,
	   BdnNodeHandle node_handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変換先のネットワーク
  BdnMgr* mNetwork;

  // Iscas89Node の ID をキーとして対応する BdnNodeHandle を収める配列
  vector<BdnNodeHandle> mNodeMap;

  // Iscas89Node の ID をキーとして対応する BdnNodeHandle が登録されている
  // かどうかを記録する配列
  vector<bool> mNodeFlag;

};

END_NAMESPACE_YM_ISCAS89


#endif // ISCAS89BDNCONV_H
