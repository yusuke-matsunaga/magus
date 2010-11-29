#ifndef LIBYM_MVN_CONV_DIVCONV_H
#define LIBYM_MVN_CONV_DIVCONV_H

/// @file libym_mvn/conv/DivConv.h
/// @brief DivConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnConv.h"


BEGIN_NAMESPACE_YM_MVN

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

#endif // LIBYM_MVN_CONV_DIVCONV_H
