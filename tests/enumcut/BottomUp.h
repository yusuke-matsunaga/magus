#ifndef BOTTOMUP_H
#define BOTTOMUP_H

/// @file ButtomUp.h
/// @brief ButtomUp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BottomUp BottomUp.h "BottomUp.h"
/// @brief ボトムアップのカット列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class BottomUp :
  public EnumCut
{
public:

  /// @brief コンストラクタ
  BottomUp();

  /// @brief デストラクタ
  virtual
  ~BottomUp();


public:

  /// @brief カット列挙を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] limit カットサイズの制限
  virtual
  void
  operator()(const BdnMgr& network,
	     ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットを保持しておく配列
  vector<list<vector<ymuint32> > > mCutListArray;

  // カットの入力を保持する作業領域
  ymuint32* mTmpInputs;

  // mTmpInputs の要素数
  ymuint32 mInputNum;

};

END_NAMESPACE_YM_NETWORKS

#endif // BOTTOMUP_H
