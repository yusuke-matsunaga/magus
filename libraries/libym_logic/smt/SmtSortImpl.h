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
  /// @param[in] id ID番号
  SmtSortImpl(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SmtSortImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を返す．
  /// @note 普通の型は kNone を返す．
  virtual
  tType
  type() const;

  /// @brief ID 番号を返す．
  /// @note ID 番号はすべての型の中で唯一のもの
  virtual
  ymuint
  id() const;

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

  /// @brief ハッシュ値を返す．
  virtual
  ymuint
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtBoolSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief Bool 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtBoolSort :
  public SmtSortImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  SmtBoolSort(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SmtBoolSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を返す．
  /// @note 普通の型は kNone を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtIntSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief Int 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtIntSort :
  public SmtSortImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  SmtIntSort(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SmtIntSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を返す．
  /// @note 普通の型は kNone を返す．
  virtual
  tType
  type() const;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtRealSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief Real 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtRealSort :
  public SmtSortImpl
{
  friend class SmtSolverImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] id ID 番号
  SmtRealSort(ymuint id);

  /// @brief デストラクタ
  virtual
  ~SmtRealSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 組み込み型を返す．
  /// @note 普通の型は kNone を返す．
  virtual
  tType
  type() const;

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
  /// @param[in] id ID番号
  SmtSimpleSort(ymuint id);

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
  /// @param[in] id ID番号
  /// @param[in] elem_list 要素の型のリスト
  SmtComplexSort(ymuint id,
		 const vector<const SmtSort*>& elem_list);

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

END_NAMESPACE_YM_SMT

#endif // SMTSORTIMPL_H
