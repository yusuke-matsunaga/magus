
/// @file MapEst.cc
/// @brief MapEst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "MapEst.h"
#include "MapRecord.h"
#include "Cut.h"

#include "SbjNode.h"
#include "SbjDff.h"
#include "SbjLatch.h"
#include "SbjPort.h"

#include "ym/BnNetwork.h"
#include "ym/BnPort.h"
#include "ym/BnDff.h"
#include "ym/TvFunc.h"
#include "ym/Range.h"
#include "ym/BnModifier.h"


#define LUTMAP_DEBUG_MAPGEN 0

// 実装に関する覚書
//
// - ノードの出力の極性について
//
// LUT は入力サイズ以下の関数なら同じコストで実現できるので回路内部の信号線
// を実現する際の極性は任意に変更できる．
//
// なので各LUTは基本的には SbjNode の正極性の論理関数を実現するようにする．
// 例外は外部出力から負極性の論理関数を要求されているときである．
// そのためにNOTゲートの機能を持つLUTを作るのは面積的にも段数的にももったいないので
// はじめから負極性の論理関数を実現するLUTを作る．
//
// すると，そのノードの正極性の論理関数を必要とするノードが存在すると問題となる．
// 単純にはどちらか一方の極性の論理関数のLUTを作っておき，他方の極性はその出力
// にNOTゲートの論理をもつLUTをつければ良いように思われるが，すると段数が1段余計に
// 必要となってしまう．そこで，両方の極性が必要とされているノードがあった場合には
// 同一のファンインを持つ正極性，負極性両方の論理関数のLUTを生成するようにする．
// 無駄のように思われるが，LUT数はNOTゲートを追加する場合と同じである．
// ただし外部入力に関してはどうしようもないので負極性が必要な場合は素直にNOTゲート
// のLUTを作る．
//
// また普通のノードから使われているノードが負極性で実現されている場合はそのまま
// 負極性の出力を使うようにする．そのため NodoInfo::mInvReq というフラグを用意しておく．
// 最初に外部出力から要求されている極性を記録する．
//
//
// - 定数ノードについて
//
// 通常は定数ノードは必要ないが，外部出力が直接定数ノードにつながっている場合に必要となる．
// 同じ値の定数ノードは複数要らないので必要に応じて定数0と定数1のノードを作成する．
//
//
// - 段数について
//
// 上記の話から普通のノードは正極性も負極性も同じ段数で実現できることがわかる．
// ただし，外部入力ノードの場合だけ段数がずれることになる．
// コードを単純にするため極性ごとの段数を持たせるようにする．
//
//
// - 参照回数の計算と極性判定のアルゴリズムについて
//
// マップ結果において複数箇所から参照されているかどうかを調べるため参照回数を計算する．
// 単純には back_trace() の最初で要求されている極性の参照回数を増加させればよいが，
// この参照回数を極性判定に用いるためには外部出力からの参照だけは最初に数えておきたい．
// そこで back_trace() の中のカットの入力ノードに対して back_trace() を再帰呼び出しする
// タイミングで参照回数の更新を行っている．
//
//
// - estimate() のアルゴリズムについて
//
// estimate() は generate() とほぼ同様のことを行っている．


BEGIN_NAMESPACE_LUTMAP

// @brief 作業領域の初期化を行う．
void
MapEst::init(
  SizeType node_num
)
{
  // 作業領域の初期化
  // 具体的な初期化は NodeInfo のコンストラクタが行っている．
  mNodeInfo.clear();
  mNodeInfo.resize(node_num);
  mConst0 = false;
  mConst1 = false;
  mLutNum = 0;

  mFanoutPointList.clear();
}

// @brief マッピング結果から見積もりを行う．
void
MapEst::estimate(
  const SbjGraph& sbjgraph,
  const MapRecord& record,
  SizeType& lut_num,
  SizeType& depth
)
{
#if LUTMAP_DEBUG_MAPEST
  cout << "MapEst::estimate() start" << endl;
#endif

  init(sbjgraph.node_num());

  SizeType ni = sbjgraph.input_num();
  SizeType no = sbjgraph.output_num();

  // 外部出力から要求されている極性を記録する．
  for ( SizeType i = 0; i < no; ++ i ) {
    auto onode = sbjgraph.output(i);
    auto node = onode->output_fanin();
    if ( node ) {
      mNodeInfo[node->id()].inc_ref(onode->output_fanin_inv());
    }
  }

  // 外部入力の生成
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto src_node = sbjgraph.input(i);
    auto& node_info = mNodeInfo[src_node->id()];
    node_info.set_map(0);
    if ( node_info.inv_req() ) {
      ++ mLutNum;

#if LUTMAP_DEBUG_MAPEST
      cout << src_node->id_str() << "[1]" << endl;
#endif
      node_info.set_map(1, true);
    }
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  lut_num = 0;
  SizeType max_depth = 0;
  for ( SizeType i = 0; i < no; ++ i ) {
    auto src_onode = sbjgraph.output(i);
    auto src_node = src_onode->output_fanin();
    bool inv = src_onode->output_fanin_inv();
    if ( src_node ) {
      est_back_trace(src_node, inv, record);
      SizeType depth = mNodeInfo[src_node->id()].depth(inv);
      if ( max_depth < depth ) {
	max_depth = depth;
      }
    }
    else {
      if ( inv ) {
	if ( !mConst1 ) {
	  mConst1 = true;

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPEST
	  cout << "constant-1" << endl;
#endif
	}
      }
      else {
	if ( !mConst0 ) {
	  mConst0 = true;

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPEST
	  cout << "constant-0" << endl;
#endif
	}
      }
    }
  }

  lut_num = mLutNum;
  depth = max_depth;

  // ファンアウトポイントを記録する．
  SizeType nl = sbjgraph.logic_num();
  for ( SizeType i = 0; i < nl; ++ i ) {
    auto src_node = sbjgraph.logic(i);
    auto& node_info = mNodeInfo[src_node->id()];
    if ( node_info.ref_count(false) > 1 || node_info.ref_count(true) > 1 ) {
      mFanoutPointList.push_back(src_node);
    }
  }

#if LUTMAP_DEBUG_MAPEST
  cout << "MapEst::estimate() end" << endl;
#endif
}

// @brief 最終結果を作るためのバックトレースを行う．
void
MapEst::est_back_trace(
  const SbjNode* src_node,
  bool inv,
  const MapRecord& record
)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "back_trace(" << src_node->id_str() << ", " << inv << ")" << endl;
#endif

  auto& node_info = mNodeInfo[src_node->id()];

  if ( node_info.is_mapped(inv) ) {
    return;
  }

  // 外部入力はどちらの極性でも生成済み
  ASSERT_COND( !src_node->is_input() );

  // src_node を根とするカットを取り出す．
  auto cut = record.get_cut(src_node);

  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
  cout << src_node->id_str() << "[" << inv << "]" << endl;
#endif

  // その入力に対応するノードを再帰的に生成する．
  SizeType ni = cut->input_num();
  SizeType depth = 0;
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto src_inode = cut->input(i);
    auto& inode_info = mNodeInfo[src_inode->id()];
    bool iinv = inode_info.inv_req();

    inode_info.inc_ref(iinv);
    est_back_trace(src_inode, iinv, record);

    SizeType depth1 = inode_info.depth(iinv);
    if ( depth < depth1 ) {
      depth = depth1;
    }
  }

  node_info.set_map(depth, inv);
}

END_NAMESPACE_LUTMAP
