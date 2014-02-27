#ifndef BDDMGRMODERN_H
#define BDDMGRMODERN_H

/// @file BddMgrModern.h
/// @brief BddMgrModern のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/bdd_nsdef.h"

#include "BddMgrImpl.h"
#include "BddNode.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス名の前方参照用の宣言および型名の定義
//////////////////////////////////////////////////////////////////////

class BmmVar;
class BmmCompTbl;
class BmmCompTbl1;
class BmmCompTbl2;
class BmmCompTbl3;
class BmmIsopTbl;


//////////////////////////////////////////////////////////////////////
// BddMgr に共通な関数を提供するためのベースクラス
//////////////////////////////////////////////////////////////////////
class BddMgrModern :
  public BddMgrImpl
{
public:
  typedef BmmCompTbl CompTbl;
  typedef BmmCompTbl1 CompTbl1;
  typedef BmmCompTbl2 CompTbl2;
  typedef BmmCompTbl3 CompTbl3;
  typedef BmmIsopTbl IsopTbl;

public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] option オプション
  BddMgrModern(const string& name = string(),
	       const string& option = string());

  // デストラクタ
  virtual
  ~BddMgrModern();


  //////////////////////////////////////////////////////////////////////
  // BDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  // e の参照回数を増やす．
  virtual
  void
  inc_rootref(BddEdge e);

  // e の参照回数を減らす．
  virtual
  void
  dec_rootref(BddEdge e);


  //////////////////////////////////////////////////////////////////////
  // 変数番号とレベルの対応づけ
  //////////////////////////////////////////////////////////////////////

  // Var 側に節点テーブルを持つとき true を返す．
  bool
  is_reorderable() const;

  // 変数を確保する．
  // 確保に失敗したら false を返す．
  // 最後の変数の後ろに挿入される．
  virtual
  bool
  new_var(VarId varid);

  // 現在登録されている変数をそのレベルの昇順で返す．
  virtual
  ymuint
  var_list(list<VarId>& vlist) const;

  // 変数番号からレベルを得る．
  // もしもレベルが割り当てられていない場合にはエラーとなる．
  virtual
  ymuint
  level(VarId varid) const;

  // レベルから変数番号を得る．
  virtual
  VarId
  varid(ymuint level) const;

  /// @brief 現在の最大レベル + 1を返す．
  virtual
  ymuint
  max_level() const;

  // 動的変数順変更を許可する．
  virtual
  void
  enable_DVO();

  // 動的変数順変更を禁止する．
  virtual
  void
  disable_DVO();


  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  // multiple compose 演算を行うために最初に呼ばれる関数．
  virtual
  void
  compose_start();

  // multiple compose 演算を行う変数と置き換え関数を登録する関数
  virtual
  void
  compose_reg(VarId id,
	      BddEdge e);

  // multiple compose 演算の本体
  virtual
  BddEdge
  compose(BddEdge e);

  // x_level の変数を y_level まで「押し込む」．
  // pol が kPolNega の時は 0-枝と 1-枝を取り替える．
  virtual
  BddEdge
  push_down(BddEdge e,
	    ymuint x_level,
	    ymuint y_level,
	    tPol pol);

#if 0
  // smoothing(elimination)
  // svars に含まれる変数を消去する．
  virtual
  BddEdge
  esmooth(BddEdge e1,
	  BddEdge e2);

  // src1 と src2 の論理積を計算して src3 の変数を消去する．
  virtual
  BddEdge
  and_exist(BddEdge e1,
	    BddEdge e2,
	    BddEdge e3);
