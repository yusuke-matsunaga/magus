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
#include "ym_utils/FragAlloc.h"
#include "BddEdge.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

class BddOp;
class BddBinOp;
class BddTriOp;
class CofOp;
class XcOp;
class SymOp;
class CountOp;
class McOp;
class W0Op;
class W1Op;
class SupOp;
class SmoothOp;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrImpl BddMgrImpl.h "BddMgrImpl.h"
/// @param[in] BDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class BddMgrImpl
{
  friend class Bdd;
  friend class BddOp;

public:
  //////////////////////////////////////////////////////////////////////
  // インスタンスを生成するクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを返す．
  static
  BddMgrImpl*
  default_mgr();

  /// @brief マネージャを生成する
  /// @param[in] type マネージャの型を表す文字列
  /// @param[in] name マネージャの名前
  /// @param[in] option オプション文字列
  static
  BddMgrImpl*
  new_mgr(const string& type,
	  const string& name = string(),
	  const string& option = string());


public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] name マネージャの名前
  BddMgrImpl(const string& name = string());

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

  /// @brief 現在の最大レベル + 1を返す．
  virtual
  ymuint
  max_level() const = 0;

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

  /// @brief リテラル関数を表すBDDを作る
  /// @param[in] varid 変数番号
  /// @param[in] pol 極性
  /// @return 生成された BDD
  BddEdge
  make_literal(VarId varid,
	       tPol pol);

  /// @brief 肯定のリテラル関数を作る
  /// @param[in] varid 変数番号
  BddEdge
  make_posiliteral(VarId varid);

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
  BddEdge
  and_op(BddEdge e1,
	 BddEdge e2);

  /// @brief e1 | e2 を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  or_op(BddEdge e1,
	BddEdge e2);

  /// @brief src1 ^ src2 を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  xor_op(BddEdge e1,
	 BddEdge e2);

  /// @brief e1 と e2 の共通部分があれば kEdge1 を返す．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  check_intersect(BddEdge e1,
		  BddEdge e2);

  /// @brief if-then-else 演算を計算する．
  /// @param[in] e1, e2, e3 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  ite_op(BddEdge e1,
	 BddEdge e2,
	 BddEdge e3);

  /// @brief 一つの変数に対する cofactor を計算する．
  /// @param[in] e 演算対象の枝
  /// @param[in] id 展開対象の変数番号
  /// @param[in] pol 極性
  /// @return 演算結果を返す．
  BddEdge
  scofactor(BddEdge e,
	    VarId id,
	    tPol pol);

  /// @brief Davio展開のモーメント項を求める処理
  /// @param[in] e 演算対象の枝
  /// @param[in] idx 展開を行う変数番号
  /// @return 演算結果を返す．
  /// @note モーメント項とは $f_{\overline{x}} \oplus f_x$ のこと．
  BddEdge
  xor_moment(BddEdge e,
	     VarId idx);

  /// @brief generalized cofactor を計算する．
  /// @param[in] e1, e2 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  gcofactor(BddEdge e1,
	    BddEdge e2);

  /// @brief smoothing(elimination)
  /// @param[in] e 演算対象の枝
  /// @param[in] s 消去対象の変数リストを表す枝
  /// @return 演算結果を返す．
  /// @note s に含まれる変数を消去する．
  BddEdge
  esmooth(BddEdge e,
	  BddEdge s);

  /// @brief src1 と src2 の論理積を計算して src3 の変数を消去する．
  /// @param[in] e1, e2, e3 演算対象の枝
  /// @return 演算結果を返す．
  BddEdge
  and_exist(BddEdge e1,
	    BddEdge e2,
	    BddEdge e3);

  /// @brief bdd がキューブの時 true を返す．
  /// @param[in] e 演算対象の枝
  bool
  check_cube(BddEdge e);

  /// @brief bdd が肯定リテラルのみからなるキューブの時 true を返す．
  /// @param[in] e 演算対象の枝
  bool
  check_posi_cube(BddEdge e);

  /// @brief 変数 xと y が対称(交換可能)な時にtrueを返す．
  /// @param[in] e 演算対象の枝
  /// @param[in] x, y 変数番号
  /// @param[in] pol 極性
  bool
  check_symmetry(BddEdge e,
		 VarId x,
		 VarId y,
		 tPol pol);

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

  /// @brief edge_list に登録されたBDDのノード数を数える．
  ymuint64
  node_count(const vector<BddEdge>& edge_list);

  /// @brief BDD の表す論理関数の minterm の数を返す．
  /// @param[in] e 根の枝
  /// @param[in] nvar 論理関数の変数の数
  /// 無限長精度の整数(mpz_class)を用いて計算する．
  mpz_class
  minterm_count(BddEdge e,
		ymuint nvar);

  /// @brief Walsh 変換の0次の係数を計算する．
  /// @param[in] e 根の枝
  /// @param[in] nvar 論理関数の変数の数
  mpz_class
  walsh0(BddEdge e,
	 ymuint nvar);

  /// @brief Walsh 変換の1次の係数を計算する．
  /// @param[in] e 根の枝
  /// @param[in] var 変数番号
  /// @param[in] nvar 論理関数の変数の数
  mpz_class
  walsh1(BddEdge e,
	 VarId var,
	 ymuint nvar);


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief edge_list に登録されたBDDのサポートに印をつける．
  /// @param[in] edge_list 根の枝のリスト
  /// @return サポート数を返す．
  ymuint
  mark_support(const vector<BddEdge>& edge_list);

  /// @brief 印のついた変数をベクタに変換する．
  /// @param[out] support 結果を格納する変数
  /// @return サポート数を返す．
  ymuint
  mark_to_vector(VarVector& support);

  /// @brief 印のついた変数をリストに変換する．
  /// @param[out] support 結果を格納する変数
  /// @return サポート数を返す．
  ymuint
  mark_to_list(VarList& support);

  /// @brief 印のついた変数をBDD(キューブ)に変換する．
  BddEdge
  mark_to_bdd();

  /// @brief smallest cube containing F 演算
  BddEdge
  SCC(BddEdge e);


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
  /// @note 過去の enable_gc()/disable_gc() の回数が多い方できまる．
  void
  enable_gc();

  /// @brief ガーベージコレクションを禁止する．
  /// @note 過去の enable_gc()/disable_gc() の回数が多い方できまる．
  void
  disable_gc();

  /// @brief ガーベージコレクションの許可状態を調べる．
  /// @retval true ガーベージコレクションを起動可能
  /// @retval false ガーベージコレクション禁止
  bool
  check_gc() const;

  /// @brief ガーベージコレクションを行なう．
  /// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  void
  gc(bool shrink_nodetable);

  /// @brief GC 前の sweep 処理を行うためのバインダーを登録する．
  virtual
  void
  reg_sweep_binder(EventBinder* binder) = 0;

  /// @brief パラメータを設定する．
  /// @param[in] param パラメータ
  /// @param[in] mask 設定する項目を指定するマスク
  void
  param(const BddMgrParam& param,
	ymuint32 mask);

  /// @brief パラメータを取得する．
  /// @param[out] param 結果を格納する変数
  void
  param(BddMgrParam& param) const;

  /// @brief GC を起動するかどうかのしきい値パラメータを得る．
  double
  gc_threshold() const;

  /// @brief GC を起動するかどうかのノード数のしきい値パラメータを得る．
  ymuint64
  gc_node_limit() const;

  /// @brief 節点テーブルの拡張を制御するパラメータを得る．
  double
  nt_load_limit() const;

  /// @brief 演算結果テーブルの拡張を制御するパラメータを得る．
  double
  rt_load_limit() const;

  /// @brief メモリ使用量の制限値パラメータを得る．
  ymuint64
  mem_limit() const;

  /// @brief 名前を得る．
  const string&
  name() const;

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
  // ノードを確保する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた枝とインデックスを持つノードを返す．
  /// @param[in] level ノードのレベル
  /// @param[in] l 0枝
  /// @param[in] h 1枝
  virtual
  BddEdge
  new_node(ymuint level,
	   BddEdge l,
	   BddEdge h) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // メモリ確保用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このマネージャで使用するメモリ領域を確保する．
  /// @param[in] size サイズ
  void*
  allocate(ymuint64 size);

  /// @brief このマネージャで確保したメモリを解放する．
  /// @param[in] ptr 解放するメモリのアドレス
  /// @param[in] size サイズ
  void
  deallocate(void* ptr,
	     ymuint64 size);


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


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのリンク数を増やし，もしロックされていなければロックする
  void
  activate(BddEdge e);

  /// @brief ノードのリンク数を減らし，他のリンクがなければロックを外す
  void
  deactivate(BddEdge e);


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

  /// @brief vp と vp の子孫のノードをロックする
  void
  lockall(BddNode* vp);

  /// @brief vp と vp の子孫ノードを(他に参照されていないもののみ)ロックを外す
  void
  unlockall(BddNode* vp);

  /// @brief lockall() 用のフック
  virtual
  void
  lock_hook(BddNode* vp) = 0;

  /// @brief unlockall() 用のフック
  virtual
  void
  unlock_hook(BddNode* vp) = 0;

  /// @brief gc() の実際の処理を行う関数
  virtual
  void
  _gc(bool shink_nodetable) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // デバッグ用の名前
  string mName;


  //////////////////////////////////////////////////////////////////////
  // ユーザーが設定するパラメータ
  // 設定は専用のメソッドを用いる．
  //////////////////////////////////////////////////////////////////////

  // ガーベージの割合がこの値を越えるとGCを起こす．
  double mGcThreshold;

  // ただし，全体のノード数がこの数以下の時はGCは起こさない．
  ymuint64 mGcNodeLimit;

  // GCの起動を制御する変数
  ymint32 mGcEnable;

  // ノード数がこの数を越えたら mGcEnable は常に true だと思う
  ymuint64 mDangerousZone;

  // 節点テーブル拡張時の制限値を決めるパラメータ
  double mNtLoadLimit;

  // 演算結果テーブル拡張時の制限値を決めるパラメータ
  double mRtLoadLimit;

  // メモリアロケータ
  FragAlloc mAlloc;

  // メモリ確保が失敗した時にオンになるフラグ
  bool mOverflow;

  // このマネージャに管理されている BDD のリスト
  // リストのためのダミーヘッダ
  // BDD としては用いない．
  Bdd* mTopBdd;

  // AND 演算用オブジェクト
  BddBinOp* mAndOp;

  // XOR 演算用オブジェクト
  BddBinOp* mXorOp;

  // intersect 演算用オブジェクト
  BddBinOp* mIntsecOp;

  // If-then-else 演算用オブジェクト
  BddTriOp* mIteOp;

  // cofactor 演算用オブジェクト
  CofOp* mCofOp;

  // xor-cofactor 演算用オブジェクト
  XcOp* mXcOp;

  // generalized cofactor 演算用オブジェクト
  BddBinOp* mGcOp;

  // check_symmetry 用の演算オブジェクト
  SymOp* mSymOp;

  // node_count 用の演算オブジェクト
  CountOp* mCountOp;

  // minerm_count 用の演算オブジェクト
  McOp* mMcOp;

  // walsh0 用の演算オブジェクト
  W0Op* mW0Op;

  // walsh1 用の演算オブジェクト
  W1Op* mW1Op;

  // support 関係の演算オブジェクト
  SupOp* mSupOp;

  // smooth 演算オブジェクト
  BddBinOp* mSmoothOp;

  // and-exist 演算オブジェクト
  BddTriOp* mAeOp;

  // 演算オブジェクトのリスト
  list<BddOp*> mOpList;


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

