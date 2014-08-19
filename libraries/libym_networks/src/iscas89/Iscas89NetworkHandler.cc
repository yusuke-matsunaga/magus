
/// @file Iscas89NetworkHandler.cc
/// @brief Iscas89NetworkHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89NetworkHandler.h"
#include "Iscas89Network.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// クラス Iscas89NetworkHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89NetworkHandler::Iscas89NetworkHandler() :
  mNetwork(NULL)
{
}

// @brief デストラクタ
Iscas89NetworkHandler::~Iscas89NetworkHandler()
{
}

// @brief 読み込む対象のネットワークを設定する．
void
Iscas89NetworkHandler::set_network(Iscas89Network* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
Iscas89NetworkHandler::init()
{
  mNetwork->clear();
  return true;
}

// @brief INPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 入力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
Iscas89NetworkHandler::read_input(const FileRegion& loc,
				  ymuint32 name_id)
{

  Iscas89Node* node = mNetwork->get_node(name_id);
  const char* name = id2str(name_id);
  mNetwork->set_node_name(node, name);
  if ( !mNetwork->set_input_type(node) ) {
    return false;
  }
  return true;
}

// @brief OUTPUT 文を読み込む．
// @param[in] loc ファイル位置
// @param[in] name_id 出力ピン名の ID 番号
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
Iscas89NetworkHandler::read_output(const FileRegion& loc,
				   ymuint32 name_id)
{
  Iscas89Node* node = mNetwork->get_node(name_id);
  const char* name = id2str(name_id);
  mNetwork->set_node_name(node, name);
  mNetwork->mPOArray.push_back(node);
  return true;
}

// @brief ゲート文を読み込む．
// @param[in] loc ファイル位置
// @param[in] type ゲートの型
// @param[in] oname_id 出力名の ID 番号
// @param[in] iname_list 入力名のリスト
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
Iscas89NetworkHandler::read_gate(const FileRegion& loc,
				 tIscas89GateType type,
				 ymuint32 oname_id,
				 const vector<ymuint32>& iname_list)
{
  ymuint ni = iname_list.size();
  Iscas89Node* node = mNetwork->get_node(oname_id);
  const char* name = id2str(oname_id);
  mNetwork->set_node_name(node, name);

  if ( !mNetwork->set_gate_type(node, ni, type) ) {
    return false;
  }

  for (ymuint i = 0; i < ni; ++ i) {
    ymuint id = iname_list[i];
    Iscas89Node* inode = mNetwork->get_node(id);
    node->mFanins[i] = inode;
  }

  return true;
}

// @brief 通常終了時の処理
void
Iscas89NetworkHandler::normal_exit()
{
}

// @brief エラー終了時の処理
void
Iscas89NetworkHandler::error_exit()
{
  mNetwork->clear();
}

END_NAMESPACE_YM_ISCAS89
