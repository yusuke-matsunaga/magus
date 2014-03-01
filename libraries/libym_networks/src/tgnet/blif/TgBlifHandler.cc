
/// @file TgBlifHandler.cc
/// @brief TgBlifHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.



#include "TgBlifHandler.h"
#include "networks/TgNetwork.h"
#include "networks/TgNode.h"

#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

// @brief コンストラクタ
TgBlifHandler::TgBlifHandler()
{
}

// @brief デストラクタ
TgBlifHandler::~TgBlifHandler()
{
}

// @brief ネットワークをセットする．
void
TgBlifHandler::set_network(TgNetwork& network)
{
  mNetwork = &network;
}

// @brief 初期化
bool
TgBlifHandler::init()
{
  mNetwork->clear();
  mConList.clear();
  return true;
}

// @brief .model 文の処理
// @param[in] loc1 .model の位置情報
// @param[in] loc2 文字列の位置情報
// @param[in] name 文字列
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgBlifHandler::model(const FileRegion& loc1,
		     const FileRegion& loc2,
		     const char* name)
{
  return true;
}

// @brief .input 文の読み込み
bool
TgBlifHandler::inputs_elem(ymuint32 name_id)
{
  const char* name = id2str(name_id);
  TgNode* node = mNetwork->find_node(name, true);
  mNetwork->set_to_input(node);
  return true;
}

// @brief .output 文の読み込み
bool
TgBlifHandler::outputs_elem(ymuint32 name_id)
{
  TgNode* onode = mNetwork->new_node();
  mNetwork->set_to_output(onode);
  const char* name = id2str(name_id);
  TgNode* node = mNetwork->find_node(name, true);
  connect(node, onode, 0);
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
TgBlifHandler::names(ymuint32 onode_id,
		     const vector<ymuint32>& inode_id_array,
		     ymuint32 nc,
		     const char* cover_pat,
		     char opat)
{
  ymuint32 ni = inode_id_array.size();

  mCurFanins.clear();
  for (ymuint32 i = 0; i < ni; ++ i) {
    ymuint32 id = inode_id_array[i];
    const char* name = id2str(id);
    TgNode* node = mNetwork->find_node(name, true);
    mCurFanins.push_back(node);
  }

  TgNode* node = mNetwork->find_node(id2str(onode_id), true);

  mChd2.reserve(ni);

  Expr expr;
  mChd1.clear();
  if ( opat == '1' ) {
    for (ymuint32 c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(Expr::make_negaliteral(VarId(i)));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(Expr::make_posiliteral(VarId(i)));
	}
      }
      mChd1.push_back(Expr::make_and(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = Expr::make_one();
    }
    else {
      expr = Expr::make_or(mChd1);
    }
  }
  else {
    for (ymuint32 c = 0; c < nc; ++ c) {
      mChd2.clear();
      for (ymuint32 i = 0; i < ni; ++ i) {
	char v = cover_pat[c * ni + i];
	if ( v == '0' ) {
	  mChd2.push_back(Expr::make_posiliteral(VarId(i)));
	}
	else if ( v == '1' ) {
	  mChd2.push_back(Expr::make_negaliteral(VarId(i)));
	}
      }
      mChd1.push_back(Expr::make_or(mChd2));
    }
    if ( mChd1.empty() ) {
      expr = Expr::make_zero();
    }
    else {
      expr = Expr::make_and(mChd1);
    }
  }

  mNetwork->set_to_logic(node, expr);

  if ( ni != node->fanin_num() ) {
    // .names の後の要素数とパタンの列数が異なる．
    ostringstream buf;
    buf << node->name() << ": illegal cover" << endl;
    MsgMgr::put_msg(__FILE__, __LINE__,
		    id2loc(onode_id),
		    kMsgError,
		    "TG_BLIF_005",
		    buf.str());
    return false;
  }
  for (ymuint32 i = 0; i < ni; ++ i) {
    TgNode* inode = mCurFanins[i];
    connect(inode, node, i);
  }
  return true;
}

// @brief .gate 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 入力ノードのID番号の配列
// @param[in] cell セル
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TgBlifHandler::gate(ymuint32 onode_id,
		    const vector<ymuint32>& inode_id_array,
		    const Cell* cell)
{
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
TgBlifHandler::latch(ymuint32 onode_id,
		     ymuint32 inode_id,
		     const FileRegion& loc4,
		     char rval)
{
  const char* oname = id2str(onode_id);
  TgNode* ffout = mNetwork->find_node(oname, true);
  TgNode* ffin = mNetwork->new_node();
  mNetwork->set_to_ff(ffin, ffout);
  const char* iname = id2str(inode_id);
  TgNode* inode = mNetwork->find_node(iname, true);
  connect(inode, ffin, 0);

  // rval は使っていない．
  return true;
}

// @brief .end 文の処理
bool
TgBlifHandler::end(const FileRegion& loc)
{
  for (vector<Connection>::iterator p = mConList.begin();
       p != mConList.end(); ++ p) {
    const Connection& con = *p;
    mNetwork->connect(con.mFrom, con.mTo, con.mPos);
  }
  mConList.clear();

  ymuint n = mNetwork->node_num();
  for (ymuint i = 0; i < n; ++ i) {
    const TgNode* node = mNetwork->node(i);
    if ( node->is_undef() ) {
      // node は定義されていない
      ostringstream buf;
      buf << node->name() << ": undefined";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "TG_BLIF_03",
		      buf.str());
      return false;
    }
  }

  return true;
}

// @brief 終了処理
void
TgBlifHandler::normal_exit()
{
  mNetwork->wrap_up();
}

// @brief エラー終了時の処理
void
TgBlifHandler::error_exit()
{
  mNetwork->clear();
}

// 可能なら接続する．
// できれなれば mConList に積む
void
TgBlifHandler::connect(TgNode* from,
		       TgNode* to,
		       ymuint pos)
{
  if ( from->is_undef() ) {
    mConList.push_back(Connection(from, to, pos));
  }
  else {
    mNetwork->connect(from, to, pos);
  }
}

END_NAMESPACE_YM_NETWORKS_BLIF