// @brief 名前を得る．
inline
const string&
BddMgrImpl::name() const
{
  return mName;
}

// @brief ガーベージコレクションの許可状態を調べる．
// @retval true ガーベージコレクションを起動可能
// @retval false ガーベージコレクション禁止
inline
bool
BddMgrImpl::check_gc() const
{
  return mGcEnable == 0;
}

// @brief GC を起動するかどうかのしきい値パラメータを得る．
inline
double
BddMgrImpl::gc_threshold() const
{
  return mGcThreshold;
}

// @brief GC を起動するかどうかのノード数のしきい値パラメータを得る．
inline
ymuint64
BddMgrImpl::gc_node_limit() const
{
  return mGcNodeLimit;
}

// @brief 節点テーブルの拡張を制御するパラメータを得る．
inline
double
BddMgrImpl::nt_load_limit() const
{
  return mNtLoadLimit;
}

// @brief 演算結果テーブルの拡張を制御するパラメータを得る．
inline
double
BddMgrImpl::rt_load_limit() const
{
  return mRtLoadLimit;
}

// @brief メモリ使用量の制限値パラメータを得る．
inline
ymuint64
BddMgrImpl::mem_limit() const
{
  return mAlloc.mem_limit();
}

// @brief リテラル関数を表すBDDを作る
// @param[in] varid 変数番号
// @param[in] pol 極性
// @return 生成された BDD
inline
BddEdge
BddMgrImpl::make_literal(VarId varid,
			 tPol pol)
{
  BddEdge ans = make_posiliteral(varid);
  return ans.addpol(pol);
}

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

// リンク数を増やし，もしロックされていなければロックする
// 多くの場合, lockall() が呼ばれることが少ないので inline にしている．
inline
void
BddMgrImpl::activate(BddEdge e)
{
  BddNode* vp = e.get_node();
  if ( vp && vp->linkinc() == 1 ) {
    lockall(vp);
  }
}

// リンク数を減らし，他のリンクがなければロックを外す
// 多くの場合, unlockall() が呼ばれることが少ないので inline にしている．
inline
void
BddMgrImpl::deactivate(BddEdge e)
{
  BddNode* vp = e.get_node();
  if ( vp && vp->linkdec() == 0 ) {
    unlockall(vp);
  }
}

END_NAMESPACE_YM_BDD

#endif // LIBYM_LOGIC_BDD_BASE_BDDMGRIMPL_H