#endif

  // lower と upper で指定された不完全指定論理関数の非冗長積和形を求める．
  virtual
  BddEdge
  isop(BddEdge l,
       BddEdge u,
       LogExpr& cover);

  // lower と upper で指定された不完全指定論理関数の主項カバーを求める．
  virtual
  LogExpr
  prime_cover(BddEdge l,
	      BddEdge u);

  // lower と upper で指定された不完全指定論理関数の極小サポート集合をすべて
  // 列挙する．解は論理関数の形で返される．そのなかの主項がサポート集合に
  // 対応している．
  virtual
  BddEdge
  minimal_support(BddEdge l,
		  BddEdge u);


  //////////////////////////////////////////////////////////////////////
  // 1へ至るパスを求める関数
  //////////////////////////////////////////////////////////////////////

  // 1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  onepath(BddEdge e);

  // 最短の1パスを求める．
  // 結果はその経路のみのBDDとなる．
  virtual
  BddEdge
  shortest_onepath(BddEdge e);

  // 最短の1パスの長さを求める．
  virtual
  ymuint
  shortest_onepath_len(BddEdge e);


  //////////////////////////////////////////////////////////////////////
  // VarSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // src1 と src2 が変数集合の時に共通部分を求める．
  virtual
  BddEdge
  vscap(BddEdge e1,
	BddEdge e2);

  // src1 と src2 が変数集合の時に集合差を求める．
  virtual
  BddEdge
  vsdiff(BddEdge e1,
	 BddEdge e2);

  // src1 と src2 が変数集合の時のインターセクションチェック
  virtual
  bool
  vsintersect(BddEdge e1,
	      BddEdge e2);


  //////////////////////////////////////////////////////////////////////
  // LitSet の内部で用いられる演算
  //////////////////////////////////////////////////////////////////////

  // src1 と src2 がリテラル集合の時に共通部分を求める．
  virtual
  BddEdge
  lscap(BddEdge e1,
	BddEdge e2);

  // src1 と src2 がリテラル集合の時に集合差を求める．
  virtual
  BddEdge
  lsdiff(BddEdge e1,
	 BddEdge e2);

  // src1 と src2 がリテラル集合の時のインターセクションチェック
  virtual
  bool
  lsintersect(BddEdge e1,
	      BddEdge e2);

  // LitSet 用のBDDからリテラルのベクタを作る．
  virtual
  ymuint
  to_literalvector(BddEdge e,
		   LiteralVector& dst);

  // LitSet 用のBDDからリテラルのリストを作る．
  virtual
  ymuint
  to_literallist(BddEdge e,
		 LiteralList& dst);


  //////////////////////////////////////////////////////////////////////
  // 内部動作の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ガーベージコレクションを許可する．
  virtual
  void
  enable_gc();

  /// @brief ガーベージコレクションを禁止する．
  virtual
  void
  disable_gc();

  // GC 前の sweep 処理を行うためのバインダーを登録する．
  virtual
  void
  reg_sweep_binder(EventBinder* binder);

  // パラメータを設定する．設定したい項目のマスクビットを1とする．
  virtual
  void
  param(const BddMgrParam& param,
	ymuint32 mask);

  // パラメータを取得する．
  virtual
  void
  param(BddMgrParam& param) const;

  // 節点テーブルの拡張を制御するパラメータを得る．
  double
  nt_load_limit() const;

  // 名前を得る．
  virtual
  const string&
  name() const;

  // 使用メモリ量(in bytes)を得る．
  virtual
  ymuint64
  used_mem() const;

  // 節点テーブルに登録されているノードの数を得る．
  virtual
  ymuint64
  node_num() const;

  // GC で回収される(フリーになる)ノード数を得る．
  virtual
  ymuint64
  garbage_num() const;

  // 利用可能なフリーノード数を得る．
  virtual
  ymuint64
  avail_num() const;

  // GC の起動された回数を得る．
  virtual
  ymuint64
  gc_count() const;


  //////////////////////////////////////////////////////////////////////
  //
  //////////////////////////////////////////////////////////////////////

  BmmVar*
  alloc_var(VarId varid);

  // 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
  // 与えられた枝とインデックスを持つノードを返す．
  BddEdge
  new_node(ymuint level,
	   BddEdge l,
	   BddEdge h);


  //////////////////////////////////////////////////////////////////////
  // 上記の XXX_op() の内部で用いられる再帰関数
  //////////////////////////////////////////////////////////////////////

  // compose 演算を行う．
  BddEdge
  compose_step(BddEdge f);

#if 0
  // existential qualification を行う．
  BddEdge
  esmooth_step(BddEdge e1);

  // and exist 演算を行う．
  BddEdge
  andexist_step(BddEdge e1,
		BddEdge e2);
