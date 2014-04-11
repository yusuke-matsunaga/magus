
/// @file IguGen.cc
/// @brief IguGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IguGen.h"
#include "PhfGraph.h"
#include "PhfNode.h"
#include "PhfEdge.h"
#include "InputFunc.h"
#include "RegVect.h"
#include "FuncVect.h"


BEGIN_NAMESPACE_YM_IGF

// @brief コンストラクタ
IguGen::IguGen()
{
}

// @brief デストラクタ
IguGen::~IguGen()
{
}

// @brief 登録ベクタを読み込む．
// @param[in] s 読み込み元のストリーム演算子
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
IguGen::read_data(istream& s)
{
  return mRvMgr.read_data(s);
}

// @brief ベクタのサイズを得る．
//
// ベクタのサイズとはベクタのビット長
ymuint
IguGen::vect_size() const
{
  return mRvMgr.vect_size();
}

// @brief ベクタのリストを得る．
const vector<const RegVect*>&
IguGen::vect_list() const
{
  return mRvMgr.vect_list();
}

// @brief インデックスのサイズを得る．
//
// インデックスのサイズとはインデックスを2進符号化するのに
// 必要なビット数 = ceil(log (k + 1)): k はベクタ数
ymuint
IguGen::index_size() const
{
  return mRvMgr.index_size();
}

// @brief conflict free 分割法で構成する．
// @param[in] h_funcs 入力ハッシュ関数のリスト
// @param[out] map_list IGUごとのベクタ番号のリスト
// @retval true 構成できた．
// @retval false 構成が失敗した．
bool
IguGen::cfp(const vector<InputFunc*>& h_funcs,
	    vector<vector<ymuint> >& map_list) const
{
  vector<const FuncVect*> func_list;
  convert(h_funcs, func_list);

  vector<ymuint> block_map;
  PhfGraph pg(func_list);
  free_func_list(func_list);

  bool stat = pg.cf_partition(block_map);
  if ( stat ) {
    ymuint m = func_list.size();
    const vector<const RegVect*>& vlist = mRvMgr.vect_list();
    ymuint nv = vlist.size();
    ymuint p = h_funcs[0]->output_width();
    ymuint exp_p = 1U << p;
    map_list.clear();
    for (ymuint i = 0; i < m; ++ i) {
      map_list.push_back(vector<ymuint>(exp_p));
    }
    for (ymuint i = 0; i < nv; ++ i) {
      const RegVect* rv = vlist[i];
      ymuint b = block_map[i];
      map_list[b][h_funcs[b]->eval(rv)] = rv->index();
    }
    return true;
  }
  return false;
}

// @brief naive parallel 法で構成する．
// @param[in] h_funcs 入力ハッシュ関数のリスト
// @param[out] map_list IGUごとのベクタ番号のリスト
// @retval true 構成できた．
// @retval false 構成が失敗した．
bool
IguGen::naive(const vector<InputFunc*>& h_funcs,
	      vector<vector<ymuint> >& map_list) const
{
  const vector<const RegVect*>& vlist = mRvMgr.vect_list();
  ymuint nv = vlist.size();
  ymuint p = h_funcs[0]->output_width();
  ymuint exp_p = 1U << p;
  ymuint m = h_funcs.size();

  vector<vector<const RegVect*> > vect_list;
  vect_list.reserve(m);
  for (ymuint i = 0; i < m; ++ i) {
    vect_list.push_back(vector<const RegVect*>(exp_p));
  }
  for (ymuint i = 0; i < nv; ++ i) {
    const RegVect* rv = vlist[i];
    bool found = false;
    for (ymuint b = 0; b < m; ++ b) {
      ymuint idx = h_funcs[b]->eval(rv);
      if ( vect_list[b][idx] == NULL ) {
	vect_list[b][idx] = rv;
	found = true;
	break;
      }
    }
    if ( !found ) {
      return false;
    }
  }
  map_list.clear();
  for (ymuint i = 0; i < m; ++ i) {
    map_list.push_back(vector<ymuint>(exp_p));
    for (ymuint j = 0; j < exp_p; ++ j) {
      const RegVect* rv = vect_list[i][j];
      if ( rv != NULL ) {
	map_list[i][j] = rv->index();
      }
    }
  }
}

// @brief minimum perfect hash 法で構成する．
// @param[in] h_funcs 入力ハッシュ関数のリスト
// @param[out] map_list IGUごとの G 関数のリスト
// @retval true 構成できた．
// @retval false 構成が失敗した．
bool
IguGen::mphf(const vector<InputFunc*>& h_funcs,
	     vector<vector<ymuint> >& map_list) const
{
  vector<const FuncVect*> func_list;
  convert(h_funcs, func_list);

  PhfGraph pg(func_list);

  if ( !pg.simple_check() ) {
    free_func_list(func_list);
    return false;
  }

  vector<PhfEdge*> edge_list;
  if ( !pg.acyclic_check(edge_list) ) {
    free_func_list(func_list);
    return false;
  }

  ymuint nf = h_funcs.size();
  ymuint nv = func_list[0]->max_val();
  map_list.clear();
  map_list.resize(nf, vector<ymuint32>(nv, 0U));

  ymuint ne = edge_list.size();
  for (ymuint i = 0; i < ne; ++ i) {
    PhfEdge* edge = edge_list[i];
    for (ymuint j = 0; j < nf; ++ j) {
      PhfNode* node1 = edge->node(j);
      if ( !node1->is_assigned() ) {
	ymuint32 val = edge->val();
	for (ymuint k = 0; k < nf; ++ k) {
	  if ( k != j ) {
	    PhfNode* node2 = edge->node(k);
	    val ^= node2->val();
	  }
	}
	node1->set_val(val);
	vector<ymuint32>& g1 = map_list[j];
	g1[node1->pat()] = val;
      }
    }
  }

  return true;
}

// @brief InputFunc から FuncVect に変換する．
void
IguGen::convert(const vector<InputFunc*>& h_funcs,
		vector<const FuncVect*>& func_list) const
{
  const vector<const RegVect*>& vlist = mRvMgr.vect_list();
  ymuint nv = vlist.size();
  ymuint m = h_funcs.size();
  ymuint p = h_funcs[0]->output_width();
  ymuint exp_p = 1U << p;

  func_list.clear();
  func_list.resize(m);
  for (ymuint i = 0; i < m; ++ i) {
    InputFunc* f = h_funcs[i];
    FuncVect* fv = new FuncVect(nv, exp_p);
    for (ymuint v = 0; v < nv; ++ v) {
      const RegVect* rv = vlist[v];
      fv->set_val(v, f->eval(rv));
    }
    func_list[i] = fv;
  }
}

// @brief func_list を削除する．
void
IguGen::free_func_list(const vector<const FuncVect*>& func_list) const
{
  ymuint n = func_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    delete func_list[i];
  }
}

END_NAMESPACE_YM_IGF
