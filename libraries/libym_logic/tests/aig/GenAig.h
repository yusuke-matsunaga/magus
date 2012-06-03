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

// 一つの関数に対するAIGリスト
// ただし最小レベルに等しいAIGのみを保持する．
struct AigList
{
  // 最小レベル
  ymuint32 mMinLevel;

  // AIGのリスト
  vector<Aig> mList;

  /// @brief コンストラクタ
  AigList() :
    mMinLevel(0xffffffff)
  {
  }

  /// @brief 追加する．
  /// @param[in] level レベル
  /// @param[in] aig AIG
  /// @retval true 最初のパタンだった．
  /// @retval false 他のパタンが登録されていた．
  bool
  push_back(ymuint32 level,
	    Aig aig)
  {
    bool ans = mList.empty();
    if ( mMinLevel > level ) {
      mMinLevel = level;
      mList.clear();
    }
    if ( mMinLevel == level ) {
      mList.push_back(aig);
    }
    return ans;
  }

};


/// @brief AIG の対を表すクラス
struct AigPair
{
  AigPair(Aig aig1,
	  Aig aig2,
	  ymuint32 func,
	  ymuint size) :
    mAig1(aig1),
    mAig2(aig2),
    mFunc(func),
    mSize(size)
  {
  }

  Aig mAig1;
  Aig mAig2;
  ymuint32 mFunc;
  ymuint32 mSize;
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
  operator()(ymuint ni);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つのから新しいパタンを合成する．
  void
  compose(ymuint32 func1,
	  ymuint32 func2,
	  ymuint level);

  /// @brief パタンを登録する．
  /// @param[in] fv 関数ベクタ
  /// @param[in] aig AIG
  /// @param[in] level レベル
  void
  add_pat(ymuint32 fv,
	  Aig aig,
	  ymuint32 level);

  /// @brief パタンを登録する．
  /// @param[in] fv 関数ベクタ
  /// @param[in] aig AIG
  /// @param[in] level レベル
  void
  add_pat1(ymuint32 fv,
	   Aig aig,
	   ymuint32 level);

  /// @brief AIG の対を登録する．
  /// @note 結果は mAigPairListArray に追加される．
  void
  add_aigpair(Aig aig1,
	      Aig aig2,
	      ymuint32 func);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 3入力のNPN同値類代表関数を表すハッシュ表
  hash_set<ymuint32> mNpn3Hash;

  // 4入力のNPN同値類代表関数を表すハッシュ表
  hash_set<ymuint32> mNpn4Hash;

  // AIGMGR
  AigMgr mMgr;

  // 現在の入力数
  ymuint32 mNi;

  // 関数ベクタの要素数
  ymuint32 mNp;

  // 関数ベクタ用のマスク
  ymuint32 mMask;

  // パタンの求まっていない関数の数．
  ymuint32 mRemainFunc;

  // 関数ベクタをキーとして AIG のリストを保持する配列
  vector<AigList> mFuncTable;

  // 全ての AIG のリスト
  vector<Aig> mAigList;

  // レベルごとのAIGの対のリスト
  vector<vector<AigPair> > mAigPairListArray;

};

END_NAMESPACE_YM

#endif // GENAIG_H
