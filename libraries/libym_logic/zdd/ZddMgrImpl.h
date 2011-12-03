#ifndef ZDDMGRIMPL_H
#define ZDDMGRIMPL_H

/// @file ZddMgrImpl.h
/// @brief ZddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ZddNode.h"
#include "CompTbl.h"
#include "ym_utils/Binder.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrImpl ZddMgrImpl.h "ZddMgrImpl.h"
/// @param[in] ZDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class ZddMgrImpl
{
  friend class Zdd;
  friend class CompTbl;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを返す．
  static
  ZddMgrImpl*
  default_mgr();

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] option オプション
  ZddMgrImpl(const string& name = string(),
	     const string& option = string());

  /// @brief デストラクタ
  virtual
  ~ZddMgrImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // ZDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の参照回数を増やす．
  void
  inc_rootref(ZddEdge e);

  /// @brief e の参照回数を減らす．
  void
  dec_rootref(ZddEdge e);


public:
  //////////////////////////////////////////////////////////////////////
  // 変数番号とレベルの対応づけ
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を確保する．
  /// @brief 確保に失敗したら false を返す．
  /// @brief 最後の変数の後ろに挿入される．
  bool
  new_var(VarId varid);

  /// @brief 現在登録されている変数をそのレベルの昇順で返す．
  /// @param[out] vlist 変数を格納するリスト
  /// @return 変数の数を返す．
  ymuint
  var_list(list<VarId>& vlist) const;

  /// @brief 変数番号からレベルを得る．
  /// @note もしもレベルが割り当てられていない場合にはエラーとなる．
  ymuint
  level(VarId varid) const;

  /// @brief レベルから変数番号を得る．
  VarId
  varid(ymuint level) const;


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD 生成用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief singletonを作る
  ZddEdge
  make_singleton(VarId varid);


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief e1 $\cap$ e2 を計算する．
  ZddEdge
  cap_op(ZddEdge e1,
	 ZddEdge e2);

  /// @brief e1 $\cap$ e2 $\cap$ e3 を計算する．
  ZddEdge
  cap_op(ZddEdge e1,
	 ZddEdge e2,
	 ZddEdge e3);

  /// @brief e1 $\cup$ e2 を計算する．
  ZddEdge
  cup_op(ZddEdge e1,
	 ZddEdge e2);

  /// @brief e1 $\cup$ e2 $\cup$ e3 を計算する．
  ZddEdge
  cup_op(ZddEdge e1,
	 ZddEdge e2,
	 ZddEdge e3);

  /// @brief src1 $\setdiff$ src2 を計算する．
  ZddEdge
  diff_op(ZddEdge e1,
	  ZddEdge e2);

  /// @brief 指定された変数を含まないコファクターを返す．
  ZddEdge
  cofactor0(ZddEdge e1,
	    VarId id);

  /// @brief 指定された変数を含むコファクターを返す．
  ZddEdge
  cofactor1(ZddEdge e1,
	    VarId id);


