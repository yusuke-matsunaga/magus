#ifndef LIBYM_CELL_DOTLIB_EXPRHANDLER_H
#define LIBYM_CELL_DOTLIB_EXPRHANDLER_H

/// @file libym_cell/dotlib/ExprHandler.h
/// @brief ExprHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ExprHandler ExprHandler.h "ExprHandler.h"
/// @brief expression の値をとる simple attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class ExprHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  ExprHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~ExprHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const string& attr_name,
	    const FileRegion& attr_loc);


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
