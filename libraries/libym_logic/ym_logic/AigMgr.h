#ifndef YM_LOGIC_AIGMGR_H
#define YM_LOGIC_AIGMGR_H

/// @file ym_logic/AigMgr.h
/// @brief AigMgrのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Aig.h"
#include "ym_logic/VarId.h"
#include "ym_logic/LogExpr.h"


BEGIN_NAMESPACE_YM_AIG

class AigMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class AigMgr AigMgr.h "ym_logic/AigMgr.h"
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
  /// @param[in] id 入力番号
  const AigNode*
  input_node(VarId id) const;

  /// @brief ノード数を得る．
  ymuint
  node_num() const;

  /// @brief ノードを取り出す．
  /// @param[in] pos ノード番号 ( 0 <= pos < node_num() )
  /// @note ANDノードの他に入力ノードも含まれる．
  const AigNode*
  node(ymuint pos) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  /// @param[in] handle_list 対象のハンドルのリスト
  void
  dump_handles(ostream& s,
	       const list<Aig>& handle_list) const;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造(AigNode)を作成するメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 定数0関数をつくる．
  Aig
  make_zero();

  /// @brief 定数1関数をつくる．
  Aig
  make_one();

  /// @brief 外部入力を作る．
  /// @param[in] id 入力番号
  Aig
  make_input(VarId id);

  /// @brief NOT をとる．
  /// @param[in] edge 入力の AIG ハンドル
  static
  Aig
  make_not(Aig aig);

  /// @brief 2つのノードの AND を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_and(Aig aig1,
	   Aig aig2);

  /// @brief 複数のノードの AND を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_and(const vector<Aig>& aig_list);

  /// @brief 複数のノードの AND を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_and(const list<Aig>& aig_list);

  /// @brief 2つのノードの NAND を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_nand(Aig aig1,
	    Aig aig2);

  /// @brief 複数のノードの NAND を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_nand(const vector<Aig>& aig_list);

  /// @brief 複数のノードの NAND を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_nand(const list<Aig>& aig_list);

  /// @brief 2つのノードの OR を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_or(Aig aig1,
	  Aig aig2);

  /// @brief 複数のノードの OR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_or(const vector<Aig>& aig_list);

  /// @brief 複数のノードの OR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_or(const list<Aig>& aig_list);

  /// @brief 2つのノードの NOR を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_nor(Aig aig1,
	   Aig aig2);

  /// @brief 複数のノードの NOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_nor(const vector<Aig>& aig_list);

  /// @brief 複数のノードの NOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_nor(const list<Aig>& aig_list);

  /// @brief 2つのノードの XOR を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_xor(Aig aig1,
	   Aig aig2);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_xor(const vector<Aig>& aig_list);

  /// @brief 複数のノードの XOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_xor(const list<Aig>& aig_list);

  /// @brief 2つのノードの XNOR を取る．
  /// @param[in] aig1, aig2 入力の AIG ハンドル
  Aig
  make_xnor(Aig aig1,
	    Aig aig2);

  /// @brief 複数のノードの XNOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_xnor(const vector<Aig>& aig_list);

  /// @brief 複数のノードの XNOR を取る．
  /// @param[in] aig_list 入力の AIG ハンドルのリスト
  Aig
  make_xnor(const list<Aig>& aig_list);

  /// @brief 論理式に対応するノード(木)をつくる．
  /// @param[in] expr 対象の論理式
  /// @param[in] input_map 入力とAIGの対応表
  Aig
  make_logic(const LogExpr& expr,
	     const hash_map<VarId, Aig>& input_map);

  /// @brief コファクターを計算する．
  /// @param[in] aig 対象の AIG ハンドル
  /// @param[in] id コファクターをとる変数番号
  /// @param[in] pol 極性
  Aig
  make_cofactor(Aig aig,
		VarId id,
		tPol pol);


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
// @param[in] aig 入力の AIG ハンドル
inline
Aig
AigMgr::make_not(Aig aig)
{
  return ~aig;
}

// @brief 2つのノードの NAND を取る．
// @param[in] aig1, aig2 入力の AIG ハンドル
inline
Aig
AigMgr::make_nand(Aig aig1,
		  Aig aig2)
{
  return make_not(make_and(aig1, aig2));
}

// @brief 複数のノードの NAND を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_nand(const vector<Aig>& aig_list)
{
  return make_not(make_and(aig_list));
}

// @brief 複数のノードの NAND を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_nand(const list<Aig>& aig_list)
{
  return make_not(make_and(aig_list));
}

// @brief 2つのノードの OR を取る．
inline
Aig
AigMgr::make_or(Aig aig1,
		Aig aig2)
{
  return make_not(make_and(make_not(aig1), make_not(aig2)));
}

// @brief 2つのノードの NOR を取る．
// @param[in] aig1, aig2 入力の AIG ハンドル
inline
Aig
AigMgr::make_nor(Aig aig1,
		 Aig aig2)
{
  return make_and(make_not(aig1), make_not(aig2));
}

// @brief 複数のノードの NOR を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_nor(const vector<Aig>& aig_list)
{
  return make_not(make_or(aig_list));
}

// @brief 複数のノードの NOR を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_nor(const list<Aig>& aig_list)
{
  return make_not(make_or(aig_list));
}

// @brief 2つのノードの XOR を取る．
inline
Aig
AigMgr::make_xor(Aig aig1,
		 Aig aig2)
{
  return make_or(make_and(aig1, make_not(aig2)),
		 make_and(make_not(aig1), aig2));
}

// @brief 2つのノードの XNOR を取る．
// @param[in] aig1, aig2 入力の AIG ハンドル
inline
Aig
AigMgr::make_xnor(Aig aig1,
		  Aig aig2)
{
  return make_nor(make_and(aig1, make_not(aig2)),
		  make_and(make_not(aig1), aig2));
}

// @brief 複数のノードの XNOR を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_xnor(const vector<Aig>& aig_list)
{
  return make_not(make_xor(aig_list));
}

// @brief 複数のノードの XNOR を取る．
// @param[in] aig_list 入力の AIG ハンドルのリスト
inline
Aig
AigMgr::make_xnor(const list<Aig>& aig_list)
{
  return make_not(make_xor(aig_list));
}

END_NAMESPACE_YM_AIG

#endif // YM_LOGIC_AIGMGR_H
