
/// @file DefineSortCmd.cc
/// @brief DefineSortCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DefineSortCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス DefineSortCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DefineSortCmd::DefineSortCmd()
{
}

// @brief デストラクタ
DefineSortCmd::~DefineSortCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] arg_top 引数の先頭ノード
// @return 実行結果を返す．
const SmtLibNode*
DefineSortCmd::exec(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  if ( arg_top == NULL ||
       arg_top->type() != kSymbolToken ) {
    goto syntax_error;
  }

  const SmtLibNode* param_list = arg_top->sibling();
  if ( param_list == NULL ||
       param_list->type() != kListToken ) {
    goto syntax_error;
  }

  const SmtLibNode* sort_node = param_list->sibling();
  if ( sort_node == NULL ||
       sort_node->sibling() != NULL ||
       !mgr()->is_sort(sort_node) ) {
    goto syntax_error;
  }

  return mgr()->define_sort(arg_top->value(), param_list, sort_node);

 syntax_error:
  // シンタックスエラー
#warning("TODO: エラーの返し方を考える．");
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2
