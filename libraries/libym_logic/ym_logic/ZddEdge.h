#ifndef YM_LOGIC_ZDDEDGE_H
#define YM_LOGIC_ZDDEDGE_H

/// @file ym_logic/ZddEdge.h
/// @brief ZddEdge のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ym_logic/VarId.h"


BEGIN_NAMESPACE_YM_ZDD

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

  /// @brief コンストラクタ
  /// @param[in] node ノード
  explicit
  ZddEdge(void* node);

  /// @brief コンストラクタ
  /// @param[in] node ノード
  ZddEdge(void* node);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  ZddEdge(const ZddEdge& src);

  /// @brief コンストラクタ
  /// @param[in] src コピー元の枝
  ZddEdge(const ZddEdge& src);

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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] val 内容
  explicit
  ZddEdge(ympuint val);


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
  // ノードのポインタ型への変換
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのポインタを取り出す．
  void*
  get_ptr() const;


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
// @param[in] node ノード
inline
ZddEdge::ZddEdge(void* node) :
  mBody(reinterpret_cast<ympuint>(node))
{
}

// @brief コンストラクタ
// @param[in] src コピー元の枝
inline
ZddEdge::ZddEdge(const ZddEdge& src) :
  mBody(src.mBody)
{
}

// @brief コンストラクタ
// @param[in] val 内容
inline
ZddEdge::ZddEdge(ympuint val) :
  mBody(val)
{
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
void*
ZddEdge::get_ptr() const
{
  return reinterpret_cast<void*>(mBody & ~3UL);
}

// @brief 極性を取り出す．
inline
tPol
ZddEdge::pol() const
{
  return static_cast<tPol>(mBody & 1UL);
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

#if 0
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
#endif

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

#endif // YM_LOGIC_ZDDEDGE_H
