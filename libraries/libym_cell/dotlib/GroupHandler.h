#ifndef LIBYM_CELL_DOTLIB_GROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_GROUPHANDLER_H

/// @file libym_cell/dotlib/GroupHandler.h
/// @brief GroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotLibHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class GroupHandler DotLibHandler.h "DotLibHandler.h"
/// @brief group attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class GroupHandler :
  public DotLibHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  GroupHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~GroupHandler();


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
  // GroupHandler の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] name 属性名
  /// @param[in] group_name グループ名
  virtual
  bool
  read_group_name(const char* name,
		  const string& group_name) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラの登録を行う．
  void
  reg_handler(const char* name,
	      DotLibHandler* handler);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハンドラの連想配列
  hash_map<string, DotLibHandler*> mHandlerMap;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H
