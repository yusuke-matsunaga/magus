#ifndef TECHMAP_TECHMAPCMD_H
#define TECHMAP_TECHMAPCMD_H

/// @file TechmapCmd.h
/// @brief TechmapCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"
#include "ym_networks/CmnMgr.h"


/// @brief techmap 用の名前空間の開始
#define BEGIN_NAMESPACE_MAGUS_TECHMAP \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsTechmap)

/// @brief techmap 用の名前空間の終了
#define END_NAMESPACE_MAGUS_TECHMAP \
END_NAMESPACE(nsTechmap) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class TechmapCmd TechmapCmd.h "TechmapCmd.h"
/// @brief テクノロジマッピングを行うコマンドの基底クラス
//////////////////////////////////////////////////////////////////////
class TechmapCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  TechmapCmd(MagMgr* mgr,
	     CmnMgr& cmnmgr);

  /// @brief デストラクタ
  virtual
  ~TechmapCmd();


protected:

  /// @brief セルネットワークを得る．
  CmnMgr&
  cmnmgr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルネットワーク
  CmnMgr& mCmnMgr;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_TECHMAPCMD_H
