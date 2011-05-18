#ifndef LIBYM_MVNBDNCONV_MVNCONV_H
#define LIBYM_MVNBDNCONV_MVNCONV_H

/// @file libym_networks/MvnConv.h
/// @brief MvnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvnbdnconv_nsdef.h"
#include "ym_networks/mvn_nsdef.h"
#include "ym_networks/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class MvnConv MvnConv.h "MvnConv.h"
/// @brief MvnNode を SbjGraph に変換するクラス
//////////////////////////////////////////////////////////////////////
class MvnConv
{
public:

  /// @brief デストラクタ
  virtual
  ~MvnConv() { };


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnNode を BdnMgr に変換する．
  /// @param[in] node ノード
  /// @param[in] bdnetwork 変換結果の BdnMgr
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvnNode* node,
	     BdnMgr& bdnetwork,
	     MvnBdnMap& nodemap) = 0;

};

END_NAMESPACE_YM_MVNBDNCONV

#endif // LIBYM_MVNBDNCONV_MVNCONV_H
