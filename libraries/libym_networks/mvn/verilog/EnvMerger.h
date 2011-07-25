#ifndef LIBYM_NETWORKS_MVN_VERILOG_ENVMERGER_H
#define LIBYM_NETWORKS_MVN_VERILOG_ENVMERGER_H

/// @file libym_networks/mvn/verilog/EnvMerger.h
/// @brief EnvMerger のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"
#include "Env.h"


BEGIN_NAMESPACE_YM_NETWORKS_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class EnvMerger EnvMerger.h "EnvMerger.h"
/// @brief 条件分岐で Env をマージするための基底クラス
//////////////////////////////////////////////////////////////////////
class EnvMerger
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr MvnMgr
  EnvMerger(MvnMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EnvMerger();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] env 対象の環境
  /// @param[in] cond 条件を表すノード
  /// @param[in] then_env 条件が成り立ったときに通るパスの環境
  /// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
  virtual
  void
  operator()(MvnModule* parent_module,
	     ProcEnv& env,
	     MvnNode* cond,
	     const ProcEnv& then_env,
	     const ProcEnv& else_env) = 0;


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入条件をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] cond 切り替え条件
  /// @param[in] then_cond cond が成り立ったときの代入条件
  /// @param[in] else_cond cond が成り立たなかったときの代入条件
  /// @note 基本的には ITE(cond, then_cond, else_cond) だが，NULL の場合がある．
  MvnNode*
  merge_cond(MvnModule* parent_module,
	     MvnNode* cond,
	     MvnNode* then_cond,
	     MvnNode* else_cond);

  /// @brief MvMgr を返す．
  MvnMgr*
  mgr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNetwork を扱うマネージャ
  MvnMgr* mMvnMgr;

};


//////////////////////////////////////////////////////////////////////
/// @class EnvMerger1 EnvMerger.h "EnvMerger.h"
/// @brief combinational always 用の merger
//////////////////////////////////////////////////////////////////////
class EnvMerger1 :
  public EnvMerger
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr MvnMgr
  EnvMerger1(MvnMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EnvMerger1();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] env 対象の環境
  /// @param[in] cond 条件を表すノード
  /// @param[in] then_env 条件が成り立ったときに通るパスの環境
  /// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
  virtual
  void
  operator()(MvnModule* parent_module,
	     ProcEnv& env,
	     MvnNode* cond,
	     const ProcEnv& then_env,
	     const ProcEnv& else_env);

};


//////////////////////////////////////////////////////////////////////
/// @class EnvMerger2 EnvMerger.h "EnvMerger.h"
/// @brief sequential always 用の merger
//////////////////////////////////////////////////////////////////////
class EnvMerger2 :
  public EnvMerger
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr MvnMgr
  /// @param[in] global_env グローバル環境
  EnvMerger2(MvnMgr* mgr,
	     Env& global_env);

  /// @brief デストラクタ
  virtual
  ~EnvMerger2();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief Env をマージする．
  /// @param[in] parent_module 親のモジュール
  /// @param[in] env 対象の環境
  /// @param[in] cond 条件を表すノード
  /// @param[in] then_env 条件が成り立ったときに通るパスの環境
  /// @param[in] else_env 条件が成り立たなかったときに通るパスの環境
  virtual
  void
  operator()(MvnModule* parent_module,
	     ProcEnv& env,
	     MvnNode* cond,
	     const ProcEnv& then_env,
	     const ProcEnv& else_env);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グローバル環境
  Env& mGlobalEnv;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief MvMgr を返す．
inline
MvnMgr*
EnvMerger::mgr() const
{
  return mMvnMgr;
}


END_NAMESPACE_YM_NETWORKS_VERILOG

#endif // LIBYM_NETWORKS_MVN_VERILOG_ENVMERGER_H
