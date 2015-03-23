
/// @file RwtMgr.cc
/// @brief RwtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "RwtMgr.h"
#include "RwtNode.h"


BEGIN_NAMESPACE_YM

extern
ymuint16 s_RwtAigSubgraphs[];

extern
ymuint16 s_RwtPracticalClasses[];

// @brief コンストラクタ
RwtMgr::RwtMgr()
{
  mNodeNum = 0;
  mNodeArray = NULL;
}

// @brief デストラクタ
RwtMgr::~RwtMgr()
{
  delete [] mNodeArray;
}


BEGIN_NONAMESPACE

ymuint
count_sub(RwtNode* node,
	  unordered_set<ymuint>& node_hash)
{
  if ( node_hash.count(node->id()) > 0 ) {
    return 0;
  }
  node_hash.insert(node->id());

  RwtNode* node0 = node->child0();
  RwtNode* node1 = node->child1();
  if ( node0 == NULL && node1 == NULL ) {
    return 0;
  }

  ymuint ans = 1;
  ans += count_sub(node0, node_hash);
  ans += count_sub(node1, node_hash);
  return ans;
}

ymuint
count_volume(RwtNode* node0,
	     RwtNode* node1)
{
  unordered_set<ymuint> node_hash;
  ymuint v0 = count_sub(node0, node_hash);
  ymuint v1 = count_sub(node1, node_hash);
  return v0 + v1;
}

END_NONAMESPACE


// @brief 初期化する．
void
RwtMgr::init()
{
  // ノードテーブルの要素数を数える．
  ymuint nn = 0;
  for ( ; ; ++ nn) {
    ymuint16 e0 = s_RwtAigSubgraphs[nn * 2 + 0];
    ymuint16 e1 = s_RwtAigSubgraphs[nn * 2 + 1];
    if ( e0 == 0 && e1 == 0 ) {
      break;
    }
  }

  // ノードテーブルを確保する．
  mNodeNum = nn + 5;
  mNodeArray = new RwtNode[mNodeNum];

  // 定義済みのノードを設定する．
  set_node(0, 0x0000, false, NULL, NULL, false, false, 0, 0);
  set_node(1, 0xaaaa, false, NULL, NULL, false, false, 0, 0);
  set_node(2, 0xcccc, false, NULL, NULL, false, false, 0, 0);
  set_node(3, 0xf0f0, false, NULL, NULL, false, false, 0, 0);
  set_node(4, 0xff00, false, NULL, NULL, false, false, 0, 0);

  // ノードの内容を設定する．
  for (ymuint i = 0; i < nn; ++ i) {
    ymuint16 e0 = s_RwtAigSubgraphs[i * 2 + 0];
    ymuint16 e1 = s_RwtAigSubgraphs[i * 2 + 1];
    bool xor_flag = e0 & 1U;
    e0 >>= 1;
    bool inv0 = (e0 & 1U);
    bool inv1 = (e1 & 1U);

    RwtNode* node0 = &mNodeArray[e0 >> 1];
    RwtNode* node1 = &mNodeArray[e1 >> 1];

    ymuint level = node0->mLevel;
    if ( level < node1->mLevel ) {
      level = node1->mLevel;
    }
    ++ level;

    ymuint volume = count_volume(node0, node1) + 1;

    ymuint16 f0 = node0->mFunc;
    if ( inv0 ) {
      f0 ^= 0xFFFF;
    }
    ymuint16 f1 = node1->mFunc;
    if ( inv1 ) {
      f1 ^= 0xFFFF;
    }
    ymuint16 func;
    if ( xor_flag ) {
      func = f0 ^ f1;
    }
    else {
      func = f0 & f1;
    }

    set_node(i + 5, func, xor_flag, node0, node1, inv0, inv1, level, volume);
  }

  for (ymuint i = 1; s_RwtPracticalClasses[i] != 0x0000; ++ i) {
    ymuint16 func = s_RwtPracticalClasses[i];
    RwtNode* node = find_node(func);
    ASSERT_COND( node != NULL );
  }

  cout << "read_abc_table end. # of nodes = " << mNodeNum << endl;
}

// @brief ノードを設定する．
void
RwtMgr::set_node(ymuint id,
		 ymuint16 func,
		 bool xor_flag,
		 RwtNode* child0,
		 RwtNode* child1,
		 bool inv0,
		 bool inv1,
		 ymuint level,
		 ymuint volume)
{
  RwtNode* node = &mNodeArray[id];
  node->mId = id;
  node->mFunc = func;
  node->mChild0 = child0;
  node->mChild1 = child1;
  node->mLevel = level;
  node->mVolume = volume;
  node->mFlags = 0U;
  if ( inv0 ) {
    node->mFlags |= 1U;
  }
  if ( inv1 ) {
    node->mFlags |= 2U;
  }
  if ( xor_flag ) {
    node->mFlags |= 4U;
  }

  if ( func == 0x0000 ) {
    mFuncHash.insert(make_pair(func, node));
  }
  else {
    for (ymuint i = 1; s_RwtPracticalClasses[i] != 0x0000; ++ i) {
      if ( func == s_RwtPracticalClasses[i] ) {
	mFuncHash.insert(make_pair(func, node));
	break;
      }
    }
  }
}

// @brief 関数に対するノードを返す．
RwtNode*
RwtMgr::find_node(ymuint16 func) const
{
  unordered_map<ymuint16, RwtNode*>::const_iterator p = mFuncHash.find(func);
  if ( p == mFuncHash.end() ) {
    return NULL;
  }
  return p->second;
}

// @brief ノード数を得る．
ymuint
RwtMgr::node_num() const
{
  return mNodeNum;
}

// @brief ノードを得る．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
RwtNode*
RwtMgr::node(ymuint pos) const
{
  ASSERT_COND( pos < mNodeNum );
  return &mNodeArray[pos];
}

// @brief 内容をダンプする．
void
RwtMgr::dump(ostream& s) const
{
  for (unordered_map<ymuint16, RwtNode*>::const_iterator p = mFuncHash.begin();
       p != mFuncHash.end(); ++ p) {
    ymuint16 func = p->first;
    RwtNode* node = p->second;
    s << "Func: " << hex << setw(4) << setfill('0') << func << dec
      << ", Node#" << node->id() << endl;
  }
  s << endl;

  for (ymuint i = 0; i < mNodeNum; ++ i) {
    RwtNode* node = &mNodeArray[i];
    s << "Node#" << i << ": "
      << hex << setw(4) << setfill('0') << node->func() << dec
      << ": Level = " << node->level()
      << ": Volume = " << node->volume() << endl;
    RwtNode* node0 = node->child0();
    RwtNode* node1 = node->child1();
    s << "  ";
    if ( node0 == NULL && node1 == NULL ) {
      switch ( node->func() ) {
      case 0x0000: s << "CONST0"; break;
      case 0xaaaa: s << "VAR0"; break;
      case 0xcccc: s << "VAR1"; break;
      case 0xf0f0: s << "VAR2"; break;
      case 0xff00: s << "VAR3"; break;
      default: ASSERT_NOT_REACHED;
      }
    }
    else {
      if ( node->is_xor() ) {
	s << "XOR";
      }
      else {
	s << "AND";
      }
      s << "(";
      if ( node->inv0() ) {
	s << "~";
      }
      s << node0->id();
      s << ", ";
      if ( node->inv1() ) {
	s << "~";
      }
      s << node1->id();
      s << ")";
    }
    s << endl;
  }
}

END_NAMESPACE_YM