public:
  //////////////////////////////////////////////////////////////////////
  // ZDD の構造に関係したメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 根の節点の変数に基づいてShannon展開を行なう．
  /// 戻り値として根の節点の変数番号を返し，その変数を0/1に固定した
  /// 時の cofactor をそれぞれ f0, f1 に入れる．
  /// もともと定数値(葉)のZDDの場合，kVarIdMax を返し，
  /// f0, f1 には自分自身を代入する．
  VarId
  root_decomp(ZddEdge e,
	      ZddEdge& e0,
	      ZddEdge& e1);

  /// @brief 根の変数番号インデックスを取り出す．
  /// @brief 定数節点の場合には kVarIdMax を返す．
  VarId
  root_var(ZddEdge e);

  /// @brief 0枝の指している cofactor を返す．
  /// 定数節点の場合には自分自身を返す．
  ZddEdge
  edge0(ZddEdge e);

  /// @brief 1枝の指している cofactor を返す．
  /// 定数節点の場合には自分自身を返す．
  ZddEdge
  edge1(ZddEdge e);

  /// @brief e の参照回数が0なら true を返す．
  bool
  check_noref(ZddEdge e);


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするZDDのノード数を数える．
  ymuint64
  size(ZddEdge e);

  /// @brief edge_list に登録されたZDDのノード数を数える．
  ymuint64
  size(const list<ZddEdge>& edge_list);

  /// @brief ZDD の表す集合の要素数を返す．
  /// 無限長精度の整数(mpz_class)を用いて計算する．
  mpz_class
  count(ZddEdge e);


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするZDDのサポートに印をつける．
  ymuint
  mark_support(ZddEdge e);

  /// @brief edge_list に登録されたZDDのサポートに印をつける．
  ymuint
  mark_support(const list<ZddEdge>& edge_list);

  /// @brief 印のついた変数をベクタに変換する．
  ymuint
  mark_to_vector(VarVector& support);

  /// @brief 印のついた変数をリストに変換する．
  ymuint
  mark_to_list(VarList& support);

  /// @brief 印のついた変数をZDD(キューブ)に変換する．
  ZddEdge
  mark_to_zdd();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部動作の設定を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ガーベージコレクションを許可する．
  void
  enable_gc();

  /// @brief ガーベージコレクションを禁止する．
  void
  disable_gc();

  /// @brief ガーベージコレクションを行なう．
  /// shrink_nodetable = true の時, 可能なら節点テーブルのサイズを縮小する．
  void
  gc(bool shrink_nodetable);

  /// @brief GC 前の sweep 処理を行うためのバインダーを登録する．
  void
  reg_sweep_binder(EventBinder* binder);

  /// @brief パラメータを設定する．設定したい項目のマスクビットを1とする．
  void
  param(const ZddMgrParam& param,
	ymuint32 mask);

  /// @brief パラメータを取得する．
  void
  param(ZddMgrParam& param) const;

  /// @brief 名前を得る．
  const string&
  name() const;

  /// @brief 使用メモリ量(in bytes)を得る．
  ymuint64
  used_mem() const;

  /// @brief 節点テーブルに登録されているノードの数を得る．
  ymuint64
  node_num() const;

  /// @brief GC で回収される(フリーになる)ノード数を得る．
  ymuint64
  garbage_num() const;

  /// @brief 利用可能なフリーノード数を得る．
  ymuint64
  avail_num() const;

  /// @brief GC の起動された回数を得る．
  ymuint64
  gc_count() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ZDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Zdd の根の枝をセットする時の関数
  void
  set_zdd(Zdd* zdd_p,
	  ZddEdge e);

  /// @brief zdd の根の枝を new_e に変更する．
  /// new_e も同一の ZddMgr に属していると仮定する．
  void
  assign(Zdd* zdd_p,
	 ZddEdge new_e);


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
  // ノードを返す基本関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
  /// @return 与えられた枝とインデックスを持つノードを返す．
  ZddEdge
  new_node(ZddVar* var,
	   ZddEdge l,
	   ZddEdge h);


