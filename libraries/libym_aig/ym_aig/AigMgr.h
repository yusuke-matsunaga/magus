#ifndef YM_AIG_AIGMGR_H
#define YM_AIG_AIGMGR_H

/// @file ym_aig/AigMgr.h
/// @brief AigMgrのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigMgr.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigHandle.h"
#include "ym_lexp/LogExpr.h"
#include "ym_sat/SatSolver.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class AigMgr AigMgr.h <ym_aig/AigMgr.h>
/// @brief AIGを管理するクラス(の純粋仮想クラス)
//////////////////////////////////////////////////////////////////////
class AigMgr
{
public:

  /// @brief コンストラクタ
  AigMgr();

  /// @brief デストラクタ
  ~AigMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノード数を得る．
  ymuint
  input_num() const;

  /// @brief 入力ノードを取り出す．
  /// @param[in] pos 入力番号 ( 0 <= pos < input_num() )
  AigNode*
  input_node(ymuint pos) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  AigNode*
  node(ymuint pos) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] handle_list 対象のハンドルのリスト
  void
  dump_handles(ostream& s,
	       const list<AigHandle>& handle_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(AigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  AigHandle
  make_zero();

  /// @brief 定数1関数をつくる．
  AigHandle
  make_one();

  /// @brief 外部入力を作る．
  AigHandle
  make_input();

  /// @brief NOT をとる．
  /// @param[in] edge 入力の AIG ハンドル
  static
  AigHandle
  make_not(AigHandle edge);

  /// @brief 2つのノードの AND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_and(AigHandle edge1,
	   AigHandle edge2);

  /// @brief 複数のノードの AND を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_and(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの AND を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_and(const list<AigHandle>& edge_list);

  /// @brief 2つのノードの NAND を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_nand(AigHandle edge1,
	    AigHandle edge2);

  /// @brief 複数のノードの NAND を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_nand(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの NAND を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_nand(const list<AigHandle>& edge_list);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_or(AigHandle edge1,
	  AigHandle edge2);

  /// @brief 複数のノードの OR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_or(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの OR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_or(const list<AigHandle>& edge_list);

  /// @brief 2つのノードの NOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_nor(AigHandle edge1,
	   AigHandle edge2);

  /// @brief 複数のノードの NOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_nor(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの NOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_nor(const list<AigHandle>& edge_list);

  /// @brief 2つのノードの XOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_xor(AigHandle edge1,
	   AigHandle edge2);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_xor(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_xor(const list<AigHandle>& edge_list);

  /// @brief 2つのノードの XNOR を取る．
  /// @param[in] edge1, edge2 入力の AIG ハンドル
  AigHandle
  make_xnor(AigHandle edge1,
	    AigHandle edge2);

  /// @brief 複数のノードの XNOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_xnor(const vector<AigHandle>& edge_list);

  /// @brief 複数のノードの XNOR を取る．
  /// @param[in] edge_list 入力の AIG ハンドルのリスト
  AigHandle
  make_xnor(const list<AigHandle>& edge_list);

  /// @brief 論理式に対応するノード(木)をつくる．
  /// @param[in] expr 対象の論理式
  /// @param[in] inputs 入力に対応する AIG ハンドル
  AigHandle
  make_logic(const LogExpr& expr,
	     const vector<AigHandle>& inputs);

  /// @brief コファクターを計算する．
  /// @param[in] edge 対象の AIG ハンドル
  /// @param[in] id コファクターをとる変数番号
  /// @param[in] pol 極性
  AigHandle
  make_cofactor(AigHandle edge,
		tVarId id,
		tPol pol);


public:
  //////////////////////////////////////////////////////////////////////
  // SAT solver とのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief SAT 問題を解く．
  /// @param[in] solver SAT-solver
  /// @param[in] edge この出力を1にできるか調べる．
  /// @param[out] model 外部入力の割り当てを入れる配列
  /// @retval kB3False 充足不能
  /// @retval kB3True 充足可能
  /// @retval kB3X 不明
  Bool3
  sat(SatSolver* solver,
      AigHandle edge,
      vector<Bool3>& model);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の AigMgr の実装
  AigMgrImpl* mImpl;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief NOT をとる．
// @param[in] edge 入力の AIG ハンドル
inline
AigHandle
AigMgr::make_not(AigHandle edge)
{
  return ~edge;
}

// @brief 2つのノードの NAND を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
AigHandle
AigMgr::make_nand(AigHandle edge1,
		  AigHandle edge2)
{
  return make_not(make_and(edge1, edge2));
}

// @brief 複数のノードの NAND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_nand(const vector<AigHandle>& edge_list)
{
  return make_not(make_and(edge_list));
}

// @brief 複数のノードの NAND を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_nand(const list<AigHandle>& edge_list)
{
  return make_not(make_and(edge_list));
}

// @brief 2つのノードの OR を取る．
inline
AigHandle
AigMgr::make_or(AigHandle edge1,
		AigHandle edge2)
{
  return make_not(make_and(make_not(edge1), make_not(edge2)));
}

// @brief 2つのノードの NOR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
AigHandle
AigMgr::make_nor(AigHandle edge1,
		 AigHandle edge2)
{
  return make_and(make_not(edge1), make_not(edge2));
}

// @brief 複数のノードの NOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_nor(const vector<AigHandle>& edge_list)
{
  return make_not(make_or(edge_list));
}

// @brief 複数のノードの NOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_nor(const list<AigHandle>& edge_list)
{
  return make_not(make_or(edge_list));
}

// @brief 2つのノードの XOR を取る．
inline
AigHandle
AigMgr::make_xor(AigHandle edge1,
		 AigHandle edge2)
{
  return make_or(make_and(edge1, make_not(edge2)),
		 make_and(make_not(edge1), edge2));
}

// @brief 2つのノードの XNOR を取る．
// @param[in] edge1, edge2 入力の AIG ハンドル
inline
AigHandle
AigMgr::make_xnor(AigHandle edge1,
		  AigHandle edge2)
{
  return make_nor(make_and(edge1, make_not(edge2)),
		  make_and(make_not(edge1), edge2));
}

// @brief 複数のノードの XNOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_xnor(const vector<AigHandle>& edge_list)
{
  return make_not(make_xor(edge_list));
}

// @brief 複数のノードの XNOR を取る．
// @param[in] edge_list 入力の AIG ハンドルのリスト
inline
AigHandle
AigMgr::make_xnor(const list<AigHandle>& edge_list)
{
  return make_not(make_xor(edge_list));
}

END_NAMESPACE_YM_AIG

#endif // YM_AIG_AIGMGR_H
