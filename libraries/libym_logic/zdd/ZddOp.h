#ifndef ZDDOP_H
#define ZDDOP_H

/// @file ZddOp.h
/// @brief ZddOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ZddEdge.h"
#include "ZddNode.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddUnOp ZddOp.h "ZddOp.h"
/// @brief ZDD の単項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddUnOp
{
public:

  /// @brief コンストラクタ
  ZddUnOp() { }

  /// @brief デストラクタ
  virtual
  ~ZddUnOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left オペランド
  /// @param[in] varid 変数番号
  virtual
  ZddEdge
  apply(ZddEdge left,
	VarId varid) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のレベルにしたがって子供のノードを求める．
  /// @param[in] f, g オペランド
  /// @param[out] f_0, f_1 f の子供
  /// @param[out] g_0, g_1 g の子供
  /// @return 根のレベルを返す．
  static
  ymuint
  split(ZddEdge f,
	ZddEdge g,
	ZddEdge& f_0,
	ZddEdge& f_1,
	ZddEdge& g_0,
	ZddEdge& g_1);

  /// @brief split() の下請け関数
  /// @param[in] top_level 根のレベル
  /// @param[in] level レベル
  /// @param[in] e 枝
  /// @param[in] node ノード
  /// @param[out] e_0 0枝
  /// @param[out] e_1 1枝
  static
  void
  split1(ymuint top_level,
	 ymuint level,
	 ZddEdge e,
	 ZddNode* node,
	 ZddEdge& e_0,
	 ZddEdge& e_1);

};


//////////////////////////////////////////////////////////////////////
/// @class ZddOp ZddOp.h "ZddOp.h"
/// @brief ZDD の二項演算を行うクラス
//////////////////////////////////////////////////////////////////////
class ZddBinOp
{
public:

  /// @brief コンストラクタ
  ZddBinOp() { }

  /// @brief デストラクタ
  virtual
  ~ZddBinOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 演算を行う関数
  /// @param[in] left, right オペランド
  virtual
  ZddEdge
  apply(ZddEdge left,
	ZddEdge right) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根のレベルにしたがって子供のノードを求める．
  /// @param[in] f, g オペランド
  /// @param[out] f_0, f_1 f の子供
  /// @param[out] g_0, g_1 g の子供
  /// @return 根のレベルを返す．
  static
  ymuint
  split(ZddEdge f,
	ZddEdge g,
	ZddEdge& f_0,
	ZddEdge& f_1,
	ZddEdge& g_0,
	ZddEdge& g_1);

  /// @brief split() の下請け関数
  /// @param[in] top_level 根のレベル
  /// @param[in] level レベル
  /// @param[in] e 枝
  /// @param[in] node ノード
  /// @param[out] e_0 0枝
  /// @param[out] e_1 1枝
  static
  void
  split1(ymuint top_level,
	 ymuint level,
	 ZddEdge e,
	 ZddNode* node,
	 ZddEdge& e_0,
	 ZddEdge& e_1);

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根のレベルにしたがって子供のノードを求める．
// @param[in] f, g オペランド
// @param[out] f_0, f_1 f の子供
// @param[out] g_0, g_1 g の子供
// @return 根のレベルを返す．
inline
ymuint
ZddBinOp::split(ZddEdge f,
		ZddEdge g,
		ZddEdge& f_0,
		ZddEdge& f_1,
		ZddEdge& g_0,
		ZddEdge& g_1)
{
  ZddNode* f_vp = f.get_node();
  ZddNode* g_vp = g.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( level > g_level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_0, f_1);
  split1(level, g_level, g, g_vp, g_0, g_1);

  return level;
}

// @brief split() の下請け関数
// @param[in] top_level 根のレベル
// @param[in] level レベル
// @param[in] e 枝
// @param[in] node ノード
// @param[out] e_0 0枝
// @param[out] e_1 1枝
inline
void
ZddBinOp::split1(ymuint top_level,
		 ymuint level,
		 ZddEdge e,
		 ZddNode* node,
		 ZddEdge& e_0,
		 ZddEdge& e_1)
{
  if ( top_level == level ) {
    e_0 = node->edge0();
    e_1 = node->edge1();
    e_0.add_zattr(e.zattr());
  }
  else {
    e_0 = e;
    e_1 = ZddEdge::make_zero();
  }
}

END_NAMESPACE_YM_ZDD

#endif // ZDDOP_H
