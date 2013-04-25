
/// @file LcPatMgr.cc
/// @brief LcPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcPatMgr.h"
#include "LcPatNode.h"
#include "LcPatHandle.h"

#include "ym_logic/LogExpr.h"
#include "ym_utils/Generator.h"


BEGIN_NONAMESPACE
#include "lcpat_table"
END_NONAMESPACE


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcPatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcPatMgr::LcPatMgr() :
  mAlloc(sizeof(LcPatNode), 1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
LcPatMgr::~LcPatMgr()
{
  delete [] mHashTable;
}

// @brief 初期化する．
void
LcPatMgr::init()
{
  mAlloc.destroy();
  mInputList.clear();
  mNodeList.clear();
  delete [] mHashTable;
  mHashTable = NULL;
  mHashSize = 0;
  alloc_table(1024);
  mPatList.clear();
  mRepList.clear();
}

// @brief 全ノード数を返す．
ymuint
LcPatMgr::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
LcPatNode*
LcPatMgr::node(ymuint pos) const
{
  return mNodeList[pos];
}

// @brief パタン数を返す．
ymuint
LcPatMgr::pat_num() const
{
  return mPatList.size();
}

// @brief パタンの根のハンドルを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
LcPatHandle
LcPatMgr::pat_root(ymuint id) const
{
  assert_cond( id < pat_num(), __FILE__, __LINE__);
  return mPatList[id];
}

// @brief パタンの属している代表関数番号を返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
ymuint
LcPatMgr::rep_id(ymuint id) const
{
  return mRepList[id];
}

// @brief 論理式から生成されるパタンを登録する．
// @param[in] expr パタンの元となる論理式
// @param[in] rep_id このパタンが属する代表関数番号
void
LcPatMgr::reg_pat(const LogExpr& expr,
		  ymuint rep_id)
{
  if ( mExprList.size() <= rep_id ) {
    mExprList.resize(rep_id + 1, vector<LogExpr>());
  }

  { // 同じ論理式を処理済みならなにもしない．
    vector<LogExpr>& expr_list = mExprList[rep_id];
    for (vector<LogExpr>::iterator p = expr_list.begin();
	 p != expr_list.end(); ++ p) {
      const LogExpr& expr1 = *p;
      if ( check_equivalent(expr, expr1) ) {
	return;
      }
    }
    // 論理式を登録しておく．
    expr_list.push_back(expr);
  }

  vector<LcPatHandle> tmp_pat_list;
  pg_sub(expr, tmp_pat_list);

  // 重複チェック
  hash_set<string> pg_hash;
  ymuint n = mPatList.size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( mRepList[i] != rep_id ) continue;
    LcPatHandle pat1 = mPatList[i];
    LcPatNode* node1 = pat1.node();
    bool inv1 = pat1.inv();
    string signature1 = node1->signature();
    if ( inv1 ) {
      signature1 = "~" + signature1;
    }
    pg_hash.insert(signature1);
  }

  for (vector<LcPatHandle>::iterator p = tmp_pat_list.begin();
       p != tmp_pat_list.end(); ++ p) {
    LcPatHandle pat1 = *p;
    LcPatNode* node1 = pat1.node();
    bool inv1 = pat1.inv();
    string signature2 = node1->signature();
    if ( inv1 ) {
      signature2 = "~" + signature2;
    }
    if ( pg_hash.find(signature2) == pg_hash.end() ) {
      // pat1 を登録する．
      mPatList.push_back(pat1);
      mRepList.push_back(rep_id);
      node1->set_locked();
    }
  }

  sweep();
}

// @brief 2つの論理式が同形かどうか調べる．
bool
LcPatMgr::check_equivalent(const LogExpr& expr1,
			   const LogExpr& expr2)
{
  if ( expr1.is_zero() ) {
    if ( expr2.is_zero() ) {
      return true;
    }
    return false;
  }
  if ( expr1.is_one() ) {
    if ( expr2.is_one() ) {
      return true;
    }
    return false;
  }
  if ( expr2.is_constant() ) {
    return false;
  }

  if ( expr1.is_posiliteral() ) {
    if ( expr2.is_posiliteral() && expr1.varid() == expr2.varid() ) {
      return true;
    }
    return false;
  }
  if ( expr1.is_negaliteral() ) {
    if ( expr2.is_negaliteral() && expr1.varid() == expr2.varid() ) {
      return true;
    }
    return false;
  }
  if ( expr2.is_literal() ) {
    return false;
  }

  ymuint n = expr1.child_num();
  if ( expr2.child_num() != n ) {
    return false;
  }

  if ( expr1.is_and() && !expr2.is_and() ) {
    return false;
  }
  if ( expr1.is_or() && !expr2.is_or() ) {
    return false;
  }
  if ( expr1.is_xor() && !expr2.is_xor() ) {
    return false;
  }

  PermGen pg(n, n);
  for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
    bool match = true;
    for (ymuint i = 0; i < n; ++ i) {
      if ( !check_equivalent(expr1.child(i), expr2.child(p(i))) ) {
	match = false;
	break;
      }
    }
    if ( match ) {
      return true;
    }
  }
  return false;
}

