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

//////////////////////////////////////////////////////////////////////
/// @class GenBase Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief CombiGen と PermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class GenBase
{
public:

  /// @brief 順列/組合わせ列挙するための反復子の基底クラス
  class iterator
  {
  public:
    /// @brief 空のコンストラクタ
    /// @note なにも指さない．
    iterator();

    /// @brief 要素の取得
    /// @param[in] pos 取り出す要素の位置 (最初の位置は 0)
    /// @return pos 番目の要素
    ymuint
    operator()(ymuint pos) const;


  protected:
    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @note 継承クラスが用いる．
    iterator(const GenBase* parent);

    /// @brief 内容をコピーする関数
    /// @param[in] src コピー元のオブジェクト
    void
    copy(const iterator& src);

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
/// @class CombiGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  /// @brief CombiGen の反復子
  class iterator :
    public GenBase::iterator
  {
    friend class CombiGen;
  public:
    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    iterator(const iterator& src);

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親の CombiGen オブジェクト
    /// @note CombiGen が用いる．
    iterator(const CombiGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(ymuint pos);

  };


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
/// @class PermGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  /// @brief PermGen の反復子
  class iterator :
    public GenBase::iterator
  {
    friend class PermGen;
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);


  public:

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親の PermGen オブジェクト
    /// @note PermGen が用いる．
    iterator(const PermGen* parent);

  };


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
/// @class MultiGenBase Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief MultiCombiGen/MultiPermGen に共通な属性を表す基底クラス
//////////////////////////////////////////////////////////////////////
class MultiGenBase
{
public:

  /// @brief 順列/組合わせを表すクラス
  class iterator {
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief デストラクタ
    ~iterator();


  public:

    /// @brief 要素の取得
    /// @param[in] grp グループ番号
    /// @param[in] pos 要素の位置
    /// @return grp 番目のグループの pos 番目の要素を取り出す．
    ymuint
    operator()(ymuint grp,
	       ymuint pos) const;


  protected:

    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @note 継承クラスが用いる．
    iterator(const MultiGenBase* parent);

    /// @brief コピーする．
    /// @param[in] src コピー元のオブジェクト
    void
    copy(const iterator& src);

    /// @brief グループ数を得る．
    /// @return グループ数
    ymuint
    ngrp() const;

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
  ngrp() const;

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
/// @class MultiCombiGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  /// @brief 一つの組合わせを表すクラス
  class iterator :
    public MultiGenBase::iterator
  {
    friend class MultiCombiGen;
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);


  public:

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @brief MultiCombiGen が用いる．
    iterator(const MultiCombiGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(ymuint g,
	 ymuint pos);

    /// @brief grp 番目のグループが終了状態の時 true を返す．
    bool
    is_end_sub(ymuint grp) const;

  };


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
/// @class MultiPermGen Generator.h "ym_utils/Generator.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  /// @brief 一つの順列を表すクラス
  class iterator :
    public MultiGenBase::iterator
  {
    friend class MultiPermGen;
  public:

    /// @brief 空のコンストラクタ
    iterator();

    /// @brief コピーコンストラクタ
    /// @param[in] src コピー元のオブジェクト
    iterator(const iterator& src);

    /// @brief 代入演算子
    /// @param[in] src コピー元のオブジェクト
    /// @return 自分自身
    const iterator&
    operator=(const iterator& src);

    /// @brief 次の要素を求める．
    /// @return 次の要素を指す反復子
    iterator
    operator++();

    /// @brief 末尾のチェック
    /// @return 末尾の時に true を返す．
    bool
    is_end() const;


  private:

    /// @brief コンストラクタ
    /// @param[in] parent 親のオブジェクト
    /// @brief MultiPermGen が用いる．
    iterator(const MultiPermGen* parent);

    /// @brief operator++() のサブルーティン
    void
    next(ymuint pos);

    /// @brief grp 番目のグループが終了状態の時 true を返す．
    bool
    is_end_sub(ymuint grp) const;

  };


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
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 全要素数を得る．
inline
ymuint
GenBase::iterator::n() const
{
  return mParent->n();
}

// 順列/組合わせ数を得る．
inline
ymuint
GenBase::iterator::k() const
{
  return mParent->k();
}

// pos 番目の要素を取り出す．
inline
ymuint
GenBase::iterator::operator()(ymuint pos) const
{
  return elem(pos);
}

// pos 番目の要素を取り出す．
inline
ymuint
GenBase::iterator::elem(ymuint pos) const
{
  return mElem[pos];
}

// pos 番目の要素への参照を取り出す．
inline
ymuint&
GenBase::iterator::elem(ymuint pos)
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
MultiGenBase::iterator::operator()(ymuint grp,
				   ymuint pos) const
{
  return (*mElemArray[grp])[pos];
}

// グループ数を得る．
inline
ymuint
MultiGenBase::iterator::ngrp() const
{
  return mParent->ngrp();
}

// grp 番目のグループの全要素数を得る．
inline
ymuint
MultiGenBase::iterator::n(ymuint grp) const
{
  return mParent->n(grp);
}

// grp 番目のグループの選択する要素数を得る．
inline
ymuint
MultiGenBase::iterator::k(ymuint grp) const
{
  return mParent->k(grp);
}

// グループ数を得る．
inline
ymuint
MultiGenBase::ngrp() const
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
