#ifndef LSIMNAIVE_H
#define LSIMNAIVE_H

/// @file LsimNaive.h
/// @brief LsimNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimNaive LsimNaive.h "LsimNaive.h"
/// @brief 単純な Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimNaive :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimNaive();

  /// @brief デストラクタ
  virtual
  ~LsimNaive();


public:
  //////////////////////////////////////////////////////////////////////
  // Lsim の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] bdn 対象のネットワーク
  virtual
  void
  set_network(const BdnMgr& bdn);

  /// @brief 論理シミュレーションを行う．
  /// @param[in] iv 入力ベクタ
  /// @param[out] ov 出力ベクタ
  virtual
  void
  eval(const vector<ymuint32>& iv,
       vector<ymuint32>& ov);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const BdnMgr* mBdnMgr;

  // 入力ノードの配列
  vector<BdnNode*> mInputList;

  // 論理ノードの配列
  vector<BdnNode*> mNodeList;

  // 出力ノードの配列
  vector<BdnNode*> mOutputList;

  // 値を入れておく配列
  // node->id() をキーにする．
  vector<ymuint32> mValArray;

};

END_NAMESPACE_YM

#endif // LSIMNAIVE_H
