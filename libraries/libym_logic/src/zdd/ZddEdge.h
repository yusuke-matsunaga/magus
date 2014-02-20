#ifndef ZDDEDGE_H
#define ZDDEDGE_H

/// @file ZddEdge.h
/// @brief ZddEdge のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/zdd_nsdef.h"
#include "logic/VarId.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddNode;

//////////////////////////////////////////////////////////////////////
/// @class ZddEdge ZddEdge.h "ym_logic/ZddEdge.h"
/// @brief ZDD の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class ZddEdge
{
  friend class Zdd;
  friend class ZddMgr;
  friend class ZddMgrImpl;

public:

  /// @brief 空のコンストラクタ
  /// @note 中身は不定
  ZddEdge();

  /// @brief ストレートなコンストラクタ
  /// @param[in] val 内容
  explicit
  ZddEdge(ympuint val);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  explicit
  ZddEdge(ZddNode* node);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  /// @param[in] zattr 0-element 属性
  ZddEdge(ZddNode* node,
	  bool zattr);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  ZddEdge(const ZddEdge& src);

  /// @brief ympuint へのキャスト演算子
  operator ympuint() const;

  /// @brief 定数0の枝を返す．
  static
  ZddEdge
  make_zero();

  /// @brief 定数1の枝を返す．
  static
  ZddEdge
  make_one();

  /// @brief オーバーフロー枝を返す．
  static
  ZddEdge
  make_overflow();

  /// @brief エラー枝を返す．
  static
  ZddEdge
  make_error();


public:
  //////////////////////////////////////////////////////////////////////
  // 比較演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価比較
  friend
  bool
  operator==(const ZddEdge& left,
	     const ZddEdge& right);

  /// @brief 非等価比較
  friend
  bool
  operator!=(const ZddEdge& left,
	     const ZddEdge& right);

  /// @brief 大小比較
  /// @note 対象演算の時の順序の正規化に使う．
  friend
  bool
  operator<(const ZddEdge& left,
	    const ZddEdge& right);

  /// @brief 大小比較
  /// @note 対象演算の時の順序の正規化に使う．
  friend
  bool
  operator>(const ZddEdge& left,
	    const ZddEdge& right);


public:
  //////////////////////////////////////////////////////////////////////
  // ノードと属性に関する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのポインタを取り出す．
  ZddNode*
  get_node() const;

  /// @brief 0-element 属性を取り出す．
  bool
  zattr() const;

  /// @brief 属性を取り去る．
  /// @return 自分自身への参照を返す．
  const ZddEdge&
  normalize();

  /// @brief 属性を取り去った枝を返す．
  ZddEdge
  get_normal() const;

  /// @brief 属性を付加する．
  /// @return 自分自身への参照を返す．
  const ZddEdge&
  add_zattr(bool zattr);

  /// @brief 参照されていない時にtrueを返す
  bool
  noref() const;


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
  /// @note 要するに定数0か定数1
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
  /// @note 定数0，定数1，エラー，オーバーフローのいずれかの時に真となる．
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

  // 内容(ノードへのポインタ or 定数値)
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

};


//////////////////////////////////////////////////////////////////////
// ZddEdge に関連する関数
//////////////////////////////////////////////////////////////////////

/// @relates ZddEdge
/// @brief 再帰的に節点のマークを消す．
/// @param[in] e 根の枝
void
clear_mark(ZddEdge e);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @note 中身は不定
inline
ZddEdge::ZddEdge() :
  mBody(kEdgeError)
{
}

// @brief コンストラクタ
// @param[in] val 内容
inline
ZddEdge::ZddEdge(ympuint val) :
  mBody(val)
{
}

// @brief コンストラクタ
// @param[in] node ノード
inline
ZddEdge::ZddEdge(ZddNode* node) :
  mBody(reinterpret_cast<ympuint>(node))
{
}

// @brief コンストラクタ
// @param[in] node ノード
// @param[in] zattr 0-element 属性
inline
ZddEdge::ZddEdge(ZddNode* node,
		 bool zattr) :
  mBody(reinterpret_cast<ympuint>(node) | zattr)
{
}

