#ifndef SBJEDGE_H
#define SBJEDGE_H

/// @file SbjEdge.h
/// @brief SbjEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjEdge SbjEdge.h "SbjEdge.h"
/// @brief サブジェクトグラフの枝を表すクラス
///
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa SbjNode SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjEdge
{
public:

  /// @brief 空のコンストラクタ
  SbjEdge() = default;

  /// @brief ノードと位置を指定したコンストラクタ
  SbjEdge(
    const SbjNode* node, ///< [in] ノード
    SizeType pos         ///< [in] ファンイン番号
  ) : mToPos{reinterpret_cast<PtrIntType>(node) | pos}
  {
  }

  /// @brief デストラクタ
  ~SbjEdge() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const
  {
    return reinterpret_cast<const SbjNode*>(mToPos & ~1ULL);
  }

  /// @brief 出力側のノードの何番目の入力かを示す．
  SizeType
  pos() const
  {
    return static_cast<int>(mToPos & 1ULL);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 出力側のノード+入力位置
  PtrIntType mToPos{0ULL};

};

END_NAMESPACE_SBJ

#endif // SBJEDGE_H
