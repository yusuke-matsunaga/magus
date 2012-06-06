#ifndef GENAIG_H
#define GENAIG_H

/// @file GenAig.h
/// @brief GenAig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/AigMgr.h"
#include "ym_logic/Aig.h"
#include "AigPat.h"
#include "FuncXform.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GenAig GenAig.h "GenAig.h"
/// @brief 最小サイズのAIGを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenAig
{
public:

  /// @brief コンストラクタ
  GenAig();

  /// @brief デストラクタ
  ~GenAig();


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

  /// @brief NPN同値類を求める．
  /// @param[in] fv 関数ベクタ
  /// @param[in] aig AIG
  /// @param[in] level レベル
  void
  npn_expand(ymuint32 fv,
	     Aig aig,
	     ymuint32 level);

  /// @brief 関数ベクタを代表関数に変換する(3入力版)
  ymuint32
  cannonical3(ymuint32 func,
	      ymuint8 perm[]);

  /// @brief 関数ベクタを代表関数に変換する(4入力版)
  ymuint32
  cannonical4(ymuint32 func,
	      ymuint8 perm[]);

  /// @brief 関数ベクタを変換する(3入力版)
  ymuint32
  xform_func3(ymuint32 fv,
	      const ymuint8 perm[]);

  /// @brief 関数ベクタを変換する(4入力版)
  ymuint32
  xform_func4(ymuint32 fv,
	      const ymuint8 perm[]);

  /// @brief AIG を変換する(3入力版)
  Aig
  xform3(Aig aig,
	 const ymuint8 perm[]);

  /// @brief xform3 の下請け関数
  Aig
  xf3_sub(Aig aig,
	  const ymuint8 perm[]);

  /// @brief AIG を変換する(4入力版)
  Aig
  xform4(Aig aig,
	 const ymuint8 perm[]);

  /// @brief xform4 の下請け関数
  Aig
  xf4_sub(Aig aig,
	  const ymuint8 perm[]);

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
	  AigPat aig2,
	  ymuint level_base);

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

  /// @brief aig の子供に印をつけてノード数を数える．
  ymuint
  count1(Aig aig);

  /// @brief count1 で印のついていないノード数を数える．
  ymuint
  count2(Aig aig);

  /// @brief count2 の下請け関数
  ymuint
  count2_sub(Aig aig,
	     hash_set<Aig>& hash);

  /// @brief 3入力関数の情報をセットアップする．
  void
  init_npn3rep();

  /// @brief 4入力関数の情報をセットアップする．
  void
  init_npn4rep();

  /// @brief AigPat の内容が正しいか調べる．
  bool
  sanity_check(AigPat aigpat);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NPN同値類代表関数を表すハッシュ表
  hash_map<ymuint32, vector<FuncXform> > mNpnHash;

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

  // パタンの求まっていない代表関数の数
  ymuint32 mRemainRep;


  //////////////////////////////////////////////////////////////////////
  // AIG モード用の変数
  //////////////////////////////////////////////////////////////////////

  // レベルごとのAigPat のリスト
  vector<vector<AigPat> > mAigList;

  // レベルごとの代表関数のAigPatのリスト
  vector<vector<AigPat> > mRepAigList;

  // AigPat の候補のリスト
  vector<vector<AigPat> > mCandListArray;

  // 関数ごとのパタンのリストを記録する配列
  vector<vector<AigPat> > mFuncArray;

  // 関数の最小レベルを記録する配列
  vector<ymuint32> mFuncLevel;

  // 最小レベルからの乖離を表すパラメータ
  ymuint32 mSlack;

  // 登録されている AIG のハッシュ
  hash_set<Aig> mAigHash;

  // count 用の AIG のハッシュ
  hash_set<Aig> mCountHash;

};

END_NAMESPACE_YM

#endif // GENAIG_H
