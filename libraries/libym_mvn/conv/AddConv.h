#ifndef LIBYM_MVN_CONV_ADDCONV_H
#define LIBYM_MVN_CONV_ADDCONV_H

/// @file libym_mvn/conv/AddConv.h
/// @brief AddConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class AddConv AddConv.h "AddConv.h"
/// @brief 加算ノードに対する処理を行う MvnConv の派生クラス
//////////////////////////////////////////////////////////////////////
class AddConv :
  public MvnConv
{
public:

  /// @brief コンストラクタ
  AddConv();

  /// @brief デストラクタ
  virtual
  ~AddConv();


public:
  //////////////////////////////////////////////////////////////////////
  // MvnConv の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvNode を SbjGraph に変換する．
  /// @param[in] node ノード
  /// @param[in] sbjgraph 変換結果のサブジェクトグラフ
  /// @param[in] nodemap ノードの対応関係を表すマップ
  /// @retval true このクラスで変換処理を行った．
  /// @retval false このクラスでは変換処理を行わなかった．
  virtual
  bool
  operator()(const MvNode* node,
	     SbjGraph& sbjgraph,
	     MvNodeMap& nodemap);

};


END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_CONV_ADDCONV_H
