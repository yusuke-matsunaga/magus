#ifndef THBDDGEN_H
#define THBDDGEN_H

/// @file ThBddGen.h
/// @brief ThBddGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/BddMgr.h"
#include "ym_logic/Bdd.h"

#include "Weight.h"
#include "ItvlList.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ThBddGen ThBddGen.h "ThBddGen.h"
/// @brief しきい値関数を表すBDDを作るためのクラス
//////////////////////////////////////////////////////////////////////
class ThBddGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] bddmgr BDDマネージャ
  ThBddGen(BddMgr& bddmgr);

  /// @brief デストラクタ
  ~ThBddGen();


public:

  /// @brief しきい値関数を表す BDD を生成する．
  /// @param[in] weight_array 係数の配列
  /// @param[in] threshold しきい値
  Bdd
  operator()(const vector<double>& weight_array,
	     double threshold);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// BDD を作る再帰関数
  Bdd
  gen_bdd(ymuint pos,
	  double slack,
	  double& lb,
	  double& ub);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {
    Cell()
    {
    }

    Cell(double lb,
	 double ub,
	 const Bdd& bdd) :
      mLb(lb),
      mUb(ub),
      mBdd(bdd)
    {
    };

    double mLb;

    double mUb;

    Bdd mBdd;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDDマネージャ
  BddMgr& mBddMgr;

  // 係数の配列
  vector<Weight> mWeightArray;

  // 区間のリストの配列
  vector<ItvlList> mItvlList;

#if 0
  // 区間のリスト
  vector<vector<Cell> > mItvlList;
#endif
};

END_NAMESPACE_YM

#endif // THBDDGEN_H