// @brief コンストラクタ
// @param[in] src コピー元の枝
inline
ZddEdge::ZddEdge(const ZddEdge& src) :
  mBody(src.mBody)
{
}

// @brief ympuint へのキャスト演算子
inline
ZddEdge::operator ympuint() const
{
  return mBody;
}

// @brief 定数0の枝を返す．
inline
ZddEdge
ZddEdge::make_zero()
{
  return ZddEdge(kEdge0);
}

// @brief 定数1の枝を返す．
inline
ZddEdge
ZddEdge::make_one()
{
  return ZddEdge(kEdge1);
}

// @brief オーバーフロー枝を返す．
inline
ZddEdge
ZddEdge::make_overflow()
{
  return ZddEdge(kEdgeOverflow);
}

// @brief エラー枝を返す．
inline
ZddEdge
ZddEdge::make_error()
{
  return ZddEdge(kEdgeError);
}

// @brief ノードのポインタを取り出す．
inline
ZddNode*
ZddEdge::get_node() const
{
  return reinterpret_cast<ZddNode*>(mBody & ~3UL);
}

// @brief 0-element 属性を取り出す．
inline
bool
ZddEdge::zattr() const
{
  return static_cast<bool>(mBody & 1UL);
}

// @brief 属性を取り去る．
// @return 自分自身への参照を返す．
inline
const ZddEdge&
ZddEdge::normalize()
{
  mBody &= ~1UL;
  return *this;
}

// @brief 属性を取り去った枝を返す．
inline
ZddEdge
ZddEdge::get_normal() const
{
  return ZddEdge(mBody & ~1UL);
}

// @brief 属性を付加する．
// @return 自分自身への参照を返す．
inline
const ZddEdge&
ZddEdge::add_zattr(bool zattr)
{
  // true = 1 だと仮定している．
  mBody |= static_cast<ympuint>(zattr);
  return *this;
}

// @brief 等価比較
inline
bool
operator==(const ZddEdge& left,
	   const ZddEdge& right)
{
  return left.mBody == right.mBody;
}

// @brief 非等価比較
inline
bool
operator!=(const ZddEdge& left,
	   const ZddEdge& right)
{
  return left.mBody != right.mBody;
}

// @brief 大小比較
// @note 対象演算の時の順序の正規化に使う．
inline
bool
operator<(const ZddEdge& left,
	  const ZddEdge& right)
{
  return left.mBody < right.mBody;
}

// @brief 大小比較
// @note 対象演算の時の順序の正規化に使う．
inline
bool
operator>(const ZddEdge& left,
	  const ZddEdge& right)
{
  return left.mBody > right.mBody;
}

// @brief 定数0ノードのチェック
inline
bool
ZddEdge::is_zero() const
{
  return mBody == kEdge0;
}

// @brief 定数1ノードのチェック
inline
bool
ZddEdge::is_one() const
{
  return mBody == kEdge1;
}

// @brief 定数ノードのチェック
inline
bool
ZddEdge::is_const() const
{
  return (mBody & ~1UL) == 0UL;
}

// @brief エラー枝のチェック
inline
bool
ZddEdge::is_error() const
{
  return mBody == kEdgeError;
}

// @brief オーバーフロー枝のチェック
inline
bool
ZddEdge::is_overflow() const
{
  return mBody == kEdgeOverflow;
}

// @brief エラーかオーバーフローのチェック
inline
bool
ZddEdge::is_invalid() const
{
  return (mBody & ~1UL) == 2UL;
}

// @brief 終端枝のチェック
inline
bool
ZddEdge::is_leaf() const
{
  return (mBody & ~3UL) == 0UL;
}

// @brief ハッシュ値を返す．
inline
ymuint
ZddEdge::hash() const
{
  return mBody;
}

END_NAMESPACE_YM_ZDD

BEGIN_NAMESPACE_HASH

// ZddEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsZdd::ZddEdge>
{
  ymuint
  operator()(const nsYm::nsZdd::ZddEdge& e) const
  {
    return e.hash();
  }
};

END_NAMESPACE_HASH

#endif // ZDDEDGE_H
