#ifndef SBJEDGE_H
#define SBJEDGE_H

/// @file SbjEdge.h
/// @brief SbjEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

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
  SbjEdge();

  /// @brief ノードと位置を指定したコンストラクタ
  SbjEdge(const SbjNode* node,
	  int pos);

  /// @brief デストラクタ
  ~SbjEdge();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const;

  /// @brief 出力側のノードの何番目の入力かを示す．
  int
  pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 出力側のノード+入力位置
  ympuint mToPos;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SbjEdge::SbjEdge() :
  mToPos(0ULL)
{
}

// @brief ノードと位置を指定したコンストラクタ
inline
SbjEdge::SbjEdge(const SbjNode* node,
		 int pos) :
  mToPos(reinterpret_cast<ympuint>(node) | pos)
{
}

// デストラクタ
inline
SbjEdge::~SbjEdge()
{
}

// 出力側のノードを得る．
inline
const SbjNode*
SbjEdge::to() const
{
  return reinterpret_cast<const SbjNode*>(mToPos & ~1ULL);
}

// 出力側のノードの何番目の入力かを示す．
inline
int
SbjEdge::pos() const
{
  return static_cast<int>(mToPos & 1ULL);
}

END_NAMESPACE_YM_SBJ

#endif // SBJEDGE_H
