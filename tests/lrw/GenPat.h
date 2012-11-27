#ifndef GENPAT_H
#define GENPAT_H

/// @file GenPat.h
/// @brief GenPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "GpMgr.h"
#include "GpNode.h"
#include "NpnXform.h"
#include "FuncXform.h"


BEGIN_NAMESPACE_YM

struct GpPair
{
  GpPair(GpHandle handle1,
	 GpHandle handle2) :
    mHandle1(handle1),
    mHandle2(handle2)
  {
  }

  GpHandle mHandle1;
  GpHandle mHandle2;
};


//////////////////////////////////////////////////////////////////////
/// @class GenPat GenPat.h "GenPat.h"
/// @brief 最小サイズのAIGを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenPat
{
public:

  /// @brief コンストラクタ
  GenPat();

  /// @brief デストラクタ
  ~GenPat();


public:

  /// @brief 4 入力の全ての関数のAIGを求める．
  void
  operator()(ymuint slack = 0);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief NPN同値類を求める．
  /// @param[in] handle 根のハンドル
  /// @param[in] level レベル
  void
  npn_expand(GpHandle handle,
	     ymuint32 level);

  /// @brief GpHandle を変換する(4入力版)
  GpHandle
  xform4(GpHandle handle,
	 NpnXform xf);

  /// @brief xform4 の下請け関数
  GpHandle
  xf4_sub(GpHandle handle,
	  NpnXform xf);

  /// @brief 2つのノードから新しいパタンを作る．
  /// @note 具体的には aig1 & aig2 と ~aig & aig
  void
  compose(GpHandle handle1,
	  GpHandle handle2,
	  ymuint level_base);

  /// @brief 候補のリストに追加する．
  void
  add_cand(GpHandle handle1,
	   GpHandle handle2,
	   ymuint level,
	   vector<vector<GpPair> >& list_array);

  /// @brief ノードの対を登録する．
  /// @note 結果は mCandListArray に追加される．
  void
  add_pat(GpHandle handle,
	  ymuint level);

  /// @brief handle の子供に印をつけてノード数を数える．
  ymuint
  count1(GpHandle handle);

  /// @brief count1 で印のついていないノード数を数える．
  ymuint
  count2(GpHandle handle);

  /// @brief count2 の下請け関数
  ymuint
  count2_sub(GpNode* node,
	     hash_set<ymuint32>& hash);

  /// @brief 4入力関数の情報をセットアップする．
  void
  init_npn4rep();

  /// @brief 関数ごとのレベルの上限をセットする．
  void
  calc_upper();

  /// @brief calc_upper() の下請け関数
  void
  add_func(ymuint16 func,
	   ymuint level);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NPN同値類代表関数を表すハッシュ表
  hash_map<ymuint32, vector<FuncXform> > mNpnHash;

  // GpNode を管理するオブジェクト
  GpMgr mMgr;

  // パタンの求まっていない関数の数．
  ymuint32 mRemainFunc;

  // パタンの求まっていない代表関数の数
  ymuint32 mRemainRep;

  // レベルごとの GpHandle のリスト
  vector<vector<GpHandle> > mGpList;

  // レベルごとの代表関数の GpHandle のリスト
  vector<vector<GpHandle> > mRepList;

  // レベルごとのAND候補ペアのリスト
  vector<vector<GpPair> > mAndCandPairListArray;

  // レベルごとのXOR候補ペアのリスト
  vector<vector<GpPair> > mXorCandPairListArray;

  // GpHandle の候補のリスト
  vector<vector<GpHandle> > mCandListArray;

  // 関数ごとのパタンのリストを記録する配列
  vector<vector<GpHandle> > mFuncArray;

  // 関数の最小レベルを記録する配列
  vector<ymuint32> mFuncLevel;

  // 最小レベルからの乖離を表すパラメータ
  ymuint32 mSlack;

  // 登録されている GpHandle のハッシュ
  hash_set<ymuint32> mGpHash;

  // count 用の AIG のハッシュ
  hash_set<ymuint32> mCountHash;

  // 関数ごとのレベルの上限を入れる配列
  vector<ymuint32> mUpperLevel;

  // レベルごとの関数のリスト
  vector<vector<ymuint16> > mFuncListArray;

};

END_NAMESPACE_YM

#endif // GENPAT_H
