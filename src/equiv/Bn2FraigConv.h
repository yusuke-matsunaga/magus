#ifndef BN2FRAIGCONV_H
#define BN2FRAIGCONV_H

/// @file Bn2FraigConv.h
/// @brief Bn2FraigConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "equiv.h"
#include "FraigHandle.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_EQUIV

class FraigMgr;

//////////////////////////////////////////////////////////////////////
/// @class Bn2FraigConv Bn2FraigConv.h "Bn2FraigConv.h"
/// @brief BnNetwork の各ノードに対応する FraigHandle を作るクラス
//////////////////////////////////////////////////////////////////////
class Bn2FraigConv
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr FraigMgr
  Bn2FraigConv(FraigMgr& mgr);

  /// @brief デストラクタ
  ~Bn2FraigConv();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークの構造に対応する Fraig を作る．
  /// @param[in] src_network 元となるネットワーク
  /// @param[in] input_list 入力ノード番号のリスト
  /// @param[in] output_list 出力ノード番号のリスト
  /// @param[in] input_handles 入力のハンドルのリスト
  /// @param[out] output_handles 出力のハンドルのリスト
  void
  operator()(const BnNetwork& src_network,
	     const vector<int>& input_list,
	     const vector<int>& output_list,
	     const vector<FraigHandle>& input_handles,
	     vector<FraigHandle>& output_handles);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BnNode に対応するハンドルを作る．
  /// @param[in] node 対象のノード
  /// @return 生成したハンドルを返す．
  ///
  /// node のファンイン側の構造は Fraig 化されていると仮定する．
  FraigHandle
  make_handle(const BnNode* node);

  /// @brief ノード番号に対応するハンドルを登録する．
  /// @param[in] node_id ノード番号
  /// @param[in] handle ハンドル
  void
  put_handle(int node_id,
	     FraigHandle handle);

  /// @brief ノード番号に対応するハンドルを返す．
  /// @param[in] node_id ノード番号
  FraigHandle
  get_handle(int node_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // BnNode::id() をキーとして対応する FraigHandle を記録する配列
  vector<FraigHandle> mHandleMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号に対応するハンドルを登録する．
// @param[in] node_id ノード番号
// @param[in] handle ハンドル
inline
void
Bn2FraigConv::put_handle(int node_id,
			 FraigHandle handle)
{
  mHandleMap[node_id] = handle;
}

// @brief ノード番号に対応するハンドルを返す．
// @param[in] node_id ノード番号
inline
FraigHandle
Bn2FraigConv::get_handle(int node_id)
{
  return mHandleMap[node_id];
}

END_NAMESPACE_EQUIV

#endif // BN2FRAIGCONV_H
