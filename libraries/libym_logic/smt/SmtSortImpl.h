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
  friend class SmtSolverImpl;

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

  /// @brief ID 番号を返す．
  /// @note ID 番号はすべての型の中で唯一のもの
  virtual
  tSmtSortId
  id() const;

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 複合型の場合の要素の型番号を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  virtual
  tSmtSortId
  elem(ymuint pos) const;

  /// @brief ハッシュ値を返す．
  virtual
  ymuint32
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  tSmtSortId mId;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtSimpleSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief 単純な型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSimpleSort :
  public SmtSortImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  SmtSimpleSort();

  /// @brief デストラクタ
  virtual
  ~SmtSimpleSort();

};


//////////////////////////////////////////////////////////////////////
/// @class SmtComplexSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief 複合型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtComplexSort :
  public SmtSortImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] elem_list 要素の型のリスト
  SmtComplexSort(const vector<tSmtSortId>& elem_list);

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
  tSmtSortId
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mElemNum;

  // 要素の型の配列
  tSmtSortId mElemList[1];

};

END_NAMESPACE_YM_SMT

#endif // SMTSORTIMPL_H
