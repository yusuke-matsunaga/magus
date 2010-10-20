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
/// @note 前処理で使われるのであまり効率を考えなく良い．
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
  operator()(const LogExpr& expr,
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

  /// @brief グラフ構造全体の内容を表示する．
  /// @param[in] s 出力先のストリーム
  void
  display_graph(ostream& s) const;

  /// @brief エッジリストの内容を表示する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] root 根のハンドル
  void
  display_edgelist(ostream& s,
		   PgHandle root) const;

  /// @brief グラフ構造全体をダンプする．
  /// @param[in] s 出力先のストリーム
  /// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
  void
  dump_graph(ostream& s) const;

  /// @brief エッジリストをダンプする．
  /// @param[in] s 出力先のストリーム
  /// @param[in] root 根のハンドル
  /// @note 内容はエッジ番号のリスト
  /// @note ただし最初に根の反転属性の情報を含む
  void
  dump_edgelist(ostream& s,
		PgHandle root) const;


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
  // ハッシュ表用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief PgNode のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    PgNode* l_node,
	    PgNode* r_node);


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

  // AND/XOR ノードのハッシュ表
  PgNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PATGEN_H
