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

//////////////////////////////////////////////////////////////////////
/// @class SmtSort SmtSort.h "ym_smtlibv2/SmtSort.h"
/// @brief 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SmtSort
{
public:

  /// @brief 単純な形のコンストラクタ
  /// @param[in] name 型名
  explicit
  SmtSort(const SmtIdentifier* name);

  /// @brief 複合型のコンストラクタ
  /// @param[in] name 型名
  /// @param[in] elem_list 部品の型のリスト
  SmtSort(const SmtIdentifier* name,
	  const vector<const SmtSort*>& elem_list);

  /// @brief デストラクタ
  ~SmtSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  const SmtIdentifier*
  name() const;

  /// @brief 複合型の場合の要素数を返す．
  /// @note 単純な型の場合には 0 を返す．
  ymuint
  elem_num() const;

  /// @brief 複合型の場合の要素の型を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
  const SmtSort*
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const SmtIdentifier* mName;

  // 部品の型のリスト
  vector<const SmtSort*> mElemList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 単純な形のコンストラクタ
// @param[in] name 型名
inline
SmtSort::SmtSort(const SmtIdentifier* name) :
  mName(name)
{
}

// @brief 複合型のコンストラクタ
// @param[in] name 型名
// @param[in] elem_list 部品の型のリスト
inline
SmtSort::SmtSort(const SmtIdentifier* name,
		 const vector<const SmtSort*>& elem_list) :
  mName(name),
  mElemList(elem_list)
{
}

// @brief デストラクタ
inline
SmtSort::~SmtSort()
{
}

// @brief 名前を返す．
inline
const SmtIdentifier*
SmtSort::name() const
{
  return mName;
}

// @brief 複合型の場合の要素数を返す．
// @note 単純な型の場合には 0 を返す．
inline
ymuint
SmtSort::elem_num() const
{
  return mElemList.size();
}

// @brief 複合型の場合の要素の型を返す．
// @param[in] pos 位置番号 ( 0 <= pos < elem_num )
inline
const SmtSort*
SmtSort::elem(ymuint pos) const
{
  assert_cond( pos < elem_num(), __FILE__, __LINE__);
  return mElemList[pos];
}

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSORT_H