private:
  //////////////////////////////////////////////////////////////////////
  // 上記の XXX_op() の内部で用いられる再帰関数
  //////////////////////////////////////////////////////////////////////

  /// @brief cap_op の下請け関数
  ZddEdge
  cap_step(ZddEdge e1,
	   ZddEdge e2);

  /// @brief cup_op の下請け関数
  ZddEdge
  cup_step(ZddEdge e1,
	   ZddEdge e2);

  /// @brief diff_op の下請け関数
  ZddEdge
  diff_step(ZddEdge e1,
	    ZddEdge e2);

  /// @brief size() の下請関数
  void
  count1(ZddEdge e);

  /// @brief count の下請関数
  mpz_class
  count_step(ZddEdge e,
	     hash_map<ZddEdge, mpz_class>& mc_map);

  /// @brief サポート変数に印をつける．
  void
  sup_step(ZddEdge e);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部メンバの管理用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 節点テーブルのサイズを返す．
  /// @return メモリ確保に失敗したら false を返す．
  bool
  resize(ymuint64 new_size);

  /// @brief 次のリミット値を計算する
  void
  set_next_limit_size();

  /// @brief ノードのリンク数を増やし，もしロックされていなければロックする
  void
  activate(ZddEdge vd);

  /// @brief ノードのリンク数を減らし，他のリンクがなければロックを外す
  void
  deactivate(ZddEdge vd);

  /// @brief vp と vp の子孫のノードをロックする
  void
  lockall(ZddNode* vp);

  /// @brief vp と vp の子孫ノードを(他に参照されていないもののみ)ロックを外す
  void
  unlockall(ZddNode* vp);

  /// @brief 演算結果テーブルを登録する．
  void
  add_table(CompTbl* tbl);

  /// @brief mVarTable 中のマークを消す．
  void
  clear_varmark();

  /// @brief level の変数を取り出す．
  ZddVar*
  var_at(ymuint level) const;

  /// @brief varid の変数を取出す．
  ZddVar*
  var_of(VarId varid) const;

  /// @brief Var を登録する．
  void
  reg_var(ZddVar* var);


  //////////////////////////////////////////////////////////////////////
  // メモリ管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 節点を確保する．
  ZddNode*
  alloc_node();

  /// @brief 節点チャンクをスキャンして参照されていない節点をフリーリストにつなぐ
  /// ただし，チャンク全体が参照されていなかった場合にはフリーリストには
  /// つながない．その場合には true を返す．
  bool
  scan_nodechunk(ZddNode* blk,
		 ymuint blk_size,
		 ZddNode**& prev);

  /// @brief 変数テーブル用のメモリを確保する．
  /// size はバイト単位ではなくエントリ数．
  ZddVar**
  alloc_vartable(ymuint size);

  /// @brief 変数テーブル用のメモリを解放する．
  /// size はバイト単位ではなくエントリ数
  void
  dealloc_vartable(ZddVar** table,
		   ymuint size);

  /// @brief 節点テーブル用のメモリを確保する．
  /// size はバイト単位ではなくエントリ数
  ZddNode**
  alloc_nodetable(ymuint64 size);

  /// @brief 節点テーブル用のメモリを解放する．
  /// size はバイト単位ではなくエントリ数
  void
  dealloc_nodetable(ZddNode** table,
		    ymuint64 size);

  /// @brief 節点チャンク用のメモリを確保する．
  ZddNode*
  alloc_nodechunk();

  /// @brief 節点チャンク用のメモリを解放する．
  void
  dealloc_nodechunk(ZddNode* chunk);

  /// @brief このマネージャで使用するメモリ領域を確保する．
  void*
  allocate(ymuint64 size);

  /// @brief このマネージャで確保したメモリを解放する．
  void
  deallocate(void* ptr,
	     ymuint64 size);


  //////////////////////////////////////////////////////////////////////
  // ZddEdge を操作するクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief mark が付いた節点のマークを消す．
  static
  void
  clear_mark(ZddEdge e);

  /// @brief vdの指すノードのマークを調べ，マークされていればtrueを返す．
  static
  bool
  mark(ZddEdge vd);

  /// @brief vdの指すノードにマークをつける
  static
  void
  setmark(ZddEdge vd);

  /// @brief idx が top に等しいときには e の子供を e_0, e_1 にセットする．
  /// 等しくなければ e をセットする．
  static
  void
  split1(ymuint top,
	 ymuint level,
	 ZddEdge e,
	 const ZddNode* vp,
	 bool zattr,
	 ZddEdge& e_0,
	 ZddEdge& e_1);

  /// @brief f と g のノードの子供のノードとレベルを求める．
  static
  ZddVar*
  split(ZddEdge f,
	ZddEdge g,
	ZddEdge& f_0,
	ZddEdge& f_1,
	ZddEdge& g_0,
	ZddEdge& g_1);


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

  // mVarTable 用に確保されたサイズ(単位はエントリ数)
  ymuint32 mVarTableSize;

  // 確保された変数の数(<= mVarTableSize)
  ymuint32 mVarNum;

  // 変数リストの先頭
  ZddVar* mVarTop;

  // 変数番号をキーにして変数のポインタを格納しているハッシュ表
  ZddVar** mVarHashTable;


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
  ZddNode** mNodeTable;


  //////////////////////////////////////////////////////////////////////
  // 演算結果テーブル
  //////////////////////////////////////////////////////////////////////

  // この ZddMgr に登録されているテーブルの先頭
  CompTbl* mTblTop;

  CompTbl2* mCapTable;
  CompTbl2* mCupTable;
  CompTbl2* mDiffTable;


  //////////////////////////////////////////////////////////////////////
  // メモリブロック管理用のメンバ
  //////////////////////////////////////////////////////////////////////

  // フリーな節点リストの先頭
  ZddNode* mFreeTop;

  // フリーな節点数
  ymuint64 mFreeNum;

  // 今までに確保したメモリブロックの先頭
  ZddNode* mTopBlk;

  // 現在使用中のブロック
  ZddNode* mCurBlk;

  // mCurBlk の何番目まで使用しているかを示すインデックス
  ymuint32 mCurIdx;

  // メモリ確保が失敗した時にオンになるフラグ
  bool mOverflow;


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
  list<ZddVar*> mVarSet;

  // dump/size で節点数を数えるための作業領域
  ymuint64 mNum;

  // smooth 用変数の最大レベル
  ymuint32 mLastLevel;

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

END_NAMESPACE_YM_ZDD

#endif // ZDDMGRIMPL_H
