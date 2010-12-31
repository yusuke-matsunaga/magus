#ifndef LIBYM_VERILOG_PARSER_PARSER_INT_H
#define LIBYM_VERILOG_PARSER_PARSER_INT_H

/// @file libym_verilog/parser/Parser_int.h
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"

//#include "PtiFactory.h"
//#include "PuHierName.h"
//#include "Lex.h"
//#include "PtMgr.h"

//#include "ym_verilog/pt/PtItem.h"
//#include "ym_verilog/pt/PtStmt.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief 現在の declhead リストをスタックに積む．
// @param[in] new_declhead 新しく設定する declhead
inline
void
Parser::push_declhead_list(PtDeclHeadList* new_declhead)
{
  mDeclHeadListStack.push_back(mCurDeclHeadList);
  if ( new_declhead == NULL ) {
    new_declhead = new PtDeclHeadList(mCellAlloc);
  }
  mCurDeclHeadList = new_declhead;
}

// @brief スタックの末尾を declhead リストに戻す．
// @param[in] delete_top true なら昔の declhead を削除する．
inline
void
Parser::pop_declhead_list(bool delete_top)
{
  if ( delete_top ) {
    delete mCurDeclHeadList;
  }
  mCurDeclHeadList = mDeclHeadListStack.back();
  mDeclHeadListStack.pop_back();
}

// @brief 現在の item リストをスタックに積む．
// @param[in] new_item 新しく設定する item リスト
inline
void
Parser::push_item_list(PtItemList* new_item)
{
  mItemListStack.push_back(mCurItemList);
  if ( new_item == NULL ) {
    new_item = new PtItemList(mCellAlloc);
  }
  mCurItemList = new_item;
}

// @brief スタックの末尾を item リストに戻す．
// @param[in] delete_top true なら昔の item を削除する．
inline
void
Parser::pop_item_list(bool delete_top)
{
  if ( delete_top ) {
    delete mCurItemList;
  }
  mCurItemList = mItemListStack.back();
  mItemListStack.pop_back();
}

// @brief 宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_decl_array()
{
  return mCurDeclHeadList->to_array(mAlloc);
}

// @brief item リストを配列に変換する．
inline
PtItemArray
Parser::get_item_array()
{
  return mCurItemList->to_array(mAlloc);
}

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_PARSER_PARSER_INT_H
