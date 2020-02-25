
/// @file MapGen.cc
/// @brief MapGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
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
#include "ym/BnLatch.h"
#include "ym/TvFunc.h"
#include "ym/Range.h"

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

// コンストラクタ
MapGen::MapGen()
{
}

// デストラクタ
MapGen::~MapGen()
{
}

// @brief 作業領域の初期化を行う．
// @param[in] node_num ノード数
void
MapGen::init(int node_num)
{
  // 作業領域の初期化
  // 具体的な初期化は NodeInfo のコンストラクタが行っている．
  mNodeInfo.clear();
  mNodeInfo.resize(node_num);
  mConst0 = kBnNullId;
  mConst1 = kBnNullId;
  mLutNum = 0;

  mFanoutPointList.clear();
}

// @brief マッピング結果を BnNetwork にセットする．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] mapgraph マッピング結果を格納するネットワーク
// @param[out] lut_num LUT数
// @param[out] depth 最大段数
void
MapGen::generate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 BnNetwork& mapgraph,
		 int& lut_num,
		 int& depth)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() start" << endl;
#endif

  mapgraph.clear();
  mapgraph.set_name(sbjgraph.name());

  init(sbjgraph.node_num());

  int ni = sbjgraph.input_num();
  int no = sbjgraph.output_num();

  // 外部出力から要求されている極性を記録する．
  for ( int i = 0; i < no; ++ i ) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    if ( node ) {
      mNodeInfo[node->id()].inc_ref(onode->output_fanin_inv());
    }
  }

  // ポートの生成
  int np = sbjgraph.port_num();
  for ( int i = 0; i < np; ++ i ) {
    const SbjPort* sbjport = sbjgraph.port(i);
    int nb = sbjport->bit_width();
    vector<int> dir_vect(nb);
    for ( int j = 0; j < nb; ++ j ) {
      const SbjNode* sbjnode = sbjport->bit(j);
      dir_vect[j] = sbjnode->is_input() ? 0 : 1;
    }
    int port_id = mapgraph.new_port(sbjport->name(), dir_vect);
    auto& bn_port = mapgraph.port(port_id);
    for ( int j = 0; j < nb; ++ j ) {
      const SbjNode* sbjnode = sbjport->bit(j);
      mNodeInfo[sbjnode->id()].set_map(bn_port.bit(j), 0);
    }
  }

  // DFFの生成
  int nf = sbjgraph.dff_num();
  for ( int i = 0; i < nf; ++ i ) {
    const SbjDff* dff = sbjgraph.dff(i);
    const SbjNode* input = dff->data_input();
    const SbjNode* output = dff->data_output();
    const SbjNode* clock = dff->clock();
    const SbjNode* clear = dff->clear();
    const SbjNode* preset = dff->preset();

    bool has_clear = (clear != nullptr);
    bool has_preset = (preset != nullptr);

    int dff_id = mapgraph.new_dff(string(), has_clear, has_preset);
    auto& bn_dff = mapgraph.dff(dff_id);

    mNodeInfo[input->id()].set_map(bn_dff.input(), 0);
    mNodeInfo[output->id()].set_map(bn_dff.output(), 0);
    mNodeInfo[clock->id()].set_map(bn_dff.clock(), 0);
    if ( has_clear ) {
      mNodeInfo[clear->id()].set_map(bn_dff.clear(), 0);
    }
    if ( has_preset ) {
      mNodeInfo[preset->id()].set_map(bn_dff.preset(), 0);
    }
  }

  // ラッチの生成
  int nlatch = sbjgraph.latch_num();
  for ( int i = 0; i < nlatch; ++ i ) {
    const SbjLatch* latch = sbjgraph.latch(i);
    const SbjNode* input = latch->data_input();
    const SbjNode* output = latch->data_output();
    const SbjNode* enable = latch->enable();
    const SbjNode* clear = latch->clear();
    const SbjNode* preset = latch->preset();

    bool has_clear = (clear != nullptr);
    bool has_preset = (preset != nullptr);

    int latch_id = mapgraph.new_latch(string(), has_clear, has_preset);
    auto& bn_latch = mapgraph.latch(latch_id);

    mNodeInfo[input->id()].set_map(bn_latch.input(), 0);
    mNodeInfo[output->id()].set_map(bn_latch.output(), 0);
    mNodeInfo[enable->id()].set_map(bn_latch.enable(), 0);
    if ( has_clear ) {
      mNodeInfo[clear->id()].set_map(bn_latch.clear(), 0);
    }
    if ( has_preset ) {
      mNodeInfo[preset->id()].set_map(bn_latch.preset(), 0);
    }
  }

  // 外部入力の生成
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& node_info = mNodeInfo[node->id()];
    int node_id = node_info.map_node();
    if ( node_info.inv_req() ) {
      // NOT ゲートを表す LUT を作る．
      TvFunc tv = TvFunc::make_nega_literal(1, VarId(0));
      int inv_id = mapgraph.new_logic(string(), tv);
      mapgraph.connect(node_id, inv_id, 0);

      ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
      cout << node->id_str() << "[1]" << endl;
#endif

      node_info.set_map(inv_id, 1, true);
    }
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  int max_depth = 0;
  for ( int i = 0; i < no; ++ i ) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    int node_id;
    int depth;
    if ( node ) {
      node_id = gen_back_trace(node, inv, record, mapgraph);
      depth = mNodeInfo[node->id()].depth(inv);
      if ( max_depth < depth ) {
	max_depth = depth;
      }
    }
    else {
      // 定数の場合
      if ( inv ) {
	if ( mConst1 == kBnNullId ) {
	  TvFunc tv = TvFunc::make_one(0);
	  mConst1 = mapgraph.new_logic(string(), tv);
	  mConst1 = node_id;

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
	  cout << onode->id_str() << endl;
#endif
	}
	node_id = mConst1;
      }
      else {
	if ( mConst0 == kBnNullId ) {
	  TvFunc tv = TvFunc::make_zero(0);
	  mConst0 = mapgraph.new_logic(string(), tv);

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
	  cout << onode->id_str() << endl;
#endif
	}
	node_id = mConst0;
      }
      depth = 0;
    }
    int onode_id = mNodeInfo[onode->id()].map_node();
    mapgraph.connect(node_id, onode_id, 0);
    // depth の設定のため
    mNodeInfo[onode->id()].set_map(onode_id, depth);
  }

  bool stat = mapgraph.wrap_up();
  ASSERT_COND( stat );

  lut_num = mLutNum;
  depth = max_depth;

  // ファンアウトポイントを記録する．
  int nl = sbjgraph.logic_num();
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(i);
    const NodeInfo& node_info = mNodeInfo[node->id()];
    if ( node_info.ref_count(false) > 1 || node_info.ref_count(true) > 1 ) {
      mFanoutPointList.push_back(node);
    }
  }

