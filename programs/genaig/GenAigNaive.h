#ifndef GENAIGNAIVE_H
#define GENAIGNAIVE_H

/// @file GenAigNaive.h
/// @brief GenAigNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "logic/AigMgr.h"
#include "logic/Aig.h"
#include "AigPat.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenAigNaive GenAigNaive.h "GenAigNaive.h"
/// @brief 最小サイズのAIGを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenAigNaive
{
public:

  /// @brief コンストラクタ
  GenAigNaive();

  /// @brief デストラクタ
  ~GenAigNaive();


public:

  /// @brief ni 入力の全ての関数のAIGを求める．
  void
  operator()(ymuint ni,
	     ymuint slack = 0);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AIG モード
  void
  aig_mode(ymuint slack);

  /// @brief パタンを登録する．
  /// @param[in] aig AIG
  /// @param[in] fv 関数ベクタ
  /// @param[in] level レベル
  void
  add_pat(Aig aig,
	  ymuint32 fv,
	  ymuint32 level);

  /// @brief 2つのAIGから新しいパタンを作る．
  /// @note 具体的には aig1 & aig2 と ~aig & aig
  void
  compose(AigPat aig1,
	  AigPat aig2);

  /// @brief AIG の対を登録する．
  /// @note 結果は mCandListArray に追加される．
  void
  add_aigpair(Aig aig1,
	      Aig aig2,
	      ymuint32 func,
	      ymuint level);

  /// @brief 候補のリストに追加する．
  void
  add_cand(Aig aig,
	   ymuint32 func,
	   ymuint level);

  /// @brief AigPat の内容が正しいか調べる．
  bool
  sanity_check(AigPat aigpat);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AIGMGR
  AigMgr mMgr;

  // 現在の入力数
  ymuint32 mNi;

  // 関数ベクタの要素数
  ymuint32 mNp;

  // 関数の数
  ymuint32 mNf;

  // 関数ベクタ用のマスク
  ymuint32 mMask;

  // パタンの求まっていない関数の数．
  ymuint32 mRemainFunc;


  //////////////////////////////////////////////////////////////////////
  // AIG モード用の変数
  //////////////////////////////////////////////////////////////////////

  // レベルごとのAigPat のリスト
  vector<vector<AigPat> > mAigList;

  // AigPat の候補のリスト
  vector<vector<AigPat> > mCandListArray;

  // 関数ごとのパタンのリストを記録する配列
  vector<vector<AigPat> > mFuncArray;

  // 関数の最小レベルを記録する配列
  vector<ymuint32> mFuncLevel;

  // 最小レベルからの乖離を表すパラメータ
  ymuint32 mSlack;

  // 登録されている AIG のハッシュ
  unordered_set<Aig> mAigHash;

};

END_NAMESPACE_YM

#endif // GENAIG_H
