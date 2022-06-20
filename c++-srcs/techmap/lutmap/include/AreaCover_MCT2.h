#ifndef AREACOVER_MCT2_H
#define AREACOVER_MCT2_H

/// @file AreaCover_MCT1.h
/// @brief AreaCover_MCT1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "mct2_nsdef.h"
#include "DagCover.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

END_NAMESPACE_LUTMAP

BEGIN_NAMESPACE_LUTMAP_MCT2

//////////////////////////////////////////////////////////////////////
/// @brief 面積モードの DAG covering のヒューリスティック
//////////////////////////////////////////////////////////////////////
class AreaCover_MCT2 :
  public DagCover
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] fanout_mode ファンアウトモード
  /// @param[in] count 試行回数
  /// @param[in] verbose verbose フラグ
  AreaCover_MCT2(bool fanout_mode,
		 int count,
		 bool verbose);

  /// @brief デストラクタ
  ~AreaCover_MCT2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief best cut の記録を行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] cut_holder 各ノードのカットを保持するオブジェクト
  /// @param[in] count 試行回数
  /// @param[out] maprec マッピング結果を記録するオブジェクト
  void
  record_cuts(const SbjGraph& sbjgraph,
	      const CutHolder& cut_holder,
	      MapRecord& maprec) override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 試行回数
  int mCount;

  // verbose フラグ
  bool mVerbose;

};

END_NAMESPACE_LUTMAP_MCT2

#endif // AREACOVER_MCT2_H
