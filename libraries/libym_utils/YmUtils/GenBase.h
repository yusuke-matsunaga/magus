#ifndef YMUTILS_GENBASE_H
#define YMUTILS_GENBASE_H

/// @file YmUtils/GenBase.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
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
/// 生成した後，++ 演算子で末尾に達するまで(!is_end())，進めれば良い．
/// 値を取り出すには () 演算子を用いる．


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenBase GenBase.h "YmUtils/GenBase.h"
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

  /// @brief コピーコンストラクタ
  GenBase(const GenBase& src);

  /// @brief デストラクタ
  ~GenBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全要素数を得る．
  /// @return 全要素数
  ymuint
  n() const;

  /// @brief 選択する要素数を得る．
  /// @return 選択する要素数
  ymuint
  k() const;

  /// @brief 最初の要素を指すように初期化する．
  void
  init();

  /// @brief 要素の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素
  ymuint
  operator()(ymuint pos) const;

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をコピーする関数
  /// @param[in] src コピー元のオブジェクト
  void
  copy(const GenBase& src);

  /// @brief 要素の参照の取得
  /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
  /// @return pos 番目の要素への参照
  ymuint&
  elem(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 全要素数
  ymuint32 mN;

  // 選択する要素数
  ymuint32 mK;

  // 現在の要素
  vector<ymuint32> mElem;

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

// pos 番目の要素を取り出す．
inline
ymuint
GenBase::operator()(ymuint pos) const
{
  ASSERT_COND( pos < mK );

  return mElem[pos];
}

// pos 番目の要素への参照を取り出す．
inline
ymuint&
GenBase::elem(ymuint pos)
{
  return mElem[pos];
}

// 末尾の時に true を返す．
inline
bool
GenBase::is_end() const
{
  // 末尾の時には範囲外の値(= n())を持っている．
  return operator()(0) == n();
}

END_NAMESPACE_YM

#endif // YMUTILS_GENBASE_H
