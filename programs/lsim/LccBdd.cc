
/// @file LsimBdd2.cc
/// @brief LsimBdd2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimBdd2.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnPort.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimBdd2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimBdd2::LsimBdd2() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimBdd2::~LsimBdd2()
{
}


BEGIN_NONAMESPACE

void
put_bdd(Bdd bdd,
	unordered_map<Bdd, ymuint32>& idmap,
	vector<Bdd>& bdd_array)
{
  if ( bdd.is_const() ) {
    return;
  }

  unordered_map<Bdd, ymuint32>::iterator p = idmap.find(bdd);
  if ( p == idmap.end() ) {
    ymuint id = bdd_array.size();
    bdd_array.push_back(bdd);
    idmap.insert(make_pair(bdd, id));

    Bdd bdd0;
    Bdd bdd1;
    (void) bdd.root_decomp(bdd0, bdd1);
    put_bdd(bdd0, idmap, bdd_array);
    put_bdd(bdd1, idmap, bdd_array);
  }
}

void
write_bdd(Bdd bdd,
	  const unordered_map<Bdd, ymuint32>& idmap)
{
  if ( bdd.is_zero() ) {
    cout << "    return 0;" << endl;
  }
  else if ( bdd.is_one() ) {
    cout << "    return 1;" << endl;
  }
  else {
    unordered_map<Bdd, ymuint32>::const_iterator p = idmap.find(bdd);
    assert_cond( p != idmap.end(), __FILE__, __LINE__);
    cout << "    goto BDD" << p->second << ";" << endl;
  }
}

