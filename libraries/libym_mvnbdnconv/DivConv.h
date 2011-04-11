#ifndef LIBYM_MVNBDNCONV_DIVCONV_H
#define LIBYM_MVNBDNCONV_DIVCONV_H

/// @file libym_mvnbdnconv/DivConv.h
/// @brief DivConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

//////////////////////////////////////////////////////////////////////
/// @class DivConv DivConv.h "DivConv.h"
/// @brief 除算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class DivConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  DivConv();

  /// @brief デストラクタ
  virtual
  ~DivConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
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
	     MvnBdnMap& nodemap);

};


END_NAMESPACE_YM_MVNBDNCONV

#endif // LIBYM_MVNBDNCONV_DIVCONV_H
