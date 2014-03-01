
/// @file BNetBlifHandler.cc
/// @brief BNetBlifHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BNetBlifHandler.h"
#include "networks/BNetwork.h"
#include "networks/BNetManip.h"
#include "cell/CellLibrary.h"
#include "cell/Cell.h"
#include "cell/CellPin.h"
#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// @brief コンストラクタ
BNetBlifHandler::BNetBlifHandler() :
  mNetwork(NULL),
  mManip(NULL)
{
}

// @brief デストラクタ
BNetBlifHandler::~BNetBlifHandler()
{
  delete mManip;
}

// @brief 読み込む対象のネットワークを設定する．
void
BNetBlifHandler::set_network(BNetwork* network)
{
  mNetwork = network;
}

// @brief 初期化
bool
BNetBlifHandler::init()
{
  assert_cond( mManip == NULL, __FILE__, __LINE__);

  mManip = new BNetManip(mNetwork);
  mNetwork->clear();

  mNodeArray.clear();

  return true;
}

// @brief .model 文中の文字列の処理
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::model(const FileRegion& loc1,
		       const FileRegion& loc2,
		       const char* name)
{
  mNetwork->set_model_name(name);
  return true;
}

// @brief .inputs 文中の文字列の処理
// @param[in] name_id 文字列の情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::inputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  BNode* node = mManip->new_input(name);
  resize(name_id);
  mNodeArray[name_id] = node;
  return true;
}

// @brief .outputs 文中の文字列の処理
// @param[in] name_id 文字列の情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::outputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  mManip->new_output(name);
  return true;
}

// @brief .names 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 各識別子のID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力カバーを表す文字列
// @param[in] opat 出力の極性
// @retval true 処理が成功した．
// @retval false エラーが起こった．
// @note cover_pat は ni 個ごとに1行のパタンを表す．
// 各要素のとりうる値は '0', '1', '-' を表す．
// @note opat は '0' か '1' のどちらか
bool
BNetBlifHandler::names(ymuint32 onode_id,
		       const vector<ymuint32>& inode_id_array,
		       ymuint32 nc,
		       const char* cover_pat,
		       char opat)
{
  BNode* node = get_node(onode_id);

  ymuint ni = inode_id_array.size();

  Expr expr;
  if ( opat == '1' ) {
    if ( ni == 0 ) {
      expr = Expr::make_one();
    }
    else {
      vector<Expr> or_expr;
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<Expr> and_expr;
	for (ymuint32 i = 0; i < ni; ++ i) {
	  VarId var(i);
	  char v = cover_pat[c * ni + i];
	  if ( v == '0' ) {
	    and_expr.push_back(Expr::make_negaliteral(var));
	  }
	  else if ( v == '1' ) {
	    and_expr.push_back(Expr::make_posiliteral(var));
	  }
	}
	or_expr.push_back(Expr::make_and(and_expr));
      }
      expr = Expr::make_or(or_expr);
    }
  }
  else {
    if ( ni == 0 ) {
      expr = Expr::make_zero();
    }
    else {
      vector<Expr> or_expr;
      for (ymuint32 c = 0; c < nc; ++ c) {
	vector<Expr> and_expr;
	for (ymuint32 i = 0; i < ni; ++ i) {
	  VarId var(i);
	  char v = cover_pat[c * ni + i];
	  if ( v == '0' ) {
	    and_expr.push_back(Expr::make_posiliteral(var));
	  }
	  else if ( v == '1' ) {
	    and_expr.push_back(Expr::make_negaliteral(var));
	  }
	}
	or_expr.push_back(Expr::make_or(and_expr));
      }
      expr = Expr::make_and(or_expr);
    }
  }
  BNodeVector fanins(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    BNode* inode = get_node(inode_id_array[i]);
    fanins[i] = inode;
  }
  bool stat = mManip->change_logic(node, expr, fanins);
  return stat;
}

// @brief .gate 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 入力ノードのID番号の配列
// @param[in] cell セル
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::gate(ymuint32 onode_id,
		      const vector<ymuint32>& inode_id_array,
		      const Cell* cell)
{
  Expr expr = cell->logic_expr(0);
  BNode* onode = get_node(onode_id);
  ymuint ni = inode_id_array.size();
  BNodeVector fanins(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    fanins[i] = get_node(inode_id_array[i]);
  }
  bool stat = mManip->change_logic(onode, expr, fanins);
  return stat;
}

// @brief .latch 文中の本体の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 入力ノードのID番号の配列
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値 ('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::latch(ymuint32 onode_id,
		       ymuint32 inode_id,
		       const FileRegion& loc4,
		       char rval)
{
  BNode* node = get_node(onode_id);
  BNode* inode = get_node(inode_id);
  int rv = 0;
  switch ( rval ) {
  case '0': rv = 0; break;
  case '1': rv = 1; break;
  default:  rv = 2; break;
  }
  return mManip->change_latch(node, inode, rv);
}

// @brief .end 文の処理
// @param[in] loc 位置情報
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
BNetBlifHandler::end(const FileRegion& loc)
{
  for (BNodeList::const_iterator p = mNetwork->outputs_begin();
       p != mNetwork->outputs_end(); ++ p) {
    BNode* onode = *p;
    BNode* node = mNetwork->find_node(onode->name());
    assert_cond(node, __FILE__, __LINE__);
    mManip->change_output(onode, node);
  }
  return true;
}

// @brief 通常終了時の処理
void
BNetBlifHandler::normal_exit()
{
  delete mManip;
  mManip = NULL;
}

// @brief エラー終了時の処理
void
BNetBlifHandler::error_exit()
{
  mNetwork->clear();
  delete mManip;
  mManip = NULL;
}

// @brief ノードの取得 & 生成
BNode*
BNetBlifHandler::get_node(ymuint32 id)
{
  resize(id);
  BNode* node = mNodeArray[id];
  if ( node == NULL ) {
    node = mManip->new_logic(id2str(id));
    mNodeArray[id] = node;
  }
  return node;
}

void
BNetBlifHandler::resize(ymuint32 id)
{
  size_t n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }
}

END_NAMESPACE_YM_NETWORKS_BLIF
