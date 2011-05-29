#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATMGR_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATMGR_H

/// @file libym_techmap/cellmap/libdump/LdPatMgr.h
/// @brief LdPatMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"
#include "ym_lexp/lexp_nsdef.h"
#include "ym_utils/Alloc.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

class LdFunc;
class LdPatNode;
class LdPatHandle;

//////////////////////////////////////////////////////////////////////
/// @class LdPatMgr LdPatMgr.h "LdPatMgr.h"
/// @brief パタングラフを生成するクラス
/// @note 前処理で使われるのであまり効率を考えなく良い．
//////////////////////////////////////////////////////////////////////
class LdPatMgr
{
public:

  /// @brief コンストラクタ
  LdPatMgr();

  /// @brief デストラクタ
  ~LdPatMgr();


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

  /// @brief 内容をバイナリダンプする．
  /// @param[in] s 出力先のストリーム
  void
  dump(ostream& s) const;

  /// @brief 内容を出力する．(デバッグ用)
  /// @param[in] s 出力先のストリーム
  void
  display(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内容情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 全ノード数を返す．
  ymuint
  node_num() const;

  /// @brief ノードを返す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  LdPatNode*
  node(ymuint pos) const;

  /// @brief パタン数を返す．
  ymuint
  pat_num() const;

  /// @brief パタンの根のハンドルを返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  LdPatHandle
  pat_root(ymuint id) const;

  /// @brief パタンの属している代表関数番号を返す．
  /// @param[in] id パタン番号 ( 0 <= id < pat_num() )
  ymuint
  rep_id(ymuint id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使われていないパタンとノードを削除してID番号を詰める．
  /// @note 同時に入力ノードの入力番号とノード番号を一致させる．
  void
  sweep();

  /// @brief パタングラフを生成する再帰関数
  /// @param[in] expr 元になる論理式
  /// @param[out] pg_list パタングラフ番号のリスト
  void
  pg_sub(const LogExpr& expr,
	 vector<LdPatHandle>& pg_list);

  /// pg_list に new_handle を追加する．
  /// ただし，同形のパタンがすでにある場合には追加しない．
  static
  void
  add_pg_list(vector<LdPatHandle>& pg_list,
	      LdPatHandle new_handle);

  /// @brief 同形か調べる．
  /// @param[in] node1, node2 根のノード
  static
  bool
  check_isomorphic(const LdPatNode* node1,
		   const LdPatNode* node2);

  /// @brief 入力ノードを作る．
  /// @param[in] id 入力番号
  /// @note 既にあるときはそれを返す．
  LdPatNode*
  make_input(ymuint id);

  /// @brief テンプレートにしたがって2分木を作る．
  LdPatHandle
  make_bintree(const LogExpr& expr,
	       const vector<LdPatHandle>& input,
	       int pat[],
	       ymuint& pos);

  /// @brief 論理式の種類に応じてノードを作る．
  LdPatHandle
  make_node(const LogExpr& expr,
	    LdPatHandle l_handle,
	    LdPatHandle r_handle);

  /// @brief ノードを作る．
  LdPatNode*
  new_node();

  /// @brief ノードを削除する．
  void
  delete_node(LdPatNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // ハッシュ表用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief LdPatNode のハッシュ関数
  static
  ymuint
  hash_func(ymuint type,
	    LdPatNode* l_node,
	    LdPatNode* r_node);


private:
  //////////////////////////////////////////////////////////////////////
  // dump() 用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝の情報をダンプする．
  /// @param[in] s 出力先のストリーム
  /// @param[in] node 親のノード
  /// @param[in] fanin_pos ファンイン番号
  static
  void
  dump_edge(ostream& s,
		    LdPatNode* node,
	    ymuint fanin_pos);


  /// @brief パタングラフを DFS でたどって内容を val_list に入れる．
  /// @param[in] node ノード
  /// @param[in] vmark 訪れたかどうかの情報を持つ配列
  /// @param[out] val_list ノードの情報を格納するリスト
  /// @return 最大入力番号+1を返す．
  static
  ymuint
  dump_dfs(LdPatNode* node,
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
	       LdPatNode* node,
	       ymuint fanin_pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LdPatNode のメモリ確保用アロケータ
  UnitAlloc mAlloc;

  // 入力ノードを収める配列
  vector<LdPatNode*> mInputList;

  // 全ノードを収める配列
  vector<LdPatNode*> mNodeList;

  // AND/XOR ノードのハッシュ表
  LdPatNode** mHashTable;

  // mHashTable のサイズ
  ymuint32 mHashSize;

  // ハッシュ表を拡大する目安
  ymuint32 mNextLimit;

  // パタンの根のハンドルのリスト
  // 配列のインデックスはパタン番号
  vector<LdPatHandle> mPatList;

  // 代表関数番号のリスト
  // 配列のインデックスはパタン番号
  vector<ymuint32> mRepList;

};

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDPATMGR_H
