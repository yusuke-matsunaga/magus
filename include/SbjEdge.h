#ifndef SBJEDGE_H
#define SBJEDGE_H

/// @file SbjEdge.h
/// @brief SbjEdge のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjEdge SbjEdge.h "SbjEdge.h"
/// @brief サブジェクトグラフの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa SbjNode SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjEdge
{
  friend class SbjNode;
  friend class SbjGraph;

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval nullptr 定数0に接続している枝の場合
  const SbjNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval nullptr 定数0に接続している枝の場合
  SbjNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  SbjNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  int
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SbjEdge();

  /// @brief デストラクタ
  ~SbjEdge();

  /// @brief from ノードをセットする．
  void
  set_from(SbjNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(SbjNode* to,
	 int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  SbjNode* mFrom;

  // 出力側のノード+入力位置
  ympuint mToPos;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SbjEdge::SbjEdge() :
  mFrom(nullptr),
  mToPos(0ULL)
{
}

// デストラクタ
inline
SbjEdge::~SbjEdge()
{
}

// 入力側のノードを得る．
inline
const SbjNode*
SbjEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const SbjNode*
SbjEdge::to() const
{
  return reinterpret_cast<const SbjNode*>(mToPos & ~1ULL);
}

// 入力側のノードを得る．
inline
SbjNode*
SbjEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
SbjNode*
SbjEdge::to()
{
  return reinterpret_cast<SbjNode*>(mToPos & ~1ULL);
}

// 出力側のノードの何番目の入力かを示す．
inline
int
SbjEdge::pos() const
{
  return static_cast<int>(mToPos & 1ULL);
}

// @brief from ノードをセットする．
inline
void
SbjEdge::set_from(SbjNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
SbjEdge::set_to(SbjNode* to,
		int pos)
{
  mToPos = reinterpret_cast<ympuint>(to) | (static_cast<ympuint>(pos) & 1ULL);
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
SbjEdge::is_poedge() const
{
  return to()->is_output();
}

END_NAMESPACE_YM_SBJ

#endif // SBJEDGE_H
