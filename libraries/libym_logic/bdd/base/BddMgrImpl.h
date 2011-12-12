#ifndef LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H
#define LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H

/// @file libym_logic/bdd/base/BddMgrImpl.h
/// @brief BddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"
#include "BddEdge.h"


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

  /// @brief デフォルトマネージャを返す．
  static
  BddMgrImpl*
  default_mgr();

  /// @brief コンストラクタ
  BddMgrImpl();

  /// @brief デストラクタ
  virtual
  ~BddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // BDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の枝の参照回数を増やす．
  /// @param[in] e 根の枝
  virtual
  void
  inc_rootref(BddEdge e) = 0;

  /// @brief 根の枝の参照回数を減らす．
  /// @param[in] e 根の枝
  virtual
  void
  dec_rootref(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 変数番号とレベルの対応づけ
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を確保する．
  /// @param[in] varid 変数番号
  /// @return 確保に失敗したら false を返す．
  /// @note 最後の変数の後ろに挿入される．
  virtual
  bool
  new_var(VarId varid) = 0;

  /// @brief 現在登録されている変数をそのレベルの昇順で返す．
  /// @param[out] vlist 変数を格納するリスト
  /// @return 変数の数 ( = vlist.size() ) を返す．
  virtual
  ymuint
  var_list(list<VarId>& vlist) const = 0;

  /// @brief 変数番号からレベルを得る．
  /// @param[in] varid 変数番号
  /// @return varid に対応するレベルを返す．
  /// @note もしもレベルが割り当てられていない場合にはエラーとなる．
  virtual
  ymuint
  level(VarId varid) const = 0;

  /// @brief レベルから変数番号を得る．
  /// @param[in] level レベル
  /// @return level に対応する変数番号を返す．
  virtual
  VarId
  varid(ymuint level) const = 0;

  /// @brief 動的変数順変更を許可する．
  virtual
  void
  enable_DVO() = 0;

  /// @brief 動的変数順変更を禁止する．
  virtual
  void
  disable_DVO() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD 生成用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 肯定のリテラル関数を作る
  /// @param[in] varid 変数番号
  virtual
  BddEdge
  make_posiliteral(VarId varid) = 0;

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] varid 変数番号
  BddEdge
  make_negaliteral(VarId varid);

  /// @brief ベクタを真理値表と見なしてBDDを作る．
  /// @param[in] v 真理値表のベクタ
  /// @param[in] vars 変数番号の配列
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @note v の大きさは 2^(vars.size()) に等しくなければならない．
  BddEdge
  tvec_to_bdd(const vector<int>& v,
	      const VarVector& vars);


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief e1 & e2 を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  and_op(BddEdge e1,
	 BddEdge e2) = 0;

  /// @brief e1 | e2 を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  or_op(BddEdge e1,
	BddEdge e2);

  /// @brief src1 ^ src2 を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  xor_op(BddEdge e1,
	 BddEdge e2) = 0;

  /// @brief e1 と e2 の共通部分があれば kEdge1 を返す．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  check_intersect(BddEdge e1,
		  BddEdge e2) = 0;

  /// @brief Davio展開のモーメント項を求める処理
  /// @param[in] e 演算対象の枝
  /// @param[in] idx 展開を行う変数番号
  /// @return 演算結果を返す．
  /// @note モーメント項とは $f_{\overline{x}} \oplus f_x$ のこと．
  virtual
  BddEdge
  xor_moment(BddEdge e,
	     VarId idx) = 0;

  /// @brief bdd がキューブの時 true を返す．
  /// @param[in] e 演算対象の枝
  virtual
  bool
  check_cube(BddEdge e) = 0;

  /// @brief bdd が肯定リテラルのみからなるキューブの時 true を返す．
  /// @param[in] e 演算対象の枝
  virtual
  bool
  check_posi_cube(BddEdge e) = 0;

  /// @brief 変数 xと y が対称(交換可能)な時にtrueを返す．
  /// @param[in] e 演算対象の枝
  /// @param[in] x, y 変数番号
  /// @param[in] pol 極性
  virtual
  bool
  check_symmetry(BddEdge e,
		 VarId x,
		 VarId y,
		 tPol pol) = 0;

  /// @brief 一つの変数に対する cofactor を計算する．
  /// @param[in] e 演算対象の枝
  /// @param[in] id 展開対象の変数番号
  /// @param[in] pol 極性
  /// @return 演算結果を返す．
  virtual
  BddEdge
  scofactor(BddEdge e,
	    VarId id,
	    tPol pol) = 0;

  /// @brief generalized cofactor を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  gcofactor(BddEdge e1,
	    BddEdge e2) = 0;

  /// @brief if-then-else 演算を計算する．
  /// @param[in] e1, e2, e3 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  ite_op(BddEdge e1,
	 BddEdge e2,
	 BddEdge e3) = 0;

  /// @brief multiple compose 演算を行うために最初に呼ばれる関数．
  virtual
  void
  compose_start() = 0;

  /// @brief multiple compose 演算を行う変数と置き換え関数を登録する関数
  /// @param[in] id 変数番号
  /// @param[in] e id に対応するBDDの根の枝
  virtual
  void
  compose_reg(VarId id,
	      BddEdge e) = 0;

  /// @brief multiple compose 演算の本体
  /// @param[in] e 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  compose(BddEdge e) = 0;

  /// @brief x_level の変数を y_level まで「押し込む」．
  /// @note pol が kPolNega の時は 0-枝と 1-枝を取り替える．
  virtual
  BddEdge
  push_down(BddEdge e,
	    ymuint x_level,
	    ymuint y_level,
	    tPol pol = kPolPosi) = 0;

  /// @brief smoothing(elimination)
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  /// @note svars に含まれる変数を消去する．
  virtual
  BddEdge
  esmooth(BddEdge e1,
	  BddEdge e2) = 0;

  /// @brief src1 と src2 の論理積を計算して src3 の変数を消去する．
  /// @param[in] e1, e2, e3 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  and_exist(BddEdge e1,
	    BddEdge e2,
	    BddEdge e3) = 0;

  /// @brief 不完全指定論理関数の非冗長積和形を求める．
  /// @param[in] l オンセットを表す関数
  /// @param[in] u オンセット＋ドントケアセットを表す関数
  /// @param[out] cover 結果の論理式を格納する変数
  /// @return 演算結果を返す．
  virtual
  BddEdge
  isop(BddEdge l,
       BddEdge u,
       LogExpr& cover) = 0;

  /// @brief 不完全指定論理関数の主項カバーを求める．
  /// @param[in] l オンセットを表す関数
  /// @param[in] u オンセット＋ドントケアセットを表す関数
  /// @return 結果の論理式を返す．
  virtual
  LogExpr
  prime_cover(BddEdge l,
	      BddEdge u) = 0;

  /// @brief 不完全指定論理関数の極小サポート集合をすべて列挙する．
  /// @param[in] l オンセットを表す関数
  /// @param[in] u オンセット＋ドントケアセットを表す関数
  /// 解は論理関数の形で返される．そのなかの主項がサポート集合に対応している．
  virtual
  BddEdge
  minimal_support(BddEdge l,
		  BddEdge u) = 0;

  /// @brief smallest cube containing F 演算
  virtual
  BddEdge
  SCC(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDD の構造に関係したメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の節点の変数に基づいてShannon展開を行なう．
  /// 戻り値として根の節点の変数番号を返し，その変数を0/1に固定した
  /// 時の cofactor をそれぞれ f0, f1 に入れる．
  /// もともと定数値(葉)のBDDの場合，kVarIdMax を返し，
  /// f0, f1 には自分自身を代入する．
  virtual
  VarId
  root_decomp(BddEdge e,
	      BddEdge& e0,
	      BddEdge& e1) = 0;

  /// @brief 根の変数番号インデックスを取り出す．
  /// @note 定数節点の場合には kVarIdMax を返す．
  virtual
  VarId
  root_var(BddEdge e) = 0;

  /// @brief 0枝の指している cofactor を返す．
  /// @note 定数節点の場合には自分自身を返す．
  virtual
  BddEdge
  edge0(BddEdge e) = 0;

  /// @brief 1枝の指している cofactor を返す．
  /// @note 定数節点の場合には自分自身を返す．
  virtual
  BddEdge
  edge1(BddEdge e) = 0;

  /// @brief e の参照回数が0なら true を返す．
  virtual
  bool
  check_noref(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 1へ至るパスを求める関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1パスを求める．
  /// @note 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  onepath(BddEdge e) = 0;

  /// @brief 最短の1パスを求める．
  /// @note 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  shortest_onepath(BddEdge e) = 0;

  /// @brief 最短の1パスの長さを求める．
  virtual
  ymuint
  shortest_onepath_len(BddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするBDDのノード数を数える．
  /// @param[in] e 根の枝
  virtual
  ymuint64
  size(BddEdge e) = 0;

  /// @brief edge_list に登録されたBDDのノード数を数える．
  virtual
  ymuint64
  size(const list<BddEdge>& edge_list) = 0;

  /// @brief BDD の表す論理関数の minterm の数を返す．
  /// @param[in] e 根の枝
  /// @param[in] nvar 論理関数の変数の数
  /// 無限長精度の整数(mpz_class)を用いて計算する．
  virtual
  mpz_class
  minterm_count(BddEdge e,
		ymuint nvar) = 0;

  /// @brief Walsh 変換の0次の係数を計算する．
  /// @param[in] e 根の枝
  /// @param[in] nvar 論理関数の変数の数
  virtual
  mpz_class
  walsh0(BddEdge e,
	 ymuint nvar) = 0;

  /// @brief Walsh 変換の1次の係数を計算する．
  /// @param[in] e 根の枝
  /// @param[in] var 変数番号
  /// @param[in] nvar 論理関数の変数の数
  virtual
  mpz_class
  walsh1(BddEdge e,
	 VarId var,
	 ymuint nvar) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief edge_list に登録されたBDDのサポートに印をつける．
  /// @param[in] edge_list 根の枝のリスト
  /// @return サポート数を返す．
  virtual
  ymuint
  mark_support(const vector<BddEdge>& edge_list) = 0;

  /// @brief 印のついた変数をベクタに変換する．
  /// @param[out] support 結果を格納する変数
  /// @return サポート数を返す．
  virtual
  ymuint
  mark_to_vector(VarVector& support) = 0;

  /// @brief 印のついた変数をリストに変換する．
  /// @param[out] support 結果を格納する変数
  /// @return サポート数を返す．
  virtual
  ymuint
  mark_to_list(VarList& support) = 0;

  /// @brief 印のついた変数をBDD(キューブ)に変換する．
  virtual
  BddEdge
  mark_to_bdd() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // VarSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数集合の共通部分を求める．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  vscap(BddEdge e1,
	BddEdge e2) = 0;

  /// @brief 変数集合の集合差を求める．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  vsdiff(BddEdge e1,
	 BddEdge e2) = 0;

  /// @brief 変数集合のインターセクションチェック
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  bool
  vsintersect(BddEdge e1,
	      BddEdge e2) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // LitSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  /// @brief リテラル集合の共通部分を求める．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  lscap(BddEdge e1,
	BddEdge e2) = 0;

  /// @brief リテラル集合の集合差を求める．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  BddEdge
  lsdiff(BddEdge e1,
	 BddEdge e2) = 0;

  /// @brief リテラル集合のインターセクションチェック
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  virtual
  bool
  lsintersect(BddEdge e1,
	      BddEdge e2) = 0;

  /// @brief LitSet 用のBDDからリテラルのベクタを作る．
  /// @param[in] e 根の枝
  /// @param[out] dst 結果を格納する変数
  /// @return リテラル数を返す．
  virtual
  ymuint
  to_literalvector(BddEdge e,
		   LiteralVector& dst) = 0;

  /// @brief LitSet 用のBDDからリテラルのリストを作る．
  /// @param[in] e 根の枝
  /// @param[out] dst 結果を格納する変数
  /// @return リテラル数を返す．
  virtual
  ymuint
  to_literallist(BddEdge e,
		 LiteralList& dst) = 0;


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

  /// @brief ガーベージコレクションを行なう．
  /// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  virtual
  void
  gc(bool shrink_nodetable) = 0;

  /// @brief GC 前の sweep 処理を行うためのバインダーを登録する．
  virtual
  void
  reg_sweep_binder(EventBinder* binder) = 0;

  /// @brief パラメータを設定する．
  /// @param[in] param パラメータ
  /// @param[in] mask 設定する項目を指定するマスク
  virtual
  void
  param(const BddMgrParam& param,
	ymuint32 mask) = 0;

  /// @brief パラメータを取得する．
  /// @param[out] param 結果を格納する変数
  virtual
  void
  param(BddMgrParam& param) const = 0;

  /// @brief 名前を得る．
  virtual
  const string&
  name() const = 0;

  /// @brief 使用メモリ量(in bytes)を得る．
  virtual
  ymuint64
  used_mem() const = 0;

  /// @brief 節点テーブルに登録されているノードの数を得る．
  virtual
  ymuint64
  node_num() const = 0;

  /// @brief GC で回収される(フリーになる)ノード数を得る．
  virtual
  ymuint64
  garbage_num() const = 0;

  /// @brief 利用可能なフリーノード数を得る．
  virtual
  ymuint64
  avail_num() const = 0;

  /// @brief GC の起動された回数を得る．
  virtual
  ymuint64
  gc_count() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // BDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD の根の枝をセットする時の関数
  /// @param[in] bdd_p BDDのポインタ
  /// @param[in] e 根の枝
  void
  set_bdd(Bdd* bdd_p,
	  BddEdge e);

  /// @brief BDD の根の枝を new_e に変更する．
  /// @param[in] bdd_p BDDのポインタ
  /// @param[in] new_e 根の枝
  /// @note new_e も同一の BddMgr に属していると仮定する．
  void
  assign(Bdd* bdd_p,
	 BddEdge new_e);


public:
  //////////////////////////////////////////////////////////////////////
  // ログ出力用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ログ出力用のストリームを設定する．
  void
  set_logstream(ostream& s);

  /// @brief ログ出力用のストリームを解除する．
  void
  unset_logstream();

  /// @brief ログ出力用のストリームを得る．
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
	   ymuint var_idx);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  ymuint32 mRefCount;


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
BddMgrImpl::make_negaliteral(VarId varid)
{
  BddEdge ans = make_posiliteral(varid);
  return ~ans;
}

// src1 | src2 を計算する．
inline
BddEdge
BddMgrImpl::or_op(BddEdge e1,
		  BddEdge e2)
{
  return ~and_op(~e1, ~e2);
}

END_NAMESPACE_YM_BDD

#endif // LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H
