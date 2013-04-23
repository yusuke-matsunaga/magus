#ifndef LCPAT2MGR_H
#define LCPAT2MGR_H

/// @file LcPat2Mgr.h
/// @brief LcPat2Mgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "ym_logic/lexp_nsdef.h"
#include "ym_utils/UnitAlloc.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcPat2Mgr LcPat2Mgr.h "LcPat2Mgr.h"
/// @brief パタングラフを生成するクラス
/// @note 前処理で使われるのであまり効率を考えていない．
//////////////////////////////////////////////////////////////////////
class LcPat2Mgr
{
public:

  /// @brief コンストラクタ
  LcPat2Mgr();

  /// @brief デストラクタ
  ~LcPat2Mgr();


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
  LcPat2Node*
  node(ymuint pos) const;

  /// @brief パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンの根のハンドルを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  LcPat2Handle
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

#if 1

  /// @brief パタングラフを生成する再帰関数
  /// @param[in] expr 元になる論理式
  LcPat2Handle
  pg_sub(const LogExpr& expr);

#else

  /// @brief 使われていないパタンとノードを削除してID番号を詰める．
  /// @note 同時に入力ノードの入力番号とノード番号を一致させる．
  void
  sweep();

  /// @brief パタングラフを生成する再帰関数
  /// @param[in] expr 元になる論理式
  /// @param[out] pg_list パタングラフ番号のリスト
  void
  pg_sub(const LogExpr& expr,
	 vector<LcPat2Handle>& pg_list);

  /// @brief pg_list に new_handle を追加する．
  /// @note ただし，同形のパタンがすでにある場合には追加しない．
  static
  void
  add_pg_list(vector<LcPat2Handle>& pg_list,
	      hash_set<string>& pg_hash,
	      LcPat2Handle new_handle);

#endif

  /// @brief 入力ノードを作る．
  /// @param[in] var 入力変数
  /// @note 既にあるときはそれを返す．
  LcPat2Node*
  make_input(VarId var);

  /// @brief 論理式の種類に応じてノードを作る．
  /// @param[in] expr 論理式 (演算の種類を表すのに用いる)
  /// @param[in] chd_handles 子供のパタンの配列
  LcPat2Handle
  make_node(const LogExpr& expr,
	    const vector<LcPat2Handle>& chd_hanles);

  /// @brief ノードを作る．
  /// @param[in] fanin_num ファンイン数
  LcPat2Node*
  new_node(ymuint fanin_num);


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief LcPat2Node のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    const vector<LcPat2Handle>& chd_array);

  /// @brief LcPat2Node のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    LcPat2Handle chd_array[]);


private:
  //////////////////////////////////////////////////////////////////////
  // dump() 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝の情報をエンコードする．
  /// @param[in] node 親のノード
  /// @param[in] fanin_pos ファンイン番号
  static
  ymuint32
  encode_edge(LcPat2Node* node,
	      ymuint fanin_pos);


  /// @brief パタングラフを DFS でたどって内容を val_list に入れる．
  /// @param[in] node ノード
  /// @param[in] vmark 訪れたかどうかの情報を持つ配列
  /// @param[out] val_list ノードの情報を格納するリスト
  /// @return 最大入力番号+1を返す．
  static
  ymuint
  dump_dfs(LcPat2Node* node,
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
	       LcPat2Node* node,
	       ymuint fanin_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LcPat2Node のメモリ確保用アロケータ
  UnitAlloc mAlloc;

  // 入力ノードを収める配列
  vector<LcPat2Node*> mInputList;

  // 全ノードを収める配列
  vector<LcPat2Node*> mNodeList;

  // AND/XOR ノードのハッシュ表
  LcPat2Node** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // パタンの根のハンドルのリスト
  // 配列のインデックスはパタン番号
  vector<LcPat2Handle> mPatList;

  // 代表関数番号のリスト
  // 配列のインデックスはパタン番号
  vector<ymuint32> mRepList;

  // 処理済みの論理式を収めたリストの配列
  // 配列のキーは代表関数番号
  vector<vector<LogExpr> > mExprList;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPAT2MGR_H
