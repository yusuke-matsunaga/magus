#ifndef YM_LOGIC_BDDEDGE_H
#define YM_LOGIC_BDDEDGE_H

/// @file ym_logic/BddEdge.h
/// @brief BddEdge のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Pol.h"
#include "ym_logic/lexp_nsdef.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddEdge BddEdge.h "ym_logic/BddEdge.h"
/// @brief BDD の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class BddEdge
{
  friend class Bdd;
  friend class BddMgr;
  friend class BddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  /// @note 中身は不定
  BddEdge();

  /// @brief コンストラクタ
  /// @param[in] node ノード
  explicit
  BddEdge(void* node);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] pol 極性
  BddEdge(void* node,
	  tPol pol);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  BddEdge(const BddEdge& src);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  /// @param[in] pol 極性
  BddEdge(const BddEdge& src,
	  tPol pol);

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


private:

  /// @brief コンストラクタ
  /// @param[in] val 内容
  explicit
  BddEdge(ympuint val);

  /// @brief コンストラクタ
  /// @param[in] val 内容
  /// @param[in] pol 極性
  BddEdge(ympuint val,
	  tPol pol);


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
  addpol(tPol pol);

  /// @brief 極性情報を取り去る．
  /// @return 自分自身への参照を返す．
  const BddEdge&
  normalize();

  /// @brief 極性情報を取り去った枝を返す．
  BddEdge
  get_normal() const;

  /// @brief ノードのポインタを取り出す．
  void*
  get_ptr() const;

  /// @brief 極性を取り出す．
  tPol
  pol() const;


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
  const ympuint kEdge0 = 1UL;

  /// @brief 終端の1を指す枝
  static
  const ympuint kEdge1 = 0UL;

  /// @brief 不正な値を表す枝
  static
  const ympuint kEdgeInvalid = 2UL;

  /// @brief オーバーフローを表す枝(invalidと同値)
  static
  const ympuint kEdgeOverflow = 2UL;

  /// @brief エラーを表す枝
  static
  const ympuint kEdgeError = 3UL;

};


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
BddEdge::BddEdge(void* node) :
  mBody(reinterpret_cast<ympuint>(node))
{
}

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] pol 極性
inline
BddEdge::BddEdge(void* node,
		 tPol pol) :
  mBody(reinterpret_cast<ympuint>(node) | static_cast<ympuint>(pol))
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
// @param[in] pol 極性
inline
BddEdge::BddEdge(const BddEdge& src,
		 tPol pol) :
  mBody(src.mBody)
{
  mBody ^= (static_cast<ympuint>(pol) ^ ((mBody & kEdgeInvalid) >> 1));
}

// @brief コンストラクタ
// @param[in] val 内容
inline
BddEdge::BddEdge(ympuint val) :
  mBody(val)
{
}

// @brief コンストラクタ
// @param[in] val 内容
// @param[in] pol 極性
inline
BddEdge::BddEdge(ympuint val,
		 tPol pol) :
  mBody(val)
{
  mBody ^= (static_cast<ympuint>(pol) ^ ((mBody & kEdgeInvalid) >> 1));
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

// @brief 極性を付加する．
// @return 自分自身への参照を返す．
inline
const BddEdge&
BddEdge::addpol(tPol pol)
{
  mBody ^= (pol ^ ((mBody & kEdgeInvalid) >> 1));
  return *this;
}

// @brief 極性情報を取り去る．
// @return 自分自身への参照を返す．
inline
const BddEdge&
BddEdge::normalize()
{
  mBody &= ~1UL;
  return *this;
}

// @brief 極性情報を取り去った枝を返す．
inline
BddEdge
BddEdge::get_normal() const
{
  return BddEdge(mBody & ~1UL);
}

// @brief ノードのポインタを取り出す．
inline
void*
BddEdge::get_ptr() const
{
  return reinterpret_cast<void*>(mBody & ~3UL);
}

// @brief 極性を取り出す．
inline
tPol
BddEdge::pol() const
{
  return static_cast<tPol>(mBody & 1UL);
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
  return (mBody & ~1UL) == kEdge1;
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

// @brief エラーがオーバーフローのチェック
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
  return (mBody & ~3UL) == kEdge1;
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

#endif // YM_LOGIC_BDDEDGE_H
