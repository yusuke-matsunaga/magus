#ifndef RWTPATGEN_H
#define RWTPATGEN_H

/// @file RwtPatGen.h
/// @brief RwtPatGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "networks/bdn.h"
#include "logic/TvFunc.h"
#include "utils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_NETWORKS

class RwtPat;
class PgNode;
class PgPat;

//////////////////////////////////////////////////////////////////////
/// @class RwtPatGen RwtPatGen.h "RwtPatGen.h"
/// @brief RwtMgr の元データを作るためのクラス
//////////////////////////////////////////////////////////////////////
class RwtPatGen
{
public:

  /// @brief コンストラクタ
  RwtPatGen();

  /// @brief デストラクタ
  ~RwtPatGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を得る．
  ymuint
  input_num() const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを得る．
  /// @param[in] id ノード番号 ( 0 <= id < node_num() )
  const PgNode*
  node(ymuint id) const;

  /// @brief 登録されている関数の数を返す．
  ymuint
  func_num() const;

  /// @brief 関数を得る．
  /// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
  TvFunc
  func(ymuint pos) const;

  /// @brief 関数に対応するパタンのリストを得る．
  /// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
  const vector<PgPat*>&
  pat_list(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // パタンを作るための関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] input_num
  /// @note 定数0ノードおよび 0 〜 (input_num - 1) までの入力ノードを作る．
  void
  init(ymuint input_num);

  /// @brief AND ノードを作る．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] fanin1 ファンイン1
  /// @param[in] inv1 ファンイン1の極性
  const PgNode*
  new_and(const PgNode* fanin0,
	  bool inv0,
	  const PgNode* fanin1,
	  bool inv1);

  /// @brief XOR ノードを作る．
  /// @param[in] fanin0 ファンイン0
  /// @param[in] fanin1 ファンイン1
  const PgNode*
  new_xor(const PgNode* fanin0,
	  const PgNode* fanin1);

  /// @brief パタンを登録する．
  void
  new_pat(const PgNode* root,
	  bool inv);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief パタンのノードをDFSで訪れる．
  void
  dfs(const PgNode* node,
      vector<bool>& mark,
      vector<const PgNode*>& inputs,
      vector<const PgNode*>& node_list);

  /// @brief ノードを返す．
  /// @param[in] type タイプ
  /// @param[in] fanin0 ファンイン0のノード番号＋極性
  /// @param[in] fanin1 ファンイン1のノード番号＋極性
  /// @note 既に同じノードが存在していたらそれを返す．
  /// @note なければ新しく作ってそれを返す．
  PgNode*
  new_node(ymuint32 type,
	   ymuint32 fanin0,
	   ymuint32 fanin1);

  /// @brief ノードを確保する．
  PgNode*
  alloc_node();

  /// @brief ハッシュ表を確保する．
  void
  alloc_table(ymuint req_size);

  /// @brief PgNode のハッシュ関数
  static
  ymuint
  hash_func(ymuint32 type,
	    ymuint32 fanin0,
	    ymuint32 fanin1);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 入力数
  ymuint32 mInputNum;

  // ノードの配列
  vector<PgNode*> mNodeArray;

  // ノードのハッシュ表
  PgNode** mNodeHashTable;

  // mNodeHashTable のサイズ
  ymuint32 mNodeHashSize;

  // ノードのハッシュ表を拡大するリミット値
  ymuint32 mNodeHashLimit;

  // 関数のリスト
  vector<TvFunc> mFuncList;

#if __GNUC__ == 4 && __GNUC_MINOR__ >= 6
  typedef unordered_map<TvFunc, ymuint32> FuncMap;
#else
  typedef hash_map<TvFunc, ymuint32> FuncMap;
#endif

  // 関数のハッシュ表
  FuncMap mFuncHash;

  // パタンのリスト
  vector<vector<PgPat*> > mPatList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 入力数を得る．
inline
ymuint
RwtPatGen::input_num() const
{
  return mInputNum;
}

// @brief ノード数を得る．
inline
ymuint
RwtPatGen::node_num() const
{
  return mNodeArray.size();
}

// @brief ノードを得る．
// @param[in] id ノード番号 ( 0 <= id < node_num() )
inline
const PgNode*
RwtPatGen::node(ymuint id) const
{
  assert_cond( id < node_num(), __FILE__, __LINE__);
  return mNodeArray[id];
}

// @brief 登録されている関数の数を返す．
inline
ymuint
RwtPatGen::func_num() const
{
  return mFuncList.size();
}

// @brief 関数を得る．
// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
inline
TvFunc
RwtPatGen::func(ymuint pos) const
{
  assert_cond( pos < func_num(), __FILE__, __LINE__);
  return mFuncList[pos];
}

// @brief 関数に対応するパタンのリストを得る．
// @param[in] pos 関数番号 ( 0 <= pos < func_num() )
inline
const vector<PgPat*>&
RwtPatGen::pat_list(ymuint pos) const
{
  assert_cond( pos < func_num(), __FILE__, __LINE__);
  return mPatList[pos];
}

// @brief PgNode のハッシュ関数
inline
ymuint
RwtPatGen::hash_func(ymuint32 type,
		     ymuint32 fanin0,
		     ymuint32 fanin1)
{
  return type + (fanin0 << 2) + (fanin1 << 3);
}

END_NAMESPACE_YM_NETWORKS

#endif // RWTPATGEN_H
