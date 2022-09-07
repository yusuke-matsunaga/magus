#ifndef SBJHANDLE_H
#define SBJHANDLE_H

/// @file SbjHandle.h
/// @brief SbjHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjHandle SbjGraph.h "SbjGraph.h"
/// @brief ノード＋極性の情報を表すクラス
//////////////////////////////////////////////////////////////////////
class SbjHandle
{
public:

  /// @brief コンストラクタ
  explicit
  SbjHandle(
    SbjNode* node = nullptr, ///< [in] ノード
    bool inv = false         ///< [in] 極性
  )
  {
    set(node, inv);
  }

  /// @brief デストラクタ
  ~SbjHandle() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0を返す．
  static
  SbjHandle
  make_zero()
  {
    return SbjHandle{static_cast<ympuint>(0ULL)};
  }

  /// @brief 定数1を返す．
  static
  SbjHandle
  make_one()
  {
    return SbjHandle(static_cast<ympuint>(1ULL));
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを返す．
  SbjNode*
  node() const
  {
    return reinterpret_cast<SbjNode*>(mData & ~1ULL);
  }

  /// @brief 極性を返す．
  bool
  inv() const
  {
    return static_cast<bool>(mData & 1ULL);
  }

  /// @brief 定数0を表しているかどうか調べる．
  bool
  is_const0() const
  {
    return mData == 0ULL;
  }

  /// @brief 定数1を表しているかどうか調べる．
  bool
  is_const1() const
  {
    return mData == 1ULL;
  }

  /// @brief 極性を反転させた結果を返す．
  SbjHandle
  operator~() const
  {
    return SbjHandle(mData ^ 1ULL);
  }

  /// @brief 反転属性を持たないハンドルを返す．
  SbjHandle
  normalize() const
  {
    return SbjHandle{mData & ~1ULL};
  }

  /// @brief 等価比較演算子
  bool
  operator==(
    const SbjHandle& right ///< [in] オペランド
  ) const
  {
    return mData == right.mData;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(
    const SbjHandle& right ///< [in] オペランド
  ) const
  {
    return !operator==(right);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を設定する．
  void
  set(
    SbjNode* node, ///< [in] ノード
    bool inv       ///< [in] 極性
  )
  {
    mData = reinterpret_cast<ympuint>(node) | static_cast<ympuint>(inv);
  }

  /// @brief 極性を反転させる．
  void
  invert()
  {
    mData ^= 1ULL;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データを直接指定したコンストラクタ
  SbjHandle(
    ympuint data ///< [in] データ
  ) : mData{data}
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードのポインタと極性をパックしたもの
  ympuint mData;

};

END_NAMESPACE_SBJ

#endif // SBJGRAPH_H
