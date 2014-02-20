#ifndef LOGIC_DGMGR_H
#define LOGIC_DGMGR_H

/// @file logic/DgMgr.h
/// @brief DgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/dec_nsdef.h"
#include "logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_DEC

//////////////////////////////////////////////////////////////////////
/// @class DgMgr DgMgr.h "logic/DgMgr.h"
/// @brief 分解グラフを管理するクラス
/// @sa Dg
//////////////////////////////////////////////////////////////////////
class DgMgr
{
  friend class Dg;

public:

  /// @brief プロファイル情報
  struct Profile {
    /// @brief Case1Or の呼ばれた回数
    int mCase1OrNum;

    /// @brief Case1Xor の呼ばれた回数
    int mCase1XorNum;

    /// @brief Case1Cplxの呼ばれた回数
    int mCase1CplxNum;

    /// @brief Case1Cplx2の呼ばれた回数
    int mCase1Cplx2Num;

    /// @brief Case2Or の呼ばれた回数
    int mCase2OrNum;

    /// @brief Case2Xor の呼ばれた回数
    int mCase2XorNum;

    /// @brief Case2Cplx の呼ばれた回数
    int mCase2CplxNum;

    /// @brief コンストラクタ
    Profile();

    /// @brief プロファイル情報を出力する．
    void
    display(ostream& s) const;
  };

public:

  /// @brief コンストラクタ
  /// @param[in] bddmgr 親の BddMgr
  explicit
  DgMgr(BddMgr& bddmgr);

  /// @brief デストラクタ
  ~DgMgr();


public:

  /// @brief BDD から DG を作る．
  /// @return 根の枝を返す．
  Dg
  decomp(const Bdd& F);

  /// @brief BDD から BDG を作る．
  /// @return 根の枝を返す．
  Dg
  bidecomp(const Bdd& F);

  /// @brief 現在持っている Decomposition Graph ハッシュ表をクリアする．
  void
  clear();

  /// @brief 参照回数が0のBDDのエントリを削除する．
  void
  sweep();

  /// @brief プロファイル情報を得る．
  void
  get_profile(Profile& prof) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  // 新たなノードを作り，それに極性を加えて枝を返す．
  tDgEdge
  new_edge(const Bdd& f,
	   const BddVarSet& sup,
	   tType type,
	   const vector<tDgEdge>& inputs,
	   tPol opol = kPolPosi);

  // 定数ノード(枝)を作る．
  // val == 0 の時0 それ以外の時1となる．
  tDgEdge
  make_const(int val);

  // リテラルを作る．
  // リテラル番号および極性を指定する．
  tDgEdge
  make_literal(tVarId varid,
	       tPol pol);

  // 複数の入力を持つORノードを作る．
  // 入力は inputs を参照する．
  // inputs[i] も OR ノードの場合には併合する．
  tDgEdge
  make_or(const vector<tDgEdge>& inputs,
	  tPol opol = kPolPosi);

  // ノード v の部分的な入力を持つ OR ノードを作る．
  // InputFlag が flag に一致するもののみを使う．
  tDgEdge
  make_or(DgNode* v,
	  int flag,
	  tPol opol = kPolPosi);

  // 複数の入力を持つXORノードを作る．
  // 入力は inputs を参照する．
  // inputs[i] も OR ノードの場合には併合する．
  tDgEdge
  make_xor(const vector<tDgEdge>& inputs,
	   tPol opol = kPolPosi);

  // ノード v の部分的な入力を持つ XOR ノードを作る．
  // InputFlag が flag に一致するもののみを使う．
  tDgEdge
  make_xor(DgNode* v,
	   int flag,
	   tPol opol = kPolPosi);

  // Cplx ノードを作る．
  // 入力は inputs を参照する．
  tDgEdge
  make_cplx(const Bdd& f,
	    const vector<DgNode*>& inputs,
	    tPol opol = kPolPosi);

  // Cplx ノードを作る．
  // こちらは入力がリテラルの時．ノード配列は必要ない．
  tDgEdge
  make_cplx(const Bdd& f,
	    const BddVarSet& support,
	    tPol opol = kPolPosi);

  // 2つの入力を持つORノードを作る．
  tDgEdge
  make_or(tDgEdge src1,
	  tDgEdge src2,
	  tPol opol = kPolPosi);

  // 2つの入力を持つXORノードを作る．
  tDgEdge
  make_xor(tDgEdge src1,
	   tDgEdge src2,
	   tPol opol = kPolPosi);

  // 2つの入力を持つANDノード(実際にはORノードの否定)を作る．
  tDgEdge
  make_and(tDgEdge src1,
	   tDgEdge src2,
	   tPol opol = kPolPosi);

  // 複数の入力を持つノードを作る．
  // type には kDgOr か kDgXor を指定する．
  tDgEdge
  make_node(tType type,
	    const vector<tDgEdge>& inputs,
	    tPol opol = kPolPosi);

  // リテラルとのORノードをつくる．
  tDgEdge
  make_litOr(tVarId varid,
	     tPol lit_pol,
	     tDgEdge src2,
	     tPol opol = kPolPosi);

  // リテラルとのXORノードをつくる．
  tDgEdge
  make_litXor(tVarId varid,
	      tPol lit_pol,
	      tDgEdge src2,
	      tPol opol = kPolPosi);

