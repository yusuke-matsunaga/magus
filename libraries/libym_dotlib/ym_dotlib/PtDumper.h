#ifndef LIBYM_DOTLIB_PTDUMPER_H
#define LIBYM_DOTLIB_PTDUMPER_H

/// @file libym_dotlib/PtDumper.h
/// @brief PtDumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtDumper PtDumper.h "dotlib/PtDumper.h"
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
	     const DotlibNode* root);

};

END_NAMESPACE_YM_DOTLIB

#endif // LIBYM_DOTLIB_PTDUMPER_H
