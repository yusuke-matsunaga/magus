#ifndef SORTTEMPL_H
#define SORTTEMPL_H

/// @file SortTempl.h
/// @brief SortTempl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SortTempl SortTempl.h "SortTempl.h"
/// @brief SmtSort を作るためのテンプレートクラス
//////////////////////////////////////////////////////////////////////
class SortTempl
{
  friend class SortMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SortTempl() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型の場合に true を返す．
  virtual
  bool
  is_param() const = 0;

  /// @brief パラメータ番号を返す．
  /// @note is_param() == true の時のみ意味を持つ．
  virtual
  ymuint
  param_id() const = 0;

  /// @brief 名前を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SmtId*
  name() const = 0;

  /// @brief パラメータ数を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  ymuint
  param_num() const = 0;

  /// @brief 要素数を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  ymuint
  elem_num() const = 0;

  /// @brief 要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SortTempl*
  elem(ymuint pos) const = 0;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SORTTEMPL_H
