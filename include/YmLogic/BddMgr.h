#ifndef YMYMLOGIC_BDDMGR_H
#define YMYMLOGIC_BDDMGR_H

/// @file YmLogic/BddMgr.h
/// @brief BddMgr のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/bdd_nsdef.h"
#include "YmLogic/VarId.h"
#include "YmLogic/Expr.h"
#include "YmUtils/Binder.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM_BDD

class BddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class BddMgrParam BddMgr.h "YmLogic/BddMgr.h"
/// @ingroup Bdd
/// @brief BddMgrにパラメータをセットするための構造体．
///
/// 全てのメンバを設定する必要はなく，設定したいメンバに対応した
/// マスクビットを1にすればよい
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
struct BddMgrParam
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
/// @class BddMgr BddMgr.h "YmLogic/BddMgr.h"
/// @ingroup Bdd
/// @brief BDDの動きを管理するクラス．
///
/// 実際には BddMgr へのスマートポインタとなっている．
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
class BddMgr
{
  friend class Bdd;
  friend class BddMgrImpl;
  friend class BddVector;
  friend class BddList;
  friend class DgMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタとデストラクタ
  // いちおう誰でも生成/破壊ができる．
  //////////////////////////////////////////////////////////////////////

  /// @brief デフォルトマネージャを返す．
  static
  BddMgr&
  default_mgr();

  /// @brief コンストラクタ
  /// @param[in] type BddMgr の型を表す文字列
  /// @param[in] name マネージャの名前
  /// @param[in] option オプション文字列
  BddMgr(const string& type,
	 const string& name = string(),
	 const string& option = string());

  /// @brief デストラクタ
  ~BddMgr();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name BDD 生成用関数
  /// @{

  /// @brief 定数0関数を作る．
  /// @return 生成された BDD
  Bdd
  make_zero();

  /// @brief 定数1関数を作る．
  /// @return 生成された BDD
  Bdd
  make_one();

  /// @brief オバーフローBDDを明示的に生成する．
  /// @return 生成された BDD
  Bdd
  make_overflow();

  /// @brief エラーBDDを明示的に生成する．
  /// @return 生成された BDD
  Bdd
  make_error();

  /// @brief リテラル関数を表すBDDを作る
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 生成された BDD
  Bdd
  make_literal(VarId varid,
	       bool inv);

  /// @brief リテラル関数を表すBDDを作る
  /// @param[in] lit リテラル
  /// @return 生成された BDD
  /// @sa Literal
  Bdd
  make_literal(const Literal& lit);

  /// @brief 肯定のリテラル関数を作る
  /// @param[in] varid 変数番号
  /// @return 生成された BDD
  Bdd
  make_posiliteral(VarId varid);

  /// @brief 否定のリテラル関数を作る．
  /// @param[in] varid 変数番号
  /// @return 生成された BDD
  Bdd
  make_negaliteral(VarId varid);

  /// @brief インデックスと左右の子供を指定してBDDを作る．
  /// @param[in] varid 変数番号
  /// @param[in] chd_0 0枝の指す BDD
  /// @param[in] chd_1 1枝の指す BDD
  /// @return 生成された BDD
  Bdd
  make_bdd(VarId varid,
	   const Bdd& chd_0,
	   const Bdd& chd_1);

  /// @brief ベクタを真理値表と見なしてBDDを作る．
  /// @param[in] v 真理値表ベクタ
  /// @param[in] ni 入力数
  /// @return 生成された BDD
  /// @note 変数は 0 〜 (ni - 1) とみなす．
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @warning v の大きさは 2^ni に等しくなければならない．
  Bdd
  tvec_to_bdd(const vector<int>& v,
	      ymuint ni);

