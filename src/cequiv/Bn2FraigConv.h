#ifndef BN2FRAIGCONV_H
#define BN2FRAIGCONV_H

/// @file Bn2FraigConv.h
/// @brief Bn2FraigConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "cec_nsdef.h"
#include "ym/bnet.h"
#include "FraigMgr.h"
#include "FraigHandle.h"
#include "ym/HashMap.h"


BEGIN_NAMESPACE_YM_CEC

//////////////////////////////////////////////////////////////////////
/// @class Bn2FraigConv Bn2FraigConv.h "Bn2FraigConv.h"
/// @brief BnNetwork の各ノードに対応する FraigHandle を作るクラス
//////////////////////////////////////////////////////////////////////
class Bn2FraigConv
{
public:

  /// @brief コンストラクタ
  /// @param[in] fraig_mgr FraigMgr
  Bn2FraigConv(FraigMgr& fraig_mgr);

  /// @brief デストラクタ
  ~Bn2FraigConv();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの構造に対応する Fraig を作る．
  /// @param[in] src_network 元となるネットワーク
  /// @param[in] input_handles 入力のノード番号と対応するハンドルの対のリスト
  void
  convert(const BnNetwork& src_network,
	  const vector<pair<int, FraigHandle>>& input_handles);

  /// @brief ノード番号に対応するハンドルを返す．
  /// @param[in] node_id ノード番号
  FraigHandle
  get_handle(int node_id);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode に対応するハンドルを作る．
  /// @param[in] node 対象のノード
  ///
  /// node のファンイン側の構造は Fraig 化されていると仮定する．
  FraigHandle
  node2handle(const BnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mFraigMgr;

  // BnNode::id() をキーとして対応する FraigHandle を記録するハッシュ表
  HashMap<int, FraigHandle> mHandleMap;

};

END_NAMESPACE_YM_CEC


#endif // BN2FRAIGCONV_H
