
/// @file BlifNetworkHandler.cc
/// @brief BlifNetworkHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNetworkHandler.h"
#include "BlifNetworkImpl.h"
#include "YmNetworks/BlifNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// @brief コンストラクタ
BlifNetworkHandler::BlifNetworkHandler() :
  mNetwork(NULL)
{
}

// @brief デストラクタ
BlifNetworkHandler::~BlifNetworkHandler()
{
}

// @brief 読み込む対象のネットワークを設定する．
void
BlifNetworkHandler::set(BlifNetworkImpl* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
BlifNetworkHandler::init()
{
  mNetwork->clear();

  return true;
}

// @brief .model 文の読み込み
// @param[in] loc 位置情報
// @param[in] name model名
bool
BlifNetworkHandler::model(const FileRegion& loc1,
			  const FileRegion& loc2,
			  const char* name)
{
  mNetwork->set_model(name);

  return true;
}

// @brief .input 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 入力ノード名
bool
BlifNetworkHandler::inputs_elem(ymuint32 name_id)
{
  mNetwork->new_input(name_id, id2str(name_id));

  return true;
}

// @brief .output 文の読み込み
// @param[in] loc 位置情報
// @param[in] name 出力ノード名
bool
BlifNetworkHandler::outputs_elem(ymuint32 name_id)
{
  mNetwork->new_output(name_id);

  return true;
}

// @brief .names 文の処理
// @param[in] onode_id ノード名のID番号
// @param[in] inode_id_array ファンイン各のID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力カバーを表す文字列
// @param[in] opat 出力の極性
// @retval true 処理が成功した．
// @retval false エラーが起こった．
// @note cover_pat は ni 個ごとに1行のパタンを表す．
// 各要素のとりうる値は '0', '1', '-' を表す．
// @note opat は '0' か '1' のどちらか
bool
BlifNetworkHandler::names(ymuint32 onode_id,
			  const vector<ymuint32>& inode_id_array,
			  ymuint32 nc,
			  const char* cover_pat,
			  char opat)
{
  mNetwork->new_logic(onode_id, id2str(onode_id), inode_id_array, nc, cover_pat, opat);

  return true;
}

// @brief .gate 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] cell セル
// @param[in] inode_id_array 入力ノードのID番号の配列
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::gate(ymuint32 onode_id,
			 const vector<ymuint32>& inode_id_array,
			 const Cell* cell)
{
  mNetwork->new_gate(onode_id, id2str(onode_id), inode_id_array, cell);

  return true;
}

// @brief .latch 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id 入力ノードのID番号
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BlifNetworkHandler::latch(ymuint32 onode_id,
			  ymuint32 inode_id,
			  const FileRegion& loc4,
			  char rval)
{
  mNetwork->new_latch(onode_id, id2str(onode_id), inode_id, rval);

  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
BlifNetworkHandler::end(const FileRegion& loc)
{
  return true;
}

// @brief 通常終了時の処理
void
BlifNetworkHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
BlifNetworkHandler::error_exit()
{
  mNetwork->clear();
}

END_NAMESPACE_YM_NETWORKS_BLIF
