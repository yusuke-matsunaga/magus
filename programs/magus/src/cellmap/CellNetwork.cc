
/// @file magus/cellmap/CellNetwork.cc
/// @brief CellNetwork の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellNetwork.cc 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "CellNetwork.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

///////////////////////////////////////////////////////////////////////
// クラス CnNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
CnNode::CnNode() :
  mNi(0),
  mFanins(NULL),
  mMark(0)
{
}
      
// デストラクタ
CnNode::~CnNode()
{
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
CnNode::scan_po()
{
  mMark &= ~kPoMask;
  for (CnEdgeList::iterator p = mFanoutList.begin();
       p != mFanoutList.end(); ++ p) {
    CnEdge* e = *p;
    if ( e->to()->is_output() ) {
      mMark |= kPoMask;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
CnNode::id_str() const
{
  ostringstream buf;
  if ( is_input() ) {
    buf << "I" << id();
  }
  else if ( is_cell() ) {
    buf << "L" << id();
  }
  else if ( is_output() ) {
    buf << "O" << id();
  }
  else {
    buf << "X" << id();
  }
  return buf.str();
}


///////////////////////////////////////////////////////////////////////
// クラス CnGraph
///////////////////////////////////////////////////////////////////////

// コンストラクタ
CnGraph::CnGraph() :
  mAlloc(4096),
  mAlloc2(4096)
{
}

// コピーコンストラクタ
CnGraph::CnGraph(const CnGraph& src) :
  mAlloc(4096),
  mAlloc2(4096)
{
  vector<CnNode*> nodemap;
  copy(src, nodemap);
}

// @brief 特殊なコピーコンストラクタ
// nodemap に対応関係が入る．
CnGraph::CnGraph(const CnGraph& src,
		   vector<CnNode*>& nodemap) :
  mAlloc(4096),
  mAlloc2(4096)
{
  copy(src, nodemap);
}

// 代入演算子
const CnGraph&
CnGraph::operator=(const CnGraph& src)
{
  if ( &src != this ) {
    clear();
    vector<CnNode*> nodemap;
    copy(src, nodemap);
  }
  return *this;
}

// デストラクタ
CnGraph::~CnGraph()
{
  //clear();
}

// 複製する．
void
CnGraph::copy(const CnGraph& src,
	      vector<CnNode*>& nodemap)
{
  size_t n = src.max_node_id();
  nodemap.clear();
  nodemap.resize(n);
  
  // 外部入力の生成
  const CnNodeList& input_list = src.input_list();
  for (CnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    CnNode* src_node = *p;
    CnNode* dst_node = new_input(src_node->name());
    nodemap[src_node->id()] = dst_node;
  }

  // CELLノードの生成
  vector<CnNode*> node_list;
  src.sort(node_list);
  size_t nl = node_list.size();
  for (size_t i = 0; i < nl; ++ i) {
    CnNode* src_node = node_list[i];
    size_t ni = src_node->ni();
    vector<CnNode*> dst_inputs(ni);
    LogExpr expr = src_node->expr();
    for (size_t j = 0; j < ni; ++ j) {
      CnNode* src_inode = src_node->fanin(j);
      CnNode* input = nodemap[src_inode->id()];
      assert_cond(input, __FILE__, __LINE__);
      dst_inputs[j] = input;
    }
    
    CnNode* dst_node = new_cell(src_node->name(), dst_inputs, expr);
    dst_node->mMark = src_node->mMark;
    nodemap[src_node->id()] = dst_node;
  }

  // 外部出力の生成
  const CnNodeList& output_list = src.output_list();
  for (CnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    CnNode* src_onode = *p;
    CnNode* src_inode = src_onode->fanin(0);
    const string& name = src_onode->name();
    CnNode* dst_inode = NULL;
    if ( src_inode ) {
      dst_inode = nodemap[src_inode->id()];
    }
    (void) new_output(name, dst_inode);
  }
}

// 空にする．
void
CnGraph::clear()
{
  // まず最初に接続を切る．
  for (CnNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    CnNode* node = *p;
    connect(NULL, node, 0);
  }
  for (CnNodeList::iterator p = mCnodeList.begin();
       p != mCnodeList.end(); ++ p) {
    CnNode* node = *p;
    size_t ni = node->ni();
    for (size_t i = 0; i < ni; ++ i) {
      connect(NULL, node, i);
    }
  }

  for (CnNodeList::iterator p = mInputList.begin();
       p != mInputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    CnNode* node = *p;
    ++ p;
    delete_input(node);
  }
  assert_cond(mInputList.empty(), __FILE__, __LINE__);

  for (CnNodeList::iterator p = mOutputList.begin();
       p != mOutputList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    CnNode* node = *p;
    ++ p;
    delete_output(node);
  }
  assert_cond(mOutputList.empty(), __FILE__, __LINE__);

  for (CnNodeList::iterator p = mCnodeList.begin();
       p != mCnodeList.end(); ) {
    // p は実際にはノード内のメンバをアクセスするので delete する前に
    // 進めておく必要がある．
    CnNode* node = *p;
    ++ p;
    delete_cell(node);
  }
  assert_cond(mCnodeList.empty(), __FILE__, __LINE__);

  mInputArray.clear();
  mOutputArray.clear();
}

// @brief ソートされたノードのリストを得る．
void
CnGraph::sort(vector<CnNode*>& node_list) const
{
  node_list.clear();
  node_list.reserve(n_cnodes());
    
  size_t n = max_node_id();
  vector<bool> mark(n, false);
    
  size_t rpos = 0;
  
  // 外部入力のみをファンインにするノードを node_list に追加する．
  for (CnNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    CnNode* node = *p;
    mark[node->id()] = true;
    const CnEdgeList& fo_list = node->fanout_list();
    for (CnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      CnEdge* e = *p;
      CnNode* onode = e->to();
      if ( mark[onode->id()] || onode->is_output() ) continue;
      bool ready = true;
      size_t ni = onode->ni();
      for (size_t i = 0; i < ni; ++ i) {
	CnNode* inode0 = onode->fanin(i);
	if ( !mark[inode0->id()] ) {
	  ready = false;
	  break;
	}
      }
      if ( ready ) {
	mark[onode->id()] = true;
	node_list.push_back(onode);
      }
    }
  }

  // 入力を持たないノードを node_list に追加する．
  for (CnNodeListConstIter p = mCnodeList.begin();
       p != mCnodeList.end(); ++ p) {
    CnNode* node = *p;
    if ( node->ni() == 0 && !mark[node->id()] ) {
      mark[node->id()] = true;
      node_list.push_back(node);
    }
  }
  
  while ( rpos < node_list.size() ) {
    CnNode* node = node_list[rpos];
    ++ rpos;
    const CnEdgeList& fo_list = node->fanout_list();
    for (CnEdgeList::const_iterator p = fo_list.begin();
	 p != fo_list.end(); ++ p) {
      CnEdge* e = *p;
      CnNode* onode = e->to();
      if ( mark[onode->id()] || onode->is_output() ) continue;
      bool ready = true;
      size_t ni = onode->ni();
      for (size_t i = 0; i < ni; ++ i) {
	CnNode* inode0 = onode->fanin(i);
	if ( !mark[inode0->id()] ) {
	  ready = false;
	  break;
	}
      }
      if ( ready ) {
	mark[onode->id()] = true;
	node_list.push_back(onode);
      }
    }
  }
  assert_cond(node_list.size() == n_cnodes(), __FILE__, __LINE__);
}

// @brief 入力ノードを作る．
// @param[in] name 名前
// @return 作成したノードを返す．
CnNode*
CnGraph::new_input(const string& name)
{
  CnNode* node = new_node(0);

  // 名前を設定
  node->mName = name;
  
  // 入力ノード配列に登録
  size_t subid = mInputArray.size();
  mInputArray.push_back(node);

  // 入力リストに登録
  mInputList.push_back(node);

  node->set_input(subid);
  
  return node;
}

// @brief 出力ノードを作る．
// @param[in] name 名前
// @param[in] inode 入力のノード
// @return 作成したノードを返す．
CnNode*
CnGraph::new_output(const string& name,
		    CnNode* inode)
{
  CnNode* node = new_node(1);

  // 名前を設定
  node->mName = name;
  
  // 出力ノード配列に登録
  size_t subid = mOutputArray.size();
  mOutputArray.push_back(node);

  // 出力リストに登録
  mOutputList.push_back(node);

  node->set_output(subid);
  connect(inode, node, 0);

  return node;
}

// @brief CELLノードを作る．
// @param[in] name 名前
// @param[in] inodes 入力ノードのベクタ
// @param[in] expr 論理式
CnNode*
CnGraph::new_cell(const string& name,
		  const vector<CnNode*>& inodes,
		  const LogExpr& expr)
{
  size_t ni = inodes.size();
  CnNode* node = new_node(ni);

  node->mName = name;
  
  // 論理ノードリストに登録
  mCnodeList.push_back(node);

  change_cell(node, inodes, expr);
  
  return node;
}

// 入力ノードの削除
void
CnGraph::delete_input(CnNode* node)
{
  assert_cond(node->is_input(), __FILE__, __LINE__);
  mInputList.erase(node);
  delete_node(node);
}

// 出力ノードの削除
void
CnGraph::delete_output(CnNode* node)
{
  assert_cond(node->is_output(), __FILE__, __LINE__);
  mOutputList.erase(node);
  delete_node(node);
}

// CELLノードの削除
void
CnGraph::delete_cell(CnNode* node)
{
  assert_cond(node->is_cell(), __FILE__, __LINE__);
  assert_cond(node->n_fanout() == 0, __FILE__, __LINE__);
  size_t ni = node->ni();
  for (size_t i = 0; i < ni; ++ i) {
    connect(NULL, node, i);
  }

  mCnodeList.erase(node);
  delete_node(node);
}

// 新しいノードを作成する．
// 作成されたノードを返す．
CnNode*
CnGraph::new_node(size_t ni)
{
  CnNode* node = NULL;

  // 空いているIDを探してノード配列へ登録
  int id = mItvlMgr.avail_num();
  mItvlMgr.erase(id);

  size_t uid = static_cast<size_t>(id);
  while( mNodeArray.size() <= uid ) {
    // mNodeArray の大きさが小さいときのための埋め草
    void* p = mAlloc.get_memory(sizeof(CnNode));
    CnNode* node = new (p) CnNode;
    mNodeArray.push_back(node);
  }
  node = mNodeArray[uid];
  node->mNi = ni;
  if ( ni ) {
    void* q = mAlloc2.get_memory(sizeof(CnEdge) * ni);
    node->mFanins = new (q) CnEdge[ni];
    for (size_t i = 0; i < ni; ++ i) {
      node->mFanins[i].set_to(node, i);
    }
  }
  else {
    node->mFanins = NULL;
  }
  node->mId = uid;
  node->mMark = 0;

  return node;
}

// node を削除する．
void
CnGraph::delete_node(CnNode* node)
{
  // new_node の逆の処理を行なう．
  mItvlMgr.add(static_cast<int>(node->mId));
  // 本当は CnEdge のデストラクタを起動する必要があるが中身がないので
  // 省略する．
  if ( node->ni() ) {
    mAlloc2.put_memory(sizeof(CnEdge)* node->ni(), node->mFanins);
  }
}
    
// @brief 出力ノードの内容を変更する
// @param[in] 変更対象の出力ノード
// @param[in] inode 入力のノード
// @param[in] inv 反転している時 true をあたえる．
void
CnGraph::change_output(CnNode* node,
		       CnNode* inode)
{
  node->set_output(node->subid());
  connect(inode, node, 0);
}

// @brief 論理ノードの内容を再設定する．
// @param[in] node 変更対象の論理ノード
// @param[in] inodes 入力ノードの配列
// @param[in] expr 論理式
void
CnGraph::change_cell(CnNode* node,
		     const vector<CnNode*>& inodes,
		     const LogExpr& expr)
{
  node->set_cell();
  node->mExpr = expr;
  change_node_fanins(node, inodes);
}
  
// @brief CELL ノードのファンインのみ変更する．
// @param[in] node 変更対象の論理ノード
// @param[in] inodes 入力ノードのベクタ
void
CnGraph::change_node_fanins(CnNode* node,
			    const vector<CnNode*>& inodes)
{
  size_t ni = inodes.size();
  if ( node->ni() != ni ) {
    if ( node->ni() ) {
      mAlloc2.put_memory(sizeof(CnEdge)* node->ni(), node->mFanins);
    }
    if ( ni ) {
      void* q = mAlloc2.get_memory(sizeof(CnEdge) * ni);
      node->mFanins = new (q) CnEdge[ni];
      for (size_t i = 0; i < ni; ++ i) {
	node->mFanins[i].set_to(node, i);
      }
    }
    else {
      node->mFanins = NULL;
    }
    node->mNi = ni;
  }
  for (size_t i = 0; i < ni; ++ i) {
    connect(inodes[i], node, i);
  }
}
  
// @brief ノードの置き換えを行う．
// @param[in] old_node 置き換え対象のノード
// @param[in] new_node 置き換わる新しいノード
// @param[in] inv true なら極性を反転させる．
void
CnGraph::subst_node(CnNode* old_node,
		    CnNode* new_node,
		    bool inv)
{
  // old_node のファンアウトは変化するのでファンアウトリストをコピーしておく
  const CnEdgeList& fo_list = old_node->fanout_list();
  vector<CnEdge*> tmp_list;
  tmp_list.reserve(old_node->n_fanout());
  for (CnEdgeList::const_iterator r = fo_list.begin();
       r != fo_list.end(); ++ r) {
    tmp_list.push_back(*r);
  }
  
  for (vector<CnEdge*>::iterator r = tmp_list.begin();
       r != tmp_list.end(); ++ r) {
    CnEdge* edge = *r;
    assert_cond(edge->from() == old_node, __FILE__, __LINE__);
    CnNode* onode = edge->to();
    size_t ni = onode->ni();
    vector<CnNode*> inodes(ni);
    for (size_t i = 0; i < ni; ++ i) {
      if ( i != edge->pos() ) {
	inodes[i] = onode->fanin(i);
      }
      else {
	inodes[i] = new_node;
      }
    }
    change_node_fanins(onode, inodes);
  }
}

// from を to の pos 番目のファンインとする．
// to の pos 番目にすでに接続があった場合には自動的に削除される．
void
CnGraph::connect(CnNode* from,
		 CnNode* to,
		 size_t pos)
{
  // CnNode::mFaoutList を変更するのはここだけ
  
  CnEdge* edge = to->fanin_edge(pos);
  CnNode* old_from = edge->from();
  if ( old_from ) {
    old_from->mFanoutList.erase(edge);
    old_from->scan_po();
  }
  edge->set_from(from);
  if ( from ) {
    from->mFanoutList.push_back(edge);
    from->scan_po();
  }
}

// 内容を s に出力する．
void
CnGraph::dump(ostream& s) const
{
  for (CnNodeList::const_iterator p = mInputList.begin();
       p != mInputList.end(); ++ p) {
    const CnNode* node = *p;
    s << "I" << node->subid() << "(" << node->id_str() << "): "
      << node->name() << endl;
  }
  for (CnNodeList::const_iterator p = mCnodeList.begin();
       p != mCnodeList.end(); ++ p) {
    const CnNode* node = *p;
    s << node->id_str() << "(" << node->name() << ") = CELL(";
    const char* comma = "";
    size_t ni = node->ni();
    for (size_t i = 0; i < ni; ++ i) {
      const CnNode* inode = node->fanin(i);
      s << comma << inode->id_str();
      comma = ", ";
    }
    s << ")";
    s << endl;
  }
  for (CnNodeList::const_iterator p = mOutputList.begin();
       p != mOutputList.end(); ++ p) {
    const CnNode* node = *p;
    const CnEdge* e = node->fanin_edge(0);
    s << "O" << node->subid() << "(" << node->id_str() << "): "
      << node->name() << " = ";
    const CnNode* inode = e->from();
    if ( inode ) {
      // 普通のノードの場合
      s << inode->id_str();
    }
    else {
      // 定数ノードの場合
      s << "0";
    }
    s << endl;
  }
}

END_NAMESPACE_MAGUS_CELLMAP
