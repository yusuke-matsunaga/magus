#ifndef YMYMLOGIC_DG_H
#define YMYMLOGIC_DG_H

/// @file YmLogic/Dg.h
/// @brief Dg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/dec_nsdef.h"

#include "YmLogic/Bdd.h"
#include "YmLogic/BddLitSet.h"
#include "YmLogic/BddVarSet.h"
#include "YmLogic/BddVector.h"


BEGIN_NAMESPACE_YM_DEC

//////////////////////////////////////////////////////////////////////
/// @class PrimDec Dg.h "YmLogic/Dg.h"
/// @brief 関数分解の基本要素: bound-set, free-set を表すクラス
///
/// ただし，複数の関数(同じ関数を異なる場合分けでコファクター
/// したもの)の同一の bound-set, free-set による関数分解をひとつに
/// 表す目的でも用いられる．そのため bound func はベクタで格納する．
//////////////////////////////////////////////////////////////////////
class PrimDec
{
public:
  /// @brief コンストラクタ
  /// @param[in] bset 束縛変数を表す変数集合
  /// @param[in] fset 自由変数を表す変数集合
  /// @param[in] bfunc 束縛関数
  PrimDec(const BddVarSet& bset,
	  const BddVarSet& fset,
	  const Bdd& bfunc);

  /// @brief コンストラクタ
  /// @param[in] bset 束縛変数を表す変数集合
  /// @param[in] fset 自由変数を表す変数集合
  /// @param[in] bfunc 束縛関数のベクタ
  PrimDec(const BddVarSet& bset,
	  const BddVarSet& fset,
	  const BddVector& bfunc);

  /// @brief bound-set を得る．
  BddVarSet
  bound_set() const
  {
    return mBoundSet;
  }

  /// @brief free-set を得る．
  BddVarSet
  free_set() const
  {
    return mFreeSet;
  }

  /// @brief bound-function を得る．
  Bdd
  bound_func(ymuint pos = 0) const
  {
    return mBoundFunc[pos];
  }

  /// @brief デバッグ用の出力
  /// @param[in] s 出力ストリーム
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // bound-set を表すサポート
  BddVarSet mBoundSet;

  // free-set を表すサポート
  BddVarSet mFreeSet;

  // bound function を表すBDDのベクタ
  BddVector mBoundFunc;

};


//////////////////////////////////////////////////////////////////////
/// @class Bidec Dg.h "YmLogic/Dg.h"
/// @brief 二項分解を表すクラス
//////////////////////////////////////////////////////////////////////
class Bidec
{
public:

  /// @brief 根の分解のタイプを表すビットパタン
  ///
  /// 実際にはこの値のいくつかのビットワイズORをとった値が
  /// 使われる．
  enum {
    kConst0 = 1,  //< 定数0
    kConst1 = 2,  //< 定数1
    kLit0   = 4,  //< 負のリテラル
    kLit1   = 8,  //< 正のリテラル
    kAnd    = 16, //< AND
    kOr     = 32, //< OR
    kXor    = 64  //< XOR
  };

public:

  /// @brief 空のコンストラクタ
  Bidec()
  {
  }

  /// @brief コンストラクタ
  /// @param[in] f global function
  /// @param[in] g1, g2 decomposition function
  Bidec(const Bdd& f,
	const Bdd& g1,
	const Bdd& g2)
  {
    set(f, g1, g2);
  }

  /// @brief global function と2つの decomposition function をセットする．
  /// @param[in] f global function
  /// @param[in] g1, g2 decomposition function
  /// @note 根のタイプ(複数の候補があり得る)を求めてセットする．
  void
  set(const Bdd& f,
      const Bdd& g1,
      const Bdd& g2);

  /// @brief 全体の関数を得る．
  Bdd
  global_func() const
  {
    return mGlobalFunc;
  }

  /// @brief 根の分解のタイプを得る．
  int
  root_type() const
  {
    return mType;
  }

