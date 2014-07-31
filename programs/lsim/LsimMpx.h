#ifndef LSIMMPX_H
#define LSIMMPX_H

/// @file LsimMpx.h
/// @brief LsimMpx のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "YmLogic/Bdd.h"
#include "YmLogic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimMpx LsimMpx.h "LsimMpx.h"
/// @brief BDD から セレクタ回路を構成する Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimMpx :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimMpx();

  /// @brief デストラクタ
  virtual
  ~LsimMpx();


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
  // 内部で用いる下請け関数
  //////////////////////////////////////////////////////////////////////

  ympuint
  make_mpx(Bdd bdd,
	   unordered_map<Bdd, ympuint>& mpx_map);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct MpxNode
  {
    MpxNode(VarId id,
	    ympuint ptr0,
	    ympuint ptr1)
    {
      mId = id;
      mFanins[0] = ptr0;
      mFanins[1] = ptr1;
    }

    // 変数番号
    VarId mId;

    // ファンイン＋極性
    ympuint mFanins[2];

    // 値
    ymuint64 mVal;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // BDD の管理用オブジェクト
  BddMgr mBddMgr;

  // 入力ノードの配列
  vector<MpxNode> mInputList;

  // MPXノードの配列
  vector<MpxNode> mNodeList;

  // 出力ノードのポインタ配列
  vector<ympuint> mOutputList;

};

END_NAMESPACE_YM

#endif // LSIMBDD_H
