
/// @file LcPat2Mgr.cc
/// @brief LcPat2Mgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcPat2Mgr.h"
#include "LcPat2Node.h"
#include "LcPat2Handle.h"

#include "ym_logic/LogExpr.h"
#include "ym_utils/Generator.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
// クラス LcPat2Mgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LcPat2Mgr::LcPat2Mgr() :
  mAlloc(sizeof(LcPat2Node), 1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
LcPat2Mgr::~LcPat2Mgr()
{
  delete [] mHashTable;
}

// @brief 初期化する．
void
LcPat2Mgr::init()
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
LcPat2Mgr::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
LcPat2Node*
LcPat2Mgr::node(ymuint pos) const
{
  return mNodeList[pos];
}

// @brief パタン数を返す．
ymuint
LcPat2Mgr::pat_num() const
{
  return mPatList.size();
}

// @brief パタンの根のハンドルを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
LcPat2Handle
LcPat2Mgr::pat_root(ymuint id) const
{
  assert_cond( id < pat_num(), __FILE__, __LINE__);
  return mPatList[id];
}

// @brief パタンの属している代表関数番号を返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
ymuint
LcPat2Mgr::rep_id(ymuint id) const
{
  return mRepList[id];
}

// @brief 論理式から生成されるパタンを登録する．
// @param[in] expr パタンの元となる論理式
// @param[in] rep_id このパタンが属する代表関数番号
void
LcPat2Mgr::reg_pat(const LogExpr& expr,
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

#if 1
  LcPat2Handle pat = pg_sub(expr);
  mPatList.push_back(pat);
  mRepList.push_back(rep_id);
#else
  vector<LcPat2Handle> tmp_pat_list;
  pg_sub(expr, tmp_pat_list);

  // 重複チェック
  hash_set<string> pg_hash;
  ymuint n = mPatList.size();
  for (ymuint i = 0; i < n; ++ i) {
    if ( mRepList[i] != rep_id ) continue;
    LcPat2Handle pat1 = mPatList[i];
    LcPat2Node* node1 = pat1.node();
    bool inv1 = pat1.inv();
    string signature1 = node1->signature();
    if ( inv1 ) {
      signature1 = "~" + signature1;
    }
    pg_hash.insert(signature1);
  }

  for (vector<LcPat2Handle>::iterator p = tmp_pat_list.begin();
       p != tmp_pat_list.end(); ++ p) {
    LcPat2Handle pat1 = *p;
    LcPat2Node* node1 = pat1.node();
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
#endif
}

// @brief 2つの論理式が同形かどうか調べる．
bool
LcPat2Mgr::check_equivalent(const LogExpr& expr1,
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

#if 1

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
LcPat2Handle
LcPat2Mgr::pg_sub(const LogExpr& expr)
{
  if ( expr.is_literal() ) {
    LcPat2Node* node = make_input(expr.varid());
    bool inv = expr.is_negaliteral();
    return LcPat2Handle(node, inv);
  }

  ymuint n = expr.child_num();
  vector<LcPat2Handle> chd_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    chd_array[i] = pg_sub(expr.child(i));
  }

  return make_node(expr, chd_array);
}

#else

// @brief 使われていないパタンとノードを削除してID番号を詰める．
void
LcPat2Mgr::sweep()
{
  // lock されていないノードの削除
  ymuint wpos = 0;
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    LcPat2Node* node = mNodeList[i];
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
	LcPat2Node** p_prev = &mHashTable[idx];
	for ( ; ; ) {
	  LcPat2Node* node1 = *p_prev;
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
    LcPat2Node* node = mNodeList[i];
    if ( !node->is_input() ) continue;
    ymuint iid = node->input_id();
    if ( iid == i ) continue;
    LcPat2Node* node1 = mNodeList[iid];
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
LcPat2Mgr::pg_sub(const LogExpr& expr,
		 vector<LcPat2Handle>& pg_list)
{
  if ( expr.is_literal() ) {
    LcPat2Node* node = make_input(expr.varid());
    bool inv = expr.is_negaliteral();
    pg_list.push_back(LcPat2Handle(node, inv));
  }
  else {
    ymuint n = expr.child_num();
    // ファンインの式に対するパタングラフを求める．
    vector<vector<LcPat2Handle> > input_pg_list(n);
    vector<pair<ymuint, ymuint> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      pg_sub(expr.child(i), input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }

    hash_set<string> pg_hash;
    // ファンインのパタンの組み合わせを列挙するオブジェクト
    MultiCombiGen mcg(nk_array);
    for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
      vector<LcPat2Handle> tmp_input(n);
      for (ymuint i = 0; i < n; ++ i) {
	tmp_input[i] = input_pg_list[i][p(i, 0)];
      }

      // 入力の順列を列挙するオブジェクト
      PermGen pg(n, n);
      vector<LcPat2Handle> input(n);
      for (PermGen::iterator q = pg.begin(); !q.is_end(); ++ q) {
	for (ymuint i = 0; i < n; ++ i) {
	  ymuint j = q(i);
	  input[j] = tmp_input[i];
	}
	switch ( n ) {
	case 2:
	  {
	    LcPat2Handle handle = make_node(expr, input[0], input[1]);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 3:
	  for (ymuint i = 0; i < n_pat3; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat3[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 4:
	  for (ymuint i = 0; i < n_pat4; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat4[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 5:
	  for (ymuint i = 0; i < n_pat5; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat5[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 6:
	  for (ymuint i = 0; i < n_pat6; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat6[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 7:
	  for (ymuint i = 0; i < n_pat7; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat7[i], pos);
	    add_pg_list(pg_list, pg_hash, handle);
	  }
	  break;

	case 8:
	  for (ymuint i = 0; i < n_pat8; ++ i) {
	    ymuint pos = 0;
	    LcPat2Handle handle = make_bintree(expr, input, pat8[i], pos);
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

// pg_list に new_handle を追加する．
// ただし，同形のパタンがすでにある場合には追加しない．
void
LcPat2Mgr::add_pg_list(vector<LcPat2Handle>& pg_list,
		      hash_set<string>& pg_hash,
		      LcPat2Handle new_handle)
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

#endif

// @brief 入力ノードを作る．
// @param[in] var 入力変数
// @note 既にあるときはそれを返す．
LcPat2Node*
LcPat2Mgr::make_input(VarId var)
{
  ymuint id = var.val();
  while ( mInputList.size() <= id ) {
    LcPat2Node* node = new_node(0);
    ymuint id1 = mInputList.size();
    node->mType = (id1 << 2) | LcPat2Node::kInput;
    mInputList.push_back(node);
  }
  LcPat2Node* node = mInputList[id];
  assert_cond( node != NULL, __FILE__, __LINE__);

  return node;
}

// @brief 論理式の種類に応じてノードを作る．
// @param[in] expr 論理式 (演算の種類を表すのに用いる)
// @param[in] chd_handles 子供のパタンの配列
LcPat2Handle
LcPat2Mgr::make_node(const LogExpr& expr,
		     const vector<LcPat2Handle>& chd_handles)
{
  ymuint ni = chd_handles.size();
  vector<LcPat2Node*> node_array(ni);
  vector<bool> inv_array(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    node_array[i] = chd_handles[i].node();
    inv_array[i] = chd_handles[i].inv();
  }

  bool oinv = false;
  ymuint32 type = 0U;
  if ( expr.is_and() ) {
    type = LcPat2Node::kAnd;
  }
  else if ( expr.is_or() ) {
    type = LcPat2Node::kAnd;
    for (ymuint i = 0; i < ni; ++ i) {
      inv_array[i] = !inv_array[i];
    }
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = LcPat2Node::kXor;
    for (ymuint i = 0; i < ni; ++ i) {
      oinv ^= inv_array[i];
      inv_array[i] = false;
    }
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  type |= (ni << 2);

  vector<LcPat2Handle> new_chd_array(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    new_chd_array[i] = LcPat2Handle(node_array[i], inv_array[i]);
  }

  // (type, new_chd_array) というノードがすでにあったらそれを使う．
  ymuint pos = hash_func(type, new_chd_array);
  ymuint idx = pos % mHashSize;
  for (LcPat2Node* node = mHashTable[idx]; node; node = node->mLink) {
    if ( node->mType != type ) {
      continue;
    }
    bool match = true;
    for (ymuint i = 0; i < ni; ++ i) {
      if ( node->mFaninArray[i] != new_chd_array[i] ) {
	match = false;
	break;
      }
    }
    if ( match ) {
      // おなじノードがあった．
      return LcPat2Handle(node, oinv);
    }
  }

  // 新しいノードを作る．
  LcPat2Node* node = new_node(ni);
  node->mType = type;
  for (ymuint i = 0; i < ni; ++ i) {
    node->mFaninArray[i] = new_chd_array[i];
  }

  // ハッシュ表に登録する．
  if ( node_num() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return LcPat2Handle(node, oinv);
}

// @brief ノードを作る．
// @param[in] fanin_num ファンイン数
LcPat2Node*
LcPat2Mgr::new_node(ymuint fanin_num)
{
  ymuint size = sizeof(LcPat2Node) + sizeof(LcPat2Handle) * (fanin_num - 1);
  void* p = mAlloc.get_memory(size);
  LcPat2Node* node = new (p) LcPat2Node();
  node->mId = mNodeList.size();
  mNodeList.push_back(node);
  return node;
}

// @brief ハッシュ表を確保する．
void
LcPat2Mgr::alloc_table(ymuint req_size)
{
  LcPat2Node** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new LcPat2Node*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      LcPat2Node* next = NULL;
      for (LcPat2Node* node = old_table[i]; node; node = next) {
	next = node->mLink;
	ymuint pos = hash_func(node->mType, node->mFaninArray);
	ymuint idx = pos % mHashSize;
	node->mLink = mHashTable[idx];
	mHashTable[idx] = node;
      }
    }
    delete [] old_table;
  }
  mNextLimit = static_cast<ymuint32>(mHashSize * 1.8);
}

// @brief LcPat2Node のハッシュ関数
ymuint
LcPat2Mgr::hash_func(ymuint type,
		     const vector<LcPat2Handle>& chd_array)
{
  ymuint ans = type;
  ymuint n = chd_array.size();
  for (ymuint i = 0; i < n; ++ i) {
    ans *= 7;
    LcPat2Node* node = chd_array[i].node();
    bool inv = chd_array[i].inv();
    ans += (node->id() * 2);
    if ( inv ) {
      ans += 1;
    }
  }
  return ans;
}

// @brief LcPat2Node のハッシュ関数
ymuint
LcPat2Mgr::hash_func(ymuint type,
		     LcPat2Handle chd_array[])
{
  ymuint ans = type;
  ymuint n = type >> 2;
  for (ymuint i = 0; i < n; ++ i) {
    ans *= 7;
    LcPat2Node* node = chd_array[i].node();
    bool inv = chd_array[i].inv();
    ans += (node->id() * 2);
    if ( inv ) {
      ans += 1;
    }
  }
  return ans;
}


BEGIN_NONAMESPACE

// @brief パタングラフを DFS でたどって内容を val_list に入れる．
// @param[in] node ノード
// @param[in] vmark 訪れたかどうかの情報を持つ配列
// @param[out] val_list ノードの情報を格納するリスト
// @return 最大入力番号+1を返す．
ymuint
dfs(LcPat2Node* node,
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
LcPat2Mgr::pat_node_list(ymuint id,
			vector<ymuint>& node_list) const
{
  LcPat2Handle root = pat_root(id);
  node_list.clear();
  node_list.reserve(node_num());
  vector<bool> vmark(node_num(), false);
  ymuint max_input = dfs(root.node(), vmark, node_list);
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
LcPat2Mgr::display(ostream& s) const
{
  s << "*** LcPat2Mgr BEGIN ***" << endl;

  s << "*** NODE SECTION ***" << endl;
  ymuint n = node_num();
  for (ymuint i = 0; i < n; ++ i) {
    LcPat2Node* node = this->node(i);
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
      ymuint ni = node->fanin_num();
      s << "( ";
      const char* comma = "";
      for (ymuint i = 0; i < ni; ++ i) {
	s << comma;
	comma = ", ";
	display_edge(s, node, i);
      }
      s << ")";
    }
    s << endl;
  }
  s << endl;

  s << "*** PATTERN SECTION ***" << endl;
  ymuint np = pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    LcPat2Handle root = pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    s << "Node#" << root.node()->id()
      << " --> Rep#" << rep_id(i)
      << endl;
  }
  s << "*** LcPat2Mgr END ***" << endl;
}

// @brief 枝の情報を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 親のノード
// @param[in] fanin_pos ファンイン番号
void
LcPat2Mgr::display_edge(ostream& s,
			LcPat2Node* node,
			ymuint fanin_pos)
{
  if ( node->fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node->fanin(fanin_pos)->id();
}

END_NAMESPACE_YM_CELL_LIBCOMP
