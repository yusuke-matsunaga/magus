#ifndef LIBYM_CELL_DOTLIB_DUMMYCOMPLEXHANDLER_H
#define LIBYM_CELL_DOTLIB_DUMMYCOMPLEXHANDLER_H

/// @file libym_cell/dotlib/DummyComplexHandler.h
/// @brief DummyComplexHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DummyComplexHandler DummyComplexHandler.h "DummyComplexHandler.h"
/// @brief ダミーの ComplexHandler
//////////////////////////////////////////////////////////////////////
class DummyComplexHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  DummyComplexHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~DummyComplexHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値の型と値を表す文字列のペアのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const list<pair<tTokenType, string> >& value_list);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DUMMYCOMPLEXHANDLER_H
