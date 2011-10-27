
/// @file LsimBdd3.cc
/// @brief LsimBdd3 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimBdd3.h"
#include "ym_networks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimBdd3
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimBdd3::LsimBdd3() :
  mBddMgr("bmc", "Bdd Manager")
{
}

// @brief デストラクタ
LsimBdd3::~LsimBdd3()
{
}


BEGIN_NONAMESPACE

void
put_bdd(Bdd bdd,
	hash_map<Bdd, ymuint32>& idmap,
	vector<Bdd>& bdd_array)
{
  if ( bdd.is_const() ) {
    return;
  }

  hash_map<Bdd, ymuint32>::iterator p = idmap.find(bdd);
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

ymuint
make_lut(Bdd bdd,
	 hash_map<Bdd, ymuint32>& lutmap,
	 ymuint& nlut);

void
make_lut_sub(Bdd bdd,
	     ymuint limit,
	     hash_map<Bdd, ymuint32>& lutmap,
	     ymuint& nulut)
{
  Bdd bdd0;
  Bdd bdd1;
  ymuint var = bdd.root_decomp(bdd0, bdd1);
  if ( var >= limit ) {
    make_lut(bdd, lutmap, nlut);
  }
  else {
    make_lut_sub(bdd0, limit, lutmap, nlut);
    make_lut_sub(bdd1, limit, lutmap, nlut);
  }
}

ymuint
make_lut(Bdd bdd,
	 hash_map<Bdd, ymuint32>& lutmap,
	 ymuint& nlut)
{
  hash_map::iterator p = lutmap.find(bdd);
  if ( p != lutmap.end() ) {
    return p->second;
  }

  ymuint addr = nlut * 1024;
  lutmap.insert(make_pair(bdd, addr));

  Bdd bdd0;
  Bdd bdd1;
  ymuint var = bdd.root_decomp(bdd0, bdd1);

  make_lut_sub(bdd0, var + 10, lutmap, nlut);
  make_lut_sub(bdd1, var + 10, lutmap, nlut);

  return addr;
}

void
make_lut2(Bdd bdd,
	  ymuint32* lut);

void
make_lut2(Bdd bdd,
	  ymuint32* lut,
	  const hash_map<Bdd, ymuint32>& lutmap)
{
  ymuint32
}

END_NONAMESPACE

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
void
LsimBdd3::set_network(const BdnMgr& bdn)
{
  ymuint n = bdn.max_node_id();
  vector<Bdd> bddmap(n);

  const BdnNodeList& input_list = bdn.input_list();
  ymuint id = 0;
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    Bdd bdd = mBddMgr.make_posiliteral(id);
    ++ id;
    bddmap[node->id()] = bdd;
  }

  vector<BdnNode*> node_list;
  bdn.sort(node_list);
  ymuint node_num = node_list.size();
  id = 0;
  for (vector<BdnNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    ++ id;
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

  hash_map<Bdd, ymuint32> idmap;
  vector<Bdd> bdd_array;

  const BdnNodeList& output_list = bdn.output_list();
  ymuint no = output_list.size();
  vector<int> output_id;
  output_id.reserve(no);

  hash_map<Bdd, ymuint32> lutmap;

  mOutputList.clear();
  mOutputList.reserve(no);
  ymuint nlut = 0;
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    BdnNode* node = *p;
    BdnNode* node0 = node->output_fanin();
    Bdd bdd = bddmap[node0->id()];
    if ( node->output_fanin_inv() ) {
      bdd = ~bdd;
    }
    put_bdd(bdd, idmap, bdd_array);

    hash_map<Bdd, ymuint32>::iterator q = idmap.find(bdd);
    assert_cond( q != idmap.end(), __FILE__, __LINE__);

    output_id.push_back(q->second);

    ymuint addr = make_lut(bdd, lutmap, nlut);
    mOutputList.push_back(addr);
  }

  mLut = new ymuint32[nlut * 1024];

  for (hash_map<Bdd, ymuint32>::iterator p = lutmap.begin();
       p != lutmap.end(); ++ ) {
    Bdd bdd = p->first;
    ymuint32 addr = p->second;
    make_lut2(bdd, &mLut[addr], lutmap);
  }
}


// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimBdd3::eval(const vector<ymuint64>& iv,
	       vector<ymuint64>& ov)
{
  ymuint no = ov.size();
  for (ymuint i = 0; i < no; ++ i) {
    ymuint addr0 = mOutputList[i];
    ov[i] = eval_lut(addr0, iv);
  }
}


BEGIN_NONAMESPACE

inline
ymuint
calc_addr(const vector<ymuint64>& iv,
	  ymuint base)
{
  ymuint ni = iv.size();
  ymuint next = base + 10;
  ymuint end = (next < ni) ? next : ni;
  ymuint addr = 0U;
  for (ymuint i = base; i < end; ++ i) {
    if ( iv[i] ) {
      addr |= (1U << (next - i));
    }
  }
  return addr;
}

END_NONAMESPACE

// @brief 1つの出力に対する評価を行う．
ymuint64
LsimBdd3::eval_lut(ymuint addr0,
		   const vector<ymuint64>& iv)
{
  ymuint ni = iv.size();
  ymuint val = 0U;
  for (ymuint b = 0; b < 64; ++ b) {
    ymuint base = 0;
    ymuint addr = addr0;
    for ( ; ; ) {
      ymuint next = base + 10;
      if ( next + 5 < ni ) {
	ymuint addr1 = calc_addr(iv, base);
	addr = mLUT[addr + addr1];
	base = next;
      }
      else {
	ymuint addr1 = calc_addr(iv, base);
	ymuint addr2 = calc_addr(iv, next);
	if ( (mLUT[addr + addr1] >> addr2) & 1UL ) {
	  val |= (1U << b);
	}
	break;
      }
    }
  }
  return val;
}

END_NAMESPACE_YM
