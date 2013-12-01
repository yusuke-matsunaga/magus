#ifndef LSIMNAIVE3_H
#define LSIMNAIVE3_H

/// @file LsimNaive3.h
/// @brief LsimNaive3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimNaive3 LsimNaive3.h "LsimNaive3.h"
/// @brief 単純な Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimNaive3 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimNaive3();

  /// @brief デストラクタ
  virtual
  ~LsimNaive3();


public:
  //////////////////////////////////////////////////////////////////////
  // Lsim の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] bdn 対象のネットワーク
  /// @param[in] order_map 順序マップ
  virtual
  void
  set_network(const BdnMgr& bdn,
	      const hash_map<string, ymuint>& order_map);

  /// @brief 論理シミュレーションを行う．
  /// @param[in] iv 入力ベクタ
  /// @param[out] ov 出力ベクタ
  virtual
  void
  eval(const vector<ymuint64>& iv,
       vector<ymuint64>& ov);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct SimNode
  {
    // 値
    ymuint64 mVal;

    // ファンイン
    ympuint mFanins[2];

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
  vector<ympuint> mOutputList;
};

END_NAMESPACE_YM

#endif // LSIMNAIVE3_H
