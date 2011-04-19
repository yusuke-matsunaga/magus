#ifndef LIBYM_CELL_DOTLIB_LATCHHANDLER_H
#define LIBYM_CELL_DOTLIB_LATCHHANDLER_H

/// @file libym_cell/dotlib/LatchHandler.h
/// @brief LatchHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LatchHandler LatchHandler.h "LatchHandler.h"
/// @brief latch グループのハンドラ
//////////////////////////////////////////////////////////////////////
class LatchHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LatchHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~LatchHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] attr_name 属性名
  /// @param[in] group_name グループ名
  virtual
  bool
  read_group_name(const string& attr_name,
		  const string& group_name);

  /// @brief グループ内のステートメントをすべて処理したときに呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LATCHHANDLER_H
