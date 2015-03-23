
/// @file GateMgr.cc
/// @brief GateMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GateMgr.h"
#include "Symbol.h"
#include "GateObj.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス GateMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
GateMgr::GateMgr() :
  mAndSymbols(3),
  mNandSymbols(3),
  mOrSymbols(3),
  mNorSymbols(3),
  mXorSymbols(3),
  mXnorSymbols(3)
{
  mInputSymbol = new Symbol(kGtInput);
  mOutputSymbol = new Symbol(kGtOutput);
  mBufferSymbol = new Symbol(kGtBuffer);
  mNotSymbol = new Symbol(kGtNot);
  for (ymuint i = 2; i < 4; ++ i) {
    mAndSymbols[i - 2] = new Symbol(kGtAnd, i);
    mNandSymbols[i - 2] = new Symbol(kGtNand, i);
    mOrSymbols[i - 2] = new Symbol(kGtOr, i);
    mNorSymbols[i - 2] = new Symbol(kGtNor, i);
    mXorSymbols[i - 2] = new Symbol(kGtXor, i);
    mXnorSymbols[i - 2] = new Symbol(kGtXnor, i);
  }
}

// @brief デストラクタ
GateMgr::~GateMgr()
{
  delete mInputSymbol;
  delete mOutputSymbol;
  delete mBufferSymbol;
  delete mNotSymbol;

  for (vector<Symbol*>::iterator p = mAndSymbols.begin();
       p != mAndSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }

  for (vector<Symbol*>::iterator p = mNandSymbols.begin();
       p != mNandSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }

  for (vector<Symbol*>::iterator p = mOrSymbols.begin();
       p != mOrSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }

  for (vector<Symbol*>::iterator p = mNorSymbols.begin();
       p != mNorSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }

  for (vector<Symbol*>::iterator p = mXorSymbols.begin();
       p != mXorSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }

  for (vector<Symbol*>::iterator p = mXnorSymbols.begin();
       p != mXnorSymbols.end(); ++ p) {
    Symbol* symbol = *p;
    delete symbol;
  }
}

// @brief ゲートを作る．
// @param[in] pos 位置
// @param[in] type 型
// @param[in] ni 入力数
GateObj*
GateMgr::new_gate(const QPoint& pos,
		  GateType type,
		  ymuint ni)
{
  Symbol* symbol = NULL;
  switch ( type ) {
  case kGtInput:  symbol = mInputSymbol; break;
  case kGtOutput: symbol = mOutputSymbol; break;
  case kGtBuffer: symbol = mBufferSymbol; break;
  case kGtNot:    symbol = mNotSymbol; break;
  case kGtAnd:    symbol = new_symbol(type, ni, mAndSymbols); break;
  case kGtNand:   symbol = new_symbol(type, ni, mNandSymbols); break;
  case kGtOr:     symbol = new_symbol(type, ni, mOrSymbols); break;
  case kGtNor:    symbol = new_symbol(type, ni, mNorSymbols); break;
  case kGtXor:    symbol = new_symbol(type, ni, mXorSymbols); break;
  case kGtXnor:   symbol = new_symbol(type, ni, mXnorSymbols); break;
  default:
    ASSERT_NOT_REACHED;
    break;
  }

  return new GateObj(symbol, pos);
}

// @brief ゲートを作る．
// @param[in] type 型
// @param[in] ni 入力数
// @param[in] x, y 位置
GateObj*
GateMgr::new_gate(int x,
		  int y,
		  GateType type,
		  ymuint ni)
{
  return new_gate(QPoint(x, y), type, ni);
}

// @brief シンボルを返す．
// @param[in] type 型
// @param[in] ni 入力数
// @param[in] symbol_array シンボルを格納する配列
// @note 場合によっては配列を拡張する．
// @note type は AND/NAND/OR/NOR/XOR/XNOR でなければならない．
Symbol*
GateMgr::new_symbol(GateType type,
		    ymuint ni,
		    vector<Symbol*>& symbol_array)
{
  ASSERT_COND( ni >= 2 );
  ymuint index = ni - 2;

  // 必要なサイズまで NULL でパディングする．
  while ( symbol_array.size() <= index ) {
    symbol_array.push_back(NULL);
  }

  // 必要なサイズのシンボルがなければ作る．
  if ( symbol_array[index] == NULL ) {
    symbol_array[index] = new Symbol(type, ni);
  }

  return symbol_array[index];
}

END_NAMESPACE_YM_LED
