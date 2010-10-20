
/// @file libym_techmap/patgen/PatGen.cc
/// @brief PatGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "PatGen.h"
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

// @brief 入力数を返す．
// @note ただし使われていない入力もありうる．
ymuint
PatGen::input_num() const
{
  return mInputList.size();
}

// @brief 入力ノードを返す．
// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
PgNode*
PatGen::input_node(ymuint pos) const
{
  return mInputList[pos];
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


BEGIN_NONAMESPACE

void
display_edge(ostream& s,
	     PgNode* node,
	     ymuint fanin_pos)
{
  s << "Edge#" << node->id() * 2 + fanin_pos<< ": ";
  if ( node->is_input() ) {
    s << "---";
  }
  else {
    s << "Node#" << node->fanin(fanin_pos)->id()
      << " -> Node#" << node->id()
      << "(" << fanin_pos << ")";
    if ( node->fanin_inv(fanin_pos) ) {
      s << "[inv]";
    }
  }
  s << endl;
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
PatGen::display_graph(ostream& s) const
{
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    PgNode* node = this->node(i);
    s << "Node#" << node->id() << ": ";
    if ( node->is_input() ) {
      s << "Input#" << node->input_id();
    }
    else if ( node->is_and() ) {
      s << "And";
    }
    else if ( node->is_xor() ) {
      s << "Xor";
    }
    s << endl;
  }
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    PgNode* node = this->node(i);
    display_edge(s, node, 0);
    display_edge(s, node, 1);
  }
}


BEGIN_NONAMESPACE

void
display_dfs(ostream& s,
	    PgNode* node,
	    vector<bool>& vmark)
{
  if ( node->is_input() || vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  s << " Edge#" << node->id() * 2;
  display_dfs(s, node->fanin(0), vmark);
  s << " Edge#" << node->id() * 2 + 1;
  display_dfs(s, node->fanin(1), vmark);
}

END_NONAMESPACE

// @brief エッジリストの内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] root 根のハンドル
void
PatGen::display_edgelist(ostream& s,
			 PgHandle root) const
{
  ymuint n = mNodeList.size();
  vector<bool> vmark(n, false);
  if ( root.inv() ) {
    s << "[inv]";
  }
  display_dfs(s, root.node(), vmark);
  s << endl;
}


BEGIN_NONAMESPACE

void
dump_edge(ostream& s,
	  PgNode* node,
	  ymuint fanin_pos)
{
  ymuint v = 0U;
  if ( !node->is_input() ) {
    v = node->fanin(fanin_pos)->id() * 2;
    if ( node->fanin_inv(fanin_pos) ) {
      v |= 1U;
    }
  }
  s << v << endl;
}

END_NONAMESPACE

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
PatGen::dump_graph(ostream& s) const
{
  ymuint v;
  v = mNodeList.size();
  s << v << endl;
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    PgNode* node = this->node(i);
    if ( node->is_input() ) {
      v = 0U;
    }
    else if ( node->is_and() ) {
      v = 2U;
    }
    else if ( node->is_xor() ) {
      v = 3U;
    }
    s << v << endl;
  }
  for (ymuint i = 0; i < mNodeList.size(); ++ i) {
    PgNode* node = this->node(i);
    dump_edge(s, node, 0);
    dump_edge(s, node, 1);
  }
}


BEGIN_NONAMESPACE

void
dump_dfs(ostream& s,
	 PgNode* node,
	 vector<bool>& vmark,
	 vector<ymuint>& val_list)
{
  if ( node->is_input() || vmark[node->id()] ) {
    return;
  }
  vmark[node->id()] = true;
  val_list.push_back(node->id() * 2);
  dump_dfs(s, node->fanin(0), vmark, val_list);
  val_list.push_back(node->id() * 2 + 1);
  dump_dfs(s, node->fanin(1), vmark, val_list);
}

END_NONAMESPACE

// @brief エッジリストをダンプする．
// @param[in] s 出力先のストリーム
// @param[in] root 根のハンドル
// @note 内容はエッジ番号のリスト
// @note ただし最初の根のノード番号×2 + 反転属性
void
PatGen::dump_edgelist(ostream& s,
		      PgHandle root) const
{
  ymuint n = mNodeList.size();
  vector<bool> vmark(n, false);
  vector<ymuint> val_list;
  val_list.reserve(n * 2);
  dump_dfs(s, root.node(), vmark, val_list);
  ymuint l = val_list.size() * 2;
  if ( root.inv() ) {
    l |= 1U;
  }
  s << l << endl;
  for (ymuint i = 0; i < l; ++ i) {
    s << val_list[i] << endl;
  }
}

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
// @param[out] pg_list パタングラフのリスト
// @note pg_list の中身はこの PatGen のインスタンスが破壊されると
// 無効になる．
// @note expr は定数を含んではいけない．
void
PatGen::operator()(const LogExpr& expr,
		   vector<PgHandle>& pg_list)
{
  assert_cond( !expr.is_constant(), __FILE__, __LINE__);
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
      operator()(expr.child(i), input_pg_list[i]);
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
    node->mType = (id1 << 2);
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
    type = 2U;
  }
  else if ( expr.is_or() ) {
    type = 2U;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    type = 3U;
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