#if LUTMAP_DEBUG_MAPGEN
  cout << "MapGen::generate() end" << endl;
#endif
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
// @param[out] mapnetwork マッピング結果のネットワーク
// @return (node, inv) を実現するノード番号を返す．
int
MapGen::gen_back_trace(const SbjNode* node,
		       bool output_inv,
		       const MapRecord& record,
		       BnNetwork& mapnetwork)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "back_trace(" << node->id_str() << ", " << output_inv << ")" << endl;
#endif
  NodeInfo& node_info = mNodeInfo[node->id()];

  int node_id = node_info.map_node(output_inv);
  if ( node_id != kBnNullId ) {
    // すでに生成済みならそのノードを返す．
    return node_id;
  }

  // 外部入力はどちらの極性でも生成済み
  ASSERT_COND( !node->is_input() );

  // node を根とするカットを取り出す．
  const Cut* cut = record.get_cut(node);
  if ( cut == nullptr ) {
    cout << "Error!: " << node->id_str()
	 << " has no cuts" << endl;
    ASSERT_NOT_REACHED;
  }

  // その入力に対応するノードを再帰的に生成する．
  int ni = cut->input_num();
  int depth = 0;
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    bool iinv = inode_info.inv_req();

    inode_info.inc_ref(iinv);
    gen_back_trace(inode, iinv, record, mapnetwork);

    // 段数の計算を行う．
    int idepth = inode_info.depth(iinv);
    if ( depth < idepth ) {
      depth = idepth;
    }
  }
  ++ depth;

  // ファンインの極性とファンインのノード番号のリストを作る．
  // 上のループで input_inv の設定を行うこともできるがそうすると
  // gen_back_trace() の再帰のたびにスタック上に input_inv が作られる
  // ことになるのであえてここで行う．
  vector<bool> input_inv(ni, false);
  vector<int> fanin_id_list(ni);
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* inode = cut->input(i);
    const NodeInfo& inode_info = mNodeInfo[inode->id()];
    bool iinv = inode_info.inv_req();
    input_inv[i] = iinv;
    fanin_id_list[i] = inode_info.map_node(iinv);
  }

  // カットの実現している関数の真理値表を得る．
  TvFunc tv = cut->make_tv(output_inv, input_inv);

  // 新しいノードを作る．
  node_id = mapnetwork.new_logic(string(), tv, fanin_id_list);

  ++ mLutNum;

  // マップ結果をセットする．
  node_info.set_map(node_id, depth, output_inv);

