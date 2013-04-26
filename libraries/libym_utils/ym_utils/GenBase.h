#ifndef YM_UTILS_GENBASE_H
#define YM_UTILS_GENBASE_H

/// @file ym_utils/GenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup GeneratorGroup 組み合わせ生成器と順列生成器
/// @ingroup YmUtils
///
/// - n 個の中から k 個の組み合わせを選びだす CombiGen
/// - n 個の中から k 個の順列を選びだす PermGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の組合わせを選びだす
///   MultiCombiGen
/// - (n1, n2, ...) 個の中から (k1, k2, ...) 個の順列を選びだす
///   MultiPermGen
///
/// 使いかたは全てのクラスで共通で，コンストラクタでオブジェクトを
/// 生成した後，メンバ関数の begin() で反復子を取り出し，その反復子が
/// 末尾に達するまで(!is_end())，繰り返せばよい．
/// 反復子から値を取り出すには () 演算子を用いる．


#include "ymtools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenBase GenBase.h "ym_utils/GenBase.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen と PermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class GenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  GenBase(ymuint n,
	  ymuint k);

  /// @brief デストラクタ
  ~GenBase();


public:

  /// @brief 全要素数を得る．
  /// @return 全要素数
  ymuint
  n() const;

  /// @brief 選択する要素数を得る．
  /// @return 選択する要素数
  ymuint
  k() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全要素数
  ymuint32 mN;

  // 選択する要素数
  ymuint32 mK;

};


//////////////////////////////////////////////////////////////////////
/// @class GenIterator GenBase.h "ym_utils/GenBase.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen::iterator と PermGen::iterator の基底クラス
//////////////////////////////////////////////////////////////////////
class GenIterator
{
protected:

  /// @brief 空のコンストラクタ
  /// @note なにも指さない．
  GenIterator();

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  /// @note 継承クラスが用いる．
  GenIterator(const GenBase* parent);

  /// @brief デストラクタ
  ~GenIterator();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素
  ymuint
  operator()(ymuint pos) const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする関数
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const GenIterator& src);

  /// @brief 全要素数を得る．
  /// @return 全要素数
  ymuint
  n() const;

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

  // 親の GenBase
  const GenBase* mParent;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 全要素数を得る．
inline
ymuint
GenBase::n() const
{
  return mN;
}

// 選択する要素数を得る．
inline
ymuint
GenBase::k() const
{
  return mK;
}

// 全要素数を得る．
inline
ymuint
GenIterator::n() const
{
  return mParent->n();
}

// 順列/組合わせ数を得る．
inline
ymuint
GenIterator::k() const
{
  return mParent->k();
}

// pos 番目の要素を取り出す．
inline
ymuint
GenIterator::operator()(ymuint pos) const
{
  return elem(pos);
}

// pos 番目の要素を取り出す．
inline
ymuint
GenIterator::elem(ymuint pos) const
{
  return mElem[pos];
}

// pos 番目の要素への参照を取り出す．
inline
ymuint&
GenIterator::elem(ymuint pos)
{
  return mElem[pos];
}

END_NAMESPACE_YM

#endif // YM_UTILS_GENBASE_H
