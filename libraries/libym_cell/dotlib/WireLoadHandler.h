#ifndef LIBYM_CELL_DOTLIB_WIRELOADHANDLER_H
#define LIBYM_CELL_DOTLIB_WIRELOADHANDLER_H

/// @file libym_cell/dotlib/WireLoadHandler.h
/// @brief WireLoadHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class WireLoadHandler WireLoadHandler.h "WireLoadHandler.h"
/// @brief wire_load グループのハンドラ
//////////////////////////////////////////////////////////////////////
class WireLoadHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  WireLoadHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~WireLoadHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] token_list トークンのリスト
  virtual
  bool
  begin_group(const string& attr_name,
	      const vector<Token>& token_list);

  /// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
  virtual
  bool
  end_group();

};


//////////////////////////////////////////////////////////////////////
/// @class WireLoadSelectionHandler WireLoadSelectionHandler.h "WireLoadSelectionHandler.h"
/// @brief wire_load グループのハンドラ
//////////////////////////////////////////////////////////////////////
class WireLoadSelectionHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  WireLoadSelectionHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~WireLoadSelectionHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] token_list トークンのリスト
  virtual
  bool
  begin_group(const string& attr_name,
	      const vector<Token>& token_list);

  /// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
  virtual
  bool
  end_group();

};


//////////////////////////////////////////////////////////////////////
/// @class WireLoadTableHandler WireLoadTableHandler.h "WireLoadTableHandler.h"
/// @brief wire_load グループのハンドラ
//////////////////////////////////////////////////////////////////////
class WireLoadTableHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  WireLoadTableHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~WireLoadTableHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] token_list トークンのリスト
  virtual
  bool
  begin_group(const string& attr_name,
	      const vector<Token>& token_list);

  /// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_WIRELOADHANDLER_H
