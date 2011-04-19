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
  // GroupHandler の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] group_name グループ名
  virtual
  bool
  read_group_name(const string& attr_name,
		  const string& group_name) = 0;

  /// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
  virtual
  bool
  end_group() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハンドラの連想配列
  hash_map<string, DotLibHandler*> mHandlerMap;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_DOTLIBHANDLER_H
