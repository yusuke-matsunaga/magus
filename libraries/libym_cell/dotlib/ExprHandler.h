#ifndef LIBYM_CELL_DOTLIB_EXPRHANDLER_H
#define LIBYM_CELL_DOTLIB_EXPRHANDLER_H

/// @file libym_cell/dotlib/ExprHandler.h
/// @brief ExprHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ExprHandler ExprHandler.h "ExprHandler.h"
/// @brief expression の値をとる simple attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class ExprHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  ExprHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~ExprHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む処理
  /// @return 値を表す PtValue を返す．
  /// @note エラーが起きたら NULL を返す．
  /// @note ここでは expression のパースを行う．
  virtual
  const PtValue*
  read_value();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primary を読み込む．
  PtValue*
  read_primary();

  /// @brief prudct を読み込む．
  PtValue*
  read_product();

  /// @brief expression を読み込む．
  PtValue*
  read_expr(tTokenType end_marker);

  /// @brief トークンを読み込む．
  tTokenType
  read_token();

  /// @brief 直前に読み込んだトークンの位置を返す．
  FileRegion
  cur_loc();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 読み戻したトークンの型
  tTokenType mUngetType;

  // 読み戻したトークンの位置
  FileRegion mUngetLoc;

  // トークンの位置
  FileRegion mCurLoc;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_EXPRHANDLER_H
