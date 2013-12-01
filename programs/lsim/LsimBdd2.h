#ifndef LSIMBDD2_H
#define LSIMBDD2_H

/// @file LsimBdd2.h
/// @brief LsimBdd2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimBdd2 LsimBdd2.h "LsimBdd2.h"
/// @brief BDD を用いた Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimBdd2 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimBdd2();

  /// @brief デストラクタ
  virtual
  ~LsimBdd2();


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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  ympuint
  make_node(Bdd bdd,
	    hash_map<Bdd, ympuint>& node_map);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct Bdd2Node {
    Bdd2Node(VarId id0,
	     VarId id1,
	     ympuint node00,
	     ympuint node01,
	     ympuint node10,
	     ympuint node11)
    {
      mId[0] = id0;
      mId[1] = id1;
      mFanins[0] = node00;
      mFanins[1] = node01;
      mFanins[2] = node10;
      mFanins[3] = node11;
    }

    VarId mId[2];
    ympuint mFanins[4];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // ノードの配列
  vector<Bdd2Node*> mNodeList;

  // 出力のノードの配列
  vector<ympuint> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD2_H
