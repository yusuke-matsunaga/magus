#ifndef CNFDDMGRIMPL_H
#define CNFDDMGRIMPL_H

/// @file CNFddMgrImpl.h
/// @brief CNFddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/CNFdd.h"
#include "CNFddEdge.h"
#include "CNFddNode.h"
#include "utils/Binder.h"


BEGIN_NAMESPACE_YM_CNFDD

class Op;
class UniOp;
class UniNOp;
class UniVOp;
class BinOp;
class SupOp;
class CNFddVar;
class CNFddNode;

//////////////////////////////////////////////////////////////////////
/// @class CNFddMgrImpl CNFddMgrImpl.h "CNFddMgrImpl.h"
/// @param[in] CNFDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class CNFddMgrImpl
{
  friend class CNFdd;
  friend class Op;
  friend class CompTbl;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを返す．
  static
  CNFddMgrImpl*
  default_mgr();

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] option オプション
  CNFddMgrImpl(const string& name = string(),
	       const string& option = string());

  /// @brief デストラクタ
  virtual
  ~CNFddMgrImpl();


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

  /// @brief 現在の最大レベル + 1を返す．
  ymuint
  max_level() const;

  /// @brief level の変数を取り出す．
  CNFddVar*
  var_at(ymuint level) const;

  /// @brief varid の変数を取出す．
  CNFddVar*
  var_of(VarId varid) const;


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief e1 $\wedge$ e2 を計算する．
  CNFddEdge
  conjunction(CNFddEdge e1,
	      CNFddEdge e2);

  /// @brief e1 $\vee$ e2 を計算する．
  CNFddEdge
  disjunction(CNFddEdge e1,
	      CNFddEdge e2);

  /// @brief src1 $\setminus$ src2 を計算する．
  CNFddEdge
  diff(CNFddEdge e1,
       CNFddEdge e2);

  /// @brief 要素ごとのユニオンを計算する．
  CNFddEdge
  merge(CNFddEdge e1,
	CNFddEdge e2);

  /// @brief 他の節に支配されている節を取り除く
  CNFddEdge
  make_minimal(CNFddEdge e1);

  /// @brief 要素数が limit 以下の要素のみを残す．
  CNFddEdge
  cut_off(CNFddEdge e1,
	  ymuint limit);

  /// @brief 指定した変数の肯定のリテラルを加える．
  /// @param[in] e 枝
  /// @param[in] var 交換を行う変数番号
  CNFddEdge
  add_posiliteral(CNFddEdge e,
		  VarId var);

  /// @brief 指定した変数の否定のリテラルを加える．
  /// @param[in] e 枝
  /// @param[in] var 交換を行う変数番号
  CNFddEdge
  add_negaliteral(CNFddEdge e,
		  VarId var);

  /// @brief 指定された変数を含まないコファクターを返す．
  /// @param[in] e 枝
  /// @param[in] var 変数番号
  CNFddEdge
  cofactor_0(CNFddEdge e,
	     VarId var);

  /// @brief 指定された変数を含むコファクターを返す．
  /// @param[in] e 枝
  /// @param[in] var 変数番号
  CNFddEdge
  cofactor_p(CNFddEdge e,
	     VarId var);

  /// @brief 指定された変数を含むコファクターを返す．
  /// @param[in] e 枝
  /// @param[in] var 変数番号
  CNFddEdge
  cofactor_n(CNFddEdge e,
	     VarId var);


public:
  //////////////////////////////////////////////////////////////////////
  // CNFDDの根の枝の参照回数関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e の参照回数を増やす．
  void
  inc_rootref(CNFddEdge e);

  /// @brief e の参照回数を減らす．
  void
  dec_rootref(CNFddEdge e);


public:
  //////////////////////////////////////////////////////////////////////
  // ノードの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
  /// @return 与えられた枝とインデックスを持つノードを返す．
  CNFddEdge
  new_node(ymuint level,
	   CNFddEdge e_0,
	   CNFddEdge e_p,
	   CNFddEdge e_n);


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするCNFDDのノード数を数える．
  ymuint64
  node_count(CNFddEdge e);

  /// @brief edge_list に登録されたCNFDDのノード数を数える．
  ymuint64
  node_count(const vector<CNFddEdge>& edge_list);

  /// @brief CNFDD の表す集合の要素数を返す．
  /// 無限長精度の整数(mpz_class)を用いて計算する．
  mpz_class
  count(CNFddEdge e);


