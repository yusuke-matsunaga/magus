
/// @file EufNode.cc
/// @brief EufNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufNode
//////////////////////////////////////////////////////////////////////

// @brief 左辺の式を得る．
// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
EufNode*
EufNode::left() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 右辺の式を得る．
// @note type() が kCon, kDis, kEq の時のみ有効
EufNode*
EufNode::right() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 識別子名を得る．
// @note type() が kFunc, kVar の時のみ有効
string
EufNode::id_name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return string();
}

// @brief 引数の数を得る．
// @note type() が kFunc の時のみ有効
ymuint
EufNode::arg_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 引数を得る．
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note type() が kFunc の時のみ有効
EufNode*
EufNode::arg(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// 関連する関数
//////////////////////////////////////////////////////////////////////

// @relates EufNode
// @brief EufNode の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node 対象のノード
void
display(ostream& s,
	const EufNode* node)
{
  display(s, vector<const EufNode*>(1, node));
}

BEGIN_NONAMESPACE

// ノードのID番号を出力する．
void
disp_id(ostream& s,
	const EufNode* node)
{
  s << "#" << setw(8) << setfill('0') << node->id();
}

void
dfs(ostream& s,
    const EufNode* node,
    hash_set<ymuint>& mark)
{
  if ( mark.count(node->id()) > 0 ) {
    return;
  }

  mark.insert(node->id());

  vector<const EufNode*> child_list;

  disp_id(s, node);
  s << ": ";
  switch( node->type() ) {
  case EufNode::kCon:
    s << "$" << node->var_id() << " ";
    s << "CON  (";
    disp_id(s, node->left());
    s << " ";
    disp_id(s, node->right());
    s << ")" << endl;
    child_list.push_back(node->left());
    child_list.push_back(node->right());
    break;

  case EufNode::kDis:
    s << "$" << node->var_id() << " ";
    s << "DIS  (";
    disp_id(s, node->left());
    s << " ";
    disp_id(s, node->right());
    s << ")" << endl;
    child_list.push_back(node->left());
    child_list.push_back(node->right());
    break;

  case EufNode::kNeg:
    s << "$" << node->var_id() << " ";
    s << "NOT  (";
    disp_id(s, node->left());
    s << ")" << endl;
    child_list.push_back(node->left());
    break;

  case EufNode::kEq:
    s << "$" << node->var_id() << " ";
    s << "EQ   (";
    disp_id(s, node->left());
    s << " ";
    disp_id(s, node->right());
    s << ")" << endl;
    child_list.push_back(node->left());
    child_list.push_back(node->right());
    break;

  case EufNode::kFunc:
    s << "FUNC [" << node->id_name() << "] (";
    for (ymuint i = 0; i < node->arg_num(); ++ i) {
      const EufNode* arg_node = node->arg(i);
      s << " ";
      disp_id(s, arg_node);
      child_list.push_back(arg_node);
    }
    s << ")" << endl;
    break;

  case EufNode::kVar:
    s << "VAR  [" << node->id_name() << "]" << endl;
    break;

  case EufNode::kBVar:
    s << "$" << node->var_id() << " ";
    s << "BVAR [" << node->id_name() << "]" << endl;
    break;
  }

  for (vector<const EufNode*>::const_iterator p = child_list.begin();
       p != child_list.end(); ++ p) {
    const EufNode* node = *p;
    dfs(s, node, mark);
  }
}

END_NONAMESPACE

// @relates EufNode
// @brief 複数の EufNode の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] node_list 対象のノードのリスト
void
display(ostream& s,
	const vector<const EufNode*>& node_list)
{
  // 処理したノードを覚えておくためのハッシュ
  hash_set<ymuint> mark;

  for (vector<const EufNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const EufNode* node = *p;
    dfs(s, node, mark);
  }
}

END_NAMESPACE_YM_LLVMEQ
