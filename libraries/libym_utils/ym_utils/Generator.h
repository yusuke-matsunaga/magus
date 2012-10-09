#ifndef YM_UTILS_GENERATOR_H
#define YM_UTILS_GENERATOR_H

/// @file ym_utils/Generator.h
/// @brief 組み合わせ生成器と順列生成器のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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

class CombiGenIterator;
class PermGenIterator;
class MultiCombiGenIterator;
class MultiPermGenIterator;

//////////////////////////////////////////////////////////////////////
/// @class GenBase Generator.h "ym_utils/Generator.h"
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
/// @class GenIterator Generator.h "ym_utils/Generator.h"
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
/// @class CombiGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  typedef CombiGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選び出す要素数
  CombiGen(ymuint n,
	   ymuint k);

  /// @brief デストラクタ
  ~CombiGen();


public:

  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class CombiGenIterator Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen の反復子
//////////////////////////////////////////////////////////////////////
class CombiGenIterator :
  public GenIterator
{
  friend class CombiGen;

public:

  /// @brief 空のコンストラクタ
  CombiGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  CombiGenIterator(const CombiGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const CombiGenIterator&
  operator=(const CombiGenIterator& src);

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  CombiGenIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親の CombiGen オブジェクト
  /// @note CombiGen が用いる．
  CombiGenIterator(const CombiGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint pos);

};


//////////////////////////////////////////////////////////////////////
/// @class PermGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  typedef PermGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 選択する要素数
  PermGen(ymuint n,
	  ymuint k);

  /// @brief デストラクタ
  ~PermGen();


public:

  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class PermGenIterator Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief PermGen の反復子
//////////////////////////////////////////////////////////////////////
class PermGenIterator :
  public GenIterator
{
  friend class PermGen;

public:

  /// @brief 空のコンストラクタ
  PermGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  PermGenIterator(const PermGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const PermGenIterator&
  operator=(const PermGenIterator& src);


public:

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  PermGenIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親の PermGen オブジェクト
  /// @note PermGen が用いる．
  PermGenIterator(const PermGen* parent);

};


//////////////////////////////////////////////////////////////////////
/// @class MultiGenBase Generator.h "ym_utils/Generator.h"
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
/// @class MultiGenIterator Generator.h "ym_utils/Generator.h"
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
/// @class MultiCombiGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  typedef MultiCombiGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief デストラクタ
  ~MultiCombiGen();


public:

  /// @brief 最初の組み合わせを取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGenIterator Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen の反復子
//////////////////////////////////////////////////////////////////////
class MultiCombiGenIterator :
  public MultiGenIterator
{
  friend class MultiCombiGen;

public:

  /// @brief 空のコンストラクタ
  MultiCombiGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiCombiGenIterator(const MultiCombiGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiCombiGenIterator&
  operator=(const MultiCombiGenIterator& src);


public:

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MultiCombiGenIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  /// @brief MultiCombiGen が用いる．
  MultiCombiGenIterator(const MultiCombiGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint g,
       ymuint pos);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(ymuint grp) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MultiPermGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  typedef MultiPermGenIterator iterator;

public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiPermGen(const vector<pair<ymuint, ymuint> >& nk_array);

  /// @brief デストラクタ
  ~MultiPermGen();


public:

  /// @brief 最初の順列を取り出す．
  iterator
  begin();

};


//////////////////////////////////////////////////////////////////////
/// @class MultiPermGenIterator Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief MultiPermGen の反復子
//////////////////////////////////////////////////////////////////////
class MultiPermGenIterator :
  public MultiGenIterator
{
  friend class MultiPermGen;

public:

  /// @brief 空のコンストラクタ
  MultiPermGenIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiPermGenIterator(const MultiPermGenIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiPermGenIterator&
  operator=(const MultiPermGenIterator& src);

  /// @brief 次の要素を求める．
  /// @return 次の要素を指す反復子
  MultiPermGenIterator
  operator++();

  /// @brief 末尾のチェック
  /// @return 末尾の時に true を返す．
  bool
  is_end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] parent 親のオブジェクト
  /// @brief MultiPermGen が用いる．
  MultiPermGenIterator(const MultiPermGen* parent);

  /// @brief operator++() のサブルーティン
  void
  next(ymuint pos);

  /// @brief grp 番目のグループが終了状態の時 true を返す．
  bool
  is_end_sub(ymuint grp) const;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

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

END_NAMESPACE_YM

#endif // YM_UTILS_GENERATOR_H