#endif

  // 最短の1パスを求める．
  BddEdge
  sp_step(BddEdge e,
	  BddEdgeEdgeMap& sp_map);

  // ISOP を求める．
  BddEdge
  isop_step(BddEdge l,
	    BddEdge u,
	    LogExpr& cov);

  // prime cover を求める．
  BddEdge
  pc_step(BddEdge l,
	  BddEdge u,
	  LogExpr& cov);

  // minimal support を求める演算
  BddEdge
  ms_step(BddEdge l,
	  BddEdge u);

  // sp_step 中で用いられる関数
  static
  ymint
  sp_len(BddEdge e);

  // shortest_onepath_len() 中で用いられる下請関数
  static
  ymint
  spl_step(BddEdge e,
	   BddEdgeIntMap& assoc);


  //////////////////////////////////////////////////////////////////////
  // 内部メンバの管理用関数
  //////////////////////////////////////////////////////////////////////

  // 節点テーブルのサイズを返す．
  // メモリ確保に失敗したら false を返す．
  bool
  resize(ymuint64 new_size);

  // 次のリミット値を計算する
  void
  set_next_limit_size();

  /// @brief lockall() 用のフック
  virtual
  void
  lock_hook(BddNode* vp);

  /// @brief unlockall() 用のフック
  virtual
  void
  unlock_hook(BddNode* vp);

  // ガーベージコレクションを行なう．
  // shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  virtual
  void
  _gc(bool shrink_nodetable);

  // 演算結果テーブルを登録する．
  void
  add_table(CompTbl* tbl);

  // mVarTable 中のマークを消す．
  void
  clear_varmark();

  // level の変数を取り出す．
  BmmVar*
  var_at(ymuint level) const;

  // varid の変数を取出す．
  BmmVar*
  var_of(VarId varid) const;

  // Var を登録する．
  void
  reg_var(BmmVar* var);


  //////////////////////////////////////////////////////////////////////
  // メモリ管理用の関数
  //////////////////////////////////////////////////////////////////////

  // 節点を確保する．
  BddNode*
  alloc_node();

  // 節点チャンクをスキャンして参照されていない節点をフリーリストにつなぐ
  // ただし，チャンク全体が参照されていなかった場合にはフリーリストには
  // つながない．その場合には true を返す．
  bool
  scan_nodechunk(BddNode* blk,
		 ymuint64 blk_size,
		 BddNode**& prev);

  // 変数テーブル用のメモリを確保する．
  // size はバイト単位ではなくエントリ数．
  BmmVar**
  alloc_vartable(ymuint64 size);

  // 変数テーブル用のメモリを解放する．
  // size はバイト単位ではなくエントリ数
  void
  dealloc_vartable(BmmVar** table,
		   ymuint64 size);

  // 節点テーブル用のメモリを確保する．
  // size はバイト単位ではなくエントリ数
  BddNode**
  alloc_nodetable(ymuint64 size);

  // 節点テーブル用のメモリを解放する．
  // size はバイト単位ではなくエントリ数
  void
  dealloc_nodetable(BddNode** table,
		    ymuint64 size);

  // 節点チャンク用のメモリを確保する．
  BddNode*
  alloc_nodechunk();

  // 節点チャンク用のメモリを解放する．
  void
  dealloc_nodechunk(BddNode* chunk);

  // このマネージャで使用するメモリ領域を確保する．
  void*
  allocate(ymuint64 size);

  // このマネージャで確保したメモリを解放する．
  void
  deallocate(void* ptr,
	     ymuint64 size);


  //////////////////////////////////////////////////////////////////////
  // BddEdge を操作するクラスメソッド
  //////////////////////////////////////////////////////////////////////

  // idx が top に等しいときには e の子供を e_0, e_1 にセットする．
  // 等しくなければ e をセットする．
  static
  void
  split1(ymuint top,
	 ymuint level,
	 BddEdge e,
	 const BddNode* vp,
	 tPol pol,
	 BddEdge& e_0,
	 BddEdge& e_1);

  // f と g のノードの子供のノードとレベルを求める．
  static
  ymuint
  split(BddEdge f,
	BddEdge g,
	BddEdge& f_0,
	BddEdge& f_1,
	BddEdge& g_0,
	BddEdge& g_1);


