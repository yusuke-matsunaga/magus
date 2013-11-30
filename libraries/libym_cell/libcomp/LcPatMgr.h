#ifndef LCPATMGR_H
#define LCPATMGR_H

/// @file LcPatMgr.h
/// @brief LcPatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "ym_logic/lexp_nsdef.h"
#include "ym_utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcPatMgr LcPatMgr.h "LcPatMgr.h"
/// @brief パタングラフを生成するクラス
/// @note 前処理で使われるのであまり効率を考えていない．
//////////////////////////////////////////////////////////////////////
class LcPatMgr
{
public:

  /// @brief コンストラクタ
  LcPatMgr();

  /// @brief デストラクタ
  ~LcPatMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 論理式から生成されるパタンを登録する．
  /// @param[in] expr パタンの元となる論理式
  /// @param[in] rep_id このパタンが属する代表関数番号
  void
  reg_pat(const LogExpr& expr,
	  ymuint rep_id);

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  LcPatNode*
  node(ymuint pos) const;

  /// @brief パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンの根のハンドルを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  LcPatHandle
  pat_root(ymuint id) const;

  /// @brief パタンのノードリストを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  /// @param[out] node_list パタンを DFS 順でたどった時のノード番号のリスト
  /// @return このパタンの入力数を返す．
  ymuint
  pat_node_list(ymuint id,
		vector<ymuint>& node_list) const;

  /// @brief パタンの属している代表関数番号を返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  ymuint
  rep_id(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの論理式が等しいかどうか調べる．
  static
  bool
  check_equivalent(const LogExpr& expr1,
		   const LogExpr& expr2);

  /// @brief 2つのパタンが同型かどうか調べる．
  static
  bool
  check_equivalent(LcPatHandle handle1,
		   LcPatHandle handle2);

  /// @brief check_equivalent の下請け関数
  static
  bool
  ceq_sub(LcPatNode* node1,
	  LcPatNode* node2,
	  hash_map<ymuint, ymuint>& map1,
	  hash_map<ymuint, ymuint>& map2);

  /// @brief パタングラフを生成する再帰関数
  /// @param[in] expr 元になる論理式
  /// @param[out] pg_list パタングラフ番号のリスト
  void
  pg_sub(const LogExpr& expr,
	 vector<LcPatHandle>& pg_list);

  /// @brief テンプレートにしたがって2分木を作る．
  /// @param[in] expr 論理式 (演算の種類を表すのに用いる)
  /// @param[in] input 入力の配列
  /// @param[in] pat 2分木の形を表す配列
  /// @param[inout] pos pat[] 中の位置を示す変数
  LcPatHandle
  make_bintree(const LogExpr& expr,
	       const vector<LcPatHandle>& input,
	       int pat[],
	       ymuint& pos);

  /// @brief 入力ノードを作る．
  /// @param[in] var 入力変数
  /// @note 既にあるときはそれを返す．
  LcPatNode*
  make_input(VarId var);

  /// @brief 論理式の種類に応じてノードを作る．
  /// @param[in] expr 論理式 (演算の種類を表すのに用いる)
  /// @param[in] l_handle, r_handle 左右の子供のパタン
  LcPatHandle
  make_node(const LogExpr& expr,
	    LcPatHandle l_handle,
	    LcPatHandle r_handle);

  /// @brief ノードを作る．
  LcPatNode*
  new_node();

  /// @brief ノードを削除する．
  void
  delete_node(LcPatNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief LcPatNode のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    LcPatNode* l_node,
	    LcPatNode* r_node);


private:
  //////////////////////////////////////////////////////////////////////
  // dump() 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝の情報をエンコードする．
  /// @param[in] node 親のノード
  /// @param[in] fanin_pos ファンイン番号
  static
  ymuint32
  encode_edge(LcPatNode* node,
	      ymuint fanin_pos);


  /// @brief パタングラフを DFS でたどって内容を val_list に入れる．
  /// @param[in] node ノード
  /// @param[in] vmark 訪れたかどうかの情報を持つ配列
  /// @param[out] val_list ノードの情報を格納するリスト
  /// @return 最大入力番号+1を返す．
  static
  ymuint
  dump_dfs(LcPatNode* node,
	   vector<bool>& vmark,
	   vector<ymuint>& val_list);


private:
  //////////////////////////////////////////////////////////////////////
  // display() 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝の情報を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 親のノード
  /// @param[in] fanin_pos ファンイン番号
  static
  void
  display_edge(ostream& s,
	       LcPatNode* node,
	       ymuint fanin_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LcPatNode のメモリ確保用アロケータ
  UnitAlloc mAlloc;

  // 入力ノードを収める配列
  vector<LcPatNode*> mInputList;

  // 全ノードを収める配列
  vector<LcPatNode*> mNodeList;

  // AND/XOR ノードのハッシュ表
  LcPatNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // パタンの根のハンドルのリスト
  // 配列のインデックスはパタン番号
  vector<LcPatHandle> mPatList;

  // 代表関数番号のリスト
  // 配列のインデックスはパタン番号
  vector<ymuint32> mRepList;

  // 処理済みの論理式を収めたリストの配列
  // 配列のキーは代表関数番号
  vector<vector<LogExpr> > mExprList;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPATMGR_H
