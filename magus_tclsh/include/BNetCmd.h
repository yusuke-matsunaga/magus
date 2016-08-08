#ifndef MAGUS_MAGBNETCMD_H
#define MAGUS_MAGBNETCMD_H

/// @file BNetCmd.h
/// @brief BNetCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "NetCmd.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BNetCmd BNetCmd.h "BNetCmd.h"
/// @ingroup MagusGroup
/// @brief BNetwork を操作対象とする Magus のコマンドオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class BNetCmd :
  public NetCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  /// @param[in] new_bnet_enable -new_bnet オプションを使用するとき true
  BNetCmd(MagMgr* mgr,
	  bool new_bnet_enable = true);

  /// @brief デストラクタ
  virtual
  ~BNetCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カレントネットワークの取得
  /// @note カレントネットワークが BNetwork でない場合には nullptr を返す．
  BnNetwork*
  cur_network() const;


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

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGBNETCMD_H
