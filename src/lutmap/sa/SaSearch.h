#ifndef SASEARCH_H
#define SASEARCH_H

/// @file SaSearch.h
/// @brief SaSearch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "AreaCover.h"
#include "MapRecord.h"
#include "MctState.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM_LUTMAP

class CutHolder;

class MctNode;

//////////////////////////////////////////////////////////////////////
/// @class SaSearch SaSearch.h "SaSearch.h"
/// @brief MCT 探索を行うクラス
//////////////////////////////////////////////////////////////////////
class SaSearch
{
public:

  /// @brief コンストラクタ
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder カットフォルダー
  /// @param[in] cut_size カットサイズ
  /// @param[in] mode area_cover のモード
  SaSearch(const SbjGraph& sbjgraph,
	    const CutHolder& cut_holder,
	    ymuint cut_size,
	    ymuint mode);

  /// @brief デストラクタ
  ~SaSearch();


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

  /// @brief verbose フラグをセットする．
  void
  set_verbose(bool val);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の割り当てのもとで評価を行う．
  ymuint
  evaluate(const vector<bool>& state);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のサブジェクトグラフ
  const SbjGraph& mSbjGraph;

  // カットを保持するオブジェクト
  const CutHolder& mCutHolder;

  // カットサイズ
  ymuint mCutSize;

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

  // ファンアウトポイントの入力サイズのリスト
  vector<ymuint> mInputSizeList;

  // マッパー
  AreaCover mAreaCover;

  // 上界
  ymuint mUpperBound;

  // 下界
  ymuint mLowerBound;

  // 上界と下界の幅
  double mWidth;

  // トータルの試行回数
  ymuint mNumAll;

  // 最良値
  ymuint mMinimumLutNum;

  // 最良解
  MapRecord mBestRecord;

  // 乱数発生器
  RandGen mRandGen;

  // verbose フラグ
  bool mVerbose;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 最良解を返す．
inline
const MapRecord&
SaSearch::best_record() const
{
  return mBestRecord;
}

END_NAMESPACE_YM_LUTMAP

#endif // SASEARCH_H
