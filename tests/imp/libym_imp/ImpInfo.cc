
/// @file ImpInfo.cc
/// @brief ImpInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpInfo.h"
#include "ImpMgr.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM_NETWORKS

BEGIN_NONAMESPACE

inline
Literal
to_literal(ymuint id,
	   ymuint val)
{
  return Literal(VarId(id), (val == 0) ? kPolNega : kPolPosi);
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ImpInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
ImpInfo::ImpInfo()
{
  mArraySize = 0;
  mArray = NULL;
}

// @brief デストラクタ
ImpInfo::~ImpInfo()
{
  delete [] mArray;
}

// @brief 含意情報のリストを取り出す．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
const vector<ImpVal>&
ImpInfo::get(ymuint src_id,
	     ymuint src_val) const
{
  return mArray[src_id * 2 + src_val];
}

// @brief 含意の総数を得る．
ymuint
ImpInfo::size(const ImpMgr& imp_mgr) const
{
  ymuint n = 0;
  for (ymuint i = 0; i < mArraySize; ++ i) {
    ymuint src_id = i / 2;
    if ( imp_mgr.is_const(src_id) ) {
      continue;
    }
    const vector<ImpVal>& imp_list = mArray[i];
    for (vector<ImpVal>::const_iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ymuint dst_id = p->id();
      if ( !imp_mgr.is_const(dst_id) ) {
	++ n;
      }
    }
  }
  return n;
}

// @brief 実際に保持している含意の総数を返す．
ymuint
ImpInfo::imp_num() const
{
  ymuint n = 0;
  for (ymuint i = 0; i < mArraySize; ++ i) {
    n += mArray[i].size();
  }
  return n;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
ImpInfo::print(ostream& s) const
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    const vector<ImpVal>& imp_list = mArray[i];
    if ( imp_list.empty() ) continue;
    ymuint src_id = i / 2;
    ymuint src_val = i % 2;
    s << "Node#" << src_id << ": " << src_val << endl;
    for (vector<ImpVal>::const_iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ymuint dst_id = p->id();
      ymuint dst_val = p->val();
      s << "   => Node#" << dst_id << ": " << dst_val << endl;
    }
    s << endl;
  }
}

// @brief 統計情報を出力する．
void
ImpInfo::print_stats(ostream& s) const
{
  s << "Total " << imp_num() << " implications ("
    << static_cast<double>(imp_num()) / (mArraySize * mArraySize) * 100
    << "%)" << endl;
}

// @brief 内容をクリアする．
void
ImpInfo::clear()
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    mArray[i].clear();
  }
  delete [] mArray;
  mArray = NULL;
}

// @brief サイズを設定する．
// @param[in] max_id ID番号の最大値
void
ImpInfo::set_size(ymuint max_id)
{
  clear();

  mArraySize = max_id * 2;
  mArray = new vector<ImpVal>[mArraySize];
}

// @brief 内容をセットする．
void
ImpInfo::set(vector<vector<ImpVal> >& imp_list_array)
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    vector<ImpVal>& imp_list = imp_list_array[i];
    sort(imp_list.begin(), imp_list.end());
    vector<ImpVal>::iterator p = unique(imp_list.begin(), imp_list.end());
    imp_list.erase(p, imp_list.end());
    mArray[i] = imp_list;
  }
}

// @brief 内容をセットする．
void
ImpInfo::set(vector<vector<ImpVal> >& imp_list_array,
	     const ImpInfo& src)
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    vector<ImpVal>& imp_list = imp_list_array[i];
    sort(imp_list.begin(), imp_list.end());
    vector<ImpVal>::iterator p = unique(imp_list.begin(), imp_list.end());
    imp_list.erase(p, imp_list.end());
    const vector<ImpVal>& src_list = src.mArray[i];
    mArray[i].clear();
    vector<ImpVal>::const_iterator p1 = imp_list.begin();
    vector<ImpVal>::const_iterator e1 = imp_list.end();
    vector<ImpVal>::const_iterator p2 = src_list.begin();
    vector<ImpVal>::const_iterator e2 = src_list.end();
    while ( p1 != e1 && p2 != e2 ) {
      const ImpVal& val1 = *p1;
      const ImpVal& val2 = *p2;
      if ( val1 < val2 ) {
	mArray[i].push_back(val1);
	++ p1;
      }
      else if ( val1 == val2 ) {
	++ p1;
	++ p2;
      }
      else { // val1 > val2
	++ p2;
      }
    }
    for ( ; p1 != e1; ++ p1) {
      mArray[i].push_back(*p1);
    }
  }
}

// 検証する．
void
verify(const ImpMgr& imp_mgr,
       const ImpInfo& imp_info)
{
  ymuint n = imp_mgr.node_num();

  SatSolver solver1;
  for (ymuint id = 0; id < n; ++ id) {
    VarId vid = solver1.new_var();
    assert_cond( vid.val() == id, __FILE__, __LINE__);
  }

  // ImpMgr から CNF を作る．
  for (ymuint id = 0; id < n; ++ id) {
    ImpNode* node = imp_mgr.node(id);
    if ( node == NULL ) continue;
    if ( node->is_input() ) continue;

    Literal lit(VarId(id), kPolPosi);

    const ImpEdge& e0 = node->fanin0();
    ImpNode* node0 = e0.src_node();
    bool inv0 = e0.src_inv();
    Literal lit0(VarId(node0->id()), inv0 ? kPolNega : kPolPosi);

    const ImpEdge& e1 = node->fanin1();
    ImpNode* node1 = e1.src_node();
    bool inv1 = e1.src_inv();
    Literal lit1(VarId(node1->id()), inv1 ? kPolNega : kPolPosi);

    solver1.add_clause(lit0, ~lit);
    solver1.add_clause(lit1, ~lit);
    solver1.add_clause(~lit0, ~lit1, lit);
  }

  ymuint nerr = 0;
  for (ymuint src_id = 0; src_id < n; ++ src_id) {
    for (ymuint src_val = 0; src_val < 2; ++ src_val) {
      Literal lit0(to_literal(src_id, src_val));
      const vector<ImpVal>& imp_list = imp_info.get(src_id, src_val);
      for (vector<ImpVal>::const_iterator p = imp_list.begin();
	   p != imp_list.end(); ++ p) {
	ymuint dst_id = p->id();
	ymuint dst_val = p->val();
	Literal lit1(to_literal(dst_id, dst_val));
	vector<Literal> tmp(2);
	tmp[0] = lit0;
	tmp[1] = ~lit1;
	vector<Bool3> model;
	if ( solver1.solve(tmp, model) != kB3False ) {
	  cout << "ERROR: Node#" << src_id << ": " << src_val
	       << " ==> Node#" << dst_id << ": " << dst_val << endl;
	  ++ nerr;
	}
      }
    }
  }
  cout << "Total " << nerr << " errors" << endl;
}

END_NAMESPACE_YM_NETWORKS
