#ifndef GATEMGR_H
#define GATEMGR_H

/// @file GateMgr.h
/// @brief GateMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "led_nsdef.h"
#include "GateType.h"


BEGIN_NAMESPACE_YM_LED

class Symbol;
class GateObj;

//////////////////////////////////////////////////////////////////////
/// @class GateMgr Gatemgr.h "GateMgr.h"
/// @brief ゲート(GateObj)を管理するクラス
//////////////////////////////////////////////////////////////////////
class GateMgr
{
public:

  /// @brief コンストラクタ
  GateMgr();

  /// @brief デストラクタ
  ~GateMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートを作る．
  /// @param[in] pos 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(const QPoint& pos,
	   GateType type,
	   ymuint ni = 0);

  /// @brief ゲートを作る．
  /// @param[in] x, y 位置
  /// @param[in] type 型
  /// @param[in] ni 入力数
  GateObj*
  new_gate(int x,
	   int y,
	   GateType type,
	   ymuint ni = 0);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シンボルを返す．
  /// @param[in] type 型
  /// @param[in] ni 入力数
  /// @param[in] symbol_array シンボルを格納する配列
  /// @note 場合によっては配列を拡張する．
  /// @note type は AND/NAND/OR/NOR/XOR/XNOR でなければならない．
  Symbol*
  new_symbol(GateType type,
	     ymuint ni,
	     vector<Symbol*>& symbol_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力シンボル
  Symbol* mInputSymbol;

  // 出力シンボル
  Symbol* mOutputSymbol;

  // バッファシンボル
  Symbol* mBufferSymbol;

  // NOTシンボル
  Symbol* mNotSymbol;

  // ANDシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mAndSymbols;

  // NANDシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mNandSymbols;

  // ORシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mOrSymbols;

  // NORシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mNorSymbols;

  // XORシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mXorSymbols;

  // XNORシンボルの配列
  // インデックスは '入力数' - 2
  vector<Symbol*> mXnorSymbols;

};

END_NAMESPACE_YM_LED

#endif // GATEMGR_H
