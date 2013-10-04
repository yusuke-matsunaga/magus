#ifndef SMTCPLXSORT_H
#define SMTCPLXSORT_H

/// @file SmtCplxSort.h
/// @brief SmtCplxSort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtCplxSort SmtCplxSort.h "SmtCplxSort.h"
/// @brief 複合型の SmtSort
//////////////////////////////////////////////////////////////////////
class SmtCplxSort :
  public SmtSort
{
  friend class SmtSortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  SmtCplxSort(const SmtId* name);

  /// @brief デストラクタ
  virtual
  ~SmtCplxSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 複合型の場合の要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  virtual
  const SmtSort*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mElemNum;

  // 要素の型の配列
  // 実際には必要な領域を確保する．
  const SmtSort* mElemList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SMTCPLXSORT_H
