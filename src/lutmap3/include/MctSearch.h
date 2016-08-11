#ifndef MCTSEARCH_H
#define MCTSEARCH_H

/// @file MctSearch.h
/// @brief MctSearch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "MapRecord.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_LUTMAP3

class MctNode;

//////////////////////////////////////////////////////////////////////
/// @class MctSearch MctSearch.h "MctSearch.h"
/// @brief MCT 探索を行うクラス
//////////////////////////////////////////////////////////////////////
class MctSearch
{
public:

  /// @brief コンストラクタ
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_size カットサイズ
  MctSearch(const SbjGraph& sbjgraph,
	    ymuint cut_size);

  /// @brief デストラクタ
  ~MctSearch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 探索を行う．
  /// @param[in] search_limit 試行回数
  void
  search(ymuint search_limit);

  /// @brief 最良解を返す．
  const MapRecord&
  best_record() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 評価値の良い子ノードを見つける．
  MctNode*
  tree_policy(MctNode* node);

  /// @brief ランダムサンプリングを行って評価値を求める．
  double
  default_policy(MctNode* node);

  /// @brief 評価値の更新を行う．
  void
  back_up(MctNode* node,
	  double val);

  /// @brief 状態を遷移させる．
  /// @param[in] cut_root カットの根のノード
  void
  move(const SbjNode* cut_root);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

  // 境界ノードのリスト
  vector<const SbjNode*> mBoundaryList;

  // 境界ノードのマップ
  vector<bool> mBoundaryMap;

  // 基準値
  double mBaseline;

  // トータルの試行回数
  ymuint mNumAll;

  // 最良値
  ymuint mMinimumLutNum;

  // 最良解
  MapRecord mBestRecord;

  // 根のノード
  MctNode* mRootNode;

  // 乱数発生器
  RandGen mRandGen;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最良解を返す．
inline
const MapRecord&
MctSearch::best_record() const
{
  return mBestRecord;
}

END_NAMESPACE_YM_LUTMAP3

#endif // MCTSEARCH_H
