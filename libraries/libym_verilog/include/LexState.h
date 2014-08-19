#ifndef LEXSTATE_H
#define LEXSTATE_H

/// @file LexState.h
/// @brief LexState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmVerilog/verilog.h"
#include "YmUtils/FileRegion.h"
#include "RawLex.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class LexState Lex.h "Lex.h"
/// @ingroup VlParser
/// @brief Lex に付随した状態を保持する基底クラス
//////////////////////////////////////////////////////////////////////
class LexState
{
public:

  /// @brief コンストラクタ
  /// @param[in] lex 親の lexer オブジェクト
  LexState(RawLex& lex);

  /// @brief デストラクタ
  virtual
  ~LexState();


public:

  /// @brief 状態を初期化する．
  /// @note デフォルトの実装はなにもしない．
  virtual
  void
  resetall(const FileRegion& file_region);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief デバッグフラグのチェック
  /// @return デバッグフラグが立っていたら true を返す．
  bool
  debug() const;

  /// @brief 親の Lex の取得
  /// @return 親の Lex オブジェクト
  RawLex&
  lex() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の Lex
  RawLex& mLex;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 親の Lex の取得
// @return 親の Lex オブジェクト
inline
RawLex&
LexState::lex() const
{
  return mLex;
}

// @brief デバッグフラグのチェック
// @return デバッグフラグが立っていたら true を返す．
inline
bool
LexState::debug() const
{
  return mLex.debug();
}

END_NAMESPACE_YM_VERILOG

#endif // LEXSTATE_H
