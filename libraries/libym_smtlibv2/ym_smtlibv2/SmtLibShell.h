#ifndef YM_SMTLIBV2_SMTLIBSHELL_H
#define YM_SMTLIBV2_SMTLIBSHELL_H

/// @file ym_smtlibv2/SmtLibShell.h
/// @brief SmtLibShell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtLibMgr;

//////////////////////////////////////////////////////////////////////
/// @class SmtLibShell SmtLibShell.h "ym_smtlibv2/SmtLibShell.h"
/// @brief SmtLib 用のシェルを実現するクラス
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

  /// @brief コマンドを登録する．
  /// @param[in] cmd_name コマンド名
  /// @param[in] cmd コマンドオブジェクト
  void
  reg_command(const string& cmd_name,
	      SmtLibCmd* cmd);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  SmtLibMgr* mMgr;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTLIBSHELL_H
