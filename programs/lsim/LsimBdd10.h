#ifndef LSIMBDD10_H
#define LSIMBDD10_H

/// @file LsimBdd10.h
/// @brief LsimBdd10 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "logic/Bdd.h"
#include "logic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimBdd10 LsimBdd10.h "LsimBdd10.h"
/// @brief BDD を用いた Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimBdd10 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimBdd10();

  /// @brief デストラクタ
  virtual
  ~LsimBdd10();


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

  ympuint
  make_node(Bdd bdd,
	    unordered_map<Bdd, ympuint>& node_map);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Bdd10Node {
    ymuint mId[10];
    ympuint mFanins[1024];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // ノードの配列
  vector<Bdd10Node*> mNodeList;

  // 出力のノードの配列
  vector<ympuint> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD10_H
