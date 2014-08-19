#ifndef MAGUS_NETCMD_H
#define MAGUS_NETCMD_H

/// @file NetCmd.h
/// @brief NetCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class NetCmd NetCmd.h "NetCmd.h"
/// @ingroup MagusGroup
/// @brief -network オプションを取る Magus のコマンドオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class NetCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  /// @param[in] new_bnet_enable -new_bnet オプションを使用するとき true
  /// @param[in] new_bdn_enable -new_bdn オプションを使用するとき true
  /// @param[in] new_mvn_enable -new_mvn オプションを使用するとき true
  NetCmd(MagMgr* mgr,
	 bool new_bnet_enable = true,
	 bool new_bdn_enable = true,
	 bool new_mvn_enable = true);

  /// @brief デストラクタ
  virtual
  ~NetCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief new_bnet オプション用の解析オブジェクトを得る．
  TclPoptObj*
  popt_new_bnet();

  /// @brief new_bdn オプション用の解析オブジェクトを得る．
  TclPoptObj*
  popt_new_bdn();

  /// @brief new_mvn オプション用の解析オブジェクトを得る．
  TclPoptObj*
  popt_new_mvn();


protected:
  //////////////////////////////////////////////////////////////////////
  // TclCmd の仮想関数のオーバーロード
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンド行の引数をパーズする関数．
  /// @param[in] objv 引数の配列
  /// objv で与えられるコマンド行引数のうちで，
  /// -network \<network名\> | -nwk \<network名\>
  /// の形のオプションがあればカレントネットワークのスタックに積む．
  /// その場合には mNetworkSpecified のフラグに true がセットされる．
  /// objv からはこれらのオプション文字列が削除される．
  /// エラーが起きた時にはメッセージをインタプリタにセットして TCL_ERROR
  /// を返す．
  virtual
  int
  before_cmd_proc(TclObjVector& objv);

  /// @brief コマンド処理関数の後で実行される関数
  /// before_cmd_proc() で退避されたカレントネットワーク，
  /// カレントライブラリを元に戻す．
  virtual
  void
  after_cmd_proc();


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // network オプションを解析するオブジェクト
  TclPoptObj* mPoptNtwk;

  // new_bnet オプションを解析するオブジェクト
  TclPoptObj* mPoptNewBNet;

  // new_bdn オプションを解析するオブジェクト
  TclPoptObj* mPoptNewBdn;

  // new_mvn オプションを解析するオブジェクト
  TclPoptObj* mPoptNewMvn;

  // コマンドラインでネットワークの指定が行われたかを示すフラグ
  bool mNetworkSpecified;

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGNETCMD_H
