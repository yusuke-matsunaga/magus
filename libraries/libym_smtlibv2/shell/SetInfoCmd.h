#ifndef SETINFOCMD_H
#define SETINFOCMD_H

/// @file SetInfoCmd.h
/// @brief SetInfoCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SetInfoCmd SetInfoCmd.h "SetInfoCmd.h"
/// @brief set-logic コマンド
//////////////////////////////////////////////////////////////////////
class SetInfoCmd :
  public SmtLibCmd
{
public:

  /// @brief コンストラクタ
  SetInfoCmd();

  /// @brief デストラクタ
  virtual
  ~SetInfoCmd();


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

#endif // SETINFOCMD_H
