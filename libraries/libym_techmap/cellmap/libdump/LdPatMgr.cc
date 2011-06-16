
/// @file libym_techmap/cellmap/libdump/LdPatMgr.cc
/// @brief LdPatMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdPatMgr.h"
#include "LdPatNode.h"
#include "LdPatHandle.h"
#include "../PatMgr.h"
#include "ym_logic/LogExpr.h"
#include "ym_utils/Generator.h"
#include "ym_utils/BinIO.h"


BEGIN_NONAMESPACE

// 3入力の分解パタン
const ymuint n_pat3 = 2;
int pat3[n_pat3][5] = {
	{ -1, 0, -1, 1, 2 },
	{ -1, -1, 0, 1, 2 }
};

// 4入力の分解パタン
const ymuint n_pat4 = 5;
int pat4[n_pat4][7] = {
	{ -1, 0, -1, 1, -1, 2, 3 },
	{ -1, 0, -1, -1, 1, 2, 3 },
	{ -1, -1, 0, 1, -1, 2, 3 },
	{ -1, -1, 0, -1, 1, 2, 3 },
	{ -1, -1, -1, 0, 1, 2, 3 }
};

// 5入力の分解パタン
const ymuint n_pat5 = 14;
int pat5[n_pat5][9] = {
	{ -1, 0, -1, 1, -1, 2, -1, 3, 4 },
	{ -1, 0, -1, 1, -1, -1, 2, 3, 4 },
	{ -1, 0, -1, -1, 1, 2, -1, 3, 4 },
	{ -1, 0, -1, -1, 1, -1, 2, 3, 4 },
	{ -1, 0, -1, -1, -1, 1, 2, 3, 4 },
	{ -1, -1, 0, 1, -1, 2, -1, 3, 4 },
	{ -1, -1, 0, 1, -1, -1, 2, 3, 4 },
	{ -1, -1, 0, -1, 1, 2, -1, 3, 4 },
	{ -1, -1, -1, 0, 1, 2, -1, 3, 4 },
	{ -1, -1, 0, -1, 1, -1, 2, 3, 4 },
	{ -1, -1, 0, -1, -1, 1, 2, 3, 4 },
	{ -1, -1, -1, 0, 1, -1, 2, 3, 4 },
	{ -1, -1, -1, 0, -1, 1, 2, 3, 4 },
	{ -1, -1, -1, -1, 0, 1, 2, 3, 4 }
};

// 6入力の分解パタン
const ymuint n_pat6 = 42;
int pat6[n_pat6][11] = {
	{ -1, 0, -1, 1, -1, 2, -1, 3, -1, 4, 5 },
	{ -1, 0, -1, 1, -1, 2, -1, -1, 3, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, 1, -1, -1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, 0, -1, -1, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, 0, 1, -1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, 1, -1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, 2, -1, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, 1, 2, -1, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, 2, 3, -1, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, 0, -1, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, 1, -1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, 2, -1, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, 0, -1, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, 1, -1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, 0, -1, 1, 2, 3, 4, 5 },
	{ -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5 }
};

