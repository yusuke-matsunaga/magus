#ifndef LIBYM_CELL_DOTLIB_PINHOLDERHANDLER_H
#define LIBYM_CELL_DOTLIB_PINHOLDERHANDLER_H

/// @file libym_cell/dotlib/PinHolderHandler.h
/// @brief PinHolderHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PinHolderHandler PinHolderHandler.h "PinHolderHandler.h"
/// @brief group attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class PinHolderHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  PinHolderHandler(DotlibParser& parser,
		   PtMgr& ptmgr) :
    GroupHandler(parser, ptmgr) { }

  /// @brief デストラクタ
  virtual
  ~PinHolderHandler() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンを追加する．
  virtual
  void
  add_pin(PtPin* pin) = 0;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PINHOLDERHANDLER_H
