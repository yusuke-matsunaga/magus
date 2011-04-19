#ifndef LIBYM_CELL_DOTLIB_DEFINEHANDLER_H
#define LIBYM_CELL_DOTLIB_DEFINEHANDLER_H

/// @file libym_cell/dotlib/DefineHandler.h
/// @brief DefineHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ComplexHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class LibraryGroupHandler;

//////////////////////////////////////////////////////////////////////
/// @class DefineHandler DefineHandler.h "DefineHandler.h"
/// @brief ダミーの ComplexHandler
//////////////////////////////////////////////////////////////////////
class DefineHandler :
  public ComplexHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  DefineHandler(LibraryGroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~DefineHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // ComplexHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 属性値の読み込み処理を行う．
  /// @param[in] attr_name 属性名
  /// @param[in] token_list トークンのリスト
  /// @return エラーが起きたら false を返す．
  virtual
  bool
  read_value(const string& attr_name,
	     const vector<Token>& token_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のハンドラ
  LibraryGroupHandler* mParent;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DEFINEHANDLER_H
