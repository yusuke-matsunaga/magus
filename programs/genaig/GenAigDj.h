#ifndef GENAIGDJ_H
#define GENAIGDJ_H

/// @file GenAigDj.h
/// @brief GenAigDj のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "YmLogic/AigMgr.h"
#include "YmLogic/Aig.h"
#include "AigPat.h"


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

  // 最小レベル
  ymuint32 mMinLevel;

  // パタンリスト
  vector<Pat> mList;

};


//////////////////////////////////////////////////////////////////////
/// @class GenAigDj GenAigDj.h "GenAigDj.h"
/// @brief 最小サイズのAIGを求めるためのクラス
//////////////////////////////////////////////////////////////////////
class GenAigDj
{
public:

  /// @brief コンストラクタ
  GenAigDj();

  /// @brief デストラクタ
  ~GenAigDj();


public:

  /// @brief ni 入力の全ての関数のAIGを求める．
  void
  operator()(ymuint ni);

  /// @brief 関数ベクタに対応するレベルを返す．
  ymuint
  level(ymuint32 func) const;


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
  // FF モードの用の変数
  //////////////////////////////////////////////////////////////////////

  // 関数ベクタをキーにしてパタンリストを保持する配列
  vector<PatList> mFuncTable;

  // レベルごとの関数のリスト
  vector<vector<ymuint32> > mFuncListArray;

};

END_NAMESPACE_YM

#endif // GENAIG_H