private:
  //////////////////////////////////////////////////////////////////////
  // デバッグ用の名前
  //////////////////////////////////////////////////////////////////////

  string mName;


  //////////////////////////////////////////////////////////////////////
  // ユーザーが設定するパラメータ
  // 設定は専用のメソッドを用いる．
  //////////////////////////////////////////////////////////////////////

  // ガーベージの割合がこの値を越えるとGCを起こす．
  double mGcThreshold;

  // ただし，全体のノード数がこの数以下の時はGCは起こさない．
  ymuint64 mGcNodeLimit;

  // 節点テーブル拡張時の制限値を決めるパラメータ
  double mNtLoadLimit;

  // 演算結果テーブル拡張時の制限値を決めるパラメータ
  double mRtLoadLimit;

  // 使用メモリ量の上限
  ymuint64 mMemLimit;


  //////////////////////////////////////////////////////////////////////
  // パッケージ内部の情報
  // 読み出しのみ可能
  //////////////////////////////////////////////////////////////////////

  // 全てのノード数
  ymuint64 mNodeNum;

  // ゴミ（誰からも参照されていない）ノード数
  ymuint64 mGarbageNum;

  // 使用メモリ量
  ymuint64 mUsedMem;

  // GCの起こった回数
  ymuint64 mGcCount;


  //////////////////////////////////////////////////////////////////////
  // 変数に関連した情報を格納しておくエリア
  //////////////////////////////////////////////////////////////////////

  // レベルをキーにして変数のポインタを格納している配列
  // 必要に応じて拡張される．
  BmmVar** mVarTable;

  // mVarTable 用に確保されたサイズ(単位はエントリ数)
  ymuint32 mVarTableSize;

  // 確保された変数の数(<= mVarTableSize)
  ymuint32 mVarNum;

  // 変数番号をキーにして変数のポインタを格納しているハッシュ表
  BmmVar** mVarHashTable;

  // 最大のレベル
  ymuint32 mMaxLevel;


  //////////////////////////////////////////////////////////////////////
  // 節点テーブル関係のメンバ
  //////////////////////////////////////////////////////////////////////

  // テーブルサイズ
  ymuint64 mTableSize;

  // テーブルサイズ - 1
  ymuint64 mTableSize_1;

  // ノード数がこの数を越えたらテーブルサイズを拡張する．
  ymuint64 mNextLimit;

  // テーブル本体
  BddNode** mNodeTable;


  //////////////////////////////////////////////////////////////////////
  // 演算結果テーブル
  //////////////////////////////////////////////////////////////////////

  // この BddMgr に登録されているテーブルの先頭
  CompTbl* mTblTop;

  CompTbl2* mAndTable;
  CompTbl2* mXorTable;
  CompTbl2* mIntTable;

  CompTbl3* mIteTable;
  CompTbl1* mCmpTable;
  CompTbl2* mCofacTable;
  CompTbl1* mXcofactorTable;

  CompTbl1* mSmTable;
  CompTbl2* mAeTable;
  CompTbl1* mCubedivTable;

  CompTbl2* mMinsupTable;
  IsopTbl* mIsopTable;
  IsopTbl* mPcTable;

  CompTbl2* mCsTable;
  CompTbl3* mCs1Table;
  CompTbl2* mCs2Table;


  //////////////////////////////////////////////////////////////////////
  // メモリブロック管理用のメンバ
  //////////////////////////////////////////////////////////////////////

  // フリーな節点リストの先頭
  BddNode* mFreeTop;

  // フリーな節点数
  ymuint64 mFreeNum;

  // 今までに確保したメモリブロックの先頭
  BddNode* mTopBlk;

  // 現在使用中のブロック
  BddNode* mCurBlk;

  // mCurBlk の何番目まで使用しているかを示すインデックス
  ymuint64 mCurIdx;


  //////////////////////////////////////////////////////////////////////
  // garbage collection 用の制御用変数
  //////////////////////////////////////////////////////////////////////

  // GCの起動を制御する変数
  int mGcEnable;

  // ノード数がこの数を越えたら mGcEnable は常に true だと思う
  ymuint64 mDangerousZone;

  // GC 前に sweep 処理を行うオブジェクトを管理するマネージャ
  EventBindMgr mSweepMgr;


  //////////////////////////////////////////////////////////////////////
  // 内部的に用いられる作業領域
  //////////////////////////////////////////////////////////////////////

  // サポート演算中で用いられる作業領域
  list<BmmVar*> mVarSet;

  // dump/size で節点数を数えるための作業領域
  ymuint64 mNum;

  // smooth 用変数の最大レベル
  ymuint32 mLastLevel;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// idx が top に等しいときには e の子供を e_0, e_1 にセットする．
// 等しくなければ e をセットする．
inline
void
BddMgrModern::split1(ymuint top,
		     ymuint level,
		     BddEdge e,
		     const BddNode* vp,
		     tPol pol,
		     BddEdge& e_0,
		     BddEdge& e_1)
{
  if ( level == top ) {
    e_0 = vp->edge0(pol);
    e_1 = vp->edge1(pol);
  }
  else {
    e_0 = e_1 = e;
  }
}

// f と g のノードの子供のノードとレベルを求める．
inline
ymuint
BddMgrModern::split(BddEdge f,
		    BddEdge g,
		    BddEdge& f_0,
		    BddEdge& f_1,
		    BddEdge& g_0,
		    BddEdge& g_1)
{
  BddNode* f_vp = f.get_node();
  BddNode * g_vp = g.get_node();
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( g_level < level ) {
    level = g_level;
  }
  split1(level, f_level, f, f_vp, f_pol, f_0, f_1);
  split1(level, g_level, g, g_vp, g_pol, g_0, g_1);
  return level;
}

// 節点テーブルの拡張を制御するパラメータを得る．
inline
double
BddMgrModern::nt_load_limit() const
{
  return mNtLoadLimit;
}

END_NAMESPACE_YM_BDD

#endif // BDDMGRMODERN_H
