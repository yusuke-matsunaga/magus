
/// @file TgNetwork.cc
/// @brief TgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/TgNetwork.h"
#include "ym_networks/TgNode.h"
#include "NameHash.h"
#include "LogicMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
// クラス TgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgNetwork::TgNetwork() :
  mNodeAlloc(sizeof(TgNode), 1024),
  mFaninAlloc(4096),
  mEdgeAlloc(4096),
  mNameHash(new NameHash),
  mLogicMgr(new LogicMgr)
{
}

// @brief デストラクタ
TgNetwork::~TgNetwork()
{
  clear();
  delete mNameHash;
  delete mLogicMgr;
}

// @brief 名前からノードを検索する
// @param[in] name 検索対象の名前
// @param[in] create_flag true なら存在しない時に新規に生成する．
// @return name という名前のノードを返す．
const TgNode*
TgNetwork::find_node(const char* name) const
{
  TgNode* node = mNameHash->find_node(name);
  return node;
}

// @brief 内容をクリアする．
void
TgNetwork::clear()
{
  mNodeArray.clear();
  mInputArray.clear();
  mOutputArray.clear();
  mLogicArray.clear();
  mFFOutArray.clear();
  mFFInArray.clear();

  mNameHash->clear();
  mLogicMgr->clear();

  mNodeAlloc.destroy();
  mFaninAlloc.destroy();
  mEdgeAlloc.destroy();
}

// @brief node の論理式を取り出す．
LogExpr
TgNetwork::get_lexp(const TgNode* node) const
{
  return mLogicMgr->get_expr(node->mTypeId);
}

// @brief ノードの論理式を取り出す．
// @note node 対象のノード
const TvFunc&
TgNetwork::get_func(const TgNode* node) const
{
  return mLogicMgr->get_func(node->mTypeId);
}

// @brief 新しいノードを生成する．
TgNode*
TgNetwork::new_node()
{
  ymuint32 gid = mNodeArray.size();
  void* p = mNodeAlloc.get_memory(sizeof(TgNode));
  TgNode* node = new (p) TgNode(gid);
  mNodeArray.push_back(node);
  return node;
}

// @brief 名前からノードを検索する
// @param[in] name 検索対象の名前
// @param[in] create_flag true なら存在しない時に新規に生成する．
// @return name という名前のノードを返す．
TgNode*
TgNetwork::find_node(const char* name,
		     bool create_flag)
{
  TgNode* node = mNameHash->find_node(name);
  if ( !create_flag || node ) {
    return node;
  }
  node = new_node();
  const char* str = mNameHash->reg_node(name, node);
  node->mName = str;
  return node;
}

// @brief ノードを外部入力に設定する．
// @param[in] node 対象のノード
void
TgNetwork::set_to_input(TgNode* node)
{
  ymuint32 lid = mInputArray.size();
  node->set_type(lid, kTgInput, 0, 0, NULL);
  mInputArray.push_back(node);
}

// @brief ノードを外部出力に設定する．
// @param[in] node 対象のノード
void
TgNetwork::set_to_output(TgNode* node)
{
  ymuint32 lid = mOutputArray.size();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*));
  node->set_type(lid, kTgOutput, 1, 0, p);
  mOutputArray.push_back(node);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] expr 論理式
void
TgNetwork::set_to_logic(TgNode* node,
			const LogExpr& expr)
{
  ymuint ni = expr.input_size();
  ymuint32 id;
  tTgNodeType type = mLogicMgr->reg_logic(expr, id);
  set_to_logic(node, type, ni, id);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] gate_type ゲートタイプ
/// @param[in] ni 入力数
void
TgNetwork::set_to_builtin_logic(TgNode* node,
				tTgGateType gate_type,
				ymuint ni)
{
  tTgNodeType node_type = static_cast<tTgNodeType>(kTgLogic | static_cast<ymuint>(gate_type));
  set_to_logic(node, node_type, ni, 0);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] ni 入力数
// @param[in] aux_id 補助ID
void
TgNetwork::set_to_cplx_logic(TgNode* node,
			     ymuint ni,
			     ymuint aux_id)
{
  set_to_logic(node, kTgCplx, ni, aux_id);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] node_type 論理タイプ
