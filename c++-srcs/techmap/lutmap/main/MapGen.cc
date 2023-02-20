
/// @file MapGen.cc
/// @brief MapGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "MapGen.h"
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
// estimate() は generate() とほぼ同様のことを行っている．ただし，実際にノード生成を
// 行うわけではないので NodeInfo::set_map() で設定するノード番号はすべて 1 となっている．


BEGIN_NAMESPACE_LUTMAP

// @brief 作業領域の初期化を行う．
void
MapGen::init(
  SizeType node_num
)
{
  // 作業領域の初期化
  // 具体的な初期化は NodeInfo のコンストラクタが行っている．
  mNodeInfo.clear();
  mNodeInfo.resize(node_num);
  mConst0 = {}; // 不正値
  mConst1 = {}; // 不正値
  mLutNum = 0;
}

// @brief マッピング結果を BnNetwork にセットする．
BnNetwork
MapGen::generate(
  const SbjGraph& sbjgraph,
  const MapRecord& record,
  SizeType& lut_num,
  SizeType& depth
)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() start" << endl;
#endif

  BnModifier mapgraph;

  mapgraph.clear();
  mapgraph.set_name(sbjgraph.name());

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

  // ポートの生成
  SizeType np = sbjgraph.port_num();
  for ( SizeType i = 0; i < np; ++ i ) {
    auto sbjport = sbjgraph.port(i);
    SizeType nb = sbjport->bit_width();
    vector<BnDir> dir_vect(nb);
    for ( SizeType j = 0; j < nb; ++ j ) {
      auto sbjnode = sbjport->bit(j);
      dir_vect[j] = sbjnode->is_input() ? BnDir::INPUT : BnDir::OUTPUT;
    }
    auto bn_port = mapgraph.new_port(sbjport->name(), dir_vect);
    for ( SizeType j = 0; j < nb; ++ j ) {
      auto sbjnode = sbjport->bit(j);
      mNodeInfo[sbjnode->id()].set_map(bn_port.bit(j), 0);
    }
  }

  // DFFの生成
  SizeType nf = sbjgraph.dff_num();
  for ( SizeType i = 0; i < nf; ++ i ) {
    auto dff = sbjgraph.dff(i);
    auto input = dff->data_input();
    auto output = dff->data_output();
    auto clock = dff->clock();
    auto clear = dff->clear();
    auto preset = dff->preset();

    bool has_clear = (clear != nullptr);
    bool has_preset = (preset != nullptr);

    auto bn_dff = mapgraph.new_dff({}, has_clear, has_preset);

    mNodeInfo[input->id()].set_map(bn_dff.data_in(), 0);
    mNodeInfo[output->id()].set_map(bn_dff.data_out(), 0);
    mNodeInfo[clock->id()].set_map(bn_dff.clock(), 0);
    if ( has_clear ) {
      mNodeInfo[clear->id()].set_map(bn_dff.clear(), 0);
    }
    if ( has_preset ) {
      mNodeInfo[preset->id()].set_map(bn_dff.preset(), 0);
    }
  }

  // ラッチの生成
  SizeType nlatch = sbjgraph.latch_num();
  for ( SizeType i = 0; i < nlatch; ++ i ) {
    auto latch = sbjgraph.latch(i);
    auto input = latch->data_input();
    auto output = latch->data_output();
    auto enable = latch->enable();
    auto clear = latch->clear();
    auto preset = latch->preset();

    bool has_clear = (clear != nullptr);
    bool has_preset = (preset != nullptr);

    auto bn_latch = mapgraph.new_latch({}, has_clear, has_preset);

    mNodeInfo[input->id()].set_map(bn_latch.data_in(), 0);
    mNodeInfo[output->id()].set_map(bn_latch.data_out(), 0);
    mNodeInfo[enable->id()].set_map(bn_latch.clock(), 0);
    if ( has_clear ) {
      mNodeInfo[clear->id()].set_map(bn_latch.clear(), 0);
    }
    if ( has_preset ) {
      mNodeInfo[preset->id()].set_map(bn_latch.preset(), 0);
    }
  }

  // 外部入力の生成
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto src_node = sbjgraph.input(i);
    auto& node_info = mNodeInfo[src_node->id()];
    if ( node_info.inv_req() ) {
      // NOT ゲートを表す LUT を作る．
      auto tv = TvFunc::make_nega_literal(1, 0);
      auto node = node_info.map_node();
      auto inv = mapgraph.new_logic_tv({}, tv, {node});

      ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
      cout << node->id_str() << "[1]" << endl;
#endif

      node_info.set_map(inv, 1, true);
    }
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  SizeType max_depth = 0;
  for ( SizeType i = 0; i < no; ++ i ) {
    auto src_onode = sbjgraph.output(i);
    auto src_node = src_onode->output_fanin();
    bool inv = src_onode->output_fanin_inv();
    BnNode dst_node;
    SizeType depth;
    if ( src_node ) {
      dst_node = gen_back_trace(src_node, inv, record, mapgraph);
      depth = mNodeInfo[src_node->id()].depth(inv);
      if ( max_depth < depth ) {
	max_depth = depth;
      }
    }
    else {
      // 定数の場合
      if ( inv ) {
	if ( mConst1.is_invalid() ) {
	  auto tv = TvFunc::make_one(0);
	  mConst1 = mapgraph.new_logic_tv({}, tv, {});

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
	  cout << src_onode->id_str() << endl;
#endif
	}
	dst_node = mConst1;
      }
      else {
	if ( mConst0.is_invalid() ) {
	  auto tv = TvFunc::make_zero(0);
	  mConst0 = mapgraph.new_logic_tv({}, tv, {});

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
	  cout << src_onode->id_str() << endl;
#endif
	}
	dst_node = mConst0;
      }
      depth = 0;
    }
    auto dst_onode = mNodeInfo[src_onode->id()].map_node();
    mapgraph.set_output_src(dst_onode, dst_node);
    // depth の設定のため
    mNodeInfo[src_onode->id()].set_map(dst_onode, depth);
  }

  lut_num = mLutNum;
  depth = max_depth;

