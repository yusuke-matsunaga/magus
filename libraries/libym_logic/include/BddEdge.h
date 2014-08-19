#ifndef BDDEDGE_H
#define BDDEDGE_H

/// @file BddEdge.h
/// @brief BddEdge のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/bdd_nsdef.h"
#include <gmpxx.h>


BEGIN_NAMESPACE_YM_BDD

class BddNode;

//////////////////////////////////////////////////////////////////////
/// @class BddEdge BddEdge.h "BddEdge.h"
/// @brief BDD の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BddEdge
{
  friend class BddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  /// @note 中身は不定
  BddEdge();

  /// @brief コンストラクタ
  /// @param[in] val 内容
  explicit
  BddEdge(ympuint val);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  explicit
  BddEdge(BddNode* node);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性
  BddEdge(void* node,
	  bool inv);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  BddEdge(const BddEdge& src);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  /// @param[in] inv 反転属性
  BddEdge(const BddEdge& src,
	  bool inv);

  /// @brief 定数0の枝を返す．
  static
  BddEdge
  make_zero();

  /// @brief 定数1の枝を返す．
  static
  BddEdge
  make_one();

  /// @brief オーバーフロー枝を返す．
  static
  BddEdge
  make_overflow();

  /// @brief エラー枝を返す．
  static
  BddEdge
  make_error();


public:
  //////////////////////////////////////////////////////////////////////
  // 極性情報に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 極性を反転させる
  /// @return 自分自身への参照を返す．
  const BddEdge&
  negate();

  /// @brief 極性を反転した枝を返す．
  BddEdge
  operator~() const;

  /// @brief 極性を付加する．
  /// @return 自分自身への参照を返す．
  const BddEdge&
  add_inv(bool inv);

  /// @brief 反転属性を取り去る．
  /// @return 自分自身への参照を返す．
  const BddEdge&
  normalize();

  /// @brief 反転属性を取り去った枝を返す．
  BddEdge
  get_normal() const;

  /// @brief ノードのポインタを取り出す．
  BddNode*
  get_node() const;

  /// @brief 反転属性を取り出す．
  bool
  inv() const;

  /// @brief 生の情報を取り出す．
  operator ympuint() const;

  /// @brief 参照されていない時にtrueを返す
  bool
  noref() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 比較演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較
  friend
  bool
  operator==(const BddEdge& left,
	     const BddEdge& right);

  /// @brief 非等価比較
  friend
  bool
  operator!=(const BddEdge& left,
	     const BddEdge& right);

  /// @brief 極性違いの時に true を返す．
  friend
  bool
  check_reverse(const BddEdge& left,
		const BddEdge& right);

  /// @brief 大小比較
  /// @note 対象演算の時の順序の正規化に使う．
  friend
  bool
  operator<(const BddEdge& left,
	    const BddEdge& right);

  /// @brief 大小比較
  /// @note 対象演算の時の順序の正規化に使う．
  friend
  bool
  operator>(const BddEdge& left,
	    const BddEdge& right);


public:
  //////////////////////////////////////////////////////////////////////
  // 終端枝に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0ノードのチェック
  bool
  is_zero() const;

  /// @brief 定数1ノードのチェック
  bool
  is_one() const;

  /// @brief 定数ノードのチェック
  bool
  is_const() const;

  /// @brief エラー枝のチェック
  bool
  is_error() const;

  /// @brief オーバーフロー枝のチェック
  bool
  is_overflow() const;

  /// @brief エラーがオーバーフローのチェック
  bool
  is_invalid() const;

  /// @brief 終端枝のチェック
  bool
  is_leaf() const;


public:
  //////////////////////////////////////////////////////////////////////
  // マーク関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief マークを返す．
  bool
  mark() const;

  /// @brief マークをつける．
  void
  set_mark();


public:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ関数用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ値を返す．
  ymuint
  hash() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 内容(ノードへのポインタ＋極性)
  ympuint mBody;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief 終端の0を指す枝
  static
  const ympuint kEdge0 = 0UL;

  /// @brief 終端の1を指す枝
  static
  const ympuint kEdge1 = 1UL;

  /// @brief 不正な値を表す枝
  static
  const ympuint kEdgeInvalid = 2UL;

  /// @brief オーバーフローを表す枝(invalidと同値)
  static
  const ympuint kEdgeOverflow = 2UL;

  /// @brief エラーを表す枝
  static
  const ympuint kEdgeError = 3UL;

  /// @brief 終端の定数を表すマスク値
  static
  const ympuint kEdgeConst = 0UL;

  /// @brief エラーも含めた終端を表すマスク値
  static
  const ympuint kEdgeLeaf = 0UL;

};

typedef unordered_map<BddEdge, BddEdge> BddEdgeEdgeMap;
typedef unordered_map<BddEdge, ymint> BddEdgeIntMap;
typedef unordered_map<BddEdge, ymuint32> BddEdgeUint32Map;
typedef unordered_map<BddEdge, mpz_class> BddEdgeMpzMap;
typedef unordered_set<BddEdge> BddEdgeSet;


//////////////////////////////////////////////////////////////////////
// BddEdge に関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates BddEdge
/// @brief 再帰的にマークを消す．
/// @param[in] e 根の枝
void
clear_mark(BddEdge);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 中身は不定
inline
BddEdge::BddEdge() :
  mBody(kEdgeError)
{
}

