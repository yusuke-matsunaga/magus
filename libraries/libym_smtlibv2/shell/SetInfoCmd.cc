
/// @file SetInfoCmd.cc
/// @brief SetInfoCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SetInfoCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
// クラス SetInfoCmd
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SetInfoCmd::SetInfoCmd()
{
}

// @brief デストラクタ
SetInfoCmd::~SetInfoCmd()
{
}

// @brief コマンドの実行を行う．
// @param[in] arg_top 引数の先頭ノード
// @return 実行結果を返す．
const SmtLibNode*
SetInfoCmd::exec(const SmtLibNode* arg_top)
{
  // このコマンドは1つか2つの引数をとる．
  if ( arg_top == NULL ||
       arg_top->type() != kKeywordToken ) {
    goto syntax_error;
  }
  // attr_val が NULL の場合もありうる
  const SmtLibNode* attr_val = arg_top->sibling();
  if ( attr_val != NULL && attr_val->sibling() != NULL ) {
    goto syntax_error;
  }

  return mgr()->set_info(arg_top->value(), attr_val);

 syntax_error:
  // シンタックスエラー
#warning("TODO: エラーの返し方を考える．");
  return NULL;
}

END_NAMESPACE_YM_SMTLIBV2
