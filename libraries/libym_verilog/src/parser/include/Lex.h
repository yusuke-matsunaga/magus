#ifndef LEX_H
#define LEX_H

/// @file Lex.h
/// @brief Lex のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

class LexModuleState;

//////////////////////////////////////////////////////////////////////
/// @class Lex Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Verilog 用の字句解析器のインターフェイス
//////////////////////////////////////////////////////////////////////
class Lex :
  public RawLex
{
public:

  /// @brief コンストラクタ
  Lex();

  /// @brief デストラクタ
  ~Lex();


public:

  /// @brief モジュールの状態を取得する．
  LexModuleState*
  module_state();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // モジュール関係のLEX状態
  LexModuleState* mModuleState;

};

END_NAMESPACE_YM_VERILOG

#endif // LEX_H
