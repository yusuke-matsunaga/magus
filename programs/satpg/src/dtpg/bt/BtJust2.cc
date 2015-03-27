
/// @file BtJust2.cc
/// @brief BtJust2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJust2.h"


BEGIN_NAMESPACE_YM_SATPG

// @brief 'Just2' タイプの生成を行なう．
BackTracer*
new_BtJust2()
{
  return new BtJust2();
}


//////////////////////////////////////////////////////////////////////
// クラス BtJust2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BtJust2::BtJust2() :
  mAlloc(sizeof(NodeList), 1024)
{
}

// @brief デストラクタ
BtJust2::~BtJust2()
{
}

// @brief ノードID番号の最大値を設定する．
// @param[in] max_id ID番号の最大値
void
BtJust2::set_max_id(ymuint max_id)
{
  mJustArray.resize(max_id, NULL);
  BtJustBase::set_max_id(max_id);
}

// @brief バックトレースを行なう．
// @param[in] fnode 故障のあるノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] input_list テストパタンに関係のある入力のリスト
// @param[in] output_list 故障伝搬の可能性のある出力のリスト
// @param[out] assign_list 値の割当リスト
void
BtJust2::operator()(TpgNode* fnode,
		    const vector<Bool3>& model,
		    const vector<TpgNode*>& input_list,
		    const vector<TpgNode*>& output_list,
		    AssignList& assign_list)
{
  // 故障差の伝搬している外部出力を選ぶ．
  ymuint nmin = 0;
  NodeList* best_list = NULL;
  for (vector<TpgNode*>::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    TpgNode* node = *p;
    if ( node_dval(node, model) == kB3True ) {
      // 正当化を行う．
      NodeList* node_list = justify(node, model);
      ymuint n = list_size(node_list);
      if ( nmin == 0 || nmin > n ) {
	nmin = n;
	best_list = node_list;
      }
    }
  }
  ASSERT_COND( nmin > 0 );

  assign_list.clear();
  for (NodeList* tmp = best_list; tmp; tmp = tmp->mLink) {
    TpgNode* node = tmp->mNode;
    record_value(node, model, assign_list);
  }

  // 一連の処理でつけたマークを消す．
  clear_justified();
}

// @brief clear_justified() 中で呼ばれるフック関数
void
BtJust2::clear_justified_hook(TpgNode* node)
{
  ASSERT_COND( node->id() < mJustArray.size() );
  list_free(mJustArray[node->id()]);
  mJustArray[node->id()] = NULL;
}

// @brief solve 中で変数割り当ての正当化を行なう．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @note node の値割り当てを正当化する．
// @note 正当化に用いられているノードには mJustifiedMark がつく．
// @note mJustifiedMmark がついたノードは mJustifiedNodeList に格納される．
BtJust2::NodeList*
BtJust2::justify(TpgNode* node,
		 const vector<Bool3>& model)
{
  if ( justified_mark(node) ) {
    return mJustArray[node->id()];
  }
  set_justified(node);

  if ( node->is_input() ) {
    // val を記録
    mJustArray[node->id()] = new_list_cell(node);
    return mJustArray[node->id()];
  }

  Bool3 gval = node_gval(node, model);
  Bool3 fval = node_fval(node, model);

  if ( gval != fval ) {
    // 正常値と故障値が異なっていたら
    // すべてのファンインをたどる．
    return just_sub1(node, model);
  }

  switch ( node->gate_type() ) {
  case kTgGateBuff:
  case kTgGateNot:
    // 無条件で唯一のファンインをたどる．
    return just_sub1(node, model);

  case kTgGateAnd:
    if ( gval == kB3True ) {
      // すべてのファンインノードをたどる．
      return just_sub1(node, model);
    }
    else if ( gval == kB3False ) {
      // 0の値を持つ最初のノードをたどる．
      return just_sub2(node, model, kB3False);
    }
    break;

  case kTgGateNand:
    if ( gval == kB3True ) {
      // 0の値を持つ最初のノードをたどる．
      return just_sub2(node, model, kB3False);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインノードをたどる．
      return just_sub1(node, model);
    }
    break;

  case kTgGateOr:
    if ( gval == kB3True ) {
      // 1の値を持つ最初のノードをたどる．
      return just_sub2(node, model, kB3True);
    }
    else if ( gval == kB3False ) {
      // すべてのファンインノードをたどる．
      return just_sub1(node, model);
    }
    break;

  case kTgGateNor:
    if ( gval == kB3True ) {
      // すべてのファンインノードをたどる．
      return just_sub1(node, model);
    }
    else if ( gval == kB3False ) {
      // 1の値を持つ最初のノードをたどる．
      return just_sub2(node, model, kB3True);
    }
    break;

  case kTgGateXor:
  case kTgGateXnor:
    // すべてのファンインノードをたどる．
    return just_sub1(node, model);
    break;

  default:
    ASSERT_NOT_REACHED;
    break;
  }

  return NULL;
}


