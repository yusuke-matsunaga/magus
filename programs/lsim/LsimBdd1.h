#ifndef LSIMBDD1_H
#define LSIMBDD1_H

/// @file LsimBdd1.h
/// @brief LsimBdd1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "logic/Bdd.h"
#include "logic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimBdd1 LsimBdd1.h "LsimBdd1.h"
/// @brief BDD を用いた Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimBdd1 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimBdd1();

  /// @brief デストラクタ
  virtual
  ~LsimBdd1();


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

  struct Bdd1Node {
    Bdd1Node(VarId id,
	     ympuint node0,
	     ympuint node1)
    {
      mId = id;
      mFanins[0] = node0;
      mFanins[1] = node1;
    }

    VarId mId;
    ympuint mFanins[2];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // ノードの配列
  vector<Bdd1Node*> mNodeList;

  // 出力のノードの配列
  vector<ympuint> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD1_H
