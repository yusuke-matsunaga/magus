#ifndef SMTSORTIMPL_H
#define SMTSORTIMPL_H

/// @file SmtSortImpl.h
/// @brief SmtSortImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_smtlibv2/SmtSort.h"


BEGIN_NAMESPACE_YM_SMTLIBV2

//////////////////////////////////////////////////////////////////////
/// @class SmtSortImpl SmtSortImpl.h "SmtSortImpl.h"
/// @brief SmtSort の実装クラス
//////////////////////////////////////////////////////////////////////
class SmtSortImpl :
  public SmtSort
{
  friend class SmtSortMgr;

protected:

  /// @brief コンストラクタ
  /// @param[in] name 型名
  explicit
  SmtSortImpl(const SmtId* name);

  /// @brief デストラクタ
  virtual
  ~SmtSortImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief パラメータ型のときに true を返す．
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
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

  // レベル
  ymuint32 mLevel;

  // 名前
  const SmtId* mName;

  // ハッシュ用のリンクポインタ
  SmtSortImpl* mLink;

};


//////////////////////////////////////////////////////////////////////
/// @class SmtCplxSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief 複合型の SmtSort
//////////////////////////////////////////////////////////////////////
class SmtCplxSort :
  public SmtSortImpl
{
  friend class SmtSortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] elem_list 要素のリスト
  SmtCplxSort(const SmtId* name,
	      const vector<const SmtSort*>& elem_list);

  /// @brief デストラクタ
  virtual
  ~SmtCplxSort();


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
  // 実際には必要な領域を確保する．
  const SmtSort* mElemList[1];

};


//////////////////////////////////////////////////////////////////////
/// @class SmtParamSort SmtSortImpl.h "SmtSortImpl.h"
/// @brief 型パラメータを表すクラス
//////////////////////////////////////////////////////////////////////
class SmtParamSort :
  public SmtSort
{
  friend class SmtSortMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] pid パラメータ番号
  explicit
  SmtParamSort(ymuint pid);

  /// @brief デストラクタ
  virtual
  ~SmtParamSort();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  const SmtId*
  name() const;

  /// @brief パラメータ型のときに true を返す．
  /// 詳しくは上のコメント参照
  virtual
  bool
  is_param() const;

  /// @brief パラメータ番号を返す．
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

  /// @brief ハッシュ値を返す．
  virtual
  ymuint
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パラメータ番号
  ymuint32 mParamId;

};

END_NAMESPACE_YM_SMTLIBV2

#endif // YM_SMTLIBV2_SMTSORT_H
