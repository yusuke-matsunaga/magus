#ifndef CUTRESUB_H
#define CUTRESUB_H

/// @file CutResub.h
/// @brief CutResub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;
class CutHolder;
class MapRecord;
class CutResubImpl;

//////////////////////////////////////////////////////////////////////
/// @class CutResub CutResub.h "CutResub.h"
/// @brief カットの置き換えを行うクラス
//////////////////////////////////////////////////////////////////////
class CutResub
{
public:

  /// @brief コンストラクタ
  CutResub();

  /// @brief デストラクタ
  ~CutResub();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief カットの置き換えを行って LUT 数の削減を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder サブジェクトグラフ上のカット集合
  /// @param[in] slack 段数のスラック(-1 で段数制約なし)
  /// @param[inout] maprec マッピング結果
  void
  operator()(const SbjGraph& sbjgraph,
	     const CutHolder& cut_holder,
	     MapRecord& maprec,
	     int slack = -1);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際に処理を行うクラス
  CutResubImpl* mImpl;

};

END_NAMESPACE_LUTMAP

#endif // CUTRESUB_H