END_NONAMESPACE


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdPatMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LdPatMgr::LdPatMgr() :
  mAlloc(sizeof(LdPatNode), 1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
LdPatMgr::~LdPatMgr()
{
  delete [] mHashTable;
}

// @brief 初期化する．
void
LdPatMgr::init()
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
LdPatMgr::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
LdPatNode*
LdPatMgr::node(ymuint pos) const
{
  return mNodeList[pos];
}

// @brief パタン数を返す．
ymuint
LdPatMgr::pat_num() const
{
  return mPatList.size();
}

// @brief パタンの根のハンドルを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
LdPatHandle
LdPatMgr::pat_root(ymuint id) const
{
  assert_cond( id < pat_num(), __FILE__, __LINE__);
  return mPatList[id];
}

// @brief パタンの属している代表関数番号を返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
ymuint
LdPatMgr::rep_id(ymuint id) const
{
  return mRepList[id];
}

// @brief 論理式から生成されるパタンを登録する．
// @param[in] expr パタンの元となる論理式
// @param[in] rep_id このパタンが属する代表関数番号
void
LdPatMgr::reg_pat(const LogExpr& expr,
		  ymuint rep_id)
{
  vector<LdPatHandle> tmp_pat_list;
  pg_sub(expr, tmp_pat_list);

  // 重複チェック
  // 今は2乗オーダーのバカなアルゴリズムを使っている．
  for (vector<LdPatHandle>::iterator p = tmp_pat_list.begin();
       p != tmp_pat_list.end(); ++ p) {
    LdPatHandle pat1 = *p;
    bool found = false;
    ymuint n = mPatList.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( mRepList[i] != rep_id ) continue;
      LdPatHandle pat2 = mPatList[i];
      if ( pat1.inv() == pat2.inv() &&
	   check_isomorphic(pat1.node(), pat2.node()) ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      // pat1 を登録する．
      mPatList.push_back(pat1);
      mRepList.push_back(rep_id);
      pat1.node()->set_locked();
    }
  }
  sweep();
}

// @brief 使われていないパタンとノードを削除してID番号を詰める．
void
LdPatMgr::sweep()
{
  // lock されていないノードの削除
  ymuint wpos = 0;
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    LdPatNode* node = mNodeList[i];
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
	LdPatNode** p_prev = &mHashTable[idx];
	for ( ; ; ) {
	  LdPatNode* node1 = *p_prev;
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
    LdPatNode* node = mNodeList[i];
    if ( !node->is_input() ) continue;
    ymuint iid = node->input_id();
    if ( iid == i ) continue;
    LdPatNode* node1 = mNodeList[iid];
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
LdPatMgr::pg_sub(const LogExpr& expr,
		 vector<LdPatHandle>& pg_list)
{
  if ( expr.is_literal() ) {
    LdPatNode* node = make_input(expr.varid());
    bool inv = expr.is_negaliteral();
    pg_list.push_back(LdPatHandle(node, inv));
  }
  else {
    ymuint n = expr.child_num();
    vector<vector<LdPatHandle> > input_pg_list(n);
    vector<pair<size_t, size_t> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      pg_sub(expr.child(i), input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }
    // ファンインのパタンの組み合わせを列挙するオブジェクト
    MultiCombiGen mcg(nk_array);
    vector<LdPatHandle> input(n);
    for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
      // 入力の順列を列挙するオブジェクト
      PermGen pg(n, n);
      for (PermGen::iterator q = pg.begin(); !q.is_end(); ++ q) {
	for (ymuint i = 0; i < n; ++ i) {
	  ymuint j = q(i);
	  input[j] = input_pg_list[i][p(i, 0)];
	}
	switch ( n ) {
	case 2:
	  {
	    LdPatHandle handle = make_node(expr, input[0], input[1]);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 3:
	  for (ymuint i = 0; i < n_pat3; ++ i) {
	    ymuint pos = 0;
	    LdPatHandle handle = make_bintree(expr, input, pat3[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 4:
	  for (ymuint i = 0; i < n_pat4; ++ i) {
	    ymuint pos = 0;
	    LdPatHandle handle = make_bintree(expr, input, pat4[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 5:
	  for (ymuint i = 0; i < n_pat5; ++ i) {
	    ymuint pos = 0;
	    LdPatHandle handle = make_bintree(expr, input, pat5[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 6:
	  for (ymuint i = 0; i < n_pat6; ++ i) {
	    ymuint pos = 0;
	    LdPatHandle handle = make_bintree(expr, input, pat6[i], pos);
	    add_pg_list(pg_list, handle);
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
LdPatMgr::add_pg_list(vector<LdPatHandle>& pg_list,
		      LdPatHandle new_handle)
{
  for (vector<LdPatHandle>::iterator p = pg_list.begin();
       p != pg_list.end(); ++ p) {
    LdPatHandle h = *p;
    // 根に反転属性はないと思ったけど念のため
    if ( h.inv() != new_handle.inv() ) continue;
    if ( check_isomorphic(h.node(), new_handle.node()) ) {
      // 同形のパタンが存在した．
      return;
    }
  }
  // なかったので追加する．
  pg_list.push_back(new_handle);
}

// @relates LdPatNode
// @brief 同形か調べる．
// @param[in] node1, node2 根のノード
bool
LdPatMgr::check_isomorphic(const LdPatNode* node1,
			   const LdPatNode* node2)
{
  if ( node1 == node2 ) {
    return true;
  }
  if ( node1->is_input() ) {
    // node1 が入力で node2 が入力でなかったら異なる．
    return node2->is_input();
  }
  if ( node2->is_input() ) {
    // ここに来ているということは node1 が入力でない．
    return false;
  }
  if ( node1->is_and() ) {
    if ( !node2->is_and() ) {
      // node1 が AND で node2 が XOR
      return false;
    }
  }
  else if ( node2->is_and() ) {
    // ここに来ているということは node1 は XOR
    return false;
  }
  if ( node1->fanin_inv(0) != node2->fanin_inv(0) ||
       node1->fanin_inv(1) != node2->fanin_inv(1) ) {
    // ファンイン極性が異なる．
    return false;
  }
  if ( !check_isomorphic(node1->fanin(0), node2->fanin(0)) ) {
    return false;
  }
  if ( !check_isomorphic(node1->fanin(1), node2->fanin(1)) ) {
    return false;
  }
  return true;
}

// @brief 入力ノードを作る．
// @param[in] id 入力番号
// @note 既にあるときはそれを返す．
LdPatNode*
LdPatMgr::make_input(ymuint id)
{
  while ( mInputList.size() <= id ) {
    LdPatNode* node = new_node();
    ymuint id1 = mInputList.size();
    node->set_input(id1);
    mInputList.push_back(node);
  }
  LdPatNode* node = mInputList[id];
  assert_cond( node != NULL, __FILE__, __LINE__);

  return node;
}

// @brief テンプレートにしたがって2分木を作る．
LdPatHandle
LdPatMgr::make_bintree(const LogExpr& expr,
		       const vector<LdPatHandle>& input,
		       int pat[],
		       ymuint& pos)
{
  int p = pat[pos];
  ++ pos;
  if ( p == -1 ) {
    // 演算ノード
    LdPatHandle l_handle = make_bintree(expr, input, pat, pos);
    LdPatHandle r_handle = make_bintree(expr, input, pat, pos);
    return make_node(expr, l_handle, r_handle);
  }
  else {
    // 終端ノード
    ymuint id = static_cast<ymuint>(p);
    return input[id];
  }
}

// @brief 論理式の種類に応じてノードを作る．
LdPatHandle
LdPatMgr::make_node(const LogExpr& expr,
		    LdPatHandle l_handle,
		    LdPatHandle r_handle)
{
  LdPatNode* l_node = l_handle.node();
  LdPatNode* r_node = r_handle.node();
  bool l_inv = l_handle.inv();
  bool r_inv = r_handle.inv();

  bool oinv = false;
  ymuint32 type = 0U;
  if ( expr.is_and() ) {
    type = LdPatNode::kAnd;
  }
  else if ( expr.is_or() ) {
    type = LdPatNode::kAnd;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = LdPatNode::kXor;
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
  for (LdPatNode* node = mHashTable[idx]; node; node = node->mLink) {
    if ( node->mType == type &&
	 node->mFanin[0] == l_node &&
	 node->mFanin[1] == r_node ) {
      // おなじノードがあった．
      return LdPatHandle(node, oinv);
    }
  }

  // 新しいノードを作る．
  LdPatNode* node = new_node();
  node->mType = type;
  node->mFanin[0] = l_node;
  node->mFanin[1] = r_node;

  // ハッシュ表に登録する．
  if ( node_num() >= mNextLimit ) {
    alloc_table(mHashSize * 2);
    // サイズが変わったのでインデックスを再計算する．
    idx = pos % mHashSize;
  }
  node->mLink = mHashTable[idx];
  mHashTable[idx] = node;

  return LdPatHandle(node, oinv);
}

// @brief ノードを作る．
LdPatNode*
LdPatMgr::new_node()
{
  void* p = mAlloc.get_memory(sizeof(LdPatNode));
  LdPatNode* node = new (p) LdPatNode();
  node->mId = mNodeList.size();
  mNodeList.push_back(node);
  return node;
}

// @brief ノードを削除する．
void
LdPatMgr::delete_node(LdPatNode* node)
{
  node->~LdPatNode();
  mAlloc.put_memory(sizeof(LdPatNode), static_cast<void*>(node));
}

// @brief ハッシュ表を確保する．
void
LdPatMgr::alloc_table(ymuint req_size)
{
  LdPatNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new LdPatNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      LdPatNode* next = NULL;
      for (LdPatNode* node = old_table[i]; node; node = next) {
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

// @brief LdPatNode のハッシュ関数
ymuint
LdPatMgr::hash_func(ymuint type,
		    LdPatNode* l_node,
		    LdPatNode* r_node)
{
  ympuint l = reinterpret_cast<ympuint>(l_node);
  ympuint r = reinterpret_cast<ympuint>(r_node);

  return type + l * 5 + r;
}


//////////////////////////////////////////////////////////////////////
// dump() 奸計の関数
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
LdPatMgr::dump(ostream& s) const
{
  // パタンノードの情報をダンプする．
  ymuint nn = node_num();
  BinIO::write_32(s, nn);
  for (ymuint i = 0; i < nn; ++ i) {
    LdPatNode* node = this->node(i);
    ymuint v = 0U;
    if ( node->is_input() ) {
      v = PatMgr::kInput | (node->input_id() << 2);
    }
    else if ( node->is_and() ) {
      v = PatMgr::kAnd;
    }
    else if ( node->is_xor() ) {
      v = PatMgr::kXor;
    }
    BinIO::write_32(s, v);
    dump_edge(s, node, 0);
    dump_edge(s, node, 1);
  }

  vector<bool> vmark(nn);
  vector<ymuint> val_list;
  val_list.reserve(nn * 2);

  // パタンの情報をダンプする．
  ymuint np = pat_num();
  BinIO::write_32(s, np);
  for (ymuint i = 0; i < np; ++ i) {
    LdPatHandle root = pat_root(i);
    vmark.clear();
    vmark.resize(nn, false);
    val_list.clear();
    ymuint max_input = dump_dfs(root.node(), vmark, val_list);
    ymuint v = max_input << 1;
    if ( root.inv() ) {
      v |= 1U;
    }
    BinIO::write_32(s, v);
    ymuint ne = val_list.size();
    BinIO::write_32(s, ne);
    for (ymuint j = 0; j < ne; ++ j) {
      BinIO::write_32(s, val_list[j]);
    }
    BinIO::write_32(s, rep_id(i));
  }
}

// @brief 枝の情報をダンプする．
// @param[in] s 出力先のストリーム
// @param[in] node 親のノード
// @param[in] fanin_pos ファンイン番号
void
LdPatMgr::dump_edge(ostream& s,
		    LdPatNode* node,
		    ymuint fanin_pos)
{
  ymuint v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  BinIO::write_32(s, v);
}

// @brief パタングラフを DFS でたどって内容を val_list に入れる．
// @param[in] node ノード
// @param[in] vmark 訪れたかどうかの情報を持つ配列
// @param[out] val_list ノードの情報を格納するリスト
// @return 最大入力番号+1を返す．
ymuint
LdPatMgr::dump_dfs(LdPatNode* node,
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
  ymuint id = dump_dfs(node->fanin(0), vmark, val_list);
  val_list.push_back(node->id() * 2 + 1);
  ymuint id1 = dump_dfs(node->fanin(1), vmark, val_list);
  if ( id < id1 ) {
    id = id1;
  }
  return id;
}


//////////////////////////////////////////////////////////////////////
// display() 関係の関数
//////////////////////////////////////////////////////////////////////

// @brief 内容を出力する．(デバッグ用)
// @param[in] s 出力先のストリーム
void
LdPatMgr::display(ostream& s) const
{
  s << "*** LdPatMgr BEGIN ***" << endl;

  s << "*** NODE SECTION ***" << endl;
  ymuint n = node_num();
  for (ymuint i = 0; i < n; ++ i) {
    LdPatNode* node = this->node(i);
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
    LdPatHandle root = pat_root(i);
    s << "Pat#" << i << ": ";
    if ( root.inv() ) {
      s << "~";
    }
    s << "Node#" << root.node()->id()
      << " --> Rep#" << rep_id(i)
      << endl;
  }
  s << "*** LdPatMgr END ***" << endl;
}

// @brief 枝の情報を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 親のノード
// @param[in] fanin_pos ファンイン番号
void
LdPatMgr::display_edge(ostream& s,
		       LdPatNode* node,
		       ymuint fanin_pos)
{
  if ( node->fanin_inv(fanin_pos) ) {
    s << "~";
  }
  s << "Node#" << node->fanin(fanin_pos)->id();
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP
