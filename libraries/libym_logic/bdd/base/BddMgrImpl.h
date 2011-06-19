#ifndef LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H
#define LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H

/// @file libym_logic/bdd/base/BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BddMgr.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @class BddMgrImpl BddMgrImpl.h "BddMgrImpl.h"
/// @param[in] BDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class BddMgrImpl
{
  friend class Bdd;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  // デフォルトマネージャを返す．
  static
  BddMgrImpl*
  default_mgr();

  // コンストラクタ
  BddMgrImpl();

  // デストラクタ
  virtual
  ~BddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // BDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  // e の参照回数を増やす．
  virtual
  void
  inc_rootref(BddEdge e) = 0;

  // e の参照回数を減らす．
  virtual
  void
  dec_rootref(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 変数番号とレベルの対応づけ
  //////////////////////////////////////////////////////////////////////

  // 変数を確保する．
  // 確保に失敗したら false を返す．
  // 最後の変数の後ろに挿入される．
  virtual
  bool
  new_var(tVarId varid) = 0;

  // 現在登録されている変数をそのレベルの昇順で返す．
  virtual
  tVarSize
  var_list(list<tVarId>& vlist) const = 0;

  // 変数番号からレベルを得る．
  // もしもレベルが割り当てられていない場合にはエラーとなる．
  virtual
  tLevel
  level(tVarId varid) const = 0;

  // レベルから変数番号を得る．
  virtual
  tVarId
  varid(tLevel level) const = 0;

  // 動的変数順変更を許可する．
  virtual
  void
  enable_DVO() = 0;

  // 動的変数順変更を禁止する．
  virtual
  void
  disable_DVO() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD 生成用関数
  //////////////////////////////////////////////////////////////////////

  // 肯定のリテラル関数を作る
  virtual
  BddEdge
  make_posiliteral(tVarId varid) = 0;

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] varid 変数番号
  BddEdge
  make_negaliteral(tVarId varid);

  /// @brief インデックスと左右の子供を指定してBDDを作る．
  /// @param[in] varid 変数番号
  /// @param[in] chd_0 0枝の子供
  /// @param[in] chd_1 1枝の子供
  BddEdge
  make_bdd(tVarId varid,
	   BddEdge chd_0,
	   BddEdge chd_1);

  /// @brief ベクタを真理値表と見なしてBDDを作る．
  /// @note 個々の変数を vars で指定する．
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @note v の大きさは 2^(vars.size()) に等しくなければならない．
  BddEdge
  tvec_to_bdd(const vector<int>& v,
	      const VarVector& vars);


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  // e1 & e2 を計算する．
  virtual
  BddEdge
  and_op(BddEdge e1,
	 BddEdge e2) = 0;

  // e1 & e2 & e3 を計算する．
  BddEdge
  and_op(BddEdge e1,
	 BddEdge e2,
	 BddEdge e3);

  // e1 | e2 を計算する．
  BddEdge
  or_op(BddEdge e1,
	BddEdge e2);

  // e1 | e2 | e3 を計算する．
  BddEdge
  or_op(BddEdge e1,
	BddEdge e2,
	BddEdge e3);

  // src1 ^ src2 を計算する．
  virtual
  BddEdge
  xor_op(BddEdge e1,
	 BddEdge e2) = 0;

  // e1 ^ e2 ^ e3 を計算する．
  BddEdge
  xor_op(BddEdge e1,
	 BddEdge e2,
	 BddEdge e3);

  // src1 と src2 の共通部分があれば kEdge1 を返す．
  virtual
  BddEdge
  check_intersect(BddEdge e1,
		  BddEdge e2) = 0;

  // Davio展開のモーメント項($f_{\overline{x}} \oplus f_x$)を
  // 求める処理
  virtual
  BddEdge
  xor_moment(BddEdge e,
	     tVarId idx) = 0;

  // bdd がキューブの時 true を返す．
  virtual
  bool
  check_cube(BddEdge e) = 0;

  // bdd が肯定リテラルのみからなるキューブの時 true を返す．
  virtual
  bool
  check_posi_cube(BddEdge e) = 0;

  // 変数xとyが対称(交換可能)な時にtrueを返す．
  virtual
  bool
  check_symmetry(BddEdge e,
		 tVarId x,
		 tVarId y,
		 tPol pol) = 0;

  // 一つの変数に対する cofactor を計算する．
  virtual
  BddEdge
  scofactor(BddEdge e1,
	    tVarId id,
	    tPol pol) = 0;

  // generalized cofactor を計算する．
  virtual
  BddEdge
  gcofactor(BddEdge e1,
	    BddEdge e2) = 0;

  // if-then-else 演算を計算する．
  virtual
  BddEdge
  ite_op(BddEdge e1,
	 BddEdge e2,
	 BddEdge e3) = 0;

  // multiple compose 演算を行うために最初に呼ばれる関数．
  virtual
  void
  compose_start() = 0;

  // multiple compose 演算を行う変数と置き換え関数を登録する関数
  virtual
  void
  compose_reg(tVarId id,
	      BddEdge e) = 0;

  // multiple compose 演算の本体
  virtual
  BddEdge
  compose(BddEdge e) = 0;

  // x_level の変数を y_level まで「押し込む」．
  // pol が kPolNega の時は 0-枝と 1-枝を取り替える．
  virtual
  BddEdge
  push_down(BddEdge e,
	    tLevel x_level,
	    tLevel y_level,
	    tPol pol = kPolPosi) = 0;

  // smoothing(elimination)
  // svars に含まれる変数を消去する．
  virtual
  BddEdge
  esmooth(BddEdge e1,
	  BddEdge e2) = 0;

  // src1 と src2 の論理積を計算して src3 の変数を消去する．
  virtual
  BddEdge
  and_exist(BddEdge e1,
	    BddEdge e2,
	    BddEdge e3) = 0;

  // lower と upper で指定された不完全指定論理関数の非冗長積和形を求める．
  virtual
  BddEdge
  isop(BddEdge l,
       BddEdge u,
       LogExpr& cover) = 0;

  // lower と upper で指定された不完全指定論理関数の主項カバーを求める．
  virtual
  LogExpr
  prime_cover(BddEdge l,
	      BddEdge u) = 0;

  // lower と upper で指定された不完全指定論理関数の極小サポート集合をすべて
  // 列挙する．解は論理関数の形で返される．そのなかの主項がサポート集合に
  // 対応している．
  virtual
  BddEdge
  minimal_support(BddEdge l,
		  BddEdge u) = 0;

  // smallest cube containing F 演算
  virtual
  BddEdge
  SCC(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD の構造に関係したメソッド
  //////////////////////////////////////////////////////////////////////

  // 根の節点の変数に基づいてShannon展開を行なう．
  // 戻り値として根の節点の変数番号を返し，その変数を0/1に固定した
  // 時の cofactor をそれぞれ f0, f1 に入れる．
  // もともと定数値(葉)のBDDの場合，kVarIdMax を返し，
  // f0, f1 には自分自身を代入する．
  virtual
  tVarId
  root_decomp(BddEdge e,
	      BddEdge& e0,
	      BddEdge& e1) = 0;

  // 根の変数番号インデックスを取り出す．
  // 定数節点の場合には kVarIdMax を返す．
  virtual
  tVarId
  root_var(BddEdge e) = 0;

  // 0枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  BddEdge
  edge0(BddEdge e) = 0;

  // 1枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  BddEdge
  edge1(BddEdge e) = 0;

  // e の参照回数が0なら true を返す．
  virtual
  bool
  check_noref(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 1へ至るパスを求める関数
  //////////////////////////////////////////////////////////////////////

  // 1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  onepath(BddEdge e) = 0;

  // 最短の1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  shortest_onepath(BddEdge e) = 0;

  // 最短の1パスの長さを求める．
  virtual
  tVarSize
  shortest_onepath_len(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDのノード数を数える．
  virtual
  size_t
  size(BddEdge e) = 0;

  // edge_list に登録されたBDDのノード数を数える．
  virtual
  size_t
  size(const list<BddEdge>& edge_list) = 0;

  // BDD の表す論理関数の minterm の数を返す．
  // 無限長精度の整数(mpz_class)を用いて計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  minterm_count(BddEdge e,
		tVarSize n) = 0;

  // Walsh 変換の0次の係数を計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  walsh0(BddEdge e,
	 tVarSize n) = 0;

  // Walsh 変換の1次の係数を計算する．
  // n は論理関数の変数の数
  virtual
  mpz_class
  walsh1(BddEdge e,
	 tVarId var,
	 tVarSize n) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(BddEdge e) = 0;

  // edge_list に登録されたBDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(const list<BddEdge>& edge_list) = 0;

  // 印のついた変数をベクタに変換する．
  virtual
  tVarSize
  mark_to_vector(VarVector& support) = 0;

  // 印のついた変数をリストに変換する．
  virtual
  tVarSize
  mark_to_list(VarList& support) = 0;

  // 印のついた変数をBDD(キューブ)に変換する．
  virtual
  BddEdge
  mark_to_bdd() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // VarSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // src1 と src2 が変数集合の時に共通部分を求める．
  virtual
  BddEdge
  vscap(BddEdge e1,
	BddEdge e2) = 0;

  // src1 と src2 が変数集合の時に集合差を求める．
  virtual
  BddEdge
  vsdiff(BddEdge e1,
	 BddEdge e2) = 0;

  // src1 と src2 が変数集合の時のインターセクションチェック
  virtual
  bool
  vsintersect(BddEdge e1,
	      BddEdge e2) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // LitSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // src1 と src2 がリテラル集合の時に共通部分を求める．
  virtual
  BddEdge
  lscap(BddEdge e1,
	BddEdge e2) = 0;

  // src1 と src2 がリテラル集合の時に集合差を求める．
  virtual
  BddEdge
  lsdiff(BddEdge e1,
	 BddEdge e2) = 0;

  // src1 と src2 がリテラル集合の時のインターセクションチェック
  virtual
  bool
  lsintersect(BddEdge e1,
	      BddEdge e2) = 0;

  // LitSet 用のBDDからリテラルのベクタを作る．
  virtual
  tVarSize
  to_literalvector(BddEdge e,
		   LiteralVector& dst) = 0;

  // LitSet 用のBDDからリテラルのリストを作る．
  virtual
  tVarSize
  to_literallist(BddEdge e,
		 LiteralList& dst) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // NPN Matcher で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // e を根とするBDDの節点に n-mark を付け，各変数ごとのノード数を数える．
  virtual
  void
  scan(BddEdge e,
       hash_map<tVarId, size_t>& node_counts) = 0;

  // e を根とするBDDのレベル level のノード数を数える．
  // ただし，n-mark の付いていないノードがあったら UINT_MAX を返す．
  virtual
  size_t
  count_at(BddEdge e,
	   tLevel level) = 0;

  // scan で付けた n-mark を消す．
  virtual
  void
  clear_scanmark(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 内部動作の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ガーベージコレクションを許可する．
  virtual
  void
  enable_gc() = 0;

  /// @brief ガーベージコレクションを禁止する．
  virtual
  void
  disable_gc() = 0;

  // ガーベージコレクションを行なう．
  // shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  virtual
  void
  gc(bool shrink_nodetable) = 0;

  // GC 前の sweep 処理を行うためのバインダーを登録する．
  virtual
  void
  reg_sweep_binder(EventBinder* binder) = 0;

  // パラメータを設定する．設定したい項目のマスクビットを1とする．
  virtual
  void
  param(const BddMgrParam& param,
	ymuint32 mask) = 0;

  // パラメータを取得する．
  virtual
  void
  param(BddMgrParam& param) const = 0;

  // 名前を得る．
  virtual
  const string&
  name() const = 0;

  // 使用メモリ量(in bytes)を得る．
  virtual
  size_t
  used_mem() const = 0;

  // 節点テーブルに登録されているノードの数を得る．
  virtual
  size_t
  node_num() const = 0;

  // GC で回収される(フリーになる)ノード数を得る．
  virtual
  size_t
  garbage_num() const = 0;

  // 利用可能なフリーノード数を得る．
  virtual
  size_t
  avail_num() const = 0;

  // GC の起動された回数を得る．
  virtual
  size_t
  gc_count() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  // Bdd の根の枝をセットする時の関数
  void
  set_bdd(Bdd* bdd_p,
	  BddEdge e);

  // bdd の根の枝を new_e に変更する．
  // new_e も同一の BddMgr に属していると仮定する．
  void
  assign(Bdd* bdd_p,
	 BddEdge new_e);


public:
  //////////////////////////////////////////////////////////////////////
  // ログ出力用の関数
  //////////////////////////////////////////////////////////////////////

  // ログ出力用のストリームを設定する．
  void
  set_logstream(ostream& s);

  // ログ出力用のストリームを解除する．
  void
  unset_logstream();

  // ログ出力用のストリームを得る．
  ostream&
  logstream() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 真理値表からBDDを作るためのサブルーティン
  BddEdge
  tvec_sub(const vector<int>& v,
	   ymuint32 top,
	   ymuint32 size,
	   const vector<BddEdge>& var_vector,
	   tVarId var_idx);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  int mRefCount;


  //////////////////////////////////////////////////////////////////////
  // このマネージャに管理されている BDD のリスト
  //////////////////////////////////////////////////////////////////////

  // リストのためのダミーヘッダ
  // BDD としては用いない．
  Bdd* mTopBdd;


  //////////////////////////////////////////////////////////////////////
  // ログ出力用のメンバ
  //////////////////////////////////////////////////////////////////////

  // ログ出力用のストリーム
  ostream* mLogFp;

  // ダミーのヌルストリーム
  ofstream* mNullStream;


private:
  //////////////////////////////////////////////////////////////////////
  // スタティックメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトのオブジェクト
  static
  BddMgrImpl* mDefaultMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の実装
//////////////////////////////////////////////////////////////////////

// 否定のリテラル関数を作る．
inline
BddEdge
BddMgrImpl::make_negaliteral(tVarId varid)
{
  BddEdge ans = make_posiliteral(varid);
  return ~ans;
}

// e1 & e2 & e3 を計算する．
inline
BddEdge
BddMgrImpl::and_op(BddEdge e1,
		   BddEdge e2,
		   BddEdge e3)
{
  BddEdge tmp = and_op(e1, e2);
  return and_op(tmp, e3);
}

// src1 | src2 を計算する．
inline
BddEdge
BddMgrImpl::or_op(BddEdge e1,
		  BddEdge e2)
{
  return ~and_op(~e1, ~e2);
}

// e1 | e2 | e3 を計算する．
inline
BddEdge
BddMgrImpl::or_op(BddEdge e1,
		  BddEdge e2,
		  BddEdge e3)
{
  BddEdge tmp = and_op(~e1, ~e2);
  return ~and_op(tmp, ~e3);
}

// e1 ^ e2 ^ e3 を計算する．
inline
BddEdge
BddMgrImpl::xor_op(BddEdge e1,
		   BddEdge e2,
		   BddEdge e3)
{
  BddEdge tmp = xor_op(e1, e2);
  return xor_op(tmp, e3);
}

// @brief インデックスと左右の子供を指定してBDDの枝を作る．
// @param[in] varid 変数番号
// @param[in] chd_0 0枝の子供
// @param[in] chd_1 1枝の子供
inline
BddEdge
BddMgrImpl::make_bdd(tVarId varid,
		     BddEdge chd_0,
		     BddEdge chd_1)
{
  BddEdge tmp = make_posiliteral(varid);
  return ite_op(tmp, chd_1, chd_0);
}

END_NAMESPACE_YM_BDD

#endif // LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H
