
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
  vector<LcPatHandle> tmp_pat_list;
  pg_sub(expr, tmp_pat_list);

  // 重複チェック
  // 今は2乗オーダーのバカなアルゴリズムを使っている．
  for (vector<LcPatHandle>::iterator p = tmp_pat_list.begin();
       p != tmp_pat_list.end(); ++ p) {
    LcPatHandle pat1 = *p;
    bool found = false;
    ymuint n = mPatList.size();
    for (ymuint i = 0; i < n; ++ i) {
      if ( mRepList[i] != rep_id ) continue;
      LcPatHandle pat2 = mPatList[i];
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
    vector<vector<LcPatHandle> > input_pg_list(n);
    vector<pair<size_t, size_t> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      pg_sub(expr.child(i), input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }
    // ファンインのパタンの組み合わせを列挙するオブジェクト
    MultiCombiGen mcg(nk_array);
    vector<LcPatHandle> input(n);
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
	    LcPatHandle handle = make_node(expr, input[0], input[1]);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 3:
	  for (ymuint i = 0; i < n_pat3; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, input, pat3[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 4:
	  for (ymuint i = 0; i < n_pat4; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, input, pat4[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 5:
	  for (ymuint i = 0; i < n_pat5; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, input, pat5[i], pos);
	    add_pg_list(pg_list, handle);
	  }
	  break;

	case 6:
	  for (ymuint i = 0; i < n_pat6; ++ i) {
	    ymuint pos = 0;
	    LcPatHandle handle = make_bintree(expr, input, pat6[i], pos);
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
LcPatMgr::add_pg_list(vector<LcPatHandle>& pg_list,
		      LcPatHandle new_handle)
{
  for (vector<LcPatHandle>::iterator p = pg_list.begin();
       p != pg_list.end(); ++ p) {
    LcPatHandle h = *p;
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

// @relates LcPatNode
// @brief 同形か調べる．
// @param[in] node1, node2 根のノード
bool
LcPatMgr::check_isomorphic(const LcPatNode* node1,
			   const LcPatNode* node2)
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
LcPatNode*
LcPatMgr::make_input(ymuint id)
{
  while ( mInputList.size() <= id ) {
    LcPatNode* node = new_node();
    ymuint id1 = mInputList.size();
    node->set_input(id1);
    mInputList.push_back(node);
  }
  LcPatNode* node = mInputList[id];
  assert_cond( node != NULL, __FILE__, __LINE__);

  return node;
}

// @brief テンプレートにしたがって2分木を作る．
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

// @brief 論理式の種類に応じてノードを作る．
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


//////////////////////////////////////////////////////////////////////
// dump() 関係の関数
//////////////////////////////////////////////////////////////////////

// @brief 内容をバイナリダンプする．
// @param[in] s 出力先のストリーム
void
LcPatMgr::dump(BinO& bos) const
{
  // パタンノードの情報をダンプする．
  ymuint32 nn = node_num();
  bos << nn;
  for (ymuint i = 0; i < nn; ++ i) {
    LcPatNode* node = this->node(i);
    ymuint32 v = 0U;
    if ( node->is_input() ) {
      v = kCellPatInput | (node->input_id() << 2);
    }
    else if ( node->is_and() ) {
      v = kCellPatAnd;
    }
    else if ( node->is_xor() ) {
      v = kCellPatXor;
    }
    bos << v
	<< encode_edge(node, 0)
	<< encode_edge(node, 1);
  }

  vector<bool> vmark(nn);
  vector<ymuint> val_list;
  val_list.reserve(nn * 2);

  // パタンの情報をダンプする．
  ymuint32 np = pat_num();
  bos << np;
  for (ymuint i = 0; i < np; ++ i) {
    LcPatHandle root = pat_root(i);
    vmark.clear();
    vmark.resize(nn, false);
    val_list.clear();
    ymuint max_input = dump_dfs(root.node(), vmark, val_list);
    ymuint32 v = max_input << 1;
    if ( root.inv() ) {
      v |= 1U;
    }
    ymuint32 ne = val_list.size();
    bos << v
	<< ne;
    for (ymuint j = 0; j < ne; ++ j) {
      bos << static_cast<ymuint32>(val_list[j]);
    }
    bos << static_cast<ymuint32>(rep_id(i));
  }
}

// @brief 枝の情報をエンコードする．
// @param[in] node 親のノード
// @param[in] fanin_pos ファンイン番号
ymuint32
LcPatMgr::encode_edge(LcPatNode* node,
		      ymuint fanin_pos)
{
  ymuint32 v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  return v;
}

// @brief パタングラフを DFS でたどって内容を val_list に入れる．
// @param[in] node ノード
// @param[in] vmark 訪れたかどうかの情報を持つ配列
// @param[out] val_list ノードの情報を格納するリスト
// @return 最大入力番号+1を返す．
ymuint
LcPatMgr::dump_dfs(LcPatNode* node,
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