// @param[in] ni 入力数
void
TgNetwork::set_to_logic(TgNode* node,
			tTgNodeType node_type,
			ymuint ni,
			ymuint aux_id)
{
  ymuint32 lid = mLogicArray.size();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*) * ni);
  node->set_type(lid, node_type, ni, aux_id, p);
  mLogicArray.push_back(node);
}

// @brief ノードを FF ノードに設定する．
// @param[in] ffin FF-in に設定するノード
// @param[in] ffout FF-out に設定するノード
void
TgNetwork::set_to_ff(TgNode* ffin,
		     TgNode* ffout)
{
  ymuint32 lid = mFFOutArray.size();
  void* p = mFaninAlloc.get_memory(sizeof(TgNode*));
  ffin->set_type(lid, kTgOutput, 1, 0, p);
  ffin->mAltNode = ffout;
  mFFInArray.push_back(ffin);
  ffout->set_type(lid, kTgInput, 0, 0, NULL);
  ffout->mAltNode = ffin;
  mFFOutArray.push_back(ffout);
}

// @brief ノード間の接続を行う．
// @param[in] from_node ファンイン元のノード
// @param[in] to_node ファンアウト先のノード
// @param[in] to_ipos ファンアウト先のノードの入力ピン番号
void
TgNetwork::connect(TgNode* from_node,
		   TgNode* to_node,
		   ymuint to_ipos)
{
  to_node->mFanins[to_ipos] = from_node;
}

// @brief ネットワークの設定後の処理を行う．
void
TgNetwork::wrap_up()
{
  ymuint n = node_num();
  ymuint ni = input_num1();
  ymuint no = output_num1();
  ymuint nl = logic_num();
  ymuint nq = ff_num();

  // ファンアウトリストの設定を行う．
  // まず，ファンアウト数を数える．
  for (ymuint i = 0; i < nl; ++ i) {
    TgNode* node = mLogicArray[i];
    ymuint ni = node->ni();
    for (ymuint j = 0; j < ni; ++ j) {
      TgNode* inode = node->mFanins[j];
      ++ inode->mFanoutNum;
    }
  }
  for (ymuint i = 0; i < no; ++ i) {
    TgNode* node = mOutputArray[i];
    TgNode* inode = node->mFanins[0];
    ++ inode->mFanoutNum;
  }
  for (ymuint i = 0; i < nq; ++ i) {
    TgNode* node = mFFInArray[i];
    TgNode* inode = node->mFanins[0];
    ++ inode->mFanoutNum;
  }
  // ファンアウト数分の配列を確保する．
  for (ymuint i = 0; i < n; ++ i) {
    TgNode* node = mNodeArray[i];
    ymuint& nfo = node->mFanoutNum;
    if ( nfo > 0 ) {
      void* p = mEdgeAlloc.get_memory(sizeof(TgEdge) * nfo);
      node->mFanouts = new (p) TgEdge[nfo];
    }
    nfo = 0;
  }
  // ファンアウトの設定を行う．
  for (ymuint i = 0; i < nl; ++ i) {
    TgNode* node = mLogicArray[i];
    ymuint ni = node->ni();
    for (ymuint j = 0; j < ni; ++ j) {
      TgNode* inode = node->mFanins[j];
      ymuint& pos = inode->mFanoutNum;
      inode->mFanouts[pos].mTo = node;
      inode->mFanouts[pos].mIpos = j;
      ++ pos;
    }
  }
  for (ymuint i = 0; i < no; ++ i) {
    TgNode* node = mOutputArray[i];
    TgNode* inode = node->mFanins[0];
    ymuint& pos = inode->mFanoutNum;
    inode->mFanouts[pos].mTo = node;
    inode->mFanouts[pos].mIpos = 0;
    ++ pos;
  }
  for (ymuint i = 0; i < nq; ++ i) {
    TgNode* node = mFFInArray[i];
    TgNode* inode = node->mFanins[0];
    ymuint& pos = inode->mFanoutNum;
    inode->mFanouts[pos].mTo = node;
    inode->mFanouts[pos].mIpos = 0;
    ++ pos;
  }

  // ソーティングを行う．
  vector<bool> visited(n, false);
  vector<TgNode*> queue(n);
  ymuint rpos = 0;
  ymuint wpos = 0;
  mSortedArray.clear();
  mSortedArray.reserve(nl);
  for (ymuint i = 0; i < ni; ++ i) {
    TgNode* node = mInputArray[i];
    visited[node->gid()] = true;
    queue[wpos] = node;
    ++ wpos;
  }
  for (ymuint i = 0; i < nq; ++ i) {
    TgNode* node = mFFOutArray[i];
    visited[node->gid()] = true;
    queue[wpos] = node;
    ++ wpos;
  }
  while ( rpos < wpos ) {
    TgNode* node = queue[rpos];
    ++ rpos;
    if ( node->is_logic() ) {
      mSortedArray.push_back(node);
    }
    ymuint nfo = node->fanout_num();
    for (ymuint i = 0; i < nfo; ++ i) {
      TgNode* fonode = node->fanout_edge(i)->mTo;
      if ( !fonode->is_logic() ) continue;
      if ( visited[fonode->gid()] ) continue;
      ymuint ni = fonode->ni();
      bool ok = true;
      for (ymuint i = 0; i < ni; ++ i) {
	const TgNode* finode = fonode->fanin(i);
	if ( !visited[finode->gid()] ) {
	  ok = false;
	  break;
	}
      }
      if ( ok ) {
	visited[fonode->gid()] = true;
	queue[wpos] = fonode;
	++ wpos;
      }
    }
  }
  assert_cond(mSortedArray.size() == nl, __FILE__, __LINE__);
}


