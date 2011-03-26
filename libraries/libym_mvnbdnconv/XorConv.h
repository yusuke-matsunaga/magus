#ifndef LIBYM_MVNBDNCONV_XORCONV_H
#define LIBYM_MVNBDNCONV_XORCONV_H

/// @file libym_mvnbdnconv/XorConv.h
/// @brief XorConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class XorConv XorConv.h "XorConv.h"
/// @brief 論理演算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class XorConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  XorConv();

  /// @brief デストラクタ
  virtual
  ~XorConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
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
	     MvnBdnMap& nodemap);

};


END_NAMESPACE_YM_MVNBDNCONV

#endif // LIBYM_MVNBDNCONV_XORCONV_H
