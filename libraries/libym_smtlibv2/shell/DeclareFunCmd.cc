
/// @file DeclareFunCmd.cc
/// @brief DeclareFunCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DeclareFunCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス DeclareFunCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DeclareFunCmd::DeclareFunCmd()
{
}

// @brief デストラクタ
DeclareFunCmd::~DeclareFunCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] arg_top 引数の先頭ノード
// @return 実行結果を返す．
const SmtLibNode*
DeclareFunCmd::exec(const SmtLibNode* arg_top)
{
  // このコマンドは3つの引数をとる．
  if ( arg_top == NULL ||
       arg_top->type() != kSymbolToken ) {
    goto syntax_error;
  }

  const SmtLibNode* arg_sort_list = arg_top->sibling();
  if ( arg_sort_list == NULL ||
       arg_sort_list->type() != kListToken ) {
    goto syntax_error;
  }

  const SmtLibNode* sort_node = arg_sort_list->sibling();
  if ( sort_node == NULL ||
       sort_node->sibling() != NULL ||
       !mgr()->is_sort(sort_node) ) {
    goto syntax_error;
  }

  return mgr()->declare_fun(arg_top->value(), arg_sort_list, sort_node);

 syntax_error:
  // シンタックスエラー
#warning("TODO: エラーの返し方を考える．");
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2
