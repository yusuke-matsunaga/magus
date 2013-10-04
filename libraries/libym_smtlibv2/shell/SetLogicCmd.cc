
/// @file SetLogicCmd.cc
/// @brief SetLogicCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SetLogicCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SetLogicCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetLogicCmd::SetLogicCmd()
{
}

// @brief デストラクタ
SetLogicCmd::~SetLogicCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] arg_top 引数の先頭ノード
// @return 実行結果を返す．
const SmtLibNode*
SetLogicCmd::exec(const SmtLibNode* arg_top)
{
  // このコマンドはただ1つの引数をとる．
  if ( arg_top == NULL ||
       arg_top->sibling() != NULL ||
       arg_top->type() != kSymbolToken ) {
    // シンタックスエラー
#warning("TODO: エラーの返し方を考える．");
    return NULL;
  }

  return mgr()->set_logic(arg_top->value());
}

END_NAMESPACE_YM_SMTLIBV2
