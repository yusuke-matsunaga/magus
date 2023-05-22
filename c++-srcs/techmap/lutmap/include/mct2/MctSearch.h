#ifndef MCT2_MCTSEARCH_H
#define MCT2_MCTSEARCH_H

/// @file MctSearch.h
/// @brief MctSearch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct2/mct2_nsdef.h"
#include "mct2/MctState.h"
#include "AreaCover.h"
#include "MapRecord.h"
#include <random>


BEGIN_NAMESPACE_LUTMAP

class CutHolder;

END_NAMESPACE_LUTMAP

BEGIN_NAMESPACE_LUTMAP_MCT2

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
  /// @param[in] cut_holder カットフォルダー
  /// @param[in] cut_size カットサイズ
  /// @param[in] flow_mode area_cover のモード
  MctSearch(const SbjGraph& sbjgraph,
	    const CutHolder& cut_holder,
	    SizeType cut_size,
	    bool flow_mode);

  /// @brief デストラクタ
  ~MctSearch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 探索を行う．
  /// @param[in] search_limit 試行回数
  /// @param[in] verbose verbose フラグ
  /// @return 最良解を返す．
  const MapRecord&
  search(SizeType search_limit,
	 bool verbose);


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のサブジェクトグラフ
  const SbjGraph& mSbjGraph;

  // カットを保持するオブジェクト
  const CutHolder& mCutHolder;

  // カットサイズ
  SizeType mCutSize;

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

  // ファンアウトポイントの入力サイズのリスト
  vector<SizeType> mInputSizeList;

  // マッパー
  AreaCover mAreaCover;

  // 状態
  MctState mState;

  // 上界
  SizeType mUpperBound;

  // 下界
  SizeType mLowerBound;

  // 上界と下界の幅
  double mWidth;

  // トータルの試行回数
  SizeType mNumAll;

  // 最良値
  SizeType mMinimumLutNum;

  // 最良解
  MapRecord mBestRecord;

  // 根のノード
  MctNode* mRootNode;

  // 乱数発生器
  std::mt19937 mRandGen;

  // verbose フラグ
  bool mVerbose;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_LUTMAP_MCT2

#endif // MCTSEARCH_H