#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() end" << endl;
#endif

  return BnNetwork{std::move(mapgraph)};
}

// @brief 最終結果を作るためのバックトレースを行う．
BnNode
MapGen::gen_back_trace(
  const SbjNode* src_node,
  bool output_inv,
  const MapRecord& record,
  BnModifier& mapnetwork
)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "back_trace(" << src_node->id_str() << ", " << output_inv << ")" << endl;
#endif
  auto& node_info = mNodeInfo[src_node->id()];

  auto dst_node = node_info.map_node(output_inv);
  if ( dst_node.is_valid() ) {
    // すでに生成済みならそのノードを返す．
    return dst_node;
  }

  // 外部入力はどちらの極性でも生成済み
  ASSERT_COND( !src_node->is_input() );

  // node を根とするカットを取り出す．
  auto cut = record.get_cut(src_node);
  if ( cut == nullptr ) {
    cout << "Error!: " << src_node->id_str()
	 << " has no cuts" << endl;
    ASSERT_NOT_REACHED;
  }

  // その入力に対応するノードを再帰的に生成する．
  SizeType ni = cut->input_num();
  SizeType depth = 0;
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto inode = cut->input(i);
    auto& inode_info = mNodeInfo[inode->id()];
    bool iinv = inode_info.inv_req();

    inode_info.inc_ref(iinv);
    gen_back_trace(inode, iinv, record, mapnetwork);

    // 段数の計算を行う．
    SizeType idepth = inode_info.depth(iinv);
    if ( depth < idepth ) {
      depth = idepth;
    }
  }
  ++ depth;

  // ファンインの極性とファンインのノードのリストを作る．
  // 上のループで input_inv の設定を行うこともできるがそうすると
  // gen_back_trace() の再帰のたびにスタック上に input_inv が作られる
  // ことになるのであえてここで行う．
  vector<bool> input_inv(ni, false);
  vector<BnNode> fanin_list(ni);
  for ( SizeType i = 0; i < ni; ++ i ) {
    auto src_inode = cut->input(i);
    auto& inode_info = mNodeInfo[src_inode->id()];
    bool iinv = inode_info.inv_req();
    input_inv[i] = iinv;
    fanin_list[i] = inode_info.map_node(iinv);
  }

  // カットの実現している関数の真理値表を得る．
  auto tv = cut->make_tv(output_inv, input_inv);

  // 新しいノードを作る．
  dst_node = mapnetwork.new_logic_tv({}, tv, fanin_list);

  ++ mLutNum;

  // マップ結果をセットする．
  node_info.set_map(dst_node, depth, output_inv);

#if LUTMAP_DEBUG_MAPGEN
  cout << src_node->id_str() << " => " << dst_node.id() << ": " << depth << endl;
#endif

  return dst_node;
}

END_NAMESPACE_LUTMAP
