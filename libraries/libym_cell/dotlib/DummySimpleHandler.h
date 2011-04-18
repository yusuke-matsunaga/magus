#ifndef LIBYM_CELL_DOTLIB_DUMMYSIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_DUMMYSIMPLEHANDLER_H

/// @file libym_cell/dotlib/DummySimpleHandler.h
/// @brief DummySimpleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SimpleHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DummySimpleHandler DummySimpleHandler.h "DummySimpleHandler.h"
/// @brief ダミーの SimpleHandler
//////////////////////////////////////////////////////////////////////
class DummySimpleHandler :
  public SimpleHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DummySimpleHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DummySimpleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] name 属性名
  /// @param[in] type 値の型
  /// @param[in] value 値を表す文字列
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const char* name,
	     tTokenType type,
	     const string& value);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DUMMYSIMPLEHANDLER_H
