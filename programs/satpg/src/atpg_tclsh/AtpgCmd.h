#ifndef ATPGCMD_H
#define ATPGCMD_H

/// @file include/AtpgCmd.h
/// @brief AtpgCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmTclpp/TclCmd.h"
#include "YmNetworks/tgnet.h"
#include "YmUtils/USTime.h"


BEGIN_NAMESPACE_YM_SATPG

// 前方参照のためのクラス宣言
class FaultMgr;
class TvMgr;
class AtpgMgr;

//////////////////////////////////////////////////////////////////////
// ATPG のコマンドオブジェクトの基底クラス
// おもな仕事はエラーが起きた時にメッセージをセット
// するような関数の殻をかぶせること．
// あとは変換などの便利関数を入れてある．
//////////////////////////////////////////////////////////////////////
class AtpgCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  AtpgCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~AtpgCmd();


protected:

  /// @brief ネットワークをセットした後に呼ぶ関数
  void
  after_set_network();

  /// @brief 故障リストを更新した後に呼ぶ関数
  void
  after_update_faults();


protected:

  /// @brief ネットワークを設定する．
  void
  set_network(TpgNetwork* network);

  /// @brief TgNetwork を取り出す．
  TpgNetwork&
  _network();

  /// @brief FaultMgr を取り出す．
  FaultMgr&
  _fault_mgr();

  /// @brief TvMgr を取り出す．
  TvMgr&
  _tv_mgr();

  /// @brief テストベクタのリストを取り出す．
  vector<TestVector*>&
  _tv_list();

  /// @brief 2値の故障シミュレータを取り出す．
  Fsim&
  _fsim();

  /// @brief 3値の故障シミュレータを返す．
  Fsim&
  _fsim3();

  /// @brief ファイル読み込みに関わる時間を得る．
  USTime
  read_time() const;

  /// @brief DTPG に関わる時間を得る．
  USTime
  dtpg_time() const;

  /// @brief 故障シミュレーションに関わる時間を得る．
  USTime
  fsim_time() const;

  /// @brief SAT に関わる時間を得る．
  USTime
  sat_time() const;

  /// @brief その他の時間を得る．
  USTime
  misc_time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // メンバ変数
  //////////////////////////////////////////////////////////////////////

  // 共通のデータを管理するオブジェクト
  AtpgMgr* mMgr;

};

END_NAMESPACE_YM_SATPG

#endif // ATPGCMD_H