// @brief すべてのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
BtJust2::NodeList*
BtJust2::just_sub1(TpgNode* node,
		   const vector<Bool3>& model)
{
  NodeList*& node_list = mJustArray[node->id()];
  ymuint ni = node->fanin_num();
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    NodeList* node_list1 = justify(inode, model);
    list_merge(node_list, node_list1);
  }
  return node_list;
}

// @brief 指定した値を持つのファンインに対して justify() を呼ぶ．
// @param[in] node 対象のノード
// @param[in] model SATの値の割り当て結果を収めた配列
// @param[in] val 値
BtJust2::NodeList*
BtJust2::just_sub2(TpgNode* node,
		   const vector<Bool3>& model,
		   Bool3 val)
{
  ymuint ni = node->fanin_num();
  // まず gval と fval が等しい場合を探す．
  ymuint pos = ni;
  ymuint min = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode, model);
    Bool3 ifval = node_fval(inode, model);
    if ( igval != ifval || igval != val ) {
      continue;
    }
    NodeList* node_list1 = justify(inode, model);
    ymuint n = list_size(node_list1);
    if ( min == 0 || min > n ) {
      pos = i;
      min = n;
    }
  }
  if ( pos < ni ) {
    NodeList*& node_list = mJustArray[node->id()];
    list_merge(node_list, mJustArray[node->fanin(pos)->id()]);
    return node_list;
  }

  ymuint gpos = ni;
  ymuint fpos = ni;
  ymuint gmin = 0;
  ymuint fmin = 0;
  for (ymuint i = 0; i < ni; ++ i) {
    TpgNode* inode = node->fanin(i);
    Bool3 igval = node_gval(inode, model);
    Bool3 ifval = node_fval(inode, model);
    if ( igval != val && ifval != val ) {
      continue;
    }
    NodeList* node_list1 = justify(inode, model);
    ymuint n = list_size(node_list1);
    if ( igval == val ) {
      if ( gmin == 0 || gmin > n ) {
	gpos = i;
	gmin = n;
      }
    }
    if ( ifval == val ) {
      if ( fmin == 0 || fmin > n ) {
	fpos = i;
	fmin = n;
      }
    }
  }
  ASSERT_COND( gpos < ni );
  ASSERT_COND( fpos < ni );
  ASSERT_COND( gpos != fpos );
  NodeList*& node_list = mJustArray[node->id()];
  list_merge(node_list, mJustArray[node->fanin(gpos)->id()]);
  list_merge(node_list, mJustArray[node->fanin(fpos)->id()]);
  return node_list;
}

// @brief 新しいリストのセルを返す．
BtJust2::NodeList*
BtJust2::new_list_cell(TpgNode* node)
{
  void* p = mAlloc.get_memory(sizeof(NodeList));
  NodeList* tmp = new (p) NodeList;
  tmp->mNode = node;
  tmp->mLink = NULL;
  return tmp;
}

// @brief リストをマージする．
void
BtJust2::list_merge(NodeList*& dst_list,
		    NodeList* src_list)
{
  NodeList** pdst = &dst_list;
  NodeList* src = src_list;
  while ( *pdst != NULL && src != NULL ) {
    if ( (*pdst)->mNode->id() < src->mNode->id() ) {
      pdst = &(*pdst)->mLink;
    }
    else if ( (*pdst)->mNode->id() > src->mNode->id() ) {
      NodeList* tmp = new_list_cell(src->mNode);
      NodeList* next = *pdst;
      *pdst = tmp;
      tmp->mLink = next;
      pdst = &tmp->mLink;
      src = src->mLink;
    }
    else {
      pdst = &(*pdst)->mLink;
      src = src->mLink;
    }
  }
  for ( ; src != NULL; src = src->mLink) {
    NodeList* tmp = new_list_cell(src->mNode);
    *pdst = tmp;
    pdst = &tmp->mLink;
  }
}

// @brief リストのサイズを返す．
ymuint
BtJust2::list_size(NodeList* node_list)
{
  ymuint n = 0;
  for (NodeList* tmp = node_list; tmp; tmp = tmp->mLink) {
    ++ n;
  }
  return n;
}

// @brief リストを削除する．
void
BtJust2::list_free(NodeList* node_list)
{
  for (NodeList* tmp = node_list; tmp; ) {
    NodeList* next = tmp->mLink;
    mAlloc.put_memory(sizeof(NodeList), tmp);
    tmp = next;
  }
}

END_NAMESPACE_YM_SATPG