  // リテラルとのANDノードをつくる．
  tDgEdge
  make_litAnd(tVarId varid,
	      tPol lit_pol,
	      tDgEdge src2,
	      tPol opol = kPolPosi);


public:
  //////////////////////////////////////////////////////////////////////
  // 以下は disjunctive decomposition 用のサブルーティン
  //////////////////////////////////////////////////////////////////////

  // v0, v1 がともにORノードで共通の入力を持つ場合の処理．
  // (nc, common), (n0, inputs0), (n1, input1) がそれぞれ
  // 共通，v0のみ，v1のみの入力番号を持つ．
  // common は v0 の入力番号
  tDgEdge
  case1or(tVarId varid,
	  DgNode* v0,
	  DgNode* v1,
	  tPol pol0,
	  ymuint nc);

  // v0, v1 がともにXORノードで共通の入力を持つ場合の処理．
  // (nc, common), (n0, inputs0), (n1, input1) がそれぞれ
  // 共通，v0のみ，v1のみの入力番号を持つ．
  // common は v0 の入力番号
  tDgEdge
  case1xor(tVarId varid,
	   DgNode* v0,
	   DgNode* v1,
	   tPol pol0,
	   tPol pol1,
	   ymuint nc);

  // v0 が OR で，r1 を入力として持つ．
  tDgEdge
  case2or(tVarId varid,
	  DgNode* v0,
	  tPol pol0,
	  ymuint pos,
	  tDgEdge r1,
	  tPol ipol);

  // v0 が XR で，r1 を入力として持つ．
  tDgEdge
  case2xor(tVarId varid,
	   DgNode* v0,
	   tPol pol0,
	   ymuint pos,
	   DgNode* v1,
	   tPol pol1,
	   tPol ipol);

  // v0, v1 がともにCPLXノードでただ1組の入力を除いて共通な入力
  // を持つ場合の処理．
  // d0, d1 は異なっている入力番号
  tDgEdge
  case1cplx(const Bdd& F,
	    tVarId varid,
	    DgNode* v0,
	    DgNode* v1,
	    ymuint d0,
	    ymuint d1,
	    tPol xpol);

  // v0, v1 がともにCPLXノードですべての入力が共通な場合．
  tDgEdge
  case1cplx2(const Bdd& F,
	     tVarId varid,
	     DgNode* v0,
	     DgNode* v1,
	     ymuint pos);

  // v0 の一部を0/1に固定してv1に一致できる場合の処理
  tDgEdge
  case2cplx(const Bdd& F,
	    tVarId varid,
	    DgNode* c_node,
	    tPol lit_pol,
	    int or_and,
	    DgNode* v);

  // CplxCase3 のサブルーティン
  // node もしくは node の TFI のうち，sup と disjoint なものを
  // inputs に入れる．OR/XORノードの場合には自動的に分割する．
  void
  find_disjoint_node(DgNode* node,
		     const BddVarSet& support,
		     vector<DgNode*>& inputs);

  // CplxCase3 のサブルーティン
  // 共通でない入力を2つ以上持つノードを探して括り出す．
  void
  find_uncommon_inputs(const vector<DgNode*>& list1,
		       int mark,
		       vector<DgNode*>& inputs);

  // CplxCase3 のサブルーティン
  // 共通な入力を2つ以上持つノードを探して，共通部分を括り出す．
  // 共通部分は inputs に追加する．
  void
  find_common_inputs(const vector<DgNode*>& list0,
		     const vector<DgNode*>& list1,
		     vector<DgNode*>& inputs);

  // リストの内容を配列に入れて，TopVarId() の昇順にソートする．
  // ただし，0番目の要素は後から入れる．
  ymuint
  blist2barray(const vector<DgNode*>& b_list);

  // 境界のノードを inputs にいれる再帰処理
  void
  list_boundary(DgNode* node,
		vector<DgNode*>& inputs);

  // index の2つのコファクターをマージする．
  tDgEdge
  merge(tVarId varid,
	tDgEdge r0,
	tDgEdge r1);

  // BDD から DG を作るサブルーティン
  tDgEdge
  decomp_step(tBddEdge e);

  // 作業領域のサイズを設定する．
  // 今から扱う論理関数の入力数を入れる．
  void
  resize_buffer(ymuint ni);

  // inputs に枝を追加する．
  void
  insert2(vector<tDgEdge>& inputs,
	       tDgEdge edge);

  // inputs にノードを追加する．
  void
  insert3(vector<DgNode*>& inputs,
	  DgNode* node);

  // 枝の先のノードの参照回数を一つ増やす
  // ただし定数枝の場合にはノードは空なのでチェックする
  void
  inc_ref(tDgEdge root);

  // 枝の先のノードの参照回数を一つ減らす
  // ただし定数枝の場合にはノードは空なのでチェックする
  void
  dec_ref(tDgEdge root);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgr& mBddMgr;

  // 今の分解が bidecomposition の時に true となるフラグ
  bool mBidecomp;

  // 分解グラフのノードを持つ連想配列
  // グローバル関数をキーにする．
  hash_map<tBddEdge, Dg> mNodeHash1;

  // 二項分解グラフのノードを持つ連想配列
  // グローバル関数をキーにする．
  hash_map<tBddEdge, Dg> mNodeHash2;

  ymuint64 mN;

  // 現在扱っている関数の入力数
  ymuint32 mNi;

  // プロファイル情報
  Profile mProf;

};

END_NAMESPACE_YM_DEC

#endif // LOGIC_DGMGR_H
