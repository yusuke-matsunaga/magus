#ifndef LIBYM_DOTLIB_FUNCHANDLER_H
#define LIBYM_DOTLIB_FUNCHANDLER_H

/// @file libym_dotlib/FuncHandler.h
/// @brief FuncHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"
#include "FhScanner.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class FuncHandler FuncHandler.h "FuncHandler.h"
/// @brief function 属性の値をパーズする simple attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class FuncHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  FuncHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~FuncHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を読み込む処理
  /// @return 値を表す DotlibNode を返す．
  /// @note エラーが起きたら NULL を返す．
  /// @note ここでは function 属性用の論理式のパースを行う．
  virtual
  DotlibNodeImpl*
  read_value();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief primary を読み込む．
  DotlibNodeImpl*
  read_primary();

  /// @brief プライム付きの primary を読み込む．
  DotlibNodeImpl*
  read_primary2();

  /// @brief prudct を読み込む．
  DotlibNodeImpl*
  read_product();

  /// @brief expression を読み込む．
  DotlibNodeImpl*
  read_expr(tTokenType end_marker);

  /// @brief トークンを読み込む．
  /// @param[out] loc 対応するファイル上の位置情報を格納する変数
  tTokenType
  read_token(FileRegion& loc);

  /// @brief 読み込んだトークンを戻す．
  /// @param[in] type トークンの型
  /// @param[in] loc トークンの位置
  void
  unget_token(tTokenType type,
	      const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  FhScanner mScanner;

  // 読み戻したトークンの型
  tTokenType mUngetType;

  // 読み戻したトークンの位置
  FileRegion mUngetLoc;

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_FUNCHANDLER_H
