
/// @file ExprMgr.cc
/// @brief ExprMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprMgr.h"
#include "ExprNode.h"


BEGIN_NAMESPACE_YM_EXPR

//////////////////////////////////////////////////////////////////////
// クラス ExprMgr
//////////////////////////////////////////////////////////////////////

// 唯一のオブジェクト
static ExprMgr* mTheObj = NULL;

// @brief 唯一のインスタンスを返す．
ExprMgr&
ExprMgr::the_obj()
{
  if ( mTheObj == NULL ) {
    mTheObj = new ExprMgr;
  }
  return *mTheObj;
}

// @brief コンストラクタ
ExprMgr::ExprMgr() :
  mNodeAlloc(4096),
  mNodeNum(0),
  mMaxNodeNum(0),
  mStuckNodeNum(0)
{
}

// デストラクタ
ExprMgr::~ExprMgr()
{
}

// @brief 確保したメモリを開放する．
// @note メモリリークチェックのための関数
void
ExprMgr::clear_memory()
{
  delete mTheObj;
  mTheObj = NULL;
}

// 恒偽関数を作る．
ExprNodePtr
ExprMgr::make_zero()
{
  if ( !mConst0 ) {
    mConst0 = alloc_node(kConst0);
    ++ mStuckNodeNum;
  }
  return mConst0;
}

// 恒真関数を作る．
ExprNodePtr
ExprMgr::make_one()
{
  if ( !mConst1 ) {
    mConst1 = alloc_node(kConst1);
    ++ mStuckNodeNum;
  }
  return mConst1;
}

// 肯定のリテラルを作る．
ExprNodePtr
ExprMgr::make_posiliteral(VarId id)
{
  make_literals(id);
  ymuint pos = id.val() << 1;
  assert_cond(pos < mLiteralArray.size(), __FILE__, __LINE__);
  return mLiteralArray[pos];
}

// 否定のリテラルを作る．
ExprNodePtr
ExprMgr::make_negaliteral(VarId id)
{
  make_literals(id);
  ymuint pos = (id.val() << 1) + 1;
  assert_cond(pos < mLiteralArray.size(), __FILE__, __LINE__);
  return mLiteralArray[pos];
}

// chd_list の要素を子供とする AND ノードを作る．
// 子供も AND ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::make_and(ymuint begin)
{
  bool const0 = false;
  ymuint end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (ymuint i = begin; i < end; ++ i) {
    const ExprNode* node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst0 ) {
      const0 = true;
      break;
    }
    if ( type == kAnd ) {
      ymuint ni = node->child_num();
      for (ymuint j = 0; j < ni; ++ j) {
	const ExprNode* node1 = node->child(j);
	if ( check_node(node1) ) {
	  // 逆相の入力があったら答は0
	  const0 = true;
	  break;
	}
      }
      if ( const0 ) {
	break;
      }
    }
    else if ( type == kConst1 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は0
	const0 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  ymuint n = mTmpNodeList.size();
  ExprNodePtr node;
  if ( const0 ) {
    node = make_zero();
  }
  else if ( n == 0 ) {
    node = make_one();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(kAnd);
  }
  return node;
}

// chd_list の要素を子供に持つ OR を表す式を作る．
// 子供も OR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::make_or(ymuint begin)
{
  bool const1 = false;
  ymuint end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (ymuint i = begin; i < end; ++ i) {
    ExprNodePtr node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst1 ) {
      const1 = true;
      break;
    }
    if ( type == kOr ) {
      ymuint ni = node->child_num();
      for (ymuint j = 0; j < ni; ++ j) {
	const ExprNode* node1 = node->child(j);
	if ( check_node(node1) ) {
	  // 逆相の入力があったら答は1
	  const1 = true;
	  break;
	}
      }
      if ( const1 ) {
	break;
      }
    }
    else if ( type == kConst0 ) {
      ; // 無視
    }
    else {
      if ( check_node(node) ) {
	// 逆相の入力があったら答は1
	const1 = true;
	break;
      }
    }
  }
  nodestack_pop(begin);

  ExprNodePtr node;
  ymuint n = mTmpNodeList.size();
  if ( const1 ) {
    node = make_one();
  }
  else if ( n == 0 ) {
    node = make_zero();
  }
  else if ( n == 1 ) {
    node = mTmpNodeList.front();
  }
  else {
    node = alloc_node(kOr);
  }
  return node;
}

