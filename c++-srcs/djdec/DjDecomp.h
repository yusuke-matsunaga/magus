#ifndef DJDECOMP_H
#define DJDECOMP_H

/// @file DjDecomp.h
/// @brief DjDecomp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "dg.h"
#include "ym/BnModifier.h"


BEGIN_NAMESPACE_DG

class DgEdge;

//////////////////////////////////////////////////////////////////////
/// @class DjDecomp DjDecomp.h "DjDecomp.h"
/// @brief disjoint decomposition を行うクラス
//////////////////////////////////////////////////////////////////////
class DjDecomp :
  public BnModifier
{
public:

  /// @brief コンストラクタ
  DjDecomp() = default;

  /// @brief デストラクタ
  ~DjDecomp() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD タイプのノードを disjoint decomposition する．
  void
  decomp(
    const BnNetwork& src_network ///< [in] 元のネットワーク
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief edge に対応するネットワークを作る．
  SizeType
  make_network(
    DgEdge edge ///< [in] 分解グラフの枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力のノード番号のリスト
  vector<SizeType> mInputList;

};

END_NAMESPACE_DG

#endif // DJDECOMP_H