#ifndef DGEDGE_H
#define DGEDGE_H

/// @file DgEdge.h
/// @brief DgEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"
#include "DgNode.h"


BEGIN_NAMESPACE_DG

//////////////////////////////////////////////////////////////////////
/// @class DgEdge DgNode.h "DgNode.h"
/// @brief DgNode を指す枝を表すクラス
//////////////////////////////////////////////////////////////////////
class DgEdge
{
public:

  /// @brief 空のコンストラクタ
  DgEdge() = default;

  /// @brief ノードと反転フラグを指定したコンストラクタ
  DgEdge(
    DgNode* node,    ///< [in] ノード
    bool inv = false ///< [in] 反転フラグ
  ) : mBody{reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv)}
  {
  }

  /// @brief 定数0の枝を返す．
  static
  DgEdge
  zero()
  {
    return DgEdge{0UL};
  }

  /// @brief 定数1の枝を返す．
  static
  DgEdge
  one()
  {
    return DgEdge{1UL};
  }

  /// @brief デストラクタ
  ~DgEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0の時に true を返す．
  bool
  is_zero() const
  {
    return mBody == 0UL;
  }

  /// @brief 定数1の時に true を返す．
  bool
  is_one() const
  {
    return mBody == 1UL;
  }

  /// @brief 定数ノードの時に true を返す．
  bool
  is_const() const
  {
    return (mBody & ~1UL) == 0UL;
  }

  /// @brief ノードを返す．
  DgNode*
  node() const
  {
    return reinterpret_cast<DgNode*>(mBody & ~1UL);
  }

  /// @brief 反転フラグを返す．
  bool
  inv() const
  {
    return static_cast<bool>(mBody & 1U);
  }

  /// @brief 反転していない枝を返す．
  DgEdge
  normal_edge() const
  {
    return DgEdge{mBody & ~1UL};
  }

  /// @brief グローバル関数を返す．
  Bdd
  global_func() const
  {
    if ( is_const() ) {
      return Bdd::invalid();
    }
    else {
      auto f = node()->global_func();
      return f ^ inv();
    }
  }

  /// @brief グローバル関数が 1 となる入力割当を返す．
  Bdd
  pat_1() const
  {
    if ( inv() ) {
      return node()->pat_0();
    }
    else {
      return node()->pat_1();
    }
  }

  /// @brief グローバル関数が 0 となる入力割当を返す．
  Bdd
  pat_0() const
  {
    if ( inv() ) {
      return node()->pat_1();
    }
    else {
      return node()->pat_0();
    }
  }

  /// @brief 反転した枝を返す．
  DgEdge
  operator~() const
  {
    return DgEdge{mBody ^ 1UL};
  }

  /// @brief 自身を反転させる．
  /// @return 自分自身を返す．
  DgEdge&
  negate()
  {
    mBody ^= 1UL;
    return *this;
  }

  /// @brief 反転フラグをかける．
  DgEdge
  operator^(
    bool inv ///< [in] 反転フラグ
  ) const
  {
    return DgEdge{mBody ^ static_cast<ympuint>(inv)};
  }

  /// @brief 反転フラグをかけて代入する．
  DgEdge&
  operator^=(
    bool inv ///< [in] 反転フラグ
  )
  {
    mBody ^= static_cast<ympuint>(inv);
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const DgEdge& right
  ) const
  {
    return mBody == right.mBody;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const DgEdge& right
  ) const
  {
    return !operator==(right);
  }

  /// @brief 2つの枝が極性違いのときに true を返す．
  static
  bool
  check_complement(
    const DgEdge& left,
    const DgEdge& right
  )
  {
    return (left.mBody ^ right.mBody) == 1UL;
  }

  /// @brief ハッシュ値を返す．
  SizeType
  hash() const
  {
    return (mBody * mBody) >> 20;
  }

  /// @brief 内容を出力する．
  void
  print(
    ostream& s
  ) const
  {
    if ( is_zero() ) {
      s << "ZERO";
    }
    else if ( is_one() ) {
      s << "ONE";
    }
    else {
      if ( inv() ) {
	s << "~";
      }
      s << "#" << node()->id();
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定したコンストラクタ
  DgEdge(
    ympuint body
  ) : mBody{body}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本体
  ympuint mBody{0UL};

};

END_NAMESPACE_DG

BEGIN_NAMESPACE_STD

// DgEdge をキーにしたハッシュ関数クラスの定義
template <>
struct hash<DG_NAMESPACE::DgEdge>
{
  SizeType
  operator()(
    const DG_NAMESPACE::DgEdge& dgedge
  ) const
  {
    return dgedge.hash();
  }
};

END_NAMESPACE_STD

#endif // DGEDGE_H
