#ifndef YM_LOGIC_BDDVARSET_H
#define YM_LOGIC_BDDVARSET_H

/// @file ym_logic/BddVarSet.h
/// @brief BddVarSet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddVarSet.h 693 2007-05-28 09:13:57Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddVarSet BddVarSet.h <ym_bdd/BddVarSet.h>
/// @ingroup YmBdds
/// @brief 変数集合を表すクラス
///
/// 実は BDD を用いて実装している．
//////////////////////////////////////////////////////////////////////
class BddVarSet
{
  friend class Bdd;
public:
  // 反復子
  class iterator
  {
    friend class BddVarSet;
  public:
    // コンストラクタ
    iterator();

    // 現在指している要素を返す．
    // 末尾かどうかのチェックは行わない．
    tVarId
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

    // == の逆
    bool
    operator!=(const iterator& src) const;


  private:

    // BddVarSet 用のコンストラクタ
    iterator(const Bdd& bdd);


  private:
    // 現在指している節点
    Bdd mCur;

  };
  // 実はこの反復子は定数反復子である．
  typedef iterator const_iterator;


public:

  /// @brief コンストラクタ
  /// @param[in] mgr BddMgr
  /// @note 空集合となるが，明示的に使う場合には下の make_empty() を使うべき
  explicit
  BddVarSet(BddMgr& mgr);

  /// @brief コピーコンストラクタ
  BddVarSet(const BddVarSet& src);

  /// @brief 代入演算子
  const BddVarSet&
  operator=(const BddVarSet& src);

  /// @brief 変数一つだけを要素とする集合を作るコンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] varid 変数番号
  BddVarSet(BddMgr& mgr,
	    tVarId varid);

  /// @brief vector からの変換用コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] src 変数番号のベクタ
  BddVarSet(BddMgr& mgr,
	    const VarVector& src);

  /// @brief list からの変換用コンストラクタ
  /// @param[in] mgr BddMgr
  /// @param[in] src 変数番号のリスト
  BddVarSet(BddMgr& mgr,
	    const VarList& src);

  /// @brief 空集合を返すクラスメソッド
  /// @param[in] mgr BddMgr
  static
  BddVarSet
  make_empty(BddMgr& mgr);


public:

  /// @brief 空の時, 真となる．
  bool
  empty() const;

  /// @brief 要素数を返す．
  ymuint
  size() const;

  /// @brief この変数集合の表す関数を返す．
  Bdd
  function() const;

  /// @brief 先頭の反復子を返す．
  iterator
  begin() const;

  /// @brief 末尾の反復子を返す．
  iterator
  end() const;

  /// @brief 変数番号のベクタに変換する．
  /// @param[out] dst 結果を格納するベクタ
  /// @return 集合のサイズを返す．
  /// @note 変数番号の順番は内部の実装に依存する．
  ymuint
  to_vector(VarVector& dst) const;

  /// @brief 変数番号のリストに変換する．
  /// @param[out] dst 結果を格納するリスト
  /// @return 集合のサイズを返す．
  /// @note 変数番号の順番は内部の実装に依存する．
  ymuint
  to_list(VarList& dst) const;

  /// @brief 集合和を計算する．
  /// @param[in] set2 オペランド
  BddVarSet
  operator+(const BddVarSet& set2) const;

  /// @brief 集合和を計算して代入する．
  /// @param[in] set2 オペランド
  const BddVarSet&
  operator+=(const BddVarSet& set2);

  /// @brief 集合積を計算する．
  /// @param[in] set2 オペランド
  BddVarSet
  operator*(const BddVarSet& set2) const;

  /// @brief 集合積を計算して代入する．
  /// @param[in] set2 オペランド
  const BddVarSet&
  operator*=(const BddVarSet& set2);

  /// @brief 集合差を計算する．
  /// @param[in] set2 オペランド
  BddVarSet
  operator-(const BddVarSet& set2) const;

  /// @brief 集合差を計算して代入する．
  /// @param[in] set2 オペランド
  const BddVarSet&
  operator-=(const BddVarSet& set2);


public:
  // これらの関数は BddVarSet(const Bdd& bdd) を使う．

  friend
  BddVarSet
  support(const BddVector& bdd_array);

  friend
  BddVarSet
  support(const BddList& bdd_array);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD を指定するコンストラクタ
  explicit
  BddVarSet(const Bdd& bdd);

  /// @brief サイズを設定する．
  void
  set_size(ymuint size) const;

  /// @brief サイズを無効化する．
  void
  invalidate_size();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  Bdd mBody;

  // 要素数を入れておくキャッシュ
  // 最下位1ビットが1の時のみ valid な値を持つ．
  mutable
  ymuint32 mSize;

};


//////////////////////////////////////////////////////////////////////
// BddVarSet 関係の non-member 関数の宣言
//////////////////////////////////////////////////////////////////////

/// @relates BddVarSet
/// @brief 等価比較演算子
/// @param[in] set1, set2 オペランド
bool
operator==(const BddVarSet& set1,
	   const BddVarSet& set2);

/// @relates BddVarSet
/// @brief 非等価比較演算子
/// @param[in] set1, set2 オペランド
bool
operator!=(const BddVarSet& set1,
	   const BddVarSet& set2);

/// @relates BddVarSet
/// @brief set1 と set2 が集合として交わっていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator&&(const BddVarSet& set1,
	   const BddVarSet& set2);

/// @relates BddVarSet
/// @brief set1 が set2 を含んでいたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator>=(const BddVarSet& set1,
	   const BddVarSet& set2);

/// @relates BddVarSet
/// @brief set1 が set2 に含まれていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator<=(const BddVarSet& set1,
	   const BddVarSet& set2);

/// @relates BddVarSet
/// @brief set1 が真に set2 を含んでいたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator>(const BddVarSet& set1,
	  const BddVarSet& set2);

/// @relates BddVarSet
/// @brief set1 が真に set2 に含まれていたら true を返す．
/// @param[in] set1, set2 オペランド
bool
operator<(const BddVarSet& set1,
	  const BddVarSet& set2);

/// @relates BddVarSet
/// @brief ストリームに出力する．
/// @param[in] s 出力ストリーム
/// @param[in] varset 対象の変数集合
ostream&
operator<<(ostream& s,
	   const BddVarSet& varset);


//////////////////////////////////////////////////////////////////////
// BddVarSet のインライン関数定義
//////////////////////////////////////////////////////////////////////

// この変数集合の表す関数を返す．
inline
Bdd
BddVarSet::function() const
{
  return mBody;
}

// 等価比較演算子
inline
bool
operator==(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return set1.function() == set2.function();
}

// 非等価比較演算子
inline
bool
operator!=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return ! operator==(set1, set2);
}

// set1 と set2 が集合として交わっていたら true を返す．
inline
bool
operator&&(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return vsintersect(set1.function(), set2.function());
}

// 集合として set1 が set2 を含んでいたら true を返す．
inline
bool
operator>=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  // 実は BDD の <= 演算となる．
  return set1.function() <= set2.function();
}

// >= の逆の演算子
inline
bool
operator<=(const BddVarSet& set1,
	   const BddVarSet& set2)
{
  return set2 >= set1;
}

// 集合として set1 が strict に set2 を含んでいたら true を返す．
inline
bool
operator>(const BddVarSet& set1,
	  const BddVarSet& set2)
{
  return set1.function() < set2.function();
}

// > の逆の演算子
inline
bool
operator<(const BddVarSet& set1,
	  const BddVarSet& set2)
{
  return set2 > set1;
}

END_NAMESPACE_YM_BDD

#endif // YM_LOGIC_BDDVARSET_H
