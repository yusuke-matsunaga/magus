#ifndef IDMGR_H
#define IDMGR_H

/// @file IdMgr.h
/// @brief IdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrImpl.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDD の節点に番号を割り振るためのクラス
//////////////////////////////////////////////////////////////////////
class IdMgr
{
public:

  /// @brief コンストラクタ
  IdMgr();

  /// @brief デストラクタ
  ~IdMgr();


public:

  /// @brief e がすでに ID 番号を持っていたら true を返す．
  bool
  has_id(BddEdge e) const;

  /// @brief e の ID 番号を返す．
  /// @note未登録ならば新しい番号を割り振る．
  ymuint64
  id(BddEdge e);

  /// @brief 登録されている節点数を返す．
  ymuint64
  num() const;

  /// @brief クリアする．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BddEdge と ID 番号の対応表
  HashMap<BddEdge, ymuint64> mMap;

  // 次に割り当てるID番号
  ymuint64 mNext;

};

END_NAMESPACE_YM_BDD

#endif // IDMGR_H
