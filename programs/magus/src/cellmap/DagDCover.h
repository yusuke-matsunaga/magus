#ifndef MAGUS_CELLMAP_DAGDCOVER_H
#define MAGUS_CELLMAP_DAGDCOVER_H

/// @file magus/cellmap/DagDCover.h
/// @brief DagDCover のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DagDCover.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2007 Yusuke Matsunaga
/// All rights reserved.

#include "cellmap.h"


BEGIN_NAMESPACE_MAGUS_CELLMAP

class SbjGraph;
class CutHolder;
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
  /// @param[in] cut_holder カットを保持するオブジェクト
  /// @param[in] slack 最小段数に対するスラック
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  virtual
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      size_t slack,
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
  DagDCover*
  operator()(const string& args);

};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_DAGDCOVER_H
