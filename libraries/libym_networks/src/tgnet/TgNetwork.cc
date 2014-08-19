
/// @file TgNetwork.cc
/// @brief TgNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/TgNetwork.h"
#include "YmNetworks/TgNode.h"
#include "NameHash.h"
#include "LogicMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// tTgGateType
//////////////////////////////////////////////////////////////////////

// @brief tTgGateType のストリーム出力
ostream&
operator<<(ostream& s,
	   tTgGateType gate_type)
{
  switch ( gate_type ) {
  case kTgGateConst0: s << "CONST0"; break;
  case kTgGateConst1: s << "CONST1"; break;

  case kTgGateBuff:   s << "BUFF"; break;
  case kTgGateNot:    s << "NOT"; break;

  case kTgGateAnd:    s << "AND"; break;
  case kTgGateNand:   s << "NAND"; break;

  case kTgGateOr:     s << "OR"; break;
  case kTgGateNor:    s << "NOR"; break;

  case kTgGateXor:    s << "XOR"; break;
  case kTgGateXnor:   s << "XNOR"; break;

  case kTgGateCplx:   s << "CPLX"; break;

  default:
    assert_not_reached(__FILE__, __LINE__);
  }
  return s;
}

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_NETWORKS_TGNET


//////////////////////////////////////////////////////////////////////
// クラス TgNetwork
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
TgNetwork::TgNetwork() :
  mAlloc(4096),
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

  mAlloc.destroy();
}

// @brief 中で使われている論理関数の数を得る．
ymuint
TgNetwork::func_num() const
{
  return mLogicMgr->logic_num();
}

// @brief 関数を取り出す．
// @note param[in] id 関数番号 ( 0 <= id < func_num() )
const TvFunc&
TgNetwork::get_func(ymuint id) const
{
  return mLogicMgr->get_func(id);
}

// @brief 関数の論理式を取り出す．
// @note param[in] id 関数番号 ( 0 <= id < func_num() )
Expr
TgNetwork::get_lexp(ymuint id) const
{
  return mLogicMgr->get_expr(id);
}

// @brief 新しいノードを生成する．
TgNode*
TgNetwork::new_node()
{
  ymuint32 gid = mNodeArray.size();
  void* p = mAlloc.get_memory(sizeof(TgNode));
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
  node->set_type(lid, TgNode::kInputType, 0, NULL);
  mInputArray.push_back(node);
}

// @brief ノードを外部出力に設定する．
// @param[in] node 対象のノード
void
TgNetwork::set_to_output(TgNode* node)
{
  ymuint32 lid = mOutputArray.size();
  void* p = mAlloc.get_memory(sizeof(TgNode*));
  node->set_type(lid, TgNode::kOutputType, 1, p);
  mOutputArray.push_back(node);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] expr 論理式
void
TgNetwork::set_to_logic(TgNode* node,
			const Expr& expr)
{
  ymuint ni = expr.input_size();
  ymuint32 id;
  tTgGateType type = mLogicMgr->reg_logic(expr, id);
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
  set_to_logic(node, gate_type, ni, 0);
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
  set_to_logic(node, kTgGateCplx, ni, aux_id);
}

// @brief ノードを論理ノードに設定する．
// @param[in] node 対象のノード
// @param[in] gate_type 論理タイプ
// @param[in] ni 入力数
// @param[in] aux_id 論理タイプが kTgCplx の場合の補助ID
void
TgNetwork::set_to_logic(TgNode* node,
			tTgGateType gate_type,
			ymuint ni,
			ymuint aux_id)
{
  ymuint32 lid = mLogicArray.size();
  void* p = mAlloc.get_memory(sizeof(TgNode*) * ni);
  node->set_type(lid, TgNode::kLogicType | (gate_type << 2), ni, p);
  node->mFuncId = aux_id;
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
  void* p = mAlloc.get_memory(sizeof(TgNode*));
  ffin->set_type(lid, TgNode::kOutputType, 1, p);
  ffin->mAltNode = ffout;
  mFFInArray.push_back(ffin);
  ffout->set_type(lid, TgNode::kInputType, 0, NULL);
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
    ymuint ni = node->fanin_num();
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
      void* p = mAlloc.get_memory(sizeof(TgEdge) * nfo);
      node->mFanouts = new (p) TgEdge[nfo];
    }
    nfo = 0;
  }
  // ファンアウトの設定を行う．
  for (ymuint i = 0; i < nl; ++ i) {
    TgNode* node = mLogicArray[i];
    ymuint ni = node->fanin_num();
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
      ymuint ni = fonode->fanin_num();
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
    s << ": " << node->gate_type();
    if ( node->is_cplx_logic() ) {
      s << "#" << node->func_id();
    }
    s << "(" << node->fanin_num() << ")" << endl;
  }

  s << endl
    << "Connections" << endl;
  ymuint nn = network.node_num();
  for (ymuint i = 0; i < nn; ++ i) {
    const TgNode* node = network.node(i);
    ymuint ni = node->fanin_num();
    for (ymuint j = 0; j < ni; ++ j) {
      const TgNode* inode = node->fanin(j);
      dump_node(s, inode);
      s << " -->> "
	<< j << "@";
      dump_node(s, node);
      s << endl;
    }
  }

  s << endl
    << "Functions" << endl;
  ymuint nf = network.func_num();
  for (ymuint i = 0; i < nf; ++ i) {
    Expr expr = network.get_lexp(i);
    s << "Func#" << i << ": " << expr << endl;
  }

  s << "=====<TgNework Dump End>=====" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス TgNode
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TgNode::TgNode(ymuint32 gid) :
  mTypeId(kUndefType),
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
// @param[in] lid ローカルID
// @param[in] type ノードのタイプ
// @param[in] ni 入力数
// @param[in] p ファンインの配列用のメモリ領域
void
TgNode::set_type(ymuint32 lid,
		 ymuint type,
		 ymuint ni,
		 void* p)
{
  mTypeId = type;
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