public:
  //////////////////////////////////////////////////////////////////////
  // サポート関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするCNFDDのサポートに印をつける．
  ymuint
  mark_support(CNFddEdge e);

  /// @brief edge_list に登録されたCNFDDのサポートに印をつける．
  ymuint
  mark_support(const vector<CNFddEdge>& edge_list);

  /// @brief 印のついた変数をベクタに変換する．
  ymuint
  mark_to_vector(VarVector& support);

  /// @brief 印のついた変数をリストに変換する．
  ymuint
  mark_to_list(VarList& support);


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
  param(const CNFddMgrParam& param,
	ymuint32 mask);

  /// @brief パラメータを取得する．
  void
  param(CNFddMgrParam& param) const;

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

  /// @brief 演算テーブル用の load_limit を得る．
  double
  rt_load_limit() const;


public:
  //////////////////////////////////////////////////////////////////////
  // CNFDDの管理用関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CNFdd の根の枝をセットする時の関数
  void
  set_zdd(CNFdd* zdd_p,
	  CNFddEdge e);

  /// @brief zdd の根の枝を new_e に変更する．
  /// new_e も同一の CNFddMgr に属していると仮定する．
  void
  assign(CNFdd* zdd_p,
	 CNFddEdge new_e);


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
  // 上記の XXX_op() の内部で用いられる再帰関数
  //////////////////////////////////////////////////////////////////////

  /// @brief size() の下請関数
  void
  count1(CNFddEdge e);

  /// @brief count の下請関数
  mpz_class
  count_step(CNFddEdge e,
	     CNFddEdgeMpzMap& mc_map);

  /// @brief サポート変数に印をつける．
  void
  sup_step(CNFddEdge e);


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
  activate(CNFddEdge vd);

  /// @brief ノードのリンク数を減らし，他のリンクがなければロックを外す
  void
  deactivate(CNFddEdge vd);

  /// @brief vp と vp の子孫のノードをロックする
  void
  lockall(CNFddNode* vp);

  /// @brief vp と vp の子孫ノードを(他に参照されていないもののみ)ロックを外す
  void
  unlockall(CNFddNode* vp);

  /// @brief mVarTable 中のマークを消す．
  void
  clear_varmark();

  /// @brief Var を登録する．
  void
  reg_var(CNFddVar* var);


