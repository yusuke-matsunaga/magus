#ifndef YM_SMTLiBV2_SMTLIBSHELL_H
#define YM_SMTLiBV2_SMTLIBSHELL_H

/// @file SmtLibShell.h
/// @brief SmtLibShell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class ShellImpl;

//////////////////////////////////////////////////////////////////////
/// @class SmtLibShell SmtLibShell.h "SmtLibShell.h"
/// @brief SmtLib の S式の evaluation を行うクラス
//////////////////////////////////////////////////////////////////////
class SmtLibShell
{
public:

  /// @brief コンストラクタ
  SmtLibShell();

  /// @brief デストラクタ
  ~SmtLibShell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 実行する．
  void
  run();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行なうクラス
  ShellImpl* mImpl;

};

END_NAMESPACE_YM_SMTLIBV2

#endif //  YM_SMTLiBV2_SMTLIBSHELL_H
