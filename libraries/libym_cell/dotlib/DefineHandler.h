#ifndef LIBYM_CELL_DOTLIB_DEFINEHANDLER_H
#define LIBYM_CELL_DOTLIB_DEFINEHANDLER_H

/// @file libym_cell/dotlib/DefineHandler.h
/// @brief DefineHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class LibraryGroupHandler;

//////////////////////////////////////////////////////////////////////
/// @class DefineHandler DefineHandler.h "DefineHandler.h"
/// @brief define attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class DefineHandler :
  public DotlibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] parent 親のハンドラ
  DefineHandler(DotlibParser& parser,
		GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~DefineHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_token 属性名を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(Token attr_token);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DEFINEHANDLER_H
