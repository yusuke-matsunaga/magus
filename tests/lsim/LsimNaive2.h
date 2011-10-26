#ifndef LSIMNAIVE2_H
#define LSIMNAIVE2_H

/// @file LsimNaive2.h
/// @brief LsimNaive2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimNaive2 LsimNaive2.h "LsimNaive2.h"
/// @brief 単純な Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimNaive2 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimNaive2();

  /// @brief デストラクタ
  virtual
  ~LsimNaive2();


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
  eval(const vector<ymuint64>& iv,
       vector<ymuint64>& ov);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct SimNode
  {
    // 値
    ymuint64 mVal;

    // ファンイン
    SimNode* mFanins[2];

    // タイプ
    ymuint64 mType;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const BdnMgr* mBdnMgr;

  // 入力ノードの配列
  vector<SimNode> mInputList;

  // 論理ノードの配列
  vector<SimNode> mNodeList;

  // 出力ノードの配列
  vector<SimNode*> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMNAIVE2_H
