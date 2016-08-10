#ifndef MAPSTATE_H
#define MAPSTATE_H

/// @file MapState.h
/// @brief MapState のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap2_nsdef.h"
#include "ym/ym_bnet.h"
#include "sbj_nsdef.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_LUTMAP2

//////////////////////////////////////////////////////////////////////
/// @class MapState MapState.h "MapState.h"
/// @brief マッピングの状態を保持するクラス
///
/// 具体的には各ノードごとに選択されたカットを保持する．
/// 新しいカットはすでに選択されているカットの根のノードよりも
/// トポロジカル順で後ろにあるノードを根とするものに限られる．
//////////////////////////////////////////////////////////////////////
class MapState
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_size カットサイズ
  MapState(const SbjGraph& sbjgraph,
	   ymuint cut_size);

  /// @brief デストラクタ
  ~MapState();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次に選択可能な POカットのリストを得る．
  /// @param[in] cut_list カットのリスト
  const vector<const SbjNode*>&
  next_pocandidates() const;

  /// @brief 次に選択可能なカットのリストを作る．
  /// @param[in] cut_list カットのリスト
  const vector<const SbjNode*>&
  next_candidates() const;

  /// @brief カット候補の重みリストを得る．
  const vector<ymuint>&
  weight_list() const;

  /// @brief カットを追加して状態を更新する．
  /// @param[in] cut_root 追加するカットの根のノード
  void
  update(const SbjNode* cut_root);

  /// @brief LUT 数を得る．
  ymuint
  lut_num() const;

  /// @brief マッピング結果を BnBuilder にセットする．
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  gen_mapgraph(BnBuilder& mapgraph,
	       ymuint& lut_num,
	       ymuint& depth);

  /// @brief 'blocked' 状態をチェックする．
  void
  check_cover();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 'blocked' マークを得る．
  bool
  is_blocked(const SbjNode* node) const;

  /// @brief 'blocked' マークをつける．
  void
  set_blocked(const SbjNode* node);

  /// @brief block されているか調べる．
  /// @return block されていたら true を返す．
  bool
  check_blocked(const SbjNode* node);

  /// @brief 'selected' マークを得る．
  bool
  is_selected(const SbjNode* node) const;

  /// @brief 'selected' マークをつける．
  void
  set_selected(const SbjNode* node);

  /// @brief カットを持っている時 true を返す．
  bool
  has_cut(const SbjNode* node) const;

  /// @brief カットを持っている印をつける．
  ///
  /// これが true のノードは mCandidateList に含まれる．
  void
  set_cut(const SbjNode* node);

  /// @brief カットを持っている印を消す．
  void
  clear_cut(const SbjNode* node);

  /// @brief テンポラリマークを得る．
  bool
  is_marked(const SbjNode* node) const;

  /// @brief テンポラリマークをつける．
  void
  set_mark(const SbjNode* node);

  /// @brief テンポラリマークを消す．
  void
  clear_mark();

  /// @brief 2つのカットの入力をマージする．
  void
  merge_inputs(const vector<const SbjNode*>& inputs1,
	       const vector<const SbjNode*>& inputs2,
	       vector<const SbjNode*>& new_inputs);

  /// @brief カットの候補を求める．
  void
  get_cut_candidates();

  /// @brief カットの入力を求めるための DFS
  void
  dfs(const SbjNode* node,
      vector<const SbjNode*>& node_list);

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[out] mapnetwork マッピング結果のネットワーク
  ymuint
  back_trace(const SbjNode* node,
	     BnBuilder& mapnetwork);

  void
  check_cover_node(const SbjNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mFlags = 0;
      mMapNode = 0;
      mDepth = 0;
    }

    // カット候補の入力
    vector<const SbjNode*> mInputs;

    // 選択されたカットの入力
    vector<const SbjNode*> mCutInputs;

    // いくつかの情報をパックしたフィールド
    ymuint mFlags;

    // マップ結果のノード番号
    ymuint mMapNode;

    // 段数
    int mDepth;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のサブジェクトグラフ
  const SbjGraph& mSbjGraph;

  // カットサイズ
  ymuint mCutSize;

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 各ノードごと作業領域を格納した配列
  // サイズは mMaxNodeId
  NodeInfo* mNodeInfo;

  // カット候補を求めるためのキュー
  vector<const SbjNode*> mQueue;

  vector<bool> mQmark;

  // カット候補の根のノードのリスト
  vector<const SbjNode*> mCandidateList;

  // mCandidateList のうち出力ノードのリスト
  vector<const SbjNode*> mPoCandidateList;

  // カット候補の入力数のリスト
  vector<ymuint> mCandNiList;

  // set_mark() でマークされたノード番号のリスト
  vector<ymuint> mMarkedList;

  // LUT数
  ymuint mLutNum;

  // カットの探索を行うノード番号
  ymuint mNextId;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 'blocked' マークを得る．
inline
bool
MapState::is_blocked(const SbjNode* node) const
{
  return static_cast<bool>((mNodeInfo[node->id()].mFlags >> 0) & 1U);
}

// @brief 'blocked' マークをつける．
inline
void
MapState::set_blocked(const SbjNode* node)
{
  mNodeInfo[node->id()].mFlags |= (1U << 0);
}

// @brief 'selected' マークを得る．
inline
bool
MapState::is_selected(const SbjNode* node) const
{
  return static_cast<bool>((mNodeInfo[node->id()].mFlags >> 1) & 1U);
}

// @brief 'selected' マークをつける．
inline
void
MapState::set_selected(const SbjNode* node)
{
  mNodeInfo[node->id()].mFlags |= (1U << 1);
}

// @brief カットを持っている時 true を返す．
inline
bool
MapState::has_cut(const SbjNode* node) const
{
  return static_cast<bool>((mNodeInfo[node->id()].mFlags >> 2) & 1U);
}

// @brief カットを持っている印をつける．
inline
void
MapState::set_cut(const SbjNode* node)
{
  mNodeInfo[node->id()].mFlags |= (1U << 2);
}

// @brief カットを持っている印を消す．
inline
void
MapState::clear_cut(const SbjNode* node)
{
  mNodeInfo[node->id()].mFlags &= ~(1U << 2);
}

// @brief merge 用のマークを得る．
inline
bool
MapState::is_marked(const SbjNode* node) const
{
  return static_cast<bool>((mNodeInfo[node->id()].mFlags >> 3) & 1U);
}

// @brief merge 用のマークをつける．
inline
void
MapState::set_mark(const SbjNode* node)
{
  mNodeInfo[node->id()].mFlags |= (1U << 3);
  mMarkedList.push_back(node->id());
}

// @brief merge 用のマークを消す．
inline
void
MapState::clear_mark()
{
  for (ymuint i = 0; i < mMarkedList.size(); ++ i) {
    ymuint id = mMarkedList[i];
    mNodeInfo[id].mFlags &= ~(1U << 3);
  }
  mMarkedList.clear();
}

END_NAMESPACE_YM_LUTMAP2

#endif // MAPSTATE_H
