#ifndef VLWBASE_H
#define VLWBASE_H

/// @file VlwBase.h
/// @brief VlwBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlWriter.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlwBase VlwBase.h "VlwBase.h"
/// @brief VlwXXX の基底クラス
//////////////////////////////////////////////////////////////////////
class VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用のオブジェクト
  VlwBase(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwBase();


public:

  /// @brief 出力用のオブジェクトを得る．
  VlWriter&
  writer();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力用のオブジェクト
  VlWriter& mWriter;

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWBASE_H