#if LUTMAP_DEBUG_MAPGEN
  cout << node->id_str() << " => " << node_id << ": " << depth << endl;
#endif

  return node_id;
}

// @brief マッピング結果から見積もりを行う．
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] record マッピング結果
// @param[out] lut_num LUT数
// @param[out] depth 最大段数
void
MapGen::estimate(const SbjGraph& sbjgraph,
		 const MapRecord& record,
		 int& lut_num,
		 int& depth)
{
#if LUTMAP_DEBUG_MAPEST
  cout << "MapGen::estimate() start" << endl;
#endif

  init(sbjgraph.node_num());

  int ni = sbjgraph.input_num();
  int no = sbjgraph.output_num();

  // 外部出力から要求されている極性を記録する．
  for ( int i = 0; i < no; ++ i ) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    if ( node ) {
      mNodeInfo[node->id()].inc_ref(onode->output_fanin_inv());
    }
  }

  // 外部入力の生成
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* node = sbjgraph.input(i);
    NodeInfo& node_info = mNodeInfo[node->id()];
    node_info.set_map(1, 0);
    if ( node_info.inv_req() ) {
      ++ mLutNum;

#if LUTMAP_DEBUG_MAPEST
      cout << node->id_str() << "[1]" << endl;
#endif
      node_info.set_map(1, 1, true);
    }
  }

  // 外部出力からバックトレースを行い全ノードの生成を行う．
  lut_num = 0;
  int max_depth = 0;
  for ( int i = 0; i < no; ++ i ) {
    const SbjNode* onode = sbjgraph.output(i);
    const SbjNode* node = onode->output_fanin();
    bool inv = onode->output_fanin_inv();
    if ( node ) {
      est_back_trace(node, inv, record);
      int depth = mNodeInfo[node->id()].depth(inv);
      if ( max_depth < depth ) {
	max_depth = depth;
      }
    }
    else {
      if ( inv ) {
	if ( mConst1 == kBnNullId ) {
	  mConst1 = 1;

	  ++ mLutNum;

#if LUTMAP_DEBUG_MAPEST
	  cout << "constant-1" << endl;
#endif
	}
      }
      else {
	if ( mConst0 == kBnNullId ) {
	  mConst0 = 1;

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
  int nl = sbjgraph.logic_num();
  for ( int i = 0; i < nl; ++ i ) {
    const SbjNode* node = sbjgraph.logic(i);
    const NodeInfo& node_info = mNodeInfo[node->id()];
    if ( node_info.ref_count(false) > 1 || node_info.ref_count(true) > 1 ) {
      mFanoutPointList.push_back(node);
    }
  }

#if LUTMAP_DEBUG_MAPEST
  cout << "MapGen::estimate() end" << endl;
#endif
}

// @brief 最終結果を作るためのバックトレースを行う．
// @param[in] node 対象のノード
// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
// @param[in] record マッピング結果
void
MapGen::est_back_trace(const SbjNode* node,
		       bool inv,
		       const MapRecord& record)
{
#if LUTMAP_DEBUG_MAPGEN
  cout << "back_trace(" << node->id_str() << ", " << inv << ")" << endl;
#endif

  NodeInfo& node_info = mNodeInfo[node->id()];

  int node_id = node_info.map_node(inv);
  if ( node_id ) {
    return;
  }

  // 外部入力はどちらの極性でも生成済み
  ASSERT_COND( !node->is_input() );

  // node を根とするカットを取り出す．
  const Cut* cut = record.get_cut(node);

  ++ mLutNum;

#if LUTMAP_DEBUG_MAPGEN
  cout << node->id_str() << "[" << inv << "]" << endl;
#endif

  // その入力に対応するノードを再帰的に生成する．
  int ni = cut->input_num();
  int depth = 0;
  for ( int i = 0; i < ni; ++ i ) {
    const SbjNode* inode = cut->input(i);
    NodeInfo& inode_info = mNodeInfo[inode->id()];
    bool iinv = inode_info.inv_req();

    inode_info.inc_ref(iinv);
    est_back_trace(inode, iinv, record);

    int depth1 = inode_info.depth(iinv);
    if ( depth < depth1 ) {
      depth = depth1;
    }
  }

  node_info.set_map(1, depth, inv);
}

END_NAMESPACE_LUTMAP
