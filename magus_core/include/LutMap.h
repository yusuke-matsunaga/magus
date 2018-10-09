#ifndef LUTMAP_H
#define LUTMAP_H

/// @file LutMap.h
/// @brief LutMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_LUTMAP

class DagCover;

//////////////////////////////////////////////////////////////////////
/// @class LutMap LutMap.h "LutMap.h"
/// @brief LUT用のテクノロジマッパー
//////////////////////////////////////////////////////////////////////
class LutMap
{
public:

  /// @brief コンストラクタ
  /// @param[in] lut_size LUTの入力数
  /// @param[in] algorithm アルゴリズムを表す文字列
  LutMap(int lut_size = 5,
	 const string& algorithm = string());

  /// @brief デストラクタ
  ~LutMap();


public:
  //////////////////////////////////////////////////////////////////////
  // メンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief LUTの入力数を設定する
  /// @param[in] lut_size LUTの入力数
  void
  set_lut_size(int lut_size);

  /// @brief アルゴリズムを設定する．
  /// @param[in] algorithm アルゴリズムを表す文字列
  void
  set_algorithm(const string& algorithm);

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  /// @param[in] src_network もとのネットワーク
  /// @param[out] dat_network マッピング結果
  void
  area_map(const BnNetwork& src_network,
	   BnNetwork& dst_network);

  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] src_network もとのネットワーク
  /// @param[in] slack 最小段数に対するスラック
  /// @param[out] dat_network マッピング結果
  void
  delay_map(const BnNetwork& src_network,
	    int slack,
	    BnNetwork& dst_network);

  /// @brief 直前のマッピング結果のLUT数を返す．
  int
  lut_num();

  /// @brief 直前のマッピング結果の段数を返す．
  int
  depth();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief アルゴリズムの文字列をパースする．
  void
  parse_algorithm();

  /// @brief マッピングを行う実際の関数
  void
  do_mapping(const BnNetwork& src_network,
	     BnNetwork& map_network,
	     DagCover& dag_cover);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // LUTの入力数
  int mLutSize;

  // スラック
  int mSlack;

  // アルゴリズム
  string mAlgorithm;

  // フローモード
  bool mFlowMode;

  // cut_resubstitution を行う時に true にするフラグ
  bool mDoCutResub;

  // 直前のマッピング結果のLUT数
  int mLutNum;

  // 直前のマッピング結果の段数
  int mDepth;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief LUTの入力数を設定する
// @param[in] lut_size LUTの入力数
inline
void
LutMap::set_lut_size(int lut_size)
{
  mLutSize = lut_size;
}

// @brief アルゴリズムを設定する．
// @param[in] algorithm アルゴリズムを表す文字列
inline
void
LutMap::set_algorithm(const string& algorithm)
{
  mAlgorithm = algorithm;
}

// @brief 直前のマッピング結果のLUT数を返す．
inline
int
LutMap::lut_num()
{
  return mLutNum;
}

// @brief 直前のマッピング結果の段数を返す．
inline
int
LutMap::depth()
{
  return mDepth;
}

END_NAMESPACE_LUTMAP

#endif // LUTMAP_H
