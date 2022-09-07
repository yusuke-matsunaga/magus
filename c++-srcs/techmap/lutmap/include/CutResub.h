#ifndef CUTRESUB_H
#define CUTRESUB_H

/// @file CutResub.h
/// @brief CutResub のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
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
  void
  operator()(
    const SbjGraph& sbjgraph,    ///< [in] サブジェクトグラフ
    const CutHolder& cut_holder, ///< [in] サブジェクトグラフ上のカット集合
    MapRecord& maprec,           ///< [inout] マッピング結果
    int slack = -1               ///< [in] 段数のスラック(-1 で段数制約なし)
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際に処理を行うクラス
  unique_ptr<CutResubImpl> mImpl;

};

END_NAMESPACE_LUTMAP

#endif // CUTRESUB_H
