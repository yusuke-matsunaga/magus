#ifndef LSIMMPX2_H
#define LSIMMPX2_H

/// @file LsimMpx2.h
/// @brief LsimMpx2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Lsim.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LsimMpx2 LsimMpx2.h "LsimMpx2.h"
/// @brief BDD から セレクタ回路を構成する Lsim の実装
//////////////////////////////////////////////////////////////////////
class LsimMpx2 :
  public Lsim
{
public:

  /// @brief コンストラクタ
  LsimMpx2();

  /// @brief デストラクタ
  virtual
  ~LsimMpx2();


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
  // 内部で用いる下請け関数
  //////////////////////////////////////////////////////////////////////

  ympuint
  make_mpx(Bdd bdd,
	   hash_map<Bdd, ympuint>& mpx_map);


public:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct MpxNode
  {
    MpxNode(ymuint id1,
	    ymuint id2,
	    ymuint id3,
	    ympuint ptr00,
	    ympuint ptr01,
	    ympuint ptr10,
	    ympuint ptr11)
    {
      mId[0] = id1;
      mId[1] = id2;
      mId[2] = id3;
      mFanins[0] = ptr00;
      mFanins[1] = ptr01;
      mFanins[2] = ptr10;
      mFanins[3] = ptr11;
    }

    // 変数番号
    ymuint mId[3];

    // ファンイン＋極性
    ympuint mFanins[4];

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

#endif // LSIMMPX2_H
