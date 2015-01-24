#ifndef YMSLFALSE_H
#define YMSLFALSE_H

/// @file YmslFalse.h
/// @brief YmslFalse のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFalse YmslFalse.h "YmslFalse.h"
/// @brief false を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslFalse :
  public YmslLeaf
{
public:

  /// @brief コンストラクタ
  YmslFalse();

  /// @brief デストラクタ
  virtual
  ~YmslFalse();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  LeafType
  leaf_type() const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFALSE_H
