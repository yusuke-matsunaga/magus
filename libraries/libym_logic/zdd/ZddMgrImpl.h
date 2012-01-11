#ifndef ZDDMGRIMPL_H
#define ZDDMGRIMPL_H

/// @file ZddMgrImpl.h
/// @brief ZddMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"
#include "ym_utils/Binder.h"
#include "ZddEdge.h"
#include "ZddNode.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddOp;
class ZddUniOp;
class ZddBinOp;
class ZddVar;
class ZddNode;
class SupOp;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrImpl ZddMgrImpl.h "ZddMgrImpl.h"
/// @param[in] ZDD を管理するクラス
//////////////////////////////////////////////////////////////////////
class ZddMgrImpl
{
  friend class Zdd;
  friend class ZddOp;

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
  ZddVar*
  var_at(ymuint level) const;

  /// @brief varid の変数を取出す．
  ZddVar*
  var_of(VarId varid) const;


public:
  //////////////////////////////////////////////////////////////////////
  // built-in タイプの論理演算
  //////////////////////////////////////////////////////////////////////

  /// @brief e1 $\cap$ e2 を計算する．
  ZddEdge
  cap(ZddEdge e1,
      ZddEdge e2);

  /// @brief e1 $\cup$ e2 を計算する．
  ZddEdge
  cup(ZddEdge e1,
      ZddEdge e2);

  /// @brief src1 $\setdiff$ src2 を計算する．
  ZddEdge
  diff(ZddEdge e1,
       ZddEdge e2);

  /// @brief 指定した変数の0枝と1枝を交換する．
  /// @param[in] e 枝
  /// @param[in] var 交換を行う変数番号
  ZddEdge
  swap(ZddEdge e,
       VarId var);

  /// @brief 指定された変数を含まないコファクターを返す．
  /// @param[in] e 枝
  /// @param[in] var 交換を行う変数番号
  ZddEdge
  cofactor0(ZddEdge e,
	    VarId var);

  /// @brief 指定された変数を含むコファクターを返す．
  /// @param[in] e 枝
  /// @param[in] var 交換を行う変数番号
  ZddEdge
  cofactor1(ZddEdge e,
	    VarId var);

  /// @brief 要素ごとのユニオンを計算する．
  ZddEdge
  merge(ZddEdge e1,
	ZddEdge e2);


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
  // ノードの生成
  //////////////////////////////////////////////////////////////////////

  /// @brief 左右の枝が同じ場合にはその枝自身を返し，それ以外の場合には，
  /// @return 与えられた枝とインデックスを持つノードを返す．
  ZddEdge
  new_node(ymuint level,
	   ZddEdge l,
	   ZddEdge h);


public:
  //////////////////////////////////////////////////////////////////////
  // ノード数の計数や真理値表密度の計算など
  //////////////////////////////////////////////////////////////////////

  /// @brief e を根とするZDDのノード数を数える．
  ymuint64
  node_count(ZddEdge e);

  /// @brief edge_list に登録されたZDDのノード数を数える．
  ymuint64
  node_count(const vector<ZddEdge>& edge_list);

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
  mark_support(const vector<ZddEdge>& edge_list);

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

  /// @brief 演算テーブル用の load_limit を得る．
  double
  rt_load_limit() const;


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


public:
  //////////////////////////////////////////////////////////////////////
  // メモリ管理関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief このマネージャで使用するメモリ領域を確保する．
  void*
  allocate(ymuint64 size);

  /// @brief このマネージャで確保したメモリを解放する．
  void
  deallocate(void* ptr,
	     ymuint64 size);


private:
  //////////////////////////////////////////////////////////////////////
  // 上記の XXX_op() の内部で用いられる再帰関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief mVarTable 中のマークを消す．
  void
  clear_varmark();

  /// @brief Var を登録する．
  void
  reg_var(ZddVar* var);


private:
  //////////////////////////////////////////////////////////////////////
  // メモリ管理用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を確保する．
  ZddVar*
  alloc_var(VarId varid);

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // デバッグ用の名前
  string mName;

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

  // cap 用の演算クラス
  ZddBinOp* mCapOp;

  // cup 用の演算クラス
  ZddBinOp* mCupOp;

  // diff 用の演算クラス
  ZddBinOp* mDiffOp;

  // swap 用の演算クラス
  ZddUniOp* mSwapOp;

  // cofactor0 用の演算クラス
  ZddUniOp* mCof0Op;

  // cofactor1 用の演算クラス
  ZddUniOp* mCof1Op;

  // support 用の演算オブジェクト
  SupOp* mSupOp;

  // merge 用の演算オブジェクト
  ZddBinOp* mMergeOp;

  // 演算オブジェクトのリスト
  list<ZddOp*> mOpList;


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


private:
  //////////////////////////////////////////////////////////////////////
  // スタティックメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトのオブジェクト
  static
  ZddMgrImpl* mDefaultMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief e を根とするZDDのノード数を数える．
inline
ymuint64
ZddMgrImpl::node_count(ZddEdge e)
{
  return node_count(vector<ZddEdge>(1, e));
}

// @brief e を根とするZDDのサポートに印をつける．
inline
ymuint
ZddMgrImpl::mark_support(ZddEdge e)
{
  return mark_support(vector<ZddEdge>(1, e));
}

// @brief ノードのリンク数を増やし，もしロックされていなければロックする
inline
void
ZddMgrImpl::activate(ZddEdge vd)
{
  ZddNode* node = vd.get_node();
  if ( node && node->linkinc() == 1 ) {
    lockall(node);
  }
}

// @brief ノードのリンク数を減らし，他のリンクがなければロックを外す
inline
void
ZddMgrImpl::deactivate(ZddEdge vd)
{
  ZddNode* node = vd.get_node();
  if ( node && node->linkdec() == 0 ) {
    unlockall(node);
  }
}

// @brief 演算テーブル用の load_limit を得る．
inline
double
ZddMgrImpl::rt_load_limit() const
{
  return mRtLoadLimit;
}

END_NAMESPACE_YM_ZDD

#endif // ZDDMGRIMPL_H
