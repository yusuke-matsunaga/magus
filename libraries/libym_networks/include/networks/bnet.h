#ifndef NETWORKS_BNET_H
#define NETWORKS_BNET_H

/// @file networks/bnet.h
/// @brief BNET の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BnetGroup Boolean Network パッケージ
///
/// Boolean Network を扱うためのクラス群
/// UCB の mis/sis と大きく異なるのはノードを論理関数を常に論理式
/// で表しているということ


#include "networks/networks_nsdef.h"
#include "utils/DlList.h"


/// @brief bnet 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_BNET	\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsBnet)

/// @brief bnet 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_BNET		\
  END_NAMESPACE(nsBnet)				\
  END_NAMESPACE_YM_NETWORKS


// エラー処理が必要な箇所を示すための人畜無害なマーカー
#if 0
#define BNET_ERROR(msg)
#else
#define BNET_ERROR(msg) cerr << msg << endl
#endif


BEGIN_NAMESPACE_YM_NETWORKS_BNET

class BNetwork;
class BNetManip;
class BNetDecomp;
class BNetGateDecomp;
class BNetSopDecomp;

class BNode;
class BNodeEdge;

class BNetChg;
class BNetworkTrace;

class BNodeList;

class BNetBlifReader;
class BNetIscas89Reader;

class BNetBlifWriter;
class BNetEquWriter;
class BNetVerilogWriter;


/// @brief 枝のリスト
/// @ingroup BnetGroup
typedef list<BNodeEdge*> BNodeEdgeList;

/// @brief ファンアウトリスト
/// @ingroup BnetGroup
typedef DlList<BNodeEdge> BNodeFoList;

/// @brief 節点のベクタ
/// @ingroup BnetGroup
typedef vector<BNode*> BNodeVector;

END_NAMESPACE_YM_NETWORKS_BNET

BEGIN_NAMESPACE_YM

using nsNetworks::nsBnet::BNetwork;
using nsNetworks::nsBnet::BNetManip;
using nsNetworks::nsBnet::BNetDecomp;
using nsNetworks::nsBnet::BNetGateDecomp;
using nsNetworks::nsBnet::BNetSopDecomp;

using nsNetworks::nsBnet::BNode;
using nsNetworks::nsBnet::BNodeVector;
using nsNetworks::nsBnet::BNodeList;

using nsNetworks::nsBnet::BNodeEdge;
using nsNetworks::nsBnet::BNodeEdgeList;

using nsNetworks::nsBnet::BNodeFoList;

using nsNetworks::nsBnet::BNetChg;
using nsNetworks::nsBnet::BNetworkTrace;

using nsNetworks::nsBnet::BNetBlifReader;
using nsNetworks::nsBnet::BNetIscas89Reader;

using nsNetworks::nsBnet::BNetBlifWriter;
using nsNetworks::nsBnet::BNetEquWriter;
using nsNetworks::nsBnet::BNetVerilogWriter;

using nsNetworks::nsBnet::BNodeEdgeList;
using nsNetworks::nsBnet::BNodeFoList;
using nsNetworks::nsBnet::BNodeVector;

END_NAMESPACE_YM

#endif // NETWORKS_BNET_H