BEGIN_NONAMESPACE

// ノード番号を出力する．
void
dump_node(ostream& s,
	  const TgNode* node)
{
  s << node->gid();
  if ( node->is_input() ) {
    s << "[I" << node->lid() << "]";
  }
  else if ( node->is_output() ) {
    s << "[O" << node->lid() << "]";
  }
  else if ( node->is_logic() ) {
    s << "[L" << node->lid() << "]";
  }
  else if ( node->is_ffin() ) {
    s << "[D" << node->lid() << "]";
  }
  else if ( node->is_ffout() ) {
    s << "[Q" << node->lid() << "]";
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

END_NONAMESPACE

// @brief TgNetwork の内容をダンプする関数
void
dump(ostream& s,
     const TgNetwork& network)
{
  s << "=====<TgNework Dump>=====" << endl;

  s << endl
    << "Logic Nodes" << endl;
  ymuint nlo = network.logic_num();
  for (ymuint i = 0; i < nlo; ++ i) {
    const TgNode* node = network.logic(i);
    dump_node(s, node);
    s << ": ni = " << node->ni() << endl;
    if ( node->is_cplx_logic() ) {
      LogExpr tmp = network.get_lexp(node);
      s << "\t" << tmp << endl;
    }
    else {
      s << "\t" << node->type() << endl;
    }
  }

  s << endl
    << "Connections" << endl;
  ymuint nn = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    ymuint ni = node->ni();
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      dump_node(s, inode);
      s << " -->> "
	<< j << "@";
      dump_node(s, node);
      s << endl;
    }
  }
  s << "=====<TgNework Dump End>=====" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス TgNode
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TgNode::TgNode(ymuint32 gid) :
  mTypeId(kTgUndef),
  mGid(gid),
  mName(NULL),
  mFanins(NULL),
  mFanoutNum(0),
  mFanouts(NULL),
  mAltNode(NULL)
{
}

// デストラクタ
TgNode::~TgNode()
{
}

// @brief タイプをセットする．
// @param[in] lid 論理ノードの通し番号
// @param[in] type 論理関数のタイプ
// @param[in] ni 入力数
// @param[in] aux_id kTgCplx 型の時の補助ID
// @param[in] p ファンインの配列用のメモリ領域
void
TgNode::set_type(ymuint32 lid,
		 tTgNodeType type,
		 ymuint ni,
		 ymuint aux_id,
		 void* p)
{
  mTypeId = type | (aux_id << 6);
  mLid = lid;
  mNi = ni;
  if ( ni > 0 ) {
    mFanins = static_cast<TgNode**>(p);
  }
  else {
    mFanins = NULL;
  }
  for (ymuint i = 0; i < ni; ++ i) {
    mFanins[i] = NULL;
  }
  mFanoutNum = 0;
}

END_NAMESPACE_YM_NETWORKS_TGNET
