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
///
/// 通常，型そのものに意味はないが，いくつかの型は特定の Theory に
/// 関して特別な意味を持つ．
///
///  - Bool : すべての Theory において定義される．
///  - Int  : INTS Theory において定義される．
///  - Real : REALS Theory において定義される．
//////////////////////////////////////////////////////////////////////
class SmtSort
{
  friend class SmtSortMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を表す列挙型
  enum tType {
    kNone,
    kBool,
    kInt,
    kReal
  };

protected:

  /// @brief デストラクタ
  virtual
  ~SmtSort() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を返す．
  /// @note 普通の型は kNone を返す．
  virtual
  tType
  type() const = 0;

  /// @brief ID 番号を返す．
  /// @note ID 番号はすべての型の中で唯一のもの
  virtual
  ymuint
  id() const = 0;

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
