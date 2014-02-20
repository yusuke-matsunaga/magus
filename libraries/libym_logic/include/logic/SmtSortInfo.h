#ifndef LOGIC_SMTSORTINFO_H
#define LOGIC_SMTSORTINFO_H

/// @file logic/SmtSortInfo.h
/// @brief SmtSort のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "logic/smt_nsdef.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtSortInfo SmtSortInfo.h "logic/SmtSortInfo.h"
/// @brief 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSortInfo
{
public:

  /// @brief デストラクタ
  virtual
  ~SmtSortInfo() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID 番号を返す．
  /// @note ID 番号はすべての型の中で唯一のもの
  virtual
  tSmtSortId
  id() const = 0;

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief 複合型の場合の要素の型番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  virtual
  tSmtSortId
  elem(ymuint pos) const = 0;

  /// @brief ハッシュ値を返す．
  virtual
  ymuint32
  hash() const = 0;

};

END_NAMESPACE_YM_SMT

#endif // LOGIC_SMTSORTINFO_H
