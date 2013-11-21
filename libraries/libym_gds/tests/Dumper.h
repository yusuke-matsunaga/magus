#ifndef DUMPER_H
#define DUMPER_H

/// @file Dumper.h
/// @brief Dumper のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class Dumper Dumper.h "Dumper.h"
/// @brief GDS-II のパース木の内容を表示するクラス
//////////////////////////////////////////////////////////////////////
class Dumper
{
public:

  /// @brief コンストラクタ
  Dumper();

  /// @brief デストラクタ
  ~Dumper();


public:

  /// @brief 内容を表示する．
  void
  operator()(ostream& s,
	     const GdsData& data);

};

END_NAMESPACE_YM_GDS

#endif // DUMPER_H
