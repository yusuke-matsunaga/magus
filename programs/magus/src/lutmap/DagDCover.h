#ifndef MAGUS_LUTMAP_DAGDCOVER_H
#define MAGUS_LUTMAP_DAGDCOVER_H

/// @file magus/lutmap/DagDCover.h
/// @brief DagDCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagDCover.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"


BEGIN_NAMESPACE_MAGUS_LUTMAP

class SbjGraph;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class DagDCover DagDCover.h "DagDCover.h"
/// @brief depth/area optimal cover を求めるためのクラス
//////////////////////////////////////////////////////////////////////
class DagDCover
{
public:

  /// @brief デストラクタ
  virtual
  ~DagDCover() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装しなければならない仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit LUT の入力数
  /// @param[in] slack 最小段数に対するスラック
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      int limit,
	      ymuint slack,
	      MapRecord& maprec) = 0;
  
};


//////////////////////////////////////////////////////////////////////
/// @class DagDCoverFactory DagDCover.h "DagDCover.h"
/// @brief DagDCover (の派生クラス)を生成するファクトリクラス
//////////////////////////////////////////////////////////////////////
class DagDCoverFactory
{
public:

  /// @brief コンストラクタ
  DagDCoverFactory();
  
  /// @brief デストラクタ
  ~DagDCoverFactory();


public:

  /// @brief DagCover (の派生クラス)を生成する．
  /// @param[in] args DagCover に渡す引数
  DagDCover*
  operator()(const string& args);

};

END_NAMESPACE_MAGUS_LUTMAP

#endif // MAGUS_LUTMAP_DAGDCOVER_H
