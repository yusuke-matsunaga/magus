#ifndef MODBASE_H
#define MODBASE_H

/// @file include/ModBase.h
/// @brief ModBase のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ModBase.h 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "ym_networks/tgnet.h"


BEGIN_NAMESPACE_YM_SATPG

class SaFault;
class AtpgMgr;
class TvMgr;

//////////////////////////////////////////////////////////////////////
/// @class ModBase ModBase.h "ModBase.h"
/// @brief テストパタン生成に関する処理を行うモジュールの基底クラス
//////////////////////////////////////////////////////////////////////
class ModBase
{
  friend class AtpgMgr;

public:

  /// @brief デストラクタ
  virtual
  ~ModBase() { }


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが用意する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットした後に呼ばれる関数
  virtual
  void
  after_set_network(const TgNetwork& network,
		    TvMgr& tvmgr) = 0;

  /// @brief 故障リストの内容が変わった後に呼ばれる関数
  virtual
  void
  after_update_faults(const vector<SaFault*>& flist) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // MODBASE_H
