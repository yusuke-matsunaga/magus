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


BEGIN_NAMESPACE_YM

/// @brief パタンを表すデータ構造
struct Pat
{
  /// @brief コンストラクタ
  /// @param[in] f0 左の子供の関数ベクタ
  /// @param[in] f1 右の子供の関数ベクタ
  /// @param[in] tv 自身の関数ベクタ
  Pat(ymuint32 f0,
      ymuint32 f1,
      ymuint32 tv) :
    mF0(f0),
    mF1(f1),
    mTv(tv)
  {
  }

  ymuint32 mF0;
  ymuint32 mF1;
  ymuint32 mTv;
};


// 一つの関数に対するパタンリスト
// ただし最小レベルに等しいパタンのみを保持する．
struct PatList
{
  // 最小レベル
  ymuint32 mMinLevel;

  // パタンリスト
  vector<Pat> mList;

  /// @brief コンストラクタ
  PatList() :
    mMinLevel(0xffffffff)
  {
  }

  /// @brief 追加する．
  /// @param[in] level レベル
  /// @param[in] f0 左の子供の関数ベクタ
  /// @param[in] f1 右の子供の関数ベクタ
  /// @param[in] tv 自身の関数ベクタ
  /// @retval true 最初のパタンだった．
  /// @retval false 他のパタンが登録されていた．
  bool
  push_back(ymuint32 level,
	    ymuint32 f0,
	    ymuint32 f1,
	    ymuint32 tv)
  {
    bool ans = mList.empty();
    if ( mMinLevel > level ) {
      mMinLevel = level;
      mList.clear();
    }
    if ( mMinLevel == level ) {
      mList.push_back(Pat(f0, f1, tv));
    }
    return ans;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class AigPat GenAig.h "GenAig.h"
/// @brief AIG とそれに付随する情報を表すクラス
//////////////////////////////////////////////////////////////////////
struct AigPat
{
  // 空のコンストラクタ
  AigPat();

  // コンストラクタ
  AigPat(Aig aig,
	 ymuint32 func,
	 ymuint level);

  // AIG
  Aig mAig;

  // 関数ベクタ
  ymuint32 mFunc;

  // レベル
  ymuint32 mLevel;

};


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

  /// @brief FF モードの関数レベルを計算する．
  void
  ff_mode();

  /// @brief パタンを登録する．
  /// @param[in] fv 関数ベクタ
  /// @param[in] f0 左の子供の関数ベクタ
  /// @param[in] f1 右の子供の関数ベクタ
  /// @param[in] tv 自分自身の関数ベクタ
  /// @param[in] level レベル
  void
  add_pat(ymuint32 fv,
	  ymuint32 f0,
	  ymuint32 f1,
	  ymuint32 tv,
	  ymuint32 level);

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
	  AigPat aig2);

  /// @brief AIG の対を登録する．
  /// @note 結果は mCandListArray に追加される．
  void
  add_aigpair(Aig aig1,
	      Aig aig2,
	      ymuint32 func,
	      ymuint level);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // NPN同値類代表関数を表すハッシュ表
  hash_set<ymuint32> mNpnHash;

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
  // FF モードの用の変数
  //////////////////////////////////////////////////////////////////////

  // 関数ベクタをキーにしてパタンリストを保持する配列
  vector<PatList> mFuncTable;

  // レベルごとの関数のリスト
  vector<vector<ymuint32> > mFuncListArray;


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
  hash_set<Aig> mAigHash;

};

END_NAMESPACE_YM

#endif // GENAIG_H
