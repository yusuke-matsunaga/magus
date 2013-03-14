#ifndef VARSPLITTER_H
#define VARSPLITTER_H

/// @file VarSplitter.h
/// @brief VarSplitter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Splitter.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class VarSplitter VarSplitter.h "VarSplitter.h"
/// @brief 変数による Splitter
//////////////////////////////////////////////////////////////////////
class VarSplitter :
  public Splitter
{
public:

  /// @brief コンストラクタ
  /// @param[in] varid 変数番号
  VarSplitter(ymuint varid);

  /// @brief デストラクタ
  virtual
  ~VarSplitter();

};

END_NAMESPACE_YM_IGF

#endif // VARSPLITTER_H
