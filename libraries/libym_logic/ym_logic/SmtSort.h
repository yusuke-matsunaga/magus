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
/// 実際には，このクラスでパラメータ付きの型を表す．
/// 例えば，
/// (define-sort Pair 2)
/// (declare-sort P ( X ) (Pair X X))
/// というコマンド中に現れる (Pair X X) は実際の型ではなく，
/// X という型パラメータを使ったパラメータ付きの型である．
/// 実装としては型パラメータを表す型の場合には
/// is_param() が true を返すようにする．
/// また，param_id() が declare_sort 中のパラメータの位置番号となる．
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

  /// @brief パラメータ型のときに true を返す．
  /// 詳しくは上のコメント参照
  virtual
  bool
  is_param() const = 0;

  /// @brief パラメータ番号を返す．
  /// 詳しくは上のコメント参照
  /// is_param() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_id() const = 0;

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


/// @relates SmtSort
/// @brief 内容を表す文字列を返す．
extern
string
sort_str(const SmtSort* sort);

END_NAMESPACE_YM_SMT

#endif // YM_LOGIC_SMTSORT_H
