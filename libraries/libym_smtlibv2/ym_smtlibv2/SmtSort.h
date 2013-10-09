#ifndef YM_SMTLIBV2_SMTSORT_H
#define YM_SMTLIBV2_SMTSORT_H

/// @file ym_smtlibv2/SmtSort.h
/// @brief SmtSort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

class SmtId;

//////////////////////////////////////////////////////////////////////
/// @class SmtSort SmtSort.h "ym_smtlibv2/SmtSort.h"
/// @brief 型を表すクラス
///
/// 実際には，このクラスでパラメータ付きの型を表す．
/// 例えば，
/// (define-sort Pair 2)
/// (declare-sort P ( X ) (Pair X X))
/// というコマンド中に現れる (Pair X X) は実際の型ではなく，
/// X という型パラメータを使ったパラメータ付きの型である．
/// 実装としては型パラメータを表す型の場合には
/// 非負の param_id() を返すようにする．
/// この値が declare_sort 中のパラメータの位置番号となる．
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

#if 0
  /// @brief ID番号を返す．
  virtual
  ymuint
  id() const = 0;

  /// @brief レベルを返す．
  virtual
  ymuint
  level() const = 0;
#endif

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief パラメータ番号を返す．
  /// @note 通常の型の場合は -1 を返す．
  /// 詳しくは上のコメント参照
  virtual
  ymint
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

  /// @brief 型テンプレートを返す．
  /// @note 通常は自分自身を返すが alias の場合は実体を返す．
  virtual
  const SmtSort*
  sort_template() const = 0;

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

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSORT_H
