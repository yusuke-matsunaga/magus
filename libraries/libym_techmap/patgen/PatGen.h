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
#include "ym_utils/Alloc.h"


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
  /// @note expr は定数を含んではいけない．
  void
  gen_pat(const LogExpr& expr,
	  vector<PgHandle>& pg_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  /// @note ただし使われていない入力もありうる．
  ymuint
  input_num() const;

  /// @brief 入力ノードを返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  PgNode*
  input_node(ymuint pos) const;

  /// @brief 全ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  PgNode*
  node(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードを作る．
  /// @param[in] id 入力番号
  /// @note 既にあるときはそれを返す．
  PgNode*
  make_input(ymuint id);

  /// @brief テンプレートにしたがって2分木を作る．
  PgHandle
  make_bintree(const LogExpr& expr,
	       const vector<PgHandle>& input,
	       int pat[],
	       ymuint& pos);

  /// @brief 論理式の種類に応じてノードを作る．
  PgHandle
  make_node(const LogExpr& expr,
	    PgHandle l_handle,
	    PgHandle r_handle);

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

  // 全ノードを収める配列
  vector<PgNode*> mNodeList;

};

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PATGEN_H
