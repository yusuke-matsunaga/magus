#ifndef YM_UTILS_MULTIGENBASE_H
#define YM_UTILS_MULTIGENBASE_H

/// @file ym_utils/MultiGenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiGenBase MultiGenBase.h "ym_utils/MultiGenBase.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen/MultiPermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenBase
{
public:

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiGenBase(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief デストラクタ
  ~MultiGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ数の取得
  /// @return グループ数
  ymuint
  group_num() const;

  /// @brief 全要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの全要素数
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの選択する要素数
  ymuint
  k(ymuint grp) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各グループごとの全要素数を選択する要素数を入れる配列
  vector<pair<ymuint, ymuint> > mNkArray;

};


//////////////////////////////////////////////////////////////////////
/// @class MultiGenIterator MultiGenBase.h "ym_utils/MultiGenBase.h"
/// @ingroup GeneratorGroup
/// @brief MultiPermGenIterator と MultiCombiGenIterator の基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenIterator
{
protected:

  /// @brief 空のコンストラクタ
  MultiGenIterator();

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  /// @note 継承クラスが用いる．
  MultiGenIterator(const MultiGenBase* parent);

  /// @brief デストラクタ
  ~MultiGenIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の取得
  /// @param[in] grp グループ番号
  /// @param[in] pos 要素の位置
  /// @return grp 番目のグループの pos 番目の要素を取り出す．
  ymuint
  operator()(ymuint grp,
	     ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コピーする．
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MultiGenIterator& src);

  /// @brief グループ数を得る．
  /// @return グループ数
  ymuint
  group_num() const;

  /// @brief 要素配列の初期化
  /// @param[in] grp グループ番号
  /// @note grp 番目のグループの要素配列を初期化する．
  void
  init(ymuint grp);

  /// @brief 要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの全要素数
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの選択する要素数
  ymuint
  k(ymuint grp) const;

  /// @brief 要素配列の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの要素配列
  vector<ymuint>&
  elem(ymuint grp);

  /// @brief 要素配列の取得
  /// @param[in] grp グループ番号
  /// @return grp 番目のグループの要素配列
  /// @note こちらは const 版
  const vector<ymuint>&
  elem(ymuint grp) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 確保したメモリを解放する
  void
  free();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の要素(二重の配列なので少しめんどくさい)
  vector<vector<ymuint>*> mElemArray;

  // 親の MultiGenBase
  const MultiGenBase* mParent;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
inline
MultiGenBase::MultiGenBase(const vector<pair<ymuint, ymuint> >& nk_array) :
  mNkArray(nk_array)
{
}

// デストラクタ
inline
MultiGenBase::~MultiGenBase()
{
}

// グループ数を得る．
inline
ymuint
MultiGenBase::group_num() const
{
  return mNkArray.size();
}

// grp 番目のグループの全要素数を得る．
inline
ymuint
MultiGenBase::n(ymuint grp) const
{
  return mNkArray[grp].first;
}

// grp 番目のグループの選択する要素数を得る．
inline
ymuint
MultiGenBase::k(ymuint grp) const
{
  return mNkArray[grp].second;
}

// 空のコンストラクタ
inline
MultiGenIterator::MultiGenIterator() :
  mElemArray(0),
  mParent(NULL)
{
}

// grp 番目のグループの pos 番目の要素を取り出す．
inline
ymuint
MultiGenIterator::operator()(ymuint grp,
			     ymuint pos) const
{
  return (*mElemArray[grp])[pos];
}

// グループ数を得る．
inline
ymuint
MultiGenIterator::group_num() const
{
  return mParent->group_num();
}

// grp 番目のグループの全要素数を得る．
inline
ymuint
MultiGenIterator::n(ymuint grp) const
{
  return mParent->n(grp);
}

// grp 番目のグループの選択する要素数を得る．
inline
ymuint
MultiGenIterator::k(ymuint grp) const
{
  return mParent->k(grp);
}

// grp 番目のグループの要素配列を得る．
inline
vector<ymuint>&
MultiGenIterator::elem(ymuint g)
{
  assert_cond(mElemArray[g], __FILE__, __LINE__);
  return *mElemArray[g];
}

// grp 番目のグループの要素配列を得る．
// こちらは const 版
inline
const vector<ymuint>&
MultiGenIterator::elem(ymuint g) const
{
  return *mElemArray[g];
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTIGENBASE_H
