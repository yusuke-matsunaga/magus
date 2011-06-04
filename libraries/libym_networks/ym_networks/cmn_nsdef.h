#ifndef YM_NETWORKS_CMN_NSDEF_H
#define YM_NETWORKS_CMN_NSDEF_H

/// @file ym_networks/cmn_nsdef.h
/// @brief Cell Mapped Networks の共通ヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: bdn_nsdef.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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


#include "ymtools.h"
#include "ym_utils/DlList.h"


/// @brief CMN 用の名前空間の開始
#define BEGIN_NAMESPACE_YM_CMN \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsCmn)

/// @brief CMN 用の名前空間の終了
#define END_NAMESPACE_YM_CMN \
END_NAMESPACE(nsCmn) \
END_NAMESPACE_YM

/// @namespace nsYm::nsCmn
/// @brief CMN 関係のクラスが属する名前空間
///
/// ただし，主要なものは nsYm にインポートされている．
/// 実装用のローカルな要素だけがこの名前空間に存在する．

BEGIN_NAMESPACE_YM_CMN

class CmnMgr;

class CmnMgrImpl;

class CmnPort;

class CmnDff;

class CmnLatch;

class CmnNode;

class CmnEdge;

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

END_NAMESPACE_YM_CMN

BEGIN_NAMESPACE_YM

using nsCmn::CmnMgr;

using nsCmn::CmnPort;

using nsCmn::CmnDff;
using nsCmn::CmnDffList;

using nsCmn::CmnLatch;
using nsCmn::CmnLatchList;

using nsCmn::CmnNode;
using nsCmn::CmnNodeVector;
using nsCmn::CmnNodeList;

using nsCmn::CmnEdge;
using nsCmn::CmnEdgeList;

using nsCmn::CmnFanoutList;

using nsCmn::CmnDumper;
using nsCmn::CmnBlifWriter;
using nsCmn::CmnVerilogWriter;

END_NAMESPACE_YM

#endif // YM_NETWORKS_CMN_NSDEF_H
