#ifndef YM_SMTLIBV2_SMTLIBSH
#define YM_SMTLIBV2_SMTLIBSH

/// @file ym_smtlibv2/SmtLibSh.h
/// @brief SmtLibSh のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtLibSh SmtLibSh.h "ym_smtlibv2/SmtLibSh.h"
/// @brief SmtLib 用のシェルを実現するクラス
//////////////////////////////////////////////////////////////////////
class SmtLibSh
{
public:

  /// @brief コンストラクタ
  SmtLibSh();

  /// @brief デストラクタ
  ~SmtLibSh();


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

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTLIBSH
