
/// @file SmtLibMgr.cc
/// @brief SmtLibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibMgr.h"
#include "SmtLibNode.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SmtLibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SmtLibMgr::SmtLibMgr()
{
}

// @brief デストラクタ
SmtLibMgr::~SmtLibMgr()
{
}

// @brief S式を識別子に変換する．
// @param[in] node S式を表すノード
const SmtId*
SmtLibMgr::eval_to_id(const SmtLibNode* node)
{
  ShString name;
  ymuint index_size = 0;
  const SmtLibNode* index_node = NULL;
  if ( node->type() == kSymbolToken ) {
    name = node->value();
  }
  else if ( node->type() == kListToken ) {
    const SmtLibNode* node1 = node->child();
    if ( node1->type() != kSymbolToken ||
	 strcmp("_", static_cast<const char*>(node1->value())) != 0 ) {
      // syntax error
      goto syntax_error;
    }
    const SmtLibNode* node2 = node1->sibling();
    if ( node2 == NULL ||
	 node2->type() != kSymbolToken ) {
      // syntax error
      goto syntax_error;
    }
    name = node2->value();
    for (const SmtLibNode* node3 = node2->sibling();
	 node3 != NULL; node3 = node3->sibling()) {
      if ( node3->type() != kNumToken ) {
	// syntax error
	goto syntax_error;
      }
      ++ index_size;
    }
    if ( index_size == 0 ) {
      // syntax error
      goto syntax_error;
    }
    index_node = node2->sibling();
  }

  return mIdMgr.new_id(name, index_size, index_node);

 syntax_error:
  return NULL;
}

// @brief S式を sort に変換する．
// @param[in] node S式を表すノード
const SmtSort*
SmtLibMgr::eval_to_sort(const SmtLibNode* node)
{
  const SmtId* id = eval_to_id(node);
  vector<const SmtSort*> elem_list;
  if ( id == NULL ) {
    if ( node->type() != kListToken ) {
      // syntax error
      goto syntax_error;
    }

    elem_list.reserve(node->child_num() - 1);

    const SmtLibNode* node1 = node->child();
    id = eval_to_id(node1);
    if ( id == NULL ) {
      // syntax error
      goto syntax_error;
    }

    for (const SmtLibNode* node2 = node1->sibling();
	 node2 != NULL; node2 = node2->sibling()) {
      const SmtSort* sort1 = eval_to_sort(node2);
      if ( sort1 == NULL ) {
	// syntax error
	goto syntax_error;
      }
      elem_list.push_back(sort1);
    }
    if ( elem_list.empty() ) {
      // syntax error
      goto syntax_error;
    }
  }

  return mSortMgr.new_sort(id, elem_list);

 syntax_error:
  return NULL;
}


END_NAMESPACE_YM_SMTLIBV2
