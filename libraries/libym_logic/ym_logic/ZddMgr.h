#ifndef YM_LOGIC_ZDDMGR_H
#define YM_LOGIC_ZDDMGR_H

/// @file ym_logic/ZddMgr.h
/// @brief ZddMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_utils/Binder.h"
#include "gmpxx.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ZddMgrParam ZddMgr.h "ym_logic/ZddMgr.h"
/// @ingroup Zdd
/// @brief ZddMgrにパラメータをセットするための構造体．
///
/// 全てのメンバを設定する必要はなく，設定したいメンバに対応した
/// マスクビットを1にすればよい
/// @sa ZddMgr
//////////////////////////////////////////////////////////////////////
struct ZddMgrParam
{
  static
  const ymuint32 GC_THRESHOLD  =  1U;
  static
  const ymuint32 GC_NODE_LIMIT =  2U;
  static
  const ymuint32 NT_LOAD_LIMIT =  4U;
  static
  const ymuint32 RT_LOAD_LIMIT =  8U;
  static
  const ymuint32 MEM_LIMIT     = 16U;

  double mGcThreshold;
  ymuint64 mGcNodeLimit;
  double mNtLoadLimit;
  double mRtLoadLimit;
  ymuint64 mMemLimit;
};


//////////////////////////////////////////////////////////////////////
/// @class ZddMgr ZddMgr.h "ym_logic/ZddMgr.h"
/// @ingroup Zdd
/// @brief ZDDの動きを管理するクラス．
///
/// 実際には ZddMgr へのスマートポインタとなっている．
/// @sa ZddMgr
//////////////////////////////////////////////////////////////////////
class ZddMgr
{
  friend class Zdd;
  friend class ZddVector;
  friend class ZddList;
  friend class ZddMgrImpl;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  // いちおう誰でも生成/破壊ができる．
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを返す．
  static
  ZddMgr&
  default_mgr();

  /// @brief コンストラクタ
  /// @param[in] type ZddMgr の型を表す文字列
  /// @param[in] name マネージャの名前
  /// @param[in] option オプション文字列
  ZddMgr(const string& type,
	 const string& name = string(),
	 const string& option = string());

  /// @brief デストラクタ
  ~ZddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ZDD 生成用関数
  /// @{

  /// @brief 定数0(A.K.A. empty set)を作る．
  /// @return 生成された ZDD
  Zdd
  make_empty();

  /// @brief 定数1(A.K.A. base set)を作る．
  /// @return 生成された ZDD
  Zdd
  make_base();

  /// @brief オバーフローZDDを明示的に生成する．
  /// @return 生成された ZDD
  Zdd
  make_overflow();

  /// @brief エラーZDDを明示的に生成する．
  /// @return 生成された ZDD
  Zdd
  make_error();

  /// @brief 2つの集合をマージする．
  Zdd
  merge(const Zdd& left,
	const Zdd& right);

  /// @brief 2つの集合をマージする．
  Zdd
  merge(const Zdd& left,
	const Zdd& right,
	ymuint limit);

  /// @brief 要素数を制限する．
  Zdd
  n_element(const Zdd& left,
	    ymuint limit);

  /// @brief 極小集合のみにする．
  Zdd
  make_minimal(const Zdd& left);

  /// @brief 要素数最小の集合を一つ返す．
  Zdd
  minimum_set(const Zdd& left);


public:
  //////////////////////////////////////////////////////////////////////
  /// @name バイナリ入出力関係の関数
  /// @{

#if 0
  /// @brief ZDDの内容をバイナリダンプする．
  /// @param[in] s 出力ストリーム
  /// @param[in] zdd 対象のZDD
  void
  dump(ODO& s,
       const Zdd& zdd);

  /// @brief ZDDの配列の内容をバイナリファイルに書き出す
  /// @param[in] s 出力先のストリーム
  /// @param[in] array ZDD を収めた配列
  void
  dump(ODO& s,
       const ZddVector& array);

  /// @brief ZDDの配列の内容をバイナリファイルに書き出す
  /// @param[in] array ZDD を収めた配列
  /// @param[in] s 出力先のストリーム
  void
  dump(ODO& s,
       const ZddList& array);

  /// @brief 保存されたZDDを読み込む．
  /// @param[in] s 入力ストリーム
  /// @return 生成された ZDD
  Zdd
  restore(IDO& s);

  /// @brief 保存されたZDDをベクタに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された ZDD を格納するベクタ
  void
  restore(IDO& s,
	  ZddVector& array);

  /// @brief 保存されたZDDをリストに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された ZDD を格納するリスト
  void
  restore(IDO& s,
	  ZddList& array);
#endif
  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 変数番号とレベルの対応づけ
  /// @{

  /// @brief 変数を確保する．
  /// @param[in] varid 変数番号
  /// @retval true 変数が確保できた
  /// @retval false 確保に失敗した
  /// @note 新しい変数は最後の変数の後ろに挿入される．
  bool
  new_var(VarId varid);

  /// @brief 現在登録されている変数をそのレベルの昇順で返す．
  /// @param[out] vlist 変数を格納するリスト
  /// @return 変数の要素数
  ymuint
  var_list(list<VarId>& vlist) const;

  /// @brief 変数番号からレベルを得る．
  /// @param[in] varid 変数番号
  /// @return varid に対応するレベル
  /// @note もしもレベルが割り当てられていない場合にはエラーとなる．
  ymuint
  level(VarId varid) const;

  /// @brief レベルから変数番号を得る．
  /// @param[in] level レベル
  /// @return level に対応する変数番号
  VarId
  varid(ymuint level) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内部動作の設定を行う関数
  /// @{

  /// @brief ガーベージコレクションを許可する．
  void
  enable_gc();

  /// @brief ガーベージコレクションを禁止する．
  void
  disable_gc();

  /// @brief ガーベージコレクションを行なう．
  /// @param[in] shrink_nodetable この値が true の時,
  /// 可能なら節点テーブルのサイズを縮小する．
  void
  gc(bool shrink_nodetable);

  /// @brief GC 前の sweep 処理を行うためのバインダーを登録する．
  /// @param[in] binder
  /// @sa EventBinder
  void
  reg_sweep_binder(EventBinder* binder);

  /// @brief ログ出力用のストリームを設定する．
  /// @param[in] s 出力ストリーム
  void
  set_logstream(ostream& s);

  /// @brief ログ出力用のストリームを解除する．
  void
  unset_logstream();

  /// @brief ログ出力用のストリームを得る．
  /// @return ログ出力用のストリーム
  ostream&
  logstream() const;

  /// @brief パラメータを設定する．
  /// @param[in] param 設定用パラメータ
  /// @param[in] mask 設定したい項目を指定するマスク
  /// @note mask のビットが 1 となっている項目のみ設定する．
  /// @sa ZddMgrParam
  void
  param(const ZddMgrParam& param,
	ymuint32 mask);

  /// @brief パラメータを取得する．
  /// @param[out] param 取得されたパラメータを格納する変数
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

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 非公開関数
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを作るためのコンストラクタ
  ZddMgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のマネージャを指すポインタ
  ZddMgrImpl* mImpl;


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトマネージャ
  static
  ZddMgr theDefaultMgr;

};

END_NAMESPACE_YM_ZDD

#endif // YM_LOGIC_ZDDMGR_H
