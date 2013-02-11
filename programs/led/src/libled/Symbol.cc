
/// @file Symbol.cc
/// @brief Symbol の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Symbol.h"
#include "SymbolImpl.h"


BEGIN_NAMESPACE_YM_LED

//////////////////////////////////////////////////////////////////////
// クラス Symbol
//////////////////////////////////////////////////////////////////////

// @brief 単純な型のシンボルを生成する．
// @param[in] type 型
// @param[in] ni 入力数
// @note 入力/出力/バッファ/NOT の場合は ni の値は無視される．
// @note それ以外の場合は ni は 2 以上でなければならない．
Symbol*
Symbol::new_symbol(GateType type,
		   ymuint ni)
{
  return new SymbolImpl(type, ni);
}

#if 0
// @brief 入力を生成する．
Symbol*
Symbol::input_symbol()
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_input();
  return symbol;
}

// @brief 出力を生成する．
Symbol*
Symbol::output_symbol()
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_output();
  return symbol;
}

// @brief バッファを生成する．
Symbol*
Symbol::buffer_symbol()
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_buffer();
  return symbol;
}

// @brief NOT ゲートを生成する．
Symbol*
Symbol::not_symbol()
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_not();
  return symbol;
}

// @brief AND ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::and_symbol(ymuint ni)
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_and(ni);
  return symbol;
}

// @brief OR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::or_symbol(ymuint ni)
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_or(ni);
  return symbol;
}

// @brief XOR ゲートを生成する．
// @param[in] ni 入力数
Symbol*
Symbol::xor_symbol(ymuint ni)
{
  SymbolImpl* symbol = new SymbolImpl();
  symbol->set_to_xor(ni);
  return symbol;
}

// @brief 出力に否定のドットをつけたシンボルを生成する．
// @param[in] src_symbol もととなるシンボル．
Symbol*
Symbol::output_inv_symbol(Symbol* src_symbol)
{
}

// @brief 入力に否定のドットをつけたシンボルを生成する．
// @param[in] src_symbol もととなるシンボル．
// @param[in] pols 入力の否定の情報 (0 で肯定，1 で否定)
Symbol*
Symbol::input_inv_symbol(Symbol* src_symbol,
			 const vector<int>& pols)
{
}
#endif

END_NAMESPACE_YM_LED
