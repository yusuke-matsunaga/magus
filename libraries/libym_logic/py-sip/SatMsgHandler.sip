
/// @file SatMsgHandler.sip
/// @brief SatMsgHandler の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


class SatMsgHandler
{
%TypeHeaderCode
#include "YmLogic/SatMsgHandler.h"
using nsYm::SatMsgHandler;
%End

public:

  /// @brief ヘッダの出力
  virtual
  void
  print_header();

  /// @brief 内部状態の出力を行う仮想関数
  /// @param[in] stats SAT ソルバの内部状態
  virtual
  void
  print_message(const SatStats& stats);

  /// @brief フッタの出力
  /// @param[in] stats SAT ソルバの内部状態
  virtual
  void
  print_footer(const SatStats& stats);

};
