#ifndef DECLARESORTCMD_H
#define DECLARESORTCMD_H

/// @file DeclareSortCmd.h
/// @brief DeclareSortCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class DeclareSortCmd DeclareSortCmd.h "DeclareSortCmd.h"
/// @brief set-logic コマンド
//////////////////////////////////////////////////////////////////////
class DeclareSortCmd :
  public SmtLibCmd
{
public:

  /// @brief コンストラクタ
  DeclareSortCmd();

  /// @brief デストラクタ
  virtual
  ~DeclareSortCmd();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンドの実行を行う．
  /// @param[in] arg_top 引数の先頭ノード
  /// @return 実行結果を返す．
  virtual
  const SmtLibNode*
  exec(const SmtLibNode* arg_top);

};

END_NAMESPACE_YM_SMTLIBV2

#endif // DECLARESORTCMD_H
