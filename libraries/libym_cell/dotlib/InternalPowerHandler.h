#ifndef LIBYM_CELL_DOTLIB_INTERNALPOWERHANDLER_H
#define LIBYM_CELL_DOTLIB_INTERNALPOWERHANDLER_H

/// @file libym_cell/dotlib/InternalPowerHandler.h
/// @brief InternalPowerHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class InternalPowerHandler InternalPowerHandler.h "InternalPowerHandler.h"
/// @brief timing グループのハンドラ
//////////////////////////////////////////////////////////////////////
class InternalPowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  InternalPowerHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~InternalPowerHandler();


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
/// @class PowerHandler InternalPowerHandler.h "InternalPowerHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class PowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  PowerHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~PowerHandler();


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
/// @class FallPowerHandler InternalPowerHandler.h "InternalPowerHandler.h"
/// @brief cell_fall グループのハンドラ
//////////////////////////////////////////////////////////////////////
class FallPowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  FallPowerHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~FallPowerHandler();


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
/// @class RisePowerHandler InternalPowerHandler.h "InternalPowerHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class RisePowerHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RisePowerHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~RisePowerHandler();


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

#endif // LIBYM_CELL_DOTLIB_INTERNALPOWERHANDLER_H