// @brief コンストラクタ
// @param[in] node ノード
inline
BddEdge::BddEdge(BddNode* node) :
  mBody(reinterpret_cast<ympuint>(node))
{
}

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性
inline
BddEdge::BddEdge(void* node,
		 bool inv) :
  mBody(reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv))
{
}

// @brief コンストラクタ
// @param[in] src コピー元の枝
inline
BddEdge::BddEdge(const BddEdge& src) :
  mBody(src.mBody)
{
}

// @brief コンストラクタ
// @param[in] src コピー元の枝
// @param[in] inv 反転属性
inline
BddEdge::BddEdge(const BddEdge& src,
		 bool inv) :
  mBody(src.mBody)
{
  mBody ^= (static_cast<ympuint>(inv) ^ ((mBody & kEdgeInvalid) >> 1));
}

// @brief コンストラクタ
// @param[in] val 内容
inline
BddEdge::BddEdge(ympuint val) :
  mBody(val)
{
}

// @brief 定数0の枝を返す．
inline
BddEdge
BddEdge::make_zero()
{
  return BddEdge(kEdge0);
}

// @brief 定数1の枝を返す．
inline
BddEdge
BddEdge::make_one()
{
  return BddEdge(kEdge1);
}

// @brief オーバーフロー枝を返す．
inline
BddEdge
BddEdge::make_overflow()
{
  return BddEdge(kEdgeOverflow);
}

// @brief エラー枝を返す．
inline
BddEdge
BddEdge::make_error()
{
  return BddEdge(kEdgeError);
}

// @brief 極性を反転させる
inline
const BddEdge&
BddEdge::negate()
{
  mBody ^= (1UL ^ ((mBody & kEdgeInvalid) >> 1));
  return *this;
}

// @brief 極性を反転した枝を返す．
inline
BddEdge
BddEdge::operator~() const
{
  return BddEdge(mBody ^ 1UL ^ ((mBody & kEdgeInvalid) >> 1));
}

// @brief 反転属性を付加する．
// @return 自分自身への参照を返す．
inline
const BddEdge&
BddEdge::add_inv(bool inv)
{
  mBody ^= (inv ^ ((mBody & kEdgeInvalid) >> 1));
  return *this;
}

// @brief 反転属性を取り去る．
// @return 自分自身への参照を返す．
inline
const BddEdge&
BddEdge::normalize()
{
  mBody &= ~1UL;
  return *this;
}

// @brief 反転属性を取り去った枝を返す．
inline
BddEdge
BddEdge::get_normal() const
{
  return BddEdge(mBody & ~1UL);
}

// @brief ノードのポインタを取り出す．
inline
BddNode*
BddEdge::get_node() const
{
  return reinterpret_cast<BddNode*>(mBody & ~3UL);
}

// @brief 反転属性を取り出す．
inline
bool
BddEdge::inv() const
{
  return static_cast<bool>(mBody & 1UL);
}

// @brief 等価比較
inline
bool
operator==(const BddEdge& left,
	   const BddEdge& right)
{
  return left.mBody == right.mBody;
}

// @brief 非等価比較
inline
bool
operator!=(const BddEdge& left,
	   const BddEdge& right)
{
  return left.mBody != right.mBody;
}

// @brief 極性違いの時に true を返す．
inline
bool
check_reverse(const BddEdge& left,
	      const BddEdge& right)
{
  return (left.mBody ^ right.mBody) == 1UL;
}

// @brief 大小比較
// @note 対象演算の時の順序の正規化に使う．
inline
bool
operator<(const BddEdge& left,
	  const BddEdge& right)
{
  return left.mBody < right.mBody;
}

// @brief 大小比較
// @note 対象演算の時の順序の正規化に使う．
inline
bool
operator>(const BddEdge& left,
	  const BddEdge& right)
{
  return left.mBody > right.mBody;
}

// @brief 定数0ノードのチェック
inline
bool
BddEdge::is_zero() const
{
  return mBody == kEdge0;
}

// @brief 定数1ノードのチェック
inline
bool
BddEdge::is_one() const
{
  return mBody == kEdge1;
}

// @brief 定数ノードのチェック
inline
bool
BddEdge::is_const() const
{
  return (mBody & ~1UL) == kEdgeConst;
}

// @brief エラー枝のチェック
inline
bool
BddEdge::is_error() const
{
  return mBody == kEdgeError;
}

// @brief オーバーフロー枝のチェック
inline
bool
BddEdge::is_overflow() const
{
  return mBody == kEdgeOverflow;
}

// @brief エラーかオーバーフローのチェック
inline
bool
BddEdge::is_invalid() const
{
  return (mBody & ~1UL) == kEdgeInvalid;
}

// @brief 終端枝のチェック
inline
bool
BddEdge::is_leaf() const
{
  return (mBody & ~3UL) == kEdgeLeaf;
}

// @brief 生の情報を取り出す．
inline
BddEdge::operator ympuint() const
{
  return mBody;
}

// @brief ハッシュ値を返す．
inline
ymuint
BddEdge::hash() const
{
  return mBody;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH

// BddEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::BddEdge>
{
  ymuint
  operator()(const nsYm::nsBdd::BddEdge& e) const
  {
    return e.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_YMYMLOGIC_BDDEDGE_H
