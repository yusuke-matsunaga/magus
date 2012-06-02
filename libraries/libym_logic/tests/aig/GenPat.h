#ifndef GENPAT_H
#define GENPAT_H

/// @file GenPat.h
/// @brief GenPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include "ym_logic/LogExpr.h"


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
/// @class GenPat GenPat.h "GenPat.h"
/// @brief 関数の最小サイズを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenPat
{
public:

  /// @brief コンストラクタ
  GenPat();

  /// @brief デストラクタ
  ~GenPat();


public:

  /// @brief ni 入力の全ての関数のパタンを求める．
  void
  operator()(ymuint ni,
	     bool allow_xor = false);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 2つの関数から新しいパタンを合成する．
  void
  compose(ymuint32 func1,
	  ymuint32 func2,
	  ymuint level);

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 3入力のNPN同値類代表関数を表すハッシュ表
  hash_set<ymuint32> mNpn3Hash;

  // 4入力のNPN同値類代表関数を表すハッシュ表
  hash_set<ymuint32> mNpn4Hash;

  // 現在の入力数
  ymuint32 mNi;

  // XOR を許すかどうかのフラグ
  bool mAllowXor;

  // 関数ベクタの要素数
  ymuint32 mNp;

  // 関数ベクタ用のマスク
  ymuint32 mMask;

  // パタンの求まっていない関数の数．
  ymuint32 mRemainFunc;

  // 関数ベクタをキーとしてパタンリストを保持する配列
  vector<PatList> mFuncTable;

  // レベルごとの関数のリスト
  vector<vector<ymuint32> > mFuncListArray;

  // 関数ベクタをキーにしてパタンリストを保持する配列
  vector<vector<LogExpr> > mLogExprTable;

};

END_NAMESPACE_YM

#endif // GENPAT_H
