#ifndef YMYMLOGIC_BDDLITSET_H
#define YMYMLOGIC_BDDLITSET_H

/// @file YmLogic/BddLitSet.h
/// @brief BddLitSet のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddLitSet BddLitSet.h "YmLogic/BddLitSet.h"
/// @ingroup YmBdds
/// @brief リテラル集合( = cube)を表すクラス
///
/// 実は BDD を用いて実装している．
//////////////////////////////////////////////////////////////////////
class BddLitSet
{
  friend class Bdd;
public:
  // 反復子
  class iterator
  {
    friend class BddLitSet;
  public:
    // コンストラクタ
    iterator();

    // 現在指している要素を返す．
    // 末尾かどうかのチェックは行わない．
    Literal
    operator*() const;

    // ひとつ先に進める．
    // 自分自身を返す．
    iterator&
    operator++();

    // ひとつ先に進める．
    // 進める前の値を返す．
    iterator
    operator++(int);

    // 等価比較
    bool
    operator==(const iterator& src) const;
    bool
    operator!=(const iterator& src) const;


  private:

    // BddLitSet 用のコンストラクタ
    iterator(const Bdd& bdd);

    // ひとつ先に進める．
    void
    next();


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 現在指している節点
    Bdd mCur;

  };


public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @note 空集合となるが，明示的に使う場合には下の make_empty() を使うべき
  explicit
  BddLitSet(BddMgr& mgr);

  /// @brief BDD を指定するコンストラクタ
  /// @param[in] bdd 対象のBDD
  explicit
  BddLitSet(const Bdd& bdd);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  BddLitSet(const BddLitSet& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const BddLitSet&
  operator=(const BddLitSet& src);

  /// @brief リテラル1つだけを要素とする集合を作るコンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] varid 変数番号
  /// @param[in] int 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  BddLitSet(BddMgr& mgr,
	    VarId varid,
	    bool inv);

  /// @brief リテラル1つだけを要素とする集合を作るコンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] lit リテラル
  BddLitSet(BddMgr& mgr,
	    const Literal& lit);

  /// @brief ベクタからの変換用コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] src リテラルのベクタ
  BddLitSet(BddMgr& mgr,
	    const LiteralVector& src);

  /// @brief list からの変換用コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] src リテラルのリスト
  BddLitSet(BddMgr& mgr,
	    const LiteralList& src);

  /// @brief 空集合を返すクラスメソッド
  /// @param[in] mgr BddMgr
  static
  BddLitSet
  make_empty(BddMgr& mgr);


public:

  /// @brief 空の時に真となる
  bool
  empty() const;

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief リテラル関数を返す．
  Bdd
  function() const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  /// @brief リテラルのベクタに変換する．
  /// @return 集合のサイズを返す．
  /// @note 順番は内部の実装に依存する．
  ymuint
  to_vector(LiteralVector& dst) const;

  /// @brief リテラルのリストに変換する．
  /// @return 集合のサイズを返す．
  /// @note 順番は内部の実装に依存する．
  ymuint
  to_list(LiteralList& dst) const;

  /// @brief 集合和を計算する．
  /// @param[in] set2 オペランド
  /// @note 肯定のリテラルと否定のリテラルが両方含まれていた場合には空集合となる．
  BddLitSet
  operator+(const BddLitSet& set2) const;

  /// @brief 集合和を計算して代入する．
  /// @param[in] set2 オペランド
  /// @note 肯定のリテラルと否定のリテラルが両方含まれていた場合には空集合となる．
  const BddLitSet&
  operator+=(const BddLitSet& set2);

  /// @brief 集合積を計算する．
  /// @param[in] set2 オペランド
  BddLitSet
  operator*(const BddLitSet& set2) const;

  /// @brief 集合積を計算して代入する．
  /// @param[in] set2 オペランド
  const BddLitSet&
  operator*=(const BddLitSet& set2);

  /// @brief 集合差を計算する．
  /// @param[in] set2 オペランド
  BddLitSet
  operator-(const BddLitSet& set2) const;

  /// @brief 集合差を計算して代入する．
  /// @param[in] set2 オペランド
  const BddLitSet&
  operator-=(const BddLitSet& set2);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief サイズを設定する．
  void
  set_size(ymuint size) const;

  // サイズを無効化する．
  void
  invalidate_size();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // リテラル集合を表すBDD
  Bdd mBody;

  // 要素数を入れておくキャッシュ
  // 最下位1ビットが1の時のみ valid な値を持つ．
  mutable
  ymuint32 mSize;

};


//////////////////////////////////////////////////////////////////////
// BddLitSet 関係の non-member 関数の宣言
//////////////////////////////////////////////////////////////////////

/// @relates BddLitSet
/// @brief 等価比較演算子
/// @param[in] set1, set2 オペランド
bool
operator==(const BddLitSet& set1,
	   const BddLitSet& set2);

/// @relates BddLitSet
/// @brief 非等価比較演算子
/// @param[in] set1, set2 オペランド
bool
operator!=(const BddLitSet& set1,
	   const BddLitSet& set2);

/// @relates BddLitSet
/// @brief set1 と set2 が集合として交わっていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator&&(const BddLitSet& set1,
	   const BddLitSet& set2);

/// @relates BddLitSet
/// @brief set1 が set2 を含んでいたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator>=(const BddLitSet& set1,
	   const BddLitSet& set2);

/// @relates BddLitSet
/// @brief set1 が set2 に含まれていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator<=(const BddLitSet& set1,
	   const BddLitSet& set2);

/// @relates BddLitSet
/// @brief set1 が真に set2 を含んでいたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator>(const BddLitSet& set1,
	  const BddLitSet& set2);

/// @param[in] set1, set2 オペランド
/// @brief set1 が真に set2 に含まれていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator<(const BddLitSet& set1,
	  const BddLitSet& set2);

/// @relates BddLitSet
/// @brief ストリームに出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] litset 対象のリテラルセット
ostream&
operator<<(ostream& s,
	   const BddLitSet& litset);


//////////////////////////////////////////////////////////////////////
// BddLitSet のインライン関数定義
//////////////////////////////////////////////////////////////////////

// リテラル関数を返す．
inline
Bdd
BddLitSet::function() const
{
  return mBody;
}

// 等価比較演算子
inline
bool
operator==(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return set1.function() == set2.function();
}

// 非等価比較演算子
inline
bool
operator!=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return ! operator==(set1, set2);
}

// set1 と set2 が集合として交わっていたら true を返す．
inline
bool
operator&&(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return lsintersect(set1.function(), set2.function());
}

// 集合として set1 が set2 を含んでいたら true を返す．
inline
bool
operator>=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  // 実は BDD の <= 演算となる．
  return set1.function() <= set2.function();
}

// >= の逆の演算子
inline
bool
operator<=(const BddLitSet& set1,
	   const BddLitSet& set2)
{
  return set2 >= set1;
}

// 集合として set1 が strict に set2 を含んでいたら true を返す．
inline
bool
operator>(const BddLitSet& set1,
	  const BddLitSet& set2)
{
  return set1.function() > set2.function();
}

// > の逆の演算子
inline
bool
operator<(const BddLitSet& set1,
	  const BddLitSet& set2)
{
  return set2 > set1;
}

END_NAMESPACE_YM_BDD

#endif // YM_YMYMLOGIC_BDDLITSET_H
