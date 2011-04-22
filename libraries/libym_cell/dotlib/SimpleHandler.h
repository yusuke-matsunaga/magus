#ifndef LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H

/// @file libym_cell/dotlib/SimpleHandler.h
/// @brief SimpleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"
#include "DotLibParser.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class SimpleHandler SimpleHandler.h "SimpleHandler.h"
/// @brief simple attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class SimpleHandler :
  public DotLibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] parent 親のハンドラ
  /// @param[in] req_type 要求する値のタイプ
  SimpleHandler(DotLibParser& parser,
		GroupHandler* parent,
		tValueType req_type);

  /// @brief デストラクタ
  virtual
  ~SimpleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotLibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_token 属性名を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(Token attr_token);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要求するタイプ
  tValueType mReqType;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
