#ifndef NETWORKS_CMN_H
#define NETWORKS_CMN_H

/// @file YmNetworks/cmn.h
/// @brief Cell Mapped Networks の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

/// @defgroup CmnGroup Cell-Mapped Network パッケージ
///
/// ネットワーク全体として以下の情報(データ)を持つ．
///
/// - CmnPort: 外部入出力ポート
///   ネットワークの外部インターフェイスを表すオブジェクト．
///   各々が ID を持つ．ID は 0 から始まる連続した整数．
///   ポートは名前を持つことができる(名無しもポートもある)．
///   ポートはビット幅を持つ．
///   ポートは一度生成されたら削除されない．
///
/// - CmnDff: D-FF
///   ネットワークの内部状態を表すオブジェクト．
///   常に 1 ビット．データ入力，クロック入力，データ出力を持つ．
///   非同期セット信号と非同期リセット信号をオプショナルに持つ．
///   各々が ID を持つ．ID は 0 以上の整数．
///   D-FF は名前を持つことができる(名無しも D-FF もある)．
///   D-FF は削除されることがある．
///
/// - CmnLatch: ラッチ
///   常に 1 ビット．データ入力，ラッチイネーブル信号，データ出力を持つ．
///   各々が ID を持つ．ID は 0 以上の整数．
///   ラッチは名前を持つことができる(名無しもラッチもある)．
///   ラッチは削除されることがある．
///
/// - CmnNode: 入力ノード
///   組み合わせ回路部分の入力を表すノード．
///   常に1ビット．
///   外部入力ポート，D-FF やラッチのデータ出力に関連付けられる．
///
/// - CmnNode: 出力ノード
///   組み合わせ回路部分の出力を表すノード．
///   常に1ビット．
///   外部出力ポート，D-FF やラッチのデータ入力などに関連付けられる．
///   ファンインの極性の情報を持つ．
///
/// - CmnNode: 論理ノード
///   対応するセルを持つ．
///   ファンアウトの枝のリストも持つ．
///
/// すべてのノードはノードIDを持つ．ノードIDは 0 から始まる整数だが，ノードが
/// 削除された場合には歯抜けの状態になる．


#include "YmNetworks/networks_nsdef.h"
#include "YmUtils/DlList.h"


/// @brief cmn 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_NETWORKS_CMN		\
  BEGIN_NAMESPACE_YM_NETWORKS			\
  BEGIN_NAMESPACE(nsCmn)

/// @brief cmn 用の名前空間の終了
#define END_NAMESPACE_YM_NETWORKS_CMN		\
  END_NAMESPACE(nsCmn)				\
  END_NAMESPACE_YM_NETWORKS


BEGIN_NAMESPACE_YM_NETWORKS_CMN

class CmnMgr;

class CmnMgrImpl;

class CmnPort;

class CmnDff;

class CmnLatch;

class CmnNode;

class CmnEdge;

class CmnDffCell;

class CmnLatchCell;

class CmnDumper;
class CmnBlifWriter;
class CmnVerilogWriter;

/// @brief 枝のリスト
/// @ingroup CmnGroup
typedef list<CmnEdge*> CmnEdgeList;

/// @brief ファンアウトリスト
/// @ingroup CmnGroup
typedef DlList<CmnEdge> CmnFanoutList;

/// @brief 節点のベクタ
/// @ingroup CmnGroup
typedef vector<CmnNode*> CmnNodeVector;

/// @brief 節点のリスト
/// @ingroup CmnGroup
typedef DlList<CmnNode> CmnNodeList;

/// @brief DFF のリスト
/// @ingroup CmnGroup
typedef DlList<CmnDff> CmnDffList;

/// @brief ラッチのリスト
/// @ingroup CmnGroup
typedef DlList<CmnLatch> CmnLatchList;

END_NAMESPACE_YM_NETWORKS_CMN

BEGIN_NAMESPACE_YM

using nsNetworks::nsCmn::CmnMgr;

using nsNetworks::nsCmn::CmnPort;

using nsNetworks::nsCmn::CmnDff;
using nsNetworks::nsCmn::CmnDffList;

using nsNetworks::nsCmn::CmnLatch;
using nsNetworks::nsCmn::CmnLatchList;

using nsNetworks::nsCmn::CmnNode;
using nsNetworks::nsCmn::CmnNodeVector;
using nsNetworks::nsCmn::CmnNodeList;

using nsNetworks::nsCmn::CmnEdge;
using nsNetworks::nsCmn::CmnEdgeList;

using nsNetworks::nsCmn::CmnFanoutList;

using nsNetworks::nsCmn::CmnDffCell;
using nsNetworks::nsCmn::CmnLatchCell;

using nsNetworks::nsCmn::CmnDumper;
using nsNetworks::nsCmn::CmnBlifWriter;
using nsNetworks::nsCmn::CmnVerilogWriter;

END_NAMESPACE_YM

#endif // NETWORKS_CMN_H
