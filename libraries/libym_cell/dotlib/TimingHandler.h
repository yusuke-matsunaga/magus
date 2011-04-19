#ifndef LIBYM_CELL_DOTLIB_TIMINGHANDLER_H
#define LIBYM_CELL_DOTLIB_TIMINGHANDLER_H

/// @file libym_cell/dotlib/TimingHandler.h
/// @brief TimingHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class TimingHandler TimingHandler.h "TimingHandler.h"
/// @brief timing グループのハンドラ
//////////////////////////////////////////////////////////////////////
class TimingHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  TimingHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~TimingHandler();


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
/// @class CellFallHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_fall グループのハンドラ
//////////////////////////////////////////////////////////////////////
class CellFallHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CellFallHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~CellFallHandler();


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
/// @class CellRiseHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class CellRiseHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  CellRiseHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~CellRiseHandler();


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
/// @class FallTransitionHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class FallTransitionHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  FallTransitionHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~FallTransitionHandler();


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
/// @class RiseTransitionHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class RiseTransitionHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RiseTransitionHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~RiseTransitionHandler();


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
/// @class FallConstraintHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class FallConstraintHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  FallConstraintHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~FallConstraintHandler();


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
/// @class RiseConstraintHandler TimingHandler.h "TimingHandler.h"
/// @brief cell_rise グループのハンドラ
//////////////////////////////////////////////////////////////////////
class RiseConstraintHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  RiseConstraintHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~RiseConstraintHandler();


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

#endif // LIBYM_CELL_DOTLIB_TIMINGHANDLER_H
