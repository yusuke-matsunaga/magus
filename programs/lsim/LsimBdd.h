#ifndef LSIMBDD_H
#define LSIMBDD_H

/// @file LsimBdd.h
/// @brief LsimBdd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"
#include "ym_logic/BddVector.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimBdd LsimBdd.h "LsimBdd.h"
/// @brief BDD を用いた Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimBdd :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimBdd();

  /// @brief デストラクタ
  virtual
  ~LsimBdd();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // 出力のBDDの配列
  BddVector mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD_H
