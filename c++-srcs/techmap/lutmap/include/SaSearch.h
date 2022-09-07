#ifndef SASEARCH_H
#define SASEARCH_H

/// @file SaSearch.h
/// @brief SaSearch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "AreaCover.h"
#include "MapRecord.h"
#include <random>


BEGIN_NAMESPACE_LUTMAP

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
  SaSearch(
    const SbjGraph& sbjgraph,    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder, ///< [in] カットフォルダー
    SizeType cut_size,           ///< [in] カットサイズ
    bool flow_mode               ///< [in] area_cover のモード
  );

  /// @brief デストラクタ
  ~SaSearch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 探索を行う．
  /// @return 最良解を返す．
  const MapRecord&
  search(
    SizeType search_limit, ///< [in] 試行回数
    bool verbose           ///< [in] verbose フラグ
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在の割り当てのもとで評価を行う．
  SizeType
  evaluate(
    const vector<bool>& state
  );


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

  // 初期温度
  double mInitTemp;

  // 終了温度
  double mEndTemp;

  // 減衰率
  double mDecrement;

  // マッパー
  AreaCover mAreaCover;

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

  // 乱数発生器
  std::mt19937 mRandGen;

  // verbose フラグ
  bool mVerbose;

};

END_NAMESPACE_LUTMAP

#endif // SASEARCH_H
