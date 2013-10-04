#ifndef SETOPTIONCMD_H
#define SETOPTIONCMD_H

/// @file SetOptionCmd.h
/// @brief SetOptionCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "SmtLibCmd.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SetOptionCmd SetOptionCmd.h "SetOptionCmd.h"
/// @brief set-logic コマンド
//////////////////////////////////////////////////////////////////////
class SetOptionCmd :
  public SmtLibCmd
{
public:

  /// @brief コンストラクタ
  SetOptionCmd();

  /// @brief デストラクタ
  virtual
  ~SetOptionCmd();


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

#endif // SETOPTIONCMD_H