private:
  //////////////////////////////////////////////////////////////////////
  // メモリ管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を確保する．
  CNFddVar*
  alloc_var(VarId varid);

  /// @brief 節点を確保する．
  CNFddNode*
  alloc_node();

  /// @brief 節点チャンクをスキャンして参照されていない節点をフリーリストにつなぐ
  /// ただし，チャンク全体が参照されていなかった場合にはフリーリストには
  /// つながない．その場合には true を返す．
  bool
  scan_nodechunk(CNFddNode* blk,
		 ymuint blk_size,
		 CNFddNode**& prev);

  /// @brief 変数テーブル用のメモリを確保する．
  /// size はバイト単位ではなくエントリ数．
  CNFddVar**
  alloc_vartable(ymuint size);

  /// @brief 変数テーブル用のメモリを解放する．
  /// size はバイト単位ではなくエントリ数
  void
  dealloc_vartable(CNFddVar** table,
		   ymuint size);

  /// @brief 節点テーブル用のメモリを確保する．
  /// size はバイト単位ではなくエントリ数
  CNFddNode**
  alloc_nodetable(ymuint64 size);

  /// @brief 節点テーブル用のメモリを解放する．
  /// size はバイト単位ではなくエントリ数
  void
  dealloc_nodetable(CNFddNode** table,
		    ymuint64 size);

  /// @brief 節点チャンク用のメモリを確保する．
  CNFddNode*
  alloc_nodechunk();

  /// @brief 節点チャンク用のメモリを解放する．
  void
  dealloc_nodechunk(CNFddNode* chunk);

  /// @brief このマネージャで使用するメモリ領域を確保する．
  void*
  allocate(ymuint64 size);

  /// @brief このマネージャで確保したメモリを解放する．
  void
  deallocate(void* ptr,
	     ymuint64 size);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // デバッグ用の名前
  string mName;

  // 参照回数
  ymuint32 mRefCount;


  //////////////////////////////////////////////////////////////////////
  // このマネージャに管理されている CNFDD のリスト
  //////////////////////////////////////////////////////////////////////

  // リストのためのダミーヘッダ
  // CNFDD としては用いない．
  CNFdd* mTopCNFdd;


  //////////////////////////////////////////////////////////////////////
  // ログ出力用のメンバ
  //////////////////////////////////////////////////////////////////////

  // ログ出力用のストリーム
  ostream* mLogFp;

  // ダミーのヌルストリーム
  ofstream* mNullStream;


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
  CNFddVar* mVarTop;

  // 変数番号をキーにして変数のポインタを格納しているハッシュ表
  CNFddVar** mVarHashTable;


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
  CNFddNode** mNodeTable;


  //////////////////////////////////////////////////////////////////////
  // 演算結果テーブル
  //////////////////////////////////////////////////////////////////////

  // この CNFddMgr に登録されている演算オブジェクトのリスト
  list<Op*> mOpList;

  // conjunction 用の演算クラス
  BinOp* mConOp;

  // disjunction 用の演算クラス
  BinOp* mDisOp;

  // diff 用の演算クラス
  BinOp* mDiffOp;

  // merge 用の演算クラス
  BinOp* mMergeOp;

  // cut_off 用の演算クラス
  UniNOp* mCutOp;

  // make_minimal 用の演算クラス
  UniOp* mMinOp;

  // add_posiliteral 用の演算クラス
  UniVOp* mLitPOp;

  // add_negaliteral 用の演算クラス
  UniVOp* mLitNOp;

  // cofactor0 用の演算クラス
  UniVOp* mCof0Op;

  // cofactorP 用の演算クラス
  UniVOp* mCofPOp;

  // cofactorN 用の演算クラス
  UniVOp* mCofNOp;

  // support 用の演算クラス
  SupOp* mSupOp;


  //////////////////////////////////////////////////////////////////////
  // メモリブロック管理用のメンバ
  //////////////////////////////////////////////////////////////////////

  // フリーな節点リストの先頭
  CNFddNode* mFreeTop;

  // フリーな節点数
  ymuint64 mFreeNum;

  // 今までに確保したメモリブロックの先頭
  CNFddNode* mTopBlk;

  // 現在使用中のブロック
  CNFddNode* mCurBlk;

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
  list<CNFddVar*> mVarSet;

  // dump/size で節点数を数えるための作業領域
  ymuint64 mNum;


private:
  //////////////////////////////////////////////////////////////////////
  // スタティックメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトのオブジェクト
  static
  CNFddMgrImpl* mDefaultMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief e を根とするCNFDDのノード数を数える．
inline
ymuint64
CNFddMgrImpl::node_count(CNFddEdge e)
{
  return node_count(vector<CNFddEdge>(1, e));
}

// @brief e を根とするCNFDDのサポートに印をつける．
inline
ymuint
CNFddMgrImpl::mark_support(CNFddEdge e)
{
  return mark_support(vector<CNFddEdge>(1, e));
}

// @brief ノードのリンク数を増やし，もしロックされていなければロックする
inline
void
CNFddMgrImpl::activate(CNFddEdge vd)
{
  CNFddNode* node = vd.get_node();
  if ( node && node->linkinc() == 1 ) {
    lockall(node);
  }
}

// @brief ノードのリンク数を減らし，他のリンクがなければロックを外す
inline
void
CNFddMgrImpl::deactivate(CNFddEdge vd)
{
  CNFddNode* node = vd.get_node();
  if ( node && node->linkdec() == 0 ) {
    unlockall(node);
  }
}

// @brief 演算テーブル用の load_limit を得る．
inline
double
CNFddMgrImpl::rt_load_limit() const
{
  return mRtLoadLimit;
}

END_NAMESPACE_YM_CNFDD

#endif // CNFDDMGRIMPL_H