// @brief 使われていないパタンとノードを削除してID番号を詰める．
void
LcPatMgr::sweep()
{
  // lock されていないノードの削除
  ymuint wpos = 0;
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    LcPatNode* node = mNodeList[i];
    if ( node->is_locked() ) {
      if ( wpos != i ) {
	mNodeList[wpos] = node;
	node->mId = wpos;
      }
      ++ wpos;
    }
    else {
      if ( node->is_and() || node->is_xor() ) {
	// ハッシュ表から取り除く
	ymuint pos = hash_func(node->mType, node->mFanin[0], node->mFanin[1]);
	ymuint idx = pos % mHashSize;
	LcPatNode** p_prev = &mHashTable[idx];
	for ( ; ; ) {
	  LcPatNode* node1 = *p_prev;
	  assert_cond( node1 != NULL, __FILE__, __LINE__);
	  if ( node == node1 ) {
	    *p_prev = node->mLink;
	    break;
	  }
	  p_prev = &(node1->mLink);
	}
      }
      delete_node(node);
    }
  }
  mNodeList.erase(mNodeList.begin() + wpos, mNodeList.end());

  // 入力ノードの整列
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    LcPatNode* node = mNodeList[i];
    if ( !node->is_input() ) continue;
    ymuint iid = node->input_id();
    if ( iid == i ) continue;
    LcPatNode* node1 = mNodeList[iid];
    mNodeList[iid] = node;
    node->mId = iid;
    mNodeList[i] = node1;
    node1->mId = i;
  }
}

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
// @param[out] pg_list パタングラフのリスト
void
LcPatMgr::pg_sub(const LogExpr& expr,
		 vector<LcPatHandle>& pg_list)
{
  if ( expr.is_literal() ) {
    LcPatNode* node = make_input(expr.varid());
    bool inv = expr.is_negaliteral();
    pg_list.push_back(LcPatHandle(node, inv));
  }
  else {
    ymuint n = expr.child_num();
    // ファンインの式に対するパタングラフを求める．
    vector<vector<LcPatHandle> > input_pg_list(n);
    vector<pair<ymuint, ymuint> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      pg_sub(expr.child(i), input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }

    hash_set<string> pg_hash;
    // ファンインのパタンの組み合わせを列挙するオブジェクト
    MultiCombiGen mcg(nk_array);
    for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
      vector<LcPatHandle> tmp_input(n);
      bool all_inputs = true;
      for (ymuint i = 0; i < n; ++ i) {
	tmp_input[i] = input_pg_list[i][p(i, 0)];
	if ( !tmp_input[i].node()->is_input() ) {
	  all_inputs = false;
	}
      }

      if ( all_inputs ) {
	switch ( n ) {
	case 2:
	  {
	    LcPatHandle handle = make_node(expr, tmp_input[0], tmp_input[1]);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 3:
	  for (ymuint i = 0; i < n_pat3; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat3[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 4:
	  for (ymuint i = 0; i < n_pat4; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat4[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 5:
	  for (ymuint i = 0; i < n_pat5; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat5[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 6:
	  for (ymuint i = 0; i < n_pat6; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat6[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 7:
	  for (ymuint i = 0; i < n_pat7; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat7[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 8:
	  for (ymuint i = 0; i < n_pat8; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, tmp_input, pat8[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	default:
	  assert_not_reached(__FILE__, __LINE__);
	  break;
	}
      }
      else {

	// 入力の順列を列挙するオブジェクト
	PermGen pg(n, n);
	vector<LcPatHandle> input(n);
	for (PermGen::iterator q = pg.begin(); !q.is_end(); ++ q) {
	  for (ymuint i = 0; i < n; ++ i) {
	    ymuint j = q(i);
	    input[j] = tmp_input[i];
	  }
	  switch ( n ) {
	  case 2:
	    {
	      LcPatHandle handle = make_node(expr, input[0], input[1]);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 3:
	    for (ymuint i = 0; i < n_pat3; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat3[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 4:
	    for (ymuint i = 0; i < n_pat4; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat4[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 5:
	    for (ymuint i = 0; i < n_pat5; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat5[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 6:
	    for (ymuint i = 0; i < n_pat6; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat6[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 7:
	    for (ymuint i = 0; i < n_pat7; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat7[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  case 8:
	    for (ymuint i = 0; i < n_pat8; ++ i) {
	      ymuint pos = 0;
	      LcPatHandle handle = make_bintree(expr, input, pat8[i], pos);
	      add_pg_list(pg_list, pg_hash, handle);
	    }
	    break;

	  default:
	    assert_not_reached(__FILE__, __LINE__);
	    break;
	  }
	}
      }
    }
  }
}

// pg_list に new_handle を追加する．
// ただし，同形のパタンがすでにある場合には追加しない．
void
LcPatMgr::add_pg_list(vector<LcPatHandle>& pg_list,
		      hash_set<string>& pg_hash,
		      LcPatHandle new_handle)
{
  string signature = new_handle.node()->signature();
  if ( new_handle.inv() ) {
    signature = "~" + signature;
  }
  if ( pg_hash.find(signature) == pg_hash.end() ) {
    pg_hash.insert(signature);
    pg_list.push_back(new_handle);
  }
}

// @brief テンプレートにしたがって2分木を作る．
// @param[in] expr 論理式 (演算の種類を表すのに用いる)
// @param[in] input 入力の配列
// @param[in] pat 2分木の形を表す配列
// @param[inout] pos pat[] 中の位置を示す変数
LcPatHandle
LcPatMgr::make_bintree(const LogExpr& expr,
		       const vector<LcPatHandle>& input,
		       int pat[],
		       ymuint& pos)
{
  int p = pat[pos];
  ++ pos;
  if ( p == -1 ) {
    // 演算ノード
    LcPatHandle l_handle = make_bintree(expr, input, pat, pos);
    LcPatHandle r_handle = make_bintree(expr, input, pat, pos);
    return make_node(expr, l_handle, r_handle);
  }
  else {
    // 終端ノード
    ymuint id = static_cast<ymuint>(p);
    return input[id];
  }
}

// @brief 入力ノードを作る．
// @param[in] var 入力変数
// @note 既にあるときはそれを返す．
LcPatNode*
LcPatMgr::make_input(VarId var)
{
  ymuint id = var.val();
  while ( mInputList.size() <= id ) {
    LcPatNode* node = new_node();
    ymuint input_id = mInputList.size();
    node->mType = (input_id << 2) | LcPatNode::kInput;
    mInputList.push_back(node);
  }
  LcPatNode* node = mInputList[id];
  assert_cond( node != NULL, __FILE__, __LINE__);

  node->mSignature = "0";

  return node;
}

// @brief 論理式の種類に応じてノードを作る．
// @param[in] expr 論理式 (演算の種類を表すのに用いる)
// @param[in] l_handle, r_handle 左右の子供のパタン
LcPatHandle
LcPatMgr::make_node(const LogExpr& expr,
		    LcPatHandle l_handle,
		    LcPatHandle r_handle)
{
  LcPatNode* l_node = l_handle.node();
  LcPatNode* r_node = r_handle.node();
  bool l_inv = l_handle.inv();
  bool r_inv = r_handle.inv();

  bool oinv = false;
  ymuint32 type = 0U;
  if ( expr.is_and() ) {
    type = LcPatNode::kAnd;
  }
  else if ( expr.is_or() ) {
    type = LcPatNode::kAnd;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = LcPatNode::kXor;
    oinv = l_inv ^ r_inv;
    l_inv = false;
    r_inv = false;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( l_inv ) {
    type |= 4U;
  }
  if ( r_inv ) {
    type |= 8U;
  }

  // (type, l_node, r_node) というノードがすでにあったらそれを使う．
  ymuint pos = hash_func(type, l_node, r_node);
  ymuint idx = pos % mHashSize;
  for (LcPatNode* node = mHashTable[idx]; node; node = node->mLink) {
    if ( node->mType == type &&
	 node->mFanin[0] == l_node &&
	 node->mFanin[1] == r_node ) {
      // おなじノードがあった．
      return LcPatHandle(node, oinv);
    }
  }

  // 新しいノードを作る．
  LcPatNode* node = new_node();
  node->mType = type;
  node->mFanin[0] = l_node;
  node->mFanin[1] = r_node;

  ostringstream buf;
  buf << type << l_node->mSignature << r_node->mSignature;
  node->mSignature = buf.str();

  // ハッシュ表に登録する．
  if ( node_num() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return LcPatHandle(node, oinv);
}

// @brief ノードを作る．
LcPatNode*
LcPatMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(LcPatNode));
  LcPatNode* node = new (p) LcPatNode();
  node->mId = mNodeList.size();
  mNodeList.push_back(node);
  return node;
}

// @brief ノードを削除する．
void
LcPatMgr::delete_node(LcPatNode* node)
{
  node->~LcPatNode();
  mAlloc.put_memory(sizeof(LcPatNode), static_cast<void*>(node));
}

// @brief ハッシュ表を確保する．
void
LcPatMgr::alloc_table(ymuint req_size)
{
  LcPatNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new LcPatNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      LcPatNode* next = NULL;
      for (LcPatNode* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->mType, node->mFanin[0], node->mFanin[1]);
	ymuint idx = pos % mHashSize;
	node->mLink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

// @brief LcPatNode のハッシュ関数
ymuint
LcPatMgr::hash_func(ymuint type,
		    LcPatNode* l_node,
		    LcPatNode* r_node)
{
  ympuint l = reinterpret_cast<ympuint>(l_node);
  ympuint r = reinterpret_cast<ympuint>(r_node);

  return type + l * 5 + r;
}


BEGIN_NONAMESPACE

// @brief パタングラフを DFS でたどって内容を val_list に入れる．
// @param[in] node ノード
// @param[in] vmark 訪れたかどうかの情報を持つ配列
// @param[out] val_list ノードの情報を格納するリスト
// @return 最大入力番号+1を返す．
ymuint
dfs(LcPatNode* node,
    vector<bool>& vmark,
    vector<ymuint>& val_list)
{
  if ( node->is_input() ) {
    return node->input_id() + 1;
  }
  if ( vmark[node->id()] ) {
    return 0;
  }
  vmark[node->id()] = true;
  val_list.push_back(node->id() * 2);
  ymuint id = dfs(node->fanin(0), vmark, val_list);
  val_list.push_back(node->id() * 2 + 1);
  ymuint id1 = dfs(node->fanin(1), vmark, val_list);
  if ( id < id1 ) {
    id = id1;
  }
  return id;
}

END_NONAMESPACE

// @brief パタンのノードリストを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
// @param[out] node_list パタンを DFS 順でたどった時のノード番号のリスト
// @return このパタンの入力数を返す．
ymuint
LcPatMgr::pat_node_list(ymuint id,
			vector<ymuint>& node_list) const
{
  LcPatHandle root = pat_root(id);
  node_list.clear();
  node_list.reserve(node_num());
  vector<bool> vmark(node_num(), false);
  ymuint max_input =dfs(root.node(), vmark, node_list);
  ymuint32 v = max_input << 1;
  if ( root.inv() ) {
    v |= 1U;
  }
  return v;
}


//////////////////////////////////////////////////////////////////////
// display() 関係の関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LcPatMgr::display(ostream& s) const
{
  s << "*** LcPatMgr BEGIN ***" << endl;

  s << "*** NODE SECTION ***" << endl;
  ymuint n = node_num();
  for (ymuint i = 0; i < n; ++ i) {
    LcPatNode* node = this->node(i);
    if ( node->is_locked() ) {
      s << "*";
    }
    else {
      s << " ";
    }
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else {
      if ( node->is_and() ) {
	s << "And";
      }
      else if ( node->is_xor() ) {
	s << "Xor";
      }
      else {
	assert_not_reached(__FILE__, __LINE__);
      }
      s << "( ";
      display_edge(s, node, 0);
      s << ", ";
      display_edge(s, node, 1);
      s << ")";
    }
    s << endl;
  }
  s << endl;

  s << "*** PATTERN SECTION ***" << endl;
  ymuint np = pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    LcPatHandle root = pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    s << "Node#" << root.node()->id()
      << " --> Rep#" << rep_id(i)
      << endl;
  }
  s << "*** LcPatMgr END ***" << endl;
}

// @brief 枝の情報を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 親のノード
// @param[in] fanin_pos ファンイン番号
void
LcPatMgr::display_edge(ostream& s,
		       LcPatNode* node,
		       ymuint fanin_pos)
{
  if ( node->fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node->fanin(fanin_pos)->id();
}

END_NAMESPACE_YM_CELL_LIBCOMP
