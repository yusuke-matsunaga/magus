#ifndef ZDDMGRIMPL_H
#define ZDDMGRIMPL_H

/// @file ZddMgrImpl.h
/// @brief ZddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ym_logic/ZddMgr.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrImpl ZddMgrImpl.h "ZddMgrImpl.h"
/// @param[in] ZDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class ZddMgrImpl
{
  friend class Zdd;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  // デフォルトマネージャを返す．
  static
  ZddMgrImpl*
  default_mgr();

  // コンストラクタ
  ZddMgrImpl();

  // デストラクタ
  virtual
  ~ZddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // ZDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  // e の参照回数を増やす．
  virtual
  void
  inc_rootref(ZddEdge e) = 0;

  // e の参照回数を減らす．
  virtual
  void
  dec_rootref(ZddEdge e) = 0;


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


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD 生成用関数
  //////////////////////////////////////////////////////////////////////

  // singletonを作る
  virtual
  ZddEdge
  make_singleton(ymuint32 varid) = 0;

  /// @brief インデックスと左右の子供を指定してZDDを作る．
  /// @param[in] varid 変数番号
  /// @param[in] chd_0 0枝の子供
  /// @param[in] chd_1 1枝の子供
  ZddEdge
  make_zdd(ymuint32 varid,
	   ZddEdge chd_0,
	   ZddEdge chd_1);


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  // e1 $\cap$ e2 を計算する．
  virtual
  ZddEdge
  cap_op(ZddEdge e1,
	 ZddEdge e2) = 0;

  // e1 $\cap$ e2 $\cap$ e3 を計算する．
  ZddEdge
  cap_op(ZddEdge e1,
	 ZddEdge e2,
	 ZddEdge e3);

  // e1 $\cup$ e2 を計算する．
  ZddEdge
  cup_op(ZddEdge e1,
	 ZddEdge e2);

  // e1 $\cup$ e2 $\cup$ e3 を計算する．
  ZddEdge
  cup_op(ZddEdge e1,
	 ZddEdge e2,
	 ZddEdge e3);

  // src1 $\setdiff$ src2 を計算する．
  virtual
  ZddEdge
  diff_op(ZddEdge e1,
	  ZddEdge e2) = 0;

  // src1 と src2 の共通部分があれば kEdge1 を返す．
  virtual
  ZddEdge
  check_intersect(ZddEdge e1,
		  ZddEdge e2) = 0;

  // 一つの変数に対する cofactor を計算する．
  virtual
  ZddEdge
  scofactor(ZddEdge e1,
	    tVarId id,
	    tPol pol) = 0;

  // compose 演算を計算する．
  virtual
  BddEdge
  compose_op(BddEdge e1,
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
	      ZddEdge e) = 0;

  // multiple compose 演算の本体
  virtual
  ZddEdge
  compose(ZddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD の構造に関係したメソッド
  //////////////////////////////////////////////////////////////////////

  // 根の節点の変数に基づいてShannon展開を行なう．
  // 戻り値として根の節点の変数番号を返し，その変数を0/1に固定した
  // 時の cofactor をそれぞれ f0, f1 に入れる．
  // もともと定数値(葉)のZDDの場合，kVarIdMax を返し，
  // f0, f1 には自分自身を代入する．
  virtual
  tVarId
  root_decomp(ZddEdge e,
	      ZddEdge& e0,
	      ZddEdge& e1) = 0;

  // 根の変数番号インデックスを取り出す．
  // 定数節点の場合には kVarIdMax を返す．
  virtual
  tVarId
  root_var(ZddEdge e) = 0;

  // 0枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  ZddEdge
  edge0(ZddEdge e) = 0;

  // 1枝の指している cofactor を返す．
  // 定数節点の場合には自分自身を返す．
  virtual
  ZddEdge
  edge1(ZddEdge e) = 0;

  // e の参照回数が0なら true を返す．
  virtual
  bool
  check_noref(ZddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  // e を根とするZDDのノード数を数える．
  virtual
  ymuint64
  size(ZddEdge e) = 0;

  // edge_list に登録されたZDDのノード数を数える．
  virtual
  ymuint64
  size(const list<ZddEdge>& edge_list) = 0;

  // ZDD の表す集合の要素数を返す．
  // 無限長精度の整数(mpz_class)を用いて計算する．
  virtual
  mpz_class
  count(ZddEdge e) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  // e を根とするZDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(ZddEdge e) = 0;

  // edge_list に登録されたZDDのサポートに印をつける．
  virtual
  tVarSize
  mark_support(const list<ZddEdge>& edge_list) = 0;

  // 印のついた変数をベクタに変換する．
  virtual
  tVarSize
  mark_to_vector(VarVector& support) = 0;

  // 印のついた変数をリストに変換する．
  virtual
  tVarSize
  mark_to_list(VarList& support) = 0;

  // 印のついた変数をZDD(キューブ)に変換する．
  virtual
  ZddEdge
  mark_to_zdd() = 0;


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
  param(const ZddMgrParam& param,
	ymuint32 mask) = 0;

  // パラメータを取得する．
  virtual
  void
  param(ZddMgrParam& param) const = 0;

  // 名前を得る．
  virtual
  const string&
  name() const = 0;

  // 使用メモリ量(in bytes)を得る．
  virtual
  ymuint64
  used_mem() const = 0;

  // 節点テーブルに登録されているノードの数を得る．
  virtual
  ymuint64
  node_num() const = 0;

  // GC で回収される(フリーになる)ノード数を得る．
  virtual
  ymuint64
  garbage_num() const = 0;

  // 利用可能なフリーノード数を得る．
  virtual
  ymuint64
  avail_num() const = 0;

  // GC の起動された回数を得る．
  virtual
  ymuin64
  gc_count() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ZDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  // Zdd の根の枝をセットする時の関数
  void
  set_zdd(Zdd* zdd_p,
	  ZddEdge e);

  // zdd の根の枝を new_e に変更する．
  // new_e も同一の ZddMgr に属していると仮定する．
  void
  assign(Zdd* zdd_p,
	 ZddEdge new_e);


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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数
  ymuint32 mRefCount;


  //////////////////////////////////////////////////////////////////////
  // このマネージャに管理されている ZDD のリスト
  //////////////////////////////////////////////////////////////////////

  // リストのためのダミーヘッダ
  // ZDD としては用いない．
  Zdd* mTopZdd;


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
  ZddMgrImpl* mDefaultMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の実装
//////////////////////////////////////////////////////////////////////

// e1 $\cap$ e2 $\cap$ e3 を計算する．
inline
ZddEdge
ZddMgrImpl::cap_op(ZddEdge e1,
		   ZddEdge e2,
		   ZddEdge e3)
{
  ZddEdge tmp = cap_op(e1, e2);
  return cap_op(tmp, e3);
}

// @brief インデックスと左右の子供を指定してZDDの枝を作る．
// @param[in] varid 変数番号
// @param[in] chd_0 0枝の子供
// @param[in] chd_1 1枝の子供
inline
ZddEdge
ZddMgrImpl::make_zdd(tVarId varid,
		     ZddEdge chd_0,
		     ZddEdge chd_1)
{
  ZddEdge tmp = make_posiliteral(varid);
  return ite_op(tmp, chd_1, chd_0);
}

END_NAMESPACE_YM_ZDD

#endif // ZDDMGRIMPL_H
