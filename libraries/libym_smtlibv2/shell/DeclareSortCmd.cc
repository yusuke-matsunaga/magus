
/// @file DeclareSortCmd.cc
/// @brief DeclareSortCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DeclareSortCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス DeclareSortCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DeclareSortCmd::DeclareSortCmd()
{
}

// @brief デストラクタ
DeclareSortCmd::~DeclareSortCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] arg_top 引数の先頭ノード
// @return 実行結果を返す．
const SmtLibNode*
DeclareSortCmd::exec(const SmtLibNode* arg_top)
{
  // このコマンドは2つの引数をとる．
  if ( arg_top == NULL ||
       arg_top->type() != kSymbolToken ) {
    goto syntax_error;
  }
  const SmtLibNode* num_node = arg_top->sibling();
  if ( num_node == NULL ||
       num_node->type() != kNumToken ||
       num_node->sibling() != NULL ) {
    goto syntax_error;
  }

  return mgr()->declare_sort(arg_top->value(), num_node->value());

 syntax_error:
  // シンタックスエラー
#warning("TODO: エラーの返し方を考える．");
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2
