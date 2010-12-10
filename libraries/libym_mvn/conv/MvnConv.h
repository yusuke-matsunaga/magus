#ifndef LIBYM_MVN_CONV_MVNCONV_H
#define LIBYM_MVN_CONV_MVNCONV_H

/// @file libym_mvn/conv/MvnConv.h
/// @brief MvnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"
#include "ym_sbj/sbj_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

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
	     MvNodeMap& nodemap) = 0;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_CONV_MVNCONV_H
