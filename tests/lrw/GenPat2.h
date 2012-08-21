#ifndef GENPAT2_H
#define GENPAT2_H

/// @file GenPat2.h
/// @brief GenPat2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "NpnNodeMgr.h"
#include "NpnXform.h"
#include "FuncXform.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenPat2 GenPat2.h "GenPat2.h"
/// @brief 最小サイズのAIGを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenPat2
{
public:

  /// @brief コンストラクタ
  GenPat2();

  /// @brief デストラクタ
  ~GenPat2();


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
  npn_expand(NpnHandle handle,
	     ymuint32 level);

  /// @brief パタンを登録する．
  /// @param[in] handle ハンドル
  /// @param[in] fv 関数ベクタ
  /// @param[in] level レベル
  void
  add_pat(NpnHandle handle,
	  ymuint32 level);

  /// @brief 2つのノードから新しいパタンを作る．
  /// @note 具体的には aig1 & aig2 と ~aig & aig
  void
  compose(NpnHandle handle1,
	  NpnHandle handle2,
	  ymuint level_base);

  /// @brief ノードの対を登録する．
  /// @note 結果は mCandListArray に追加される．
  void
  add_pair(NpnHandle handle,
	   ymuint level);

  /// @brief 候補のリストに追加する．
  void
  add_cand(NpnHandle handle,
	   ymuint level);

  /// @brief handle の子供に印をつけてノード数を数える．
  ymuint
  count1(NpnHandle handle);

  /// @brief count1 で印のついていないノード数を数える．
  ymuint
  count2(NpnHandle handle);

  /// @brief count2 の下請け関数
  ymuint
  count2_sub(NpnHandle handle,
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

  // NpnNode を管理するオブジェクト
  NpnNodeMgr mMgr;

  // パタンの求まっていない関数の数．
  ymuint32 mRemainFunc;

  // パタンの求まっていない代表関数の数
  ymuint32 mRemainRep;

  // レベルごとの NpnHandle のリスト
  vector<vector<NpnHandle> > mNpnNodeList;

  // レベルごとの代表関数の NpnHandle のリスト
  vector<vector<NpnHandle> > mRepList;

  // NpnHandle の候補のリスト
  vector<vector<NpnHandle> > mCandListArray;

  // 関数ごとのパタンのリストを記録する配列
  vector<vector<NpnHandle> > mFuncArray;

  // 関数の最小レベルを記録する配列
  vector<ymuint32> mFuncLevel;

  // 最小レベルからの乖離を表すパラメータ
  ymuint32 mSlack;

  // 登録されている NpnHandle のハッシュ
  hash_set<ymuint32> mNpnHandleHash;

  // count 用の AIG のハッシュ
  hash_set<ymuint32> mCountHash;

  // 関数ごとのレベルの上限を入れる配列
  vector<ymuint32> mUpperLevel;

  // レベルごとの関数のリスト
  vector<vector<ymuint16> > mFuncListArray;

};

END_NAMESPACE_YM

#endif // GENPAT2_H
