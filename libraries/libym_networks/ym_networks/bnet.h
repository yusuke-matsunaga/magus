#ifndef YM_NETWORKS_BNET_H
#define YM_NETWORKS_BNET_H

/// @file ym_networks/bnet.h
/// @brief BNET の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bnet.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup BnetGroup Boolean Network パッケージ
///
/// Boolean Network を扱うためのクラス群
/// UCB の mis/sis と大きく異なるのはノードを論理関数を常に論理式
/// で表しているということ


#include "ym_networks/networks_nsdef.h"
#include "ym_utils/DlList.h"


// エラー処理が必要な箇所を示すための人畜無害なマーカー
#if 0
#define BNET_ERROR(msg)
#else
#define BNET_ERROR(msg) cerr << msg << endl
#endif


BEGIN_NAMESPACE_YM_NETWORKS

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

class BNet2Sbj;


/// @brief 枝のリスト
/// @ingroup BnetGroup
typedef list<BNodeEdge*> BNodeEdgeList;

/// @brief ファンアウトリスト
/// @ingroup BnetGroup
typedef DlList<BNodeEdge> BNodeFoList;

/// @brief 節点のベクタ
/// @ingroup BnetGroup
typedef vector<BNode*> BNodeVector;

END_NAMESPACE_YM_NETWORKS

BEGIN_NAMESPACE_YM

using nsNetworks::BNetwork;
using nsNetworks::BNetManip;
using nsNetworks::BNetDecomp;
using nsNetworks::BNetGateDecomp;
using nsNetworks::BNetSopDecomp;

using nsNetworks::BNode;
using nsNetworks::BNodeVector;
using nsNetworks::BNodeList;

using nsNetworks::BNodeEdge;
using nsNetworks::BNodeEdgeList;

using nsNetworks::BNodeFoList;

using nsNetworks::BNetChg;
using nsNetworks::BNetworkTrace;

using nsNetworks::BNetBlifReader;
using nsNetworks::BNetIscas89Reader;

using nsNetworks::BNetBlifWriter;
using nsNetworks::BNetEquWriter;
using nsNetworks::BNetVerilogWriter;

using nsNetworks::BNet2Sbj;

using nsNetworks::BNodeEdgeList;
using nsNetworks::BNodeFoList;
using nsNetworks::BNodeVector;

END_NAMESPACE_YM

#endif // YM_NETWORKS_BNET_H
