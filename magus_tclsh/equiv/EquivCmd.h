#ifndef EQUIVCMD_H
#define EQUIVCMD_H

/// @file EquivCmd.h
/// @brief EquivCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2017 Yusuke Matsunaga
/// All rights reserved.

#include "MagCmd.h"
#include "NetHandle.h"
#include "ym/BnNetwork.h"
#include "ym/SatSolverType.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class EquivCmdBase EquivCmd.h "EquivCmd.h"
/// @brief 検証を行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class EquivCmdBase :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  EquivCmdBase(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EquivCmdBase();


protected:
  //////////////////////////////////////////////////////////////////////
  // 派生クラスのためのインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 共通のオプション解析と前処理を行う．
  /// @return エラーが起きたら TCL_ERROR を返す．
  int
  prologue(TclObjVector& objv);

  /// @brief ログレベルを返す．
  int
  log_level() const;

  /// @brief 時間制限を返す．
  int
  time_limit() const;

  /// @brief 使用する SAT-Solver の種類を表すオブジェクト
  SatSolverType
  sat_type() const;

  /// @brief ネットワーク1を返す．
  const BnNetwork&
  network1() const;

  /// @brief ネットワーク2を返す．
  const BnNetwork&
  network2() const;

  /// @brief 入力のノード番号のペアのリストを返す．
  const vector<pair<int, int>>&
  input_pair_list() const;

  /// @brief 出力のノード番号のペアのリストを返す．
  const vector<pair<int, int>>&
  output_pair_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 時間を指定した文字列から時間を取り出す．
  /// @return エラーが起きたら TCL_ERROR を返す．
  int
  get_time(const string& str,
	   int& time);

  /// @brief 順番で対応をとり，ID番号のペアのリストを作る．
  void
  assoc_by_order();

  /// @brief 名前で対応をとり, ID番号のペアのリストを作る．
  /// @return エラーが起きたら TCL_ERROR を返す．
  bool
  assoc_by_name();

  /// @brief 対象のネットワークを BDN に変換する．
  /// @param[in] net_handle ネットワークハンドル
  /// @param[out] dst_network 変換したネットワークを格納する変数
  void
  conv_to_bnet(const NetHandle* net_handle,
	       BnNetwork& dst_network);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // loglevel オプション解析用のオブジェクト
  TclPoptInt* mPoptLoglevel;

  // assoc-by-order オプション解析用のオブジェクト
  TclPopt* mPoptOrder;

  // assoc-by-name オプション解析用のオブジェクト
  TclPopt* mPoptName;

  // time_limit オプション解析用のオブジェクト
  TclPoptObj* mPoptTimeLimit;

  // sat オプション解析用のオブジェクト
  TclPoptStr* mPoptSat;

  // sat-option オプション解析用のオブジェクト
  TclPoptStr* mPoptSatOption;

  // satlog オプション解析用のオブジェクト
  TclPoptStr* mPoptSatlog;

  // ログレベル
  int mLogLevel;

  // 時間制限
  int mTimeLimit;

  // SAT-Solver のタイプ
  SatSolverType mSatType;

  // SAT-Solver のログをファイルに出力する時のストリーム
  ofstream mSatLogFile;

  // 検証対象のネットワーク1
  BnNetwork mNetwork1;

  // 検証対象のネットワーク2
  BnNetwork mNetwork2;

  // 入力のノード番号の対のリスト
  vector<pair<int, int>> mInputPairList;

  // 出力のノード番号の対のリスト
  vector<pair<int, int>> mOutputPairList;

};


//////////////////////////////////////////////////////////////////////
/// @class EquivCmd EquivCmd.h "EquivCmd.h"
/// @brief 検証を行うコマンド
//////////////////////////////////////////////////////////////////////
class EquivCmd :
  public EquivCmdBase
{
public:

  /// @brief コンストラクタ
  EquivCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EquivCmd();


protected:

  /// @brief コマンド処理関数
  /// @param[in] objv 引数の配列
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // loglevel オプション解析用のオブジェクト
  TclPoptInt* mPoptLoglevel;

  // sigsize オプション解析用のオブジェクト
  TclPoptInt* mPoptSigSize;

};


//////////////////////////////////////////////////////////////////////
/// @class EquivCmd2 EquivCmd.h "EquivCmd.h"
/// @brief 検証を行うコマンド
//////////////////////////////////////////////////////////////////////
class EquivCmd2 :
  public EquivCmdBase
{
public:

  /// @brief コンストラクタ
  EquivCmd2(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~EquivCmd2();


protected:

  /// @brief コマンド処理関数
  /// @param[in] objv 引数の配列
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // loglevel オプション解析用のオブジェクト
  TclPoptInt* mPoptLoglevel;

  // sigsize オプション解析用のオブジェクト
  TclPoptInt* mPoptSigSize;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ログレベルを返す．
inline
int
EquivCmdBase::log_level() const
{
  return mLogLevel;
}

// @brief 時間制限を返す．
inline
int
EquivCmdBase::time_limit() const
{
  return mTimeLimit;
}

// @brief 使用する SAT-Solver の種類を表す文字列を返す．
inline
SatSolverType
EquivCmdBase::sat_type() const
{
  return mSatType;
}

// @brief 検証対象のネットワーク1を返す．
inline
const BnNetwork&
EquivCmdBase::network1() const
{
  return mNetwork1;
}

// @brief 検証対象のネットワーク2を返す．
inline
const BnNetwork&
EquivCmdBase::network2() const
{
  return mNetwork2;
}

// @brief 入力のノード番号の対のリストを返す．
inline
const vector<pair<int, int>>&
EquivCmdBase::input_pair_list() const
{
  return mInputPairList;
}

// @brief 出力のノード番号の対のリストを返す．
inline
const vector<pair<int, int>>&
EquivCmdBase::output_pair_list() const
{
  return mOutputPairList;
}

END_NAMESPACE_MAGUS

#endif // MAGUS_EQUIV_EQUIVCMD_H