// chd_list の要素を子供に持つ XOR を表す式を作る．
// 子供も XOR ノードの場合にはマージする．
// 子供が定数ノードの場合には値に応じた簡単化を行う．
ExprNodePtr
ExprMgr::make_xor(ymuint begin)
{
  bool inv = false;
  ymuint end = nodestack_top();
  mTmpNodeList.clear();
  mTmpNodeList.reserve(end - begin);
  for (ymuint i = begin; i < end; ++ i) {
    ExprNodePtr node = mNodeStack[i];
    tType type = node->type();
    if ( type == kConst1 ) {
      inv = !inv;
    }
    else if ( type == kXor ) {
      ymuint ni = node->child_num();
      for (ymuint j = 0; j < ni; ++ j) {
	const ExprNode* node1 = node->child(j);
	if ( check_node2(node1) ) {
	  inv = !inv;
	}
      }
    }
    else if ( type == kConst0 ) {
      ; // 無視
    } else {
      if ( check_node2(node) ) {
	inv = !inv;
      }
    }
  }
  nodestack_pop(begin);

  ExprNodePtr node;
  ymuint n = mTmpNodeList.size();
  if ( n == 0 ) {
    node = make_zero();
  }
  else {
    if ( n == 1 ) {
      node = mTmpNodeList.front();
    }
    else {
      node = alloc_node(kXor);
    }
  }
  if ( inv ) {
    return complement(node);
  }
  else {
    return node;
  }
}

