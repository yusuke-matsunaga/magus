#ifndef LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
#define LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H

/// @file libym_cell/dotlib/SimpleHandler.h
/// @brief SimpleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"
#include "Token.h"


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
  SimpleHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~SimpleHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotLibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値を読み込む．
  /// @param[in] attr_name 属性名
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const string& attr_name);

  /// @brief ハンドラの登録を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] handler 対応付けるハンドラ
  /// @note エラーが起きたら false を返す．
  virtual
  bool
  reg_handler(const string& attr_name,
	      DotLibHandler* handler);

  /// @brief ハンドラを取り出す．
  /// @param[in] attr_name 属性名
  /// @note なければ NULL を返す．
  virtual
  DotLibHandler*
  find_handler(const string& name);


private:
  //////////////////////////////////////////////////////////////////////
  // SimpleHandler の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値を表すトークン
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& name,
	     Token token) = 0;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_SIMPLEHANDLER_H
