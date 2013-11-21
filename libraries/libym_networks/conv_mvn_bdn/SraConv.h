#ifndef SRACONV_H
#define SRACONV_H

/// @file SraConv.h
/// @brief SraConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_NETWORKSBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class SraConv SraConv.h "SraConv.h"
/// @brief SRA 演算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class SraConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  SraConv();

  /// @brief デストラクタ
  virtual
  ~SraConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnNode を BdnMgr に変換する．
  /// @param[in] node ノード
  /// @param[in] bdnetwork 変換結果のサブジェクトグラフ
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvnNode* node,
	     BdnMgr& bdnetwork,
	     MvnBdnMap& nodemap);

};


END_NAMESPACE_YM_NETWORKSBDNCONV

#endif // SRACONV_H
