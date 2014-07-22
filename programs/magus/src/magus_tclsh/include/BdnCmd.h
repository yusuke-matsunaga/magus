#ifndef MAGUS_BDNCMD_H
#define MAGUS_BDNCMD_H

/// @file BdnCmd.h
/// @brief BdnCmd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MagCmd.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "NetCmd.h"
#include "networks/bdn.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class BdnCmd BdnCmd.h "BdnCmd.h"
/// @ingroup MagusGroup
/// @brief BdnMgr を操作対象とする Magus のコマンドオブジェクトの基底クラス
//////////////////////////////////////////////////////////////////////
class BdnCmd :
  public NetCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  /// @param[in] new_bdn_enable -new_bnet オプションを使用するとき true
  BdnCmd(MagMgr* mgr,
	 bool new_bdn_enable = true);

  /// @brief デストラクタ
  virtual
  ~BdnCmd();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief カレントネットワークの取得
  /// @note カレントネットワークが BNetwork でない場合には NULL を返す．
  BdnMgr*
  cur_network() const;


protected:
  //////////////////////////////////////////////////////////////////////
  // TclCmd の仮想関数のオーバーロード
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンド行の引数をパーズする関数．
  /// @param[in] objv 引数の配列
  virtual
  int
  before_cmd_proc(TclObjVector& objv);

  /// @brief コマンド処理関数の後で実行される関数
  virtual
  void
  after_cmd_proc();

};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAGBDNCMD_H