// node が node_list の中に含まれていないか調べる．
// 同相で同じものがふくまれていたらなにもせず false を返す．
// 逆相で同じものがふくまれていたらなにもせず true を返す．
// 同じものがふくまれていなかったら node を node_list に追加して
// false を返す．
bool
ExprMgr::check_node(const ExprNode* node)
{
  for (ExprNodeList::iterator p = mTmpNodeList.begin();
       p != mTmpNodeList.end(); ++ p) {
    const ExprNode* node1 = *p;
    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていた場合
      return false;
    }
    if ( nega_equiv(node, node1) ) {
      // 逆相で重なっていた場合
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// node が node_list の中に含まれていないか調べる(XOR用)．
// 同相で同じものが含まれていたら重複したものを node_list から削除して
// false を返す．
// 逆相で同じものが含まれていたら重複したものを node_list から削除して
// true を返す．
// 同じものが含まれていなかったら node を node_list に追加して
// false を返す．
bool
ExprMgr::check_node2(const ExprNode* node)
{
  for (ExprNodeList::iterator p = mTmpNodeList.begin();
       p != mTmpNodeList.end(); ) {
    const ExprNode* node1 = *p;
    // ループ中で削除する場合があるので反復子をコピーしてから進めておく
    ExprNodeList::iterator q = p;
    ++ p;

    if ( posi_equiv(node, node1) ) {
      // 同相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return false;
    }
    if ( nega_equiv(node, node1) ) {
      // 逆相で重なっていたら両方取り除く
      mTmpNodeList.erase(q);
      return true;
    }
  }
  mTmpNodeList.push_back(node);
  return false;
}

// @brief ノードスタックにノードを入れる．
void
ExprMgr::nodestack_push(const ExprNode* node)
{
  mNodeStack.push_back(node);
}

// @brief ノードスタックの先頭位置を返す．
ymuint
ExprMgr::nodestack_top()
{
  return mNodeStack.size();
}

// @brief ノードスタックの先頭位置を戻す．
void
ExprMgr::nodestack_pop(ymuint oldtop)
{
  assert_cond(oldtop < mNodeStack.size(), __FILE__, __LINE__);
  mNodeStack.erase(mNodeStack.begin() + oldtop, mNodeStack.end());
}

// 否定形を返す．
ExprNodePtr
ExprMgr::complement(const ExprNode* node)
{
  switch ( node->type() ) {
  case kConst0:
    return make_one();
  case kConst1:
    return make_zero();
  case kPosiLiteral:
    return make_negaliteral(node->varid());
  case kNegaLiteral:
    return make_posiliteral(node->varid());
  case kAnd:
    {
      ymuint n = node->child_num();
      ymuint begin = nodestack_top();
      for (ymuint i = 0; i < n; ++ i) {
	nodestack_push(complement(node->child(i)));
      }
      return make_or(begin);
    }
  case kOr:
    {
      ymuint n = node->child_num();
      ymuint begin = nodestack_top();
      for (ymuint i = 0; i < n; ++ i) {
	nodestack_push(complement(node->child(i)));
      }
      return make_and(begin);
    }
  case kXor:
    {
      ymuint n = node->child_num();
      ymuint begin = nodestack_top();
      nodestack_push(complement(node->child(0)));
      for (ymuint i = 1; i < n; ++ i) {
	nodestack_push(node->child(i));
      }
      return make_xor(begin);
    }
  }

  // ここに来ることはない．
  assert_not_reached(__FILE__, __LINE__);

  return 0;
}

// id 番目の変数を sub に置き換える．
ExprNodePtr
ExprMgr::compose(const ExprNode* node,
		 VarId id,
		 const ExprNodePtr& sub)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;

  case kPosiLiteral:
    if ( node->varid() == id ) {
      return sub;
    }
    return node;

  case kNegaLiteral:
    if ( node->varid() == id ) {
      return complement(sub);
    }
    return node;
  default:
    break;
  }

  ymuint n = node->child_num();
  ymuint begin = nodestack_top();
  bool ident = true;
  for (ymuint i = 0; i < n; ++ i) {
    ExprNodePtr chd = compose(node->child(i), id, sub);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// comp_map にしたがってリテラルを式に置き換える．
ExprNodePtr
ExprMgr::compose(const ExprNode* node,
		 const VarExprMap& comp_map)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;

  case kPosiLiteral:
    {
      VarExprMap::const_iterator p = comp_map.find(node->varid());
      if ( p != comp_map.end() ) {
	return (p->second).root();
      }
    }
    return node;

  case kNegaLiteral:
    {
      VarExprMap::const_iterator p = comp_map.find(node->varid());
      if ( p != comp_map.end() ) {
	return complement((p->second).root());
      }
    }
    return node;

  default:
    break;
  }

  ymuint n = node->child_num();
  ymuint begin = nodestack_top();
  bool ident = true;
  for (ymuint i = 0; i < n; ++ i) {
    ExprNodePtr chd = compose(node->child(i), comp_map);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// 変数番号をマッピングし直す
ExprNodePtr
ExprMgr::remap_var(const ExprNode* node,
		   const VarVarMap& varmap)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
    return node;

  case kPosiLiteral:
    {
      VarVarMap::const_iterator p = varmap.find(node->varid());
      if ( p != varmap.end() ) {
	return make_posiliteral(p->second);
      }
    }
    return node;

  case kNegaLiteral:
    {
      VarVarMap::const_iterator p = varmap.find(node->varid());
      if ( p != varmap.end() ) {
	return make_negaliteral(p->second);
      }
    }
    return node;

  default: break;
  }

  ymuint n = node->child_num();
  ymuint begin = nodestack_top();
  bool ident = true;
  for (ymuint i = 0; i < n; ++ i) {
    ExprNodePtr chd = remap_var(node->child(i), varmap);
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// 簡単化を行う．
ExprNodePtr
ExprMgr::simplify(const ExprNode* node)
{
  switch ( node->type() ) {
  case kConst0:
  case kConst1:
  case kPosiLiteral:
  case kNegaLiteral:
    return node;

  default:
    break;
  }

  ymuint n = node->child_num();
  ymuint begin = nodestack_top();
  bool ident = true;
  for (ymuint i = 0; i < n; ++ i) {
    ExprNodePtr chd = simplify(node->child(i));
    if ( chd != node->child(i) ) {
      ident = false;
    }
    nodestack_push(chd);
  }
  if ( ident ) {
    nodestack_pop(begin);
    return node;
  }

  switch ( node->type() ) {
  case kAnd: return make_and(begin);
  case kOr:  return make_or(begin);
  case kXor: return make_xor(begin);
  default:   break;
  }

  // ここに来ることはない．
  assert_not_reached(__FILE__, __LINE__);

  return NULL;
}

// @brief 使用されているメモリ量を返す．
ymuint
ExprMgr::used_size()
{
  return mNodeAlloc.used_size();
}

// @brief 使用されているノード数を返す．
ymuint
ExprMgr::node_num()
{
  return mNodeNum;
}

// @brief used_size() の最大値を返す．
ymuint
ExprMgr::max_used_size()
{
  return mNodeAlloc.max_used_size();
}

// @brief nodenum() の最大値を返す．
ymuint
ExprMgr::max_node_num()
{
  return mMaxNodeNum;
}

// @brief 実際に確保したメモリ量を返す．
ymuint
ExprMgr::allocated_size()
{
  return mNodeAlloc.allocated_size();
}

// @brief 実際に確保した回数を返す．
ymuint
ExprMgr::allocated_count()
{
  return mNodeAlloc.allocated_count();
}

// @brief 内部状態を出力する．
void
ExprMgr::print_stats(ostream& s)
{
  s << "maximum used size: " << max_used_size() << endl
    << "maximum node num:  " << max_node_num() << endl
    << "current used size: " << used_size() << endl
    << "current node num:  " << node_num()
    << " ( " << mStuckNodeNum << " )" << endl
    << "allocated size:    " << allocated_size() << endl
    << "allocated count:   " << allocated_count() << endl
    << endl;
}

// id 番めまでのリテラルノードを作る．
void
ExprMgr::make_literals(VarId id)
{
  ymuint last = mLiteralArray.size() / 2;
  while ( last <= id.val() ) {
    ExprNode* posi = alloc_node(kPosiLiteral);
    posi->mNc = last;
    mLiteralArray.push_back(posi);
    ++ mStuckNodeNum;

    ExprNode* nega = alloc_node(kNegaLiteral);
    nega->mNc = last;
    mLiteralArray.push_back(nega);
    ++ mStuckNodeNum;
    ++ last;
  }
}

// ExprNode を確保して内容を設定する．
ExprNode*
ExprMgr::alloc_node(tType type)
{
  ++ mNodeNum;
  if ( mMaxNodeNum < mNodeNum ) {
    mMaxNodeNum = mNodeNum;
  }

  ymuint nc = 0;
  if ( type == kAnd || type == kOr || type == kXor ) {
    nc = mTmpNodeList.size();
  }

  ymuint req_size = calc_size(nc);
  void* p = mNodeAlloc.get_memory(req_size);
  ExprNode* node = new (p) ExprNode;
  node->mRefType = static_cast<ymuint32>(type);
  node->mNc = nc;
  for (ymuint i = 0; i < nc; ++ i) {
    node->mChildArray[i] = mTmpNodeList[i];
    node->child(i)->inc_ref();
  }
  return node;
}

// ExprNode を削除する．
void
ExprMgr::free_node(ExprNode* node)
{
  ymuint n = node->child_num();
  for (ymuint i = 0; i < n; ++ i) {
    node->child(i)->dec_ref();
  }

  -- mNodeNum;

  ymuint req_size = calc_size(n);
  mNodeAlloc.put_memory(req_size, node);
}

// ExprNode の入力数から必要なサイズを計算する．
ymuint
ExprMgr::calc_size(ymuint nc)
{
  return sizeof(ExprNode) + sizeof(ExprNode*) * (nc - 1);
}

END_NAMESPACE_YM_EXPR
