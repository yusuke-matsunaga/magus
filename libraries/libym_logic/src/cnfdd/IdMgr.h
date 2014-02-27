#ifndef IDMGR_H
#define IDMGR_H

/// @file IdMgr.h
/// @brief IdMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "CNFddEdge.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
/// @class IdMgr IdMgr.h "IdMgr.h"
/// @brief CNFDD の節点に番号を割り振るためのクラス
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
  has_id(CNFddEdge e) const;

  /// @brief e の ID 番号を返す．
  /// @note未登録ならば新しい番号を割り振る．
  ymuint64
  id(CNFddEdge e);

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

#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
  typedef unordered_map<CNFddEdge, ymuint64> IdMap;
#else
  typedef hash_map<CNFddEdge, ymuint64> IdMap;
#endif

  // CNFddEdge と ID 番号の対応表
  IdMap mMap;

  // 次に割り当てるID番号
  ymuint64 mNext;

};

END_NAMESPACE_YM_CNFDD

#endif // IDMGR_H
