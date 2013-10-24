#ifndef YM_LOGIC_SMTSORT_H
#define YM_LOGIC_SMTSORT_H

/// @file ym_logic/SmtSort.h
/// @brief SmtSort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtSort SmtSort.h "ym_logic/SmtSort.h"
/// @brief 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSort
{
  friend class SmtSortMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SmtSort() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief 複合型の場合の要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  virtual
  const SmtSort*
  elem(ymuint pos) const = 0;

  /// @brief ハッシュ値を返す．
  virtual
  ymuint
  hash() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // YM_LOGIC_SMTSORT_H
