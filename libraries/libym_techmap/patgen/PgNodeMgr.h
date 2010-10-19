#ifndef LIBYM_TECHMAP_PATGEN_PGNODEMGR_H
#define LIBYM_TECHMAP_PATGEN_PGNODEMGR_H

/// @file libym_techmap/patgen/PgNode.h
/// @brief PgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgNode;

//////////////////////////////////////////////////////////////////////
/// @class PgNodeMgr PgNode.h "PgNode.h"
/// @brief PgNode を管理するクラス
//////////////////////////////////////////////////////////////////////
class PgNodeMgr
{
public:

  /// @brief コンストラクタ
  PgNodeMgr();

  /// @brief デストラクタ
  ~PgNodeMgr();


public:

  /// @brief 入力ノードを作る．
  /// @param[in] id 入力番号
  /// @note 既にあるときはそれを返す．
  PgNode*
  new_input(ymuint id);

  /// @brief AND ノードを作る．
  /// @param[in] fanin0, fanin1 ファンインのノード
  /// @param[in] inv0, inv1 ファンインの反転属性
  PgNode*
  new_and(PgNode* fanin0,
	  PgNode* fanin1,
	  bool inv0,
	  bool inv1);

  /// @brief XOR ノードを作る．
  /// @param[in] fanin0, fanin1 ファンインのノード
  PgNode*
  new_xor(PgNode* fanin0,
	  PgNode* fanin1);

  /// @brief ノードを削除する．
  void
  delete_node(PgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを作る．
  PgNode*
  new_node();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PgNode のメモリ確保用アロケータ
  UnitAlloc mAlloc;

  // 入力ノードを収める配列
  vector<PgNode*> mInputList;

};

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGNODEMGR_H
