#ifndef LSIMBDD3_H
#define LSIMBDD3_H

/// @file LsimBdd3.h
/// @brief LsimBdd3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "YmLogic/Bdd.h"
#include "YmLogic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimBdd3 LsimBdd3.h "LsimBdd3.h"
/// @brief BDD を用いた Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimBdd3 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimBdd3();

  /// @brief デストラクタ
  virtual
  ~LsimBdd3();


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
	      const unordered_map<string, ymuint>& order_map);

  /// @brief 論理シミュレーションを行う．
  /// @param[in] iv 入力ベクタ
  /// @param[out] ov 出力ベクタ
  virtual
  void
  eval(const vector<ymuint64>& iv,
       vector<ymuint64>& ov);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1つの出力に対する評価を行う．
  ymuint64
  eval_lut(ymuint addr0,
	   const vector<ymuint64>& iv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // 巨大な Look-Up Table
  ymuint32* mLUT;

  // 出力の LUT のエントリポイント
  vector<ymuint32> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD3_H
