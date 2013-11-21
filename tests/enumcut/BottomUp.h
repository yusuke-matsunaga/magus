#ifndef BOTTOMUP_H
#define BOTTOMUP_H

/// @file ButtomUp.h
/// @brief ButtomUp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "EnumCut2.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class BottomUp BottomUp.h "BottomUp.h"
/// @brief ボトムアップのカット列挙を行うクラス
//////////////////////////////////////////////////////////////////////
class BottomUp :
  public EnumCut2
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
  operator()(BdnMgr& network,
	     ymuint limit,
	     EnumCutOp2* op);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  mark_cut1(BdnNode* node);

  void
  clear_cut1(BdnNode* node);

  void
  mark_cut2(BdnNode* node);

  void
  clear_cut2(BdnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct NodeInfo
  {
    // カットのリスト
    list<vector<BdnNode*> > mCutList;

    // マーク1
    ymuint8 mMark1;

    // マーク2
    ymuint8 mMark2;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの情報
  vector<NodeInfo> mNodeInfo;

  // カットの入力を保持する作業領域
  BdnNode** mTmpInputs;

  // mTmpInputs の要素数
  ymuint32 mInputNum;

};

END_NAMESPACE_YM_NETWORKS

#endif // BOTTOMUP_H