  /// @brief pos番目(iは0か1)の入力関数を得る．
  Bdd
  input_func(ymuint pos) const
  {
    return mInputFunc[pos];
  }

  /// @brief 内容を s に書き出す．
  /// デバッグ用の関数
  void
  display(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力全体の関数
  Bdd mGlobalFunc;

  // 二項分解のタイプ
  int mType;

  // 入力の関数
  Bdd mInputFunc[2];

};


//////////////////////////////////////////////////////////////////////
/// @class Dg Dg.h "YmLogic/Dg.h"
/// @brief 関数分解を表すグラフを表すクラス
///
/// ユーザがこのクラスを直接生成／削除することはできない．
/// DgMgr を用いること．
/// @sa DgMgr
//////////////////////////////////////////////////////////////////////
class Dg
{
  friend class DgMgr;

public:
  /// @brief 空のコンストラクタ
  Dg();

  /// @brief コピーコンストラクタ
  Dg(const Dg& src);

  /// @brief 代入演算子
  const Dg&
  operator=(const Dg& src);

  /// @brief デストラクタ
  ~Dg();

  /// @brief 根のノードが分解を持つとき，true を返す．
  /// @note 具体的には入力がすべてリテラルの CPLX ノード以外は真を返す．
  bool
  has_decomp() const;

  /// @brief root で表される分解グラフが CBF のとき，true を返す．
  bool
  is_CBF() const;

  /// @brief 根の分解のタイプを返す．
  tType
  type() const;

  /// @brief 根の分解のタイプが二項分解のときに true を返す．
  bool
  is_bidecomp() const;

  /// @brief 根の分解の入力数を返す．
  ymuint
  input_num() const;

  /// @brief 根の分解の pos 番目の入力の分解を得る．
  Dg
  input(ymuint pos) const;

  /// @brief global function を得る．
  Bdd
  global_func() const;

  /// @brief 入力を iset のみに制限した場合の global function を求める．
  Bdd
  partial_func(const vector<ymuint>& iset) const;

  /// @brief サポートを得る．
  BddVarSet
  support() const;

  /// @brief サポートのサイズを得る．
  ymuint
  support_size() const;

  /// @brief 直交関数分解の列挙
  /// @note うち，bound-set の入力数が llimit 以上 ulimit 以下
  /// のものを列挙する．
  void
  enum_djdec(ymuint llimit,
	     ymuint ulimit,
	     PrimDecList& dec_list) const;

  /// @brief 内容を出力する
  void
  display(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  // 枝を指定したコンストラクタ
  Dg(tDgEdge root,
     DgMgr* mgr);

  // enum_dec のための再帰関数
  void
  ed_recur(ymuint llimit,
	   ymuint ulimit,
	   const BddVarSet& all_sup,
	   PrimDecList& dec_list) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根の枝
  tDgEdge mRoot;

  // このグラフを管理するオブジェクト
  DgMgr* mMgr;

};

/// @brief 2つの関数分解が直交しているか調べる．
///
/// たがいの bound-set が交わらない分解を直交していると
/// 定義する．
/// @param[in] dec1, dec2 2つの関数分解
/// @retval true dec1 と dec2 は直交している
/// @retval false dec1 と dec2 は直交していない
inline
bool
independent(const PrimDec& dec1,
	    const PrimDec& dec2)
{
  return !(dec1.bound_set() && dec2.bound_set());
}

/// @brief 元の関数 f と束縛関数 bound_func から出力の関数を得る．
/// @param[in] f 元の関数
/// @param[in] bound_func 束縛関数
/// @param[in] hvar 束縛関数の出力に対応する変数番号
/// @return 出力の関数
Bdd
root_func(const Bdd& f,
	  const Bdd& bound_func,
	  VarId hvar);

/// @brief dec_list をダンプする．
/// @param[in] dec_list 基本分解のリスト
/// @param[in] s 出力ストリーム
void
dump_primdec_list(const PrimDecList& dec_list,
		  ostream& s);

END_NAMESPACE_YM_DEC

#endif // YM_YMYMLOGIC_DG_H