END_NONAMESPACE

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimBdd2::set_network(const BdnMgr& bdn,
		      const unordered_map<string, ymuint>& order_map)
{
  ymuint n = bdn.max_node_id();
  vector<Bdd> bddmap(n);

  const BdnNodeList& input_list = bdn.input_list();
  ymuint ni = input_list.size();

  if ( order_map.empty() ) {
    ymuint id = 0;
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      Bdd bdd = mBddMgr.make_posiliteral(id);
      ++ id;
      bddmap[node->id()] = bdd;
    }
  }
  else {
    for (BdnNodeList::const_iterator p = input_list.begin();
	 p != input_list.end(); ++ p) {
      const BdnNode* node = *p;
      string name = node->port()->name();
      unordered_map<string, ymuint>::const_iterator q = order_map.find(name);
      if ( q == order_map.end() ) {
	cerr << "No order for " << name << endl;
	abort();
      }
      ymuint id = q->second;
      Bdd bdd = mBddMgr.make_posiliteral(id);
      bddmap[node->id()] = bdd;
    }
  }

  vector<BdnNode*> node_list;
  bdn.sort(node_list);
  for (vector<BdnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* fanin0 = node->fanin0();
    Bdd bdd0 = bddmap[fanin0->id()];
    if ( node->fanin0_inv() ) {
      bdd0 = ~bdd0;
    }
    const BdnNode* fanin1 = node->fanin1();
    Bdd bdd1 = bddmap[fanin1->id()];
    if ( node->fanin1_inv() ) {
      bdd1 = ~bdd1;
    }
#if 0
    cout << " " << bdd0.size() << " x " << bdd1.size();
    cout.flush();
#endif
    if ( node->is_and() ) {
      Bdd bdd = bdd0 & bdd1;
      bddmap[node->id()] = bdd;
    }
    else if ( node->is_xor() ) {
      Bdd bdd = bdd0 ^ bdd1;
      bddmap[node->id()] = bdd;
    }
    else {
      assert_not_reached(__FILE__, __LINE__);
    }
#if 0
    cout << " = " << bddmap[node->id()].size();
#endif
  }

  mBddMgr.disable_gc();

  unordered_map<Bdd, ymuint32> idmap;
  vector<Bdd> bdd_array;

  const BdnNodeList& output_list = bdn.output_list();
  ymuint no = output_list.size();
  vector<int> output_id;
  output_id.reserve(no);
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BdnNode* node = *p;
    BdnNode* node0 = node->output_fanin();
    Bdd bdd;
    if ( node0 != NULL ) {
      bdd = bddmap[node0->id()];
    }
    else {
      bdd = mBddMgr.make_zero();
    }
    if ( node->output_fanin_inv() ) {
      bdd = ~bdd;
    }
    put_bdd(bdd, idmap, bdd_array);

    unordered_map<Bdd, ymuint32>::iterator q = idmap.find(bdd);
    assert_cond( q != idmap.end(), __FILE__, __LINE__);

    output_id.push_back(q->second);
  }

  cout << "/// @file lcc_main.cc" << endl
       << "/// @brief コンパイル法シミュレータのメイン関数" << endl
       << "/// @author Yusuke Matsunaga (松永 裕介)" << endl
       << "///" << endl
       << "/// Copyright (C) 2005-2011 Yusuke Matsunaga" << endl
       << "/// All rights reserved." << endl
       << endl
       << "#include <iostream>" << endl
       << endl;

  cout << "int" << endl
       << "eval_logic(int ivals[]," << endl
       << "           int opos)" << endl
       << "{" << endl;
  for (ymuint i = 0; i < no; ++ i) {
    cout << "  if ( opos == " << i << " ) {" << endl
	 << "    goto BDD" << output_id[i] << ";" << endl
	 << "  }" << endl;
  }
  cout << "  return 0;" << endl; // ダミー

  for (vector<Bdd>::iterator p = bdd_array.begin();
       p != bdd_array.end(); ++ p) {
    Bdd bdd = *p;
    unordered_map<Bdd, ymuint32>::iterator q = idmap.find(bdd);
    assert_cond( q != idmap.end(), __FILE__, __LINE__);
    ymuint32 id = q->second;
    Bdd bdd0;
    Bdd bdd1;
    ymuint32 var = bdd.root_decomp(bdd0, bdd1);
    cout << "BDD" << id << ":" << endl
	 << "  if ( ivals[" << var << "] == 0 ) {" << endl;
    write_bdd(bdd0, idmap);
    cout << "  }" << endl
	 << "  else {" << endl;
    write_bdd(bdd1, idmap);
    cout << "  }" << endl
	 << endl;
  }
  cout << "}" << endl
       << endl;

  cout << "void" << endl
       << "eval_logic(unsigned long ivals[]," << endl
       << "           unsigned long ovals[])" << endl
       << "{" << endl
       << "  for (int i = 0; i < " << no << "; ++ i) {" << endl
       << "    ovals[i] = 0UL;" << endl
       << "  }" << endl
       << "  int tmp_ivals[" << ni << "];" << endl
       << "  for (int b = 0; b < 64; ++ b) {" << endl
       << "    for (int i = 0; i < " << ni << "; ++ i) {" << endl
       << "      tmp_ivals[i] = (ivals[i] >> b) & 1;" << endl
       << "    }" << endl
       << "    for (int i = 0; i < " << no << "; ++ i) {" << endl
       << "      int o = eval_logic(tmp_ivals, i);" << endl
       << "      ovals[i] |= (o << b);" << endl
       << "    }" << endl
       << "  }" << endl
       << "}" << endl
       << endl;

  cout << "int" << endl
       << "main(int argc," << endl
       << "const char** argv)"<< endl
       << "{" << endl
       << "  using namespace std;" << endl
       << endl
       << "  if ( argc != 2 ) {" << endl
       << "    cerr << \"Usage: lcc <loop-num>\" << endl;" << endl
       << "    return 1;" << endl
       << "  }" << endl
       << endl
       << "  int n = atoi(argv[1]);" << endl
       << "  unsigned long ivals[" << ni << "];" << endl
       << "  unsigned long ovals[" << no << "];" << endl
       << "  for (int i = 0; i < n; ++ i) {" << endl
       << "    for (int j = 0; j < " << ni << "; ++ j) {" << endl
       << "      ivals[j] = random();" << endl
       << "    }" << endl
       << "    eval_logic(ivals, ovals);" << endl
       << "  }" << endl
       << endl
       << "  return 0;" << endl
       << "}" << endl;
}


BEGIN_NONAMESPACE

ymuint64
eval_bdd(Bdd bdd,
	 const vector<ymuint64>& iv)
{
  ymuint val = 0U;
  for (ymuint b = 0; b < 64; ++ b) {
    for ( ; ; ) {
      if ( bdd.is_one() ) {
	val |= (1U << b);
	break;
      }
      else if ( bdd.is_zero() ) {
	break;
      }
      Bdd bdd0;
      Bdd bdd1;
      ymuint id = bdd.root_decomp(bdd0, bdd1);
      ymuint ival = iv[id];
      if ( (ival & (1 << b)) ) {
	bdd = bdd1;
      }
      else {
	bdd = bdd0;
      }
    }
  }
  return val;
}

END_NONAMESPACE


// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimBdd2::eval(const vector<ymuint64>& iv,
	       vector<ymuint64>& ov)
{
}

END_NAMESPACE_YM
