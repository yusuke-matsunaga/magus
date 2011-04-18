#ifndef LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H
#define LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H

/// @file libym_cell/dotlib/ComplexHandler.h
/// @brief ComplexHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class ComplexHandler DotLibHandler.h "DotLibHandler.h"
/// @brief complex attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class ComplexHandler :
  public DotLibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  ComplexHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~ComplexHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotLibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 構文要素を処理する．
  /// @param[in] name 属性名
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_attr(const char* name);


private:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const char* name,
	     const list<pair<tTokenType, string> >& value_list) = 0;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_COMPLEXHANDLER_H
