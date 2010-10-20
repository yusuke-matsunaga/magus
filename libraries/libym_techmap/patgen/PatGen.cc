
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

const ymuint n_pat3 = 2;
int pat3[n_pat3][5] = {
	{ -1, 0, -1, 1, 2 },
	{ -1, -1, 0, 1, 2 }
};

const ymuint n_pat4 = 5;
int pat4[n_pat4][7] = {
	{ -1, 0, -1, 1, -1, 2, 3 },
	{ -1, 0, -1, -1, 1, 2, 3 },
	{ -1, -1, 0, 1, -1, 2, 3 },
	{ -1, -1, 0, -1, 1, 2, 3 },
	{ -1, -1, -1, 0, 1, 2, 3 }
};

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
  mAlloc(sizeof(PgNode), 1024)

{
}

// @brief デストラクタ
PatGen::~PatGen()
{
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

// @brief パタングラフを生成する再帰関数
// @param[in] expr 元になる論理式
// @param[out] pg_list パタングラフのリスト
// @note pg_list の中身はこの PatGen のインスタンスが破壊されると
// 無効になる．
// @note expr は定数を含んではいけない．
void
PatGen::gen_pat(const LogExpr& expr,
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
      gen_pat(expr.child(i), input_pg_list[i]);
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
    mInputList.push_back(NULL);
  }
  PgNode* node = mInputList[id];
  if ( node == NULL ) {
    node = new_node();
    node->mType = (id << 2);
  }
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
  PgNode* node = new_node();
  node->mFanin[0] = l_node;
  node->mFanin[1] = r_node;
  bool oinv = false;
  if ( expr.is_and() ) {
    node->mType = 2U;
  }
  else if ( expr.is_or() ) {
    node->mType = 2U;
    l_inv = !l_inv;
    r_inv = !r_inv;
    oinv = true;
  }
  else if ( expr.is_xor() ) {
    node->mType = 3U;
    oinv = l_inv ^ r_inv;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
  if ( l_inv ) {
    node->mType |= 4U;
  }
  if ( r_inv ) {
    node->mType |= 8U;
  }
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

END_NAMESPACE_YM_TECHMAP_PATGEN
