#ifndef LOGIC_BDDDECOMP_H
#define LOGIC_BDDDECOMP_H

/// @file logic/BddDecomp.h
/// @brief BddDecomp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/dec_nsdef.h"
#include "logic/bdd_nsdef.h"


BEGIN_NAMESPACE_YM_DEC

//////////////////////////////////////////////////////////////////////
/// @class BddDecomp BddDecomp.h "logic/BddDecomp.h"
/// @brief BDD ベースの関数分解器
/// @sa Dg DgMgr
//////////////////////////////////////////////////////////////////////
class BddDecomp
{
public:

  /// @brief コンストラクタ
  /// @param[in] bddmgr 親の BddMgr
  explicit
  BddDecomp(BddMgr& bddmgr);

  /// @brief デストラクタ
  ~BddDecomp();


public:

  /// @brief 単純直交分解を行う．
  /// @param[in] F 関数をあらす BDD
  /// @return 分解グラフを返す．
  Dg
  djdecomp(const Bdd& F);

  /// @brief 直交二項分解を行う．
  /// @param[in] F 関数を表す BDD
  /// @return 分解グラフを返す．
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

  /// @brief 直交関数分解の列挙
  ///
  /// 直交関数分解のうち，bound-set の入力数が llimit 以上 ulimit 以下
  /// のものを列挙する．
  /// ただし，free-set が空のものや bound-set が空のもの, 入力数が 1 の
  /// ものも含む．
  /// @param[in] f 対象の論理関数
  /// @param[in] llimit 束縛変数集合の要素数の下限
  /// @param[in] ulimit 束縛変数集合の要素数の上限
  /// @param[out] dec_list 関数分解を格納するリスト
  void
  enum_djdec(const Bdd& f,
	     ymuint llimit,
	     ymuint ulimit,
	     PrimDecList& dec_list);

  /// @brief cf[] に共通な直交分解を求める．
  /// @param[in] cf コファクターごとの論理関数を納めた BDD ベクタ
  /// @param[in] limit 求める関数分解の入力数の上限
  /// @param[out] cd_list 求まった関数分解を納めるリスト
  void
  common_djdec(const BddVector& cf,
	       ymuint limit,
	       PrimDecList& cd_list);

  /// @brief 複数の Bidec をマージする．
  /// @param[in] idx_vector インデックスのリスト
  /// @param[in] bidec_vector idex_vector によるコファクターの
  /// 2項分解の結果のリスト
  /// @param[in] m_bidec 共通な2項分解を格納する変数
  /// @return 共通部分をもつとき true を返す．
  /// \f$2^|\mbox{idx\_vector}|\f$個の2項分解 bidec_vector の共通部分を求める．
  bool
  merge_bidec(const VarVector& idx_vector,
	      const BidecVector& bidec_vector,
	      Bidec& m_bidec);

  /// @brief 2^|idx| 個の関数 cf[] が同種の二項分解を持つとき true を返す．
  ///
  /// その時のマージした二項分解を m_bidec に入れる．
  /// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
  /// とする．
  bool
  find_common_bidec(const VarVector& idx,
		    const BddVector& cf,
		    ymuint limit1,
		    ymuint limit2,
		    Bidec& m_bidec);

  /// @brief 関数のベクタ cf[] の両立する二項分解を求める．
  ///
  /// 両立する分解とはサポートが同一である分解のこと
  /// ただし，第一項の入力数は limit1 以下, 第二項の入力数は limit2 以下
  /// とする．
  void
  enum_common_bidec(const BddVector& cf,
		    ymuint limit1,
		    ymuint limit2,
		    list<BidecVector>& bidec_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgr& mMgr;

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

#endif // LOGIC_BDDDECOMP_H
