#ifndef LIBYM_CELL_DOTLIB_PTDUMPER_H
#define LIBYM_CELL_DOTLIB_PTDUMPER_H

/// @file libym_cell/dotlib/PtDumper.h
/// @brief PtDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtDumper PtDumper.h "PtDumper.h"
/// @brief PtNode の内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class PtDumper
{
public:

  /// @brief コンストラクタ
  PtDumper();

  /// @brief デストラクタ
  ~PtDumper();


public:

  /// @brief root 以下の内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] root パース木のノード
  void
  operator()(ostream& s,
	     const PtLibrary* root);

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTDUMPER_H
