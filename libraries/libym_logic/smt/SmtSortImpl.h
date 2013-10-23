#ifndef SMTSORTIMPL_H
#define SMTSORTIMPL_H

/// @file SmtSortImpl.h
/// @brief SmtSortImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SmtSort.h"


BEGIN_NAMESPACE_YM_SMT

//////////////////////////////////////////////////////////////////////
/// @class SmtSortImpl SmtSortImpl.h "SmtSortImpl.h"
/// @brief SmtSort の実装クラス
/// @note 単純な型を表すクラスでもある．
//////////////////////////////////////////////////////////////////////
class SmtSortImpl :
  public SmtSort
{
  friend class SmtSortMgr;

protected:

  /// @brief コンストラクタ
  SmtSortImpl();

  /// @brief デストラクタ
  virtual
  ~SmtSortImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型のときに true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
  /// is_param() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_id() const;

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

};


//////////////////////////////////////////////////////////////////////
/// @class SmtComplexSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief 複合型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtComplexSort :
  public SmtSortImpl
{
  friend class SmtSortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] elem_list 要素の型のリスト
  SmtComplexSort(const vector<const SmtSort*>& elem_list);

  /// @brief デストラクタ
  virtual
  ~SmtComplexSort();


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
  const SmtSort* mElemList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtParamSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief パラメータ型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtParamSort :
  public SmtSortImpl
{
  friend class SmtSortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] param_id パラメータ番号
  SmtParamSort(ymuint param_id);

  /// @brief デストラクタ
  virtual
  ~SmtParamSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型のときに true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
  /// is_param() == false の場合にはエラーとなる．
  virtual
  ymuint
  param_id() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ番号
  ymuint32 mParamId;

};

END_NAMESPACE_YM_SMT

#endif // SMTSORTIMPL_H
