#ifndef MAXCOMPAT_H
#define MAXCOMPAT_H

/// @file MaxCompat.h
/// @brief MaxCompat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "YmLogic/Bdd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MaxCompat MaxCompat.h "MaxCompat.h"
/// @brief maximal compatible set を求めるクラス
//////////////////////////////////////////////////////////////////////
class MaxCompat
{
public:

  /// @brief コンストラクタ
  /// @param[in] tf_array 故障検出関数のリスト
  MaxCompat(const vector<Bdd>& tf_array);

  /// @brief デストラクタ
  ~MaxCompat();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief maximal compatible set を求める．
  void
  operator()(ymuint f0,
	     const vector<bool>& covered,
	     vector<ymuint>& f_set);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  kernel(const Bdd& c0,
	 const vector<ymuint>& f_list,
	 ymuint pos,
	 const vector<ymuint>& cur_set);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  vector<Bdd> mTfArray;

  ymuint mCurMax;

  vector<ymuint> mCurMaxSet;

  vector<bool> mCurVector;

  ymuint mMaxUpdates;

};

END_NAMESPACE_YM_SATPG

#endif // MAXCOMPAT_H
