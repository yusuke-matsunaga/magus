#ifndef YM_UTILS_MULTISETGENBASE_H
#define YM_UTILS_MULTISETGENBASE_H

/// @file MultiSetGenBase.h
/// @brief MultiSetGenBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetGenBase MultiSetGenBase.h "MultiSetGenBase.h"
/// @brief MultiSetCombiGen と MultiSetPermGen の親クラス
//////////////////////////////////////////////////////////////////////
class MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] num_array 各要素の重複度を納めた配列
  /// @param[in] k 選び出す要素数
  MultiSetGenBase(const vector<ymuint>& num_array,
		  ymuint k);

  /// @brief デストラクタ
  ~MultiSetGenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の種類の数を得る．
  ymuint
  group_num() const;

  /// @brief 各要素の重複度を得る．
  /// @param[in] grp ( 0 <= grp < group_num() )
  ymuint
  n(ymuint grp) const;

  /// @brief 選択する要素数を返す．
  ymuint
  k() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の重複度
  vector<ymuint32> mNumArray;

  // 選択する要素数
  ymuint32 mK;

};


//////////////////////////////////////////////////////////////////////
/// @class MsGenIterator MultiSetGenBase.h "MultiSetGenBase.h"
/// @brief MultiSetCombiGen::iterator と MultiSetPermGen::iterator の親クラス
//////////////////////////////////////////////////////////////////////
class MsGenIterator
{
protected:

  /// @brief 空のコンストラクタ
  MsGenIterator();

  /// @brief コピーコンストラクタ
  MsGenIterator(const MsGenIterator& src);

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  MsGenIterator(const MultiSetGenBase* parent);

  /// @brief デストラクタ
  ~MsGenIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置
  ymuint
  operator()(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親を返す．
  const MultiSetGenBase*
  parent() const;

  /// @brief 内容をコピーする関数
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const MsGenIterator& src);

  /// @brief グループ数を得る．
  ymuint
  group_num() const;

  /// @brief グループの要素数を得る．
  /// @param[in] grp_id グループ番号 ( 0 <= grp_id < group_num() )
  /// @return 要素数
  ymuint
  n(ymuint grp_id) const;

  /// @brief 順列/組合わせ数を得る．
  /// @return 順列/組み合わせ数
  ymuint
  k() const;

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素
  /// @note operator() の別名
  ymuint
  elem(ymuint pos) const;

  /// @brief 要素の参照の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素への参照
  ymuint&
  elem(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の要素
  vector<ymuint32> mElem;

  // 親の MultiSetGenBase
  const MultiSetGenBase* mParent;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] num_array 各要素の重複度を納めた配列
// @param[in] k 選び出す要素数
inline
MultiSetGenBase::MultiSetGenBase(const vector<ymuint>& num_array,
				 ymuint k) :
  mNumArray(num_array),
  mK(k)
{
}

// @brief デストラクタ
inline
MultiSetGenBase::~MultiSetGenBase()
{
}

// @brief 要素の種類の数を得る．
inline
ymuint
MultiSetGenBase::group_num() const
{
  return mNumArray.size();
}

// @brief 各要素の重複度を得る．
// @param[in] grp ( 0 <= grp < group_num() )
inline
ymuint
MultiSetGenBase::n(ymuint grp) const
{
  assert_cond( grp < group_num(), __FILE__, __LINE__);
  return mNumArray[grp];
}

// @brief 選択する要素数を返す．
inline
ymuint
MultiSetGenBase::k() const
{
  return mK;
}

// @brief 空のコンストラクタ
inline
MsGenIterator::MsGenIterator()
{
  mParent = NULL;
}

// @brief コピーコンストラクタ
inline
MsGenIterator::MsGenIterator(const MsGenIterator& src) :
  mElem(src.mElem),
  mParent(src.mParent)
{
}

// @brief デストラクタ
inline
MsGenIterator::~MsGenIterator()
{
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置
inline
ymuint
MsGenIterator::operator()(ymuint pos) const
{
  return elem(pos);
}

// @brief 親を返す．
inline
const MultiSetGenBase*
MsGenIterator::parent() const
{
  return mParent;
}

// @brief グループ数を得る．
inline
ymuint
MsGenIterator::group_num() const
{
  assert_cond( mParent != NULL, __FILE__, __LINE__);
  return mParent->group_num();
}

// @brief グループの要素数を得る．
// @param[in] grp_id グループ番号 ( 0 <= grp_id < group_num() )
// @return 要素数
inline
ymuint
MsGenIterator::n(ymuint grp_id) const
{
  assert_cond( mParent != NULL, __FILE__, __LINE__);
  return mParent->n(grp_id);
}

// @brief 順列/組合わせ数を得る．
// @return 順列/組み合わせ数
inline
ymuint
MsGenIterator::k() const
{
  assert_cond( mParent != NULL, __FILE__, __LINE__);
  return mParent->k();
}

// @brief 要素の取得
// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
// @return pos 番目の要素
// @note operator() の別名
inline
ymuint
MsGenIterator::elem(ymuint pos) const
{
  return mElem[pos];
}

// @brief 要素の参照の取得
// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
// @return pos 番目の要素への参照
inline
ymuint&
MsGenIterator::elem(ymuint pos)
{
  return mElem[pos];
}

END_NAMESPACE_YM

#endif // YM_UTILS_MULTISETGENBASE_H