  /// @brief ベクタを真理値表と見なしてBDDを作る(変数割当て付き)．
  /// @param[in] v 真理値表ベクタ
  /// @param[in] vars 変数番号のベクタ
  /// @return 生成された BDD
  /// @note こちらは個々の変数を vars で指定したもの．
  /// @note ベクタの値は非ゼロを true とみなす．
  /// @warning v の大きさは 2^(vars.size()) に等しくなければならない．
  Bdd
  tvec_to_bdd(const vector<int>& v,
	      const VarVector& vars);

  /// @brief 論理式からBDDを作る．
  /// @param[in] expr 論理式
  /// @param[in] varmap 変数番号の割当てを表す連想配列
  /// @return 生成された BDD
  /// @note varmapは論理式中にあらわれる変数から BDD 用の変数
  /// への写像を表す．
  /// varmap に登録されていない場合には恒等変換だと思う．
  Bdd
  expr_to_bdd(const Expr& expr,
	      const VarVarMap& varmap = VarVarMap());

  /// @brief 論理式からBDDを作る(compose版)．
  /// @param[in] expr 論理式
  /// @param[in] varmap 変数番号とBDDの割当てを表す連想配列
  /// @return 生成された BDD
  /// @note varmapは論理式中にあらわれる変数から論理関数への
  /// 写像を表す．
  /// map に登録されていない場合には恒等変換だと思う．
  Bdd
  expr_to_bdd(const Expr& expr,
	      const VarBddMap& varmap);

  /// @brief ストリームを論理式と見なしてBDDを作る．
  /// @param[in] in 入力ストリーム
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note ストリームが正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(istream& in,
	      string& err_msg);

  /// @brief 文字列を読んで論理式を作り,そこからBDDを作る．
  /// @param[in] s 論理式を表す文字列
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note 文字列が正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(const char* s,
	      string& err_msg);

  /// @brief 文字列を読んで論理式を作り,そこからBDDを作る．
  /// @param[in] s 論理式を表す文字列
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 生成された BDD
  /// @note 文字列が正しくないときにはエラーBDDを作り，
  /// err_msg にエラーメッセージをセットする．
  Bdd
  expr_to_bdd(const string& s,
	      string& err_msg);

  /// @brief しきい値関数を表すBDDを返す．
  /// @param[in] n 入力数
  /// @param[in] th しきい値
  /// @return 生成された BDD
  /// @note しきい値関数とは n 入力で th 個以上の入力が1のときに1
  /// となる関数のこと．
  /// @note 関数処理で作るよりも効率的．
  Bdd
  make_thfunc(ymuint n,
	      ymuint th);

  /// @brief 保存されたBDDをベクタに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された BDD を格納するベクタ
  void
  restore(IDO& s,
	  BddVector& array);

  /// @brief 保存されたBDDをリストに読み込む．
  /// @param[in] s 入力ストリーム
  /// @param[out] array 生成された BDD を格納するリスト
  void
  restore(IDO& s,
	  BddList& array);

  /// @}
  //////////////////////////////////////////////////////////////////////


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

  /// @brief 動的変数順変更を許可する．
  /// @note BddMgr の実装によっては動的変数順の変更をサポートして
  /// いない場合がある
  void
  enable_DVO();

  /// @brief 動的変数順変更を禁止する．
  /// @note BddMgr の実装によっては動的変数順の変更をサポートして
  /// いない場合がある
  void
  disable_DVO();

  /// @}
  //////////////////////////////////////////////////////////////////////


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
  /// @sa BddMgrParam
  void
  param(const BddMgrParam& param,
	ymuint32 mask);

  /// @brief パラメータを取得する．
  /// @param[out] param 取得されたパラメータを格納する変数
  void
  param(BddMgrParam& param) const;

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
  BddMgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際のマネージャを指すポインタ
  BddMgrImpl* mImpl;


private:
  //////////////////////////////////////////////////////////////////////
  // クラスメンバ
  //////////////////////////////////////////////////////////////////////

  // デフォルトマネージャ
  static
  BddMgr theDefaultMgr;

};

END_NAMESPACE_YM_BDD

#endif // YMYMLOGIC_BDDMGR_H
