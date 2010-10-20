#ifndef LIBYM_TECHMAP_PATGEN_PATGEN_H
#define LIBYM_TECHMAP_PATGEN_PATGEN_H

/// @file libym_techmap/patgen/PatGen.h
/// @brief PatGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "ym_lexp/lexp_nsdef.h"
#include "PgNode.h"
#include "PgNodeMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN


//////////////////////////////////////////////////////////////////////
/// @class PatGen PatGen.h "PatGen.h"
/// @brief パタングラフを生成するクラス
//////////////////////////////////////////////////////////////////////
class PatGen
{
public:

  /// @brief コンストラクタ
  PatGen();

  /// @brief デストラクタ
  ~PatGen();


public:

  /// @brief 論理式から対応するパタングラフを生成する．
  /// @param[in] expr 元になる論理式
  /// @param[out] pg_list パタングラフのリスト
  /// @note pg_list の中身はこの PatGen のインスタンスが破壊されると
  /// 無効になる．
  void
  operator()(const LogExpr& expr,
	     PgNodeMgr& mgr,
	     vector<PgHandle>& pg_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタングラフを生成する再帰関数
  void
  gen_pat(const LogExpr& expr,
	  PgNodeMgr& mgr,
	  vector<PgHandle>& pg_list);

  /// @brief テンプレートにしたがって2分木を作る．
  PgHandle
  make_graph(PgNodeMgr& mgr,
	     const LogExpr& expr,
	     const vector<PgHandle>& input,
	     int pat[],
	     ymuint& pos);

  /// @brief 論理式の種類に応じてノードを作る．
  PgHandle
  make_node(PgNodeMgr& mgr,
	    const LogExpr& expr,
	    PgHandle l_handle,
	    PgHandle r_handle);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // PgNode を管理するオブジェクト
  PgNodeMgr mNodeMgr;

};

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PATGEN_H
