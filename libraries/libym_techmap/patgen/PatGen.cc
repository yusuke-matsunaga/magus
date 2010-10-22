
/// @file libym_techmap/patgen/PatGen.cc
/// @brief PatGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGen.h"
#include "PgNode.h"
#include "ym_lexp/LogExpr.h"
#include "ym_utils/Generator.h"


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

BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

//////////////////////////////////////////////////////////////////////
// クラス PgNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PgNode::PgNode() :
  mType(0U)
{
  mFanin[0] = NULL;
  mFanin[1] = NULL;
}

// @brief デストラクタ
PgNode::~PgNode()
{
}


//////////////////////////////////////////////////////////////////////
// クラス PatGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PatGen::PatGen() :
  mAlloc(sizeof(PgNode), 1024),
  mHashTable(NULL),
  mHashSize(0)
{
  alloc_table(1024);
}

// @brief デストラクタ
PatGen::~PatGen()
{
  delete [] mHashTable;
}

// @brief 全ノード数を返す．
ymuint
PatGen::node_num() const
{
  return mNodeList.size();
}

// @brief ノードを返す．
// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
PgNode*
PatGen::node(ymuint pos) const
{
  return mNodeList[pos];
}

// @brief パタン数を返す．
ymuint
PatGen::pat_num() const
{
  return mPatList.size();
}

// @brief パタンの根のハンドルを返す．
// @param[in] id パタン番号 ( 0 <= id < pat_num() )
PgHandle
PatGen::pat_root(ymuint id) const
{
  return mPatList[id];
}


BEGIN_NONAMESPACE

// 同形チェックの再帰関数
bool
isom_recur(PgNode* node1,
	   PgNode* node2)
{
  if ( node1 == node2 ) {
    return true;
  }
  if ( node1->is_input() ) {
    return node2->is_input();
  }
  if ( node2->is_input() ) {
    return false;
  }
  if ( node1->is_and() ) {
    if ( !node2->is_and() ) {
      return false;
    }
  }
  else if ( node2->is_and() ) {
    return false;
  }
  if ( node1->fanin_inv(0) != node2->fanin_inv(0) ||
       node1->fanin_inv(1) != node2->fanin_inv(1) ) {
    return false;
  }
  if ( !isom_recur(node1->fanin(0), node2->fanin(0)) ) {
    return false;
  }
  if ( !isom_recur(node1->fanin(1), node2->fanin(1)) ) {
    return false;
  }
  return true;
}

END_NONAMESPACE

