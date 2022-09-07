
/// @file LutmapMgr.cc
/// @brief LutmapMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "LutmapMgr.h"
#include "Bn2Sbj.h"
#include "SbjGraph.h"
#include "AreaCover.h"
#include "DelayCover.h"
#include "CutHolder.h"
#include "CutResub.h"
#include "MapGen.h"
#include "MapRecord.h"
#include "ym/OptionParser.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス LutmapMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lut_size LUTの入力数
// @param[in] option オプション文字列
LutmapMgr::LutmapMgr(
  SizeType lut_size,
  const string& option
) : mLutSize{lut_size},
    mFanoutMode{false},
    mDoCutResub{false}
{
  set_option(option);
}

// @brief デストラクタ
LutmapMgr::~LutmapMgr()
{
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] src_network もとのネットワーク
BnNetwork
LutmapMgr::area_map(
  const BnNetwork& src_network
)
{
  using namespace nsLutmap;

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, mLutSize);

  int slack = -1;

  // 最良カットを記録する．
  MapRecord maprec;

  // 本当は mAlgorithm に応じた処理を行う．
  AreaCover area_cover(mFanoutMode);
  area_cover.record_cuts(sbjgraph, cut_holder, maprec);

  if ( mDoCutResub ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  return gen.generate(sbjgraph, maprec, mLutNum, mDepth);
}

// @brief 段数最小化 DAG covering のヒューリスティック関数
BnNetwork
LutmapMgr::delay_map(
  const BnNetwork& src_network,
  int slack
)
{
  using namespace nsLutmap;

  SbjGraph sbjgraph;
  Bn2Sbj bn2sbj;
  bn2sbj.convert(src_network, sbjgraph);

  // カットを列挙する．
  CutHolder cut_holder;
  cut_holder.enum_cut(sbjgraph, mLutSize);

  // 最良カットを記録する．
  MapRecord maprec;

  // 本当は mAlgorithm に応じた処理を行う．
  DelayCover delay_cover(mFanoutMode, slack);
  delay_cover.record_cuts(sbjgraph, cut_holder, maprec);

  if ( mDoCutResub ) {
    // cut resubstituion
    CutResub cut_resub;
    cut_resub(sbjgraph, cut_holder, maprec, slack);
  }

  // 最終的なネットワークを生成する．
  MapGen gen;
  return gen.generate(sbjgraph, maprec, mLutNum, mDepth);
}

// @brief オプション文字列を設定する．
// @param[in] option オプション文字列
void
LutmapMgr::set_option(
  const string& option
)
{
  mOption = option;
  mFanoutMode = true;
  mDoCutResub = true;
  OptionParser parser;
  auto opt_list = parser.parse(mOption);
  for ( auto p: opt_list ) {
    auto key = p.first;
    auto val = p.second;
    if ( key == string("algorithm") ) {
      mAlgorithm = val;
    }
    else if ( key == string("fanout") ) {
      mFanoutMode = true;
    }
    else if ( key == string("flow") ) {
      mFanoutMode = false;
    }
    else if ( key == string("cut_resub") ) {
      mDoCutResub = true;
    }
    else if ( key == string("no_cut_resub") ) {
      mDoCutResub = false;
    }
  }
}

END_NAMESPACE_MAGUS
