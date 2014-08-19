#ifndef SORTELEM_H
#define SORTELEM_H

/// @file SortElem.h
/// @brief SortElem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "smtlibv2_nsdef.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SortElem SortElem.h "SortElem.h"
/// @brief 型テンプレートの要素クラスの基底クラス
//////////////////////////////////////////////////////////////////////
class SortElem
{
  friend class SortMgr;

protected:

  /// @brief デストラクタ
  virtual
  ~SortElem();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型の場合に true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
  /// @note is_param() == true の時のみ意味を持つ．
  virtual
  ymuint
  param_id() const;

  /// @brief 名前を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SmtId*
  name() const;

  /// @brief 要素数を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const SortElem*
  elem(ymuint pos) const;

};


//////////////////////////////////////////////////////////////////////
/// @class ParamSort SortElem.h "SortElem.h"
/// @brief パラメータ型を表すクラス
//////////////////////////////////////////////////////////////////////
class ParamSort :
  public SortElem
{
  friend class SortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] param_id パラメータ番号
  ParamSort(ymuint param_id);

  /// @brief デストラクタ
  virtual
  ~ParamSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief パラメータ型の場合に true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
  /// @note is_param() == true の時のみ意味を持つ．
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


//////////////////////////////////////////////////////////////////////
/// @class SimpleSort SortElem.h "SortElem.h"
/// @brief 単純な型テンプレートを表すクラス
//////////////////////////////////////////////////////////////////////
class SimpleSort :
  public SortElem
{
  friend class SortMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  SimpleSort(const SmtId* name_id);

  /// @brief デストラクタ
  virtual
  ~SimpleSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  /// @note is_param() == false の時のみ意味を持つ．
  virtual
  const SmtId*
  name() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const SmtId* mName;

};


//////////////////////////////////////////////////////////////////////
/// @class ComplexSort SortElem.h "SortElem.h"
/// @brief 通常の型テンプレートを表すクラス
//////////////////////////////////////////////////////////////////////
class ComplexSort :
  public SimpleSort
{
  friend class SortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name_id 名前を表す識別子
  /// @param[in] elem_list 要素の型のリスト
  ComplexSort(const SmtId* name_id,
	      const vector<const SortElem*>& elem_list);

  /// @brief デストラクタ
  virtual
  ~ComplexSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  virtual
  ymuint
  elem_num() const;

  /// @brief 要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num() )
  virtual
  const SortElem*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mElemNum;

  // 要素のリスト
  const SortElem* mElemList[1];

};

END_NAMESPACE_YM_SMTLIBV2

#endif // SORTELEM_H