// @brief 2つのパタンが同形かどうか調べる．
// @param[in] pat1, pat2 パタン番号
// @retval true pat1 と pat2 は同形だった．
// @retval false pat1 と pat2 は同形ではなかった．
// @note ここでいう「同形」とは終端番号以外がおなじこと
bool
PatGen::check_isomorphic(ymuint pat1,
			 ymuint pat2) const
{
  if ( pat1 == pat2 ) {
    // 番号が等しければもちろん同形
    return true;
  }

  PgHandle h1 = pat_root(pat1);
  PgHandle h2 = pat_root(pat2);
  if ( h1.inv() != h2.inv() ) {
    return false;
  }

  // あとは実際に再帰して調べる．
  return isom_recur(h1.node(), h2.node());
}

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
// @param[out] pat_list パタン番号のリスト
// @note expr は定数を含んではいけない．
void
PatGen::operator()(const LogExpr& expr,
		   vector<ymuint>& pat_list)
{
  assert_cond( !expr.is_constant(), __FILE__, __LINE__);
  vector<PgHandle> pg_list;
  pg_sub(expr, pg_list);

  ymuint n = pg_list.size();
  mPatList.reserve(mPatList.size() + n);
  for (ymuint i = 0; i < n; ++ i) {
    ymuint id = mPatList.size();
    pat_list.push_back(id);
    mPatList.push_back(pg_list[i]);
  }
}

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
// @param[out] pg_list パタングラフのリスト
void
PatGen::pg_sub(const LogExpr& expr,
	       vector<PgHandle>& pg_list)
{
  if ( expr.is_literal() ) {
    PgNode* node = make_input(expr.varid());
    bool inv = expr.is_negaliteral();
    pg_list.push_back(PgHandle(node, inv));
  }
  else {
    ymuint n = expr.child_num();
    vector<vector<PgHandle> > input_pg_list(n);
    vector<pair<size_t, size_t> > nk_array(n);
    for (ymuint i = 0; i < n; ++ i) {
      pg_sub(expr.child(i), input_pg_list[i]);
      nk_array[i] = make_pair(input_pg_list[i].size(), 1);
    }
    MultiCombiGen mcg(nk_array);
    for (MultiCombiGen::iterator p = mcg.begin(); !p.is_end(); ++ p) {
      vector<PgHandle> input(n);
      for (ymuint i = 0; i < n; ++ i) {
	input[i] = input_pg_list[i][p(i, 0)];
      }
      switch ( n ) {
      case 2:
	{
	  PgHandle handle = make_node(expr, input[0], input[1]);
	  pg_list.push_back(handle);
	}
	break;

      case 3:
	for (ymuint i = 0; i < n_pat3; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_bintree(expr, input, pat3[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 4:
	for (ymuint i = 0; i < n_pat4; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_bintree(expr, input, pat4[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 5:
	for (ymuint i = 0; i < n_pat5; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_bintree(expr, input, pat5[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      case 6:
	for (ymuint i = 0; i < n_pat6; ++ i) {
	  ymuint pos = 0;
	  PgHandle handle = make_bintree(expr, input, pat6[i], pos);
	  pg_list.push_back(handle);
	}
	break;

      default:
	assert_not_reached(__FILE__, __LINE__);
	break;
      }
    }
  }
}

// @brief 入力ノードを作る．
// @param[in] id 入力番号
// @note 既にあるときはそれを返す．
PgNode*
PatGen::make_input(ymuint id)
{
  while ( mInputList.size() <= id ) {
    PgNode* node = new_node();
    ymuint id1 = mInputList.size();
    node->set_input(id1);
    mInputList.push_back(node);
  }
  PgNode* node = mInputList[id];
  assert_cond( node != NULL, __FILE__, __LINE__);

  return node;
}

// @brief テンプレートにしたがって2分木を作る．
PgHandle
PatGen::make_bintree(const LogExpr& expr,
		     const vector<PgHandle>& input,
		     int pat[],
		     ymuint& pos)
{
  int p = pat[pos];
  ++ pos;
  if ( p == -1 ) {
    // 演算ノード
    PgHandle l_handle = make_bintree(expr, input, pat, pos);
    PgHandle r_handle = make_bintree(expr, input, pat, pos);
    return make_node(expr, l_handle, r_handle);
  }
  else {
    // 終端ノード
    ymuint id = static_cast<ymuint>(p);
    return input[id];
  }
}

// @brief 論理式の種類に応じてノードを作る．
PgHandle
PatGen::make_node(const LogExpr& expr,
		  PgHandle l_handle,
		  PgHandle r_handle)
{
  PgNode* l_node = l_handle.node();
  PgNode* r_node = r_handle.node();
  bool l_inv = l_handle.inv();
  bool r_inv = r_handle.inv();

  bool oinv = false;
  ymuint32 type = 0U;
  if ( expr.is_and() ) {
    type = PgNode::kAnd;
  }
  else if ( expr.is_or() ) {
    type = PgNode::kAnd;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = PgNode::kXor;
    oinv = l_inv ^ r_inv;
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
  for (PgNode* node = mHashTable[idx]; node; node = node->mLink) {
    if ( node->mType == type &&
	 node->mFanin[0] == l_node &&
	 node->mFanin[1] == r_node ) {
      // おなじノードがあった．
      return PgHandle(node, oinv);
    }
  }

  // 新しいノードを作る．
  PgNode* node = new_node();
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

  return PgHandle(node, oinv);
}

// @brief ノードを作る．
PgNode*
PatGen::new_node()
{
  void* p = mAlloc.get_memory(sizeof(PgNode));
  PgNode* node = new (p) PgNode();
  node->mId = mNodeList.size();
  mNodeList.push_back(node);
  return node;
}

// @brief ハッシュ表を確保する．
void
PatGen::alloc_table(ymuint req_size)
{
  PgNode** old_table = mHashTable;
  ymuint old_size = mHashSize;

  if ( mHashSize == 0 ) {
    mHashSize = 1024;
  }
  while ( mHashSize < req_size ) {
    mHashSize <<= 1;
  }
  mHashTable = new PgNode*[mHashSize];
  for (ymuint i = 0; i < mHashSize; ++ i) {
    mHashTable[i] = NULL;
  }
  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      PgNode* next = NULL;
      for (PgNode* node = old_table[i]; node; node = next) {
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

// @brief PgNode のハッシュ関数
ymuint
PatGen::hash_func(ymuint type,
		  PgNode* l_node,
		  PgNode* r_node)
{
  ympuint l = reinterpret_cast<ympuint>(l_node);
  ympuint r = reinterpret_cast<ympuint>(r_node);

  return type + l * 5 + r;
}

END_NAMESPACE_YM_TECHMAP_PATGEN
