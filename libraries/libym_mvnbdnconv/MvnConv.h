#ifndef LIBYM_MVNBDNCONV_MVNCONV_H
#define LIBYM_MVNBDNCONV_MVNCONV_H

/// @file libym_mvnbdnconv/MvnConv.h
/// @brief MvnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvnbdnconv/mvnbdnconv_nsdef.h"
#include "ym_mvn/mvn_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class MvnConv MvnConv.h "MvnConv.h"
/// @brief MvNode を SbjGraph に変換するクラス
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

  /// @brief MvNode を BdNetwork に変換する．
  /// @param[in] node ノード
  /// @param[in] bdnetwork 変換結果の BdNetwork
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvNode* node,
	     BdNetwork& bdnetwork,
	     MvnBdnMap& nodemap) = 0;

};

END_NAMESPACE_YM_MVNBDNCONV

#endif // LIBYM_MVNBDNCONV_MVNCONV_H
