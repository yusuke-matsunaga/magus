#ifndef FSIMNEW2_H
#define FSIMNew2_H

/// @file FsimNew2.h
/// @brief FsimNew2 のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "fsim2_nsdef.h"
#include "FsimNew.h"
#include "PackedVal.h"
#include "EventQ.h"
#include "FsimFault.h"


BEGIN_NAMESPACE_YM_SATPG_FSIM2

class SimFFR;
class SimNode;

//////////////////////////////////////////////////////////////////////
/// @class FsimNew2 FsimNew2.h "FsimNew2.h"
/// @brief 故障シミュレーションを行うモジュール
//////////////////////////////////////////////////////////////////////
class FsimNew2 :
  public FsimNew
{
public:

  /// @brief コンストラクタ
  FsimNew2();

  /// @brief デストラクタ
  virtual
  ~FsimNew2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  /// @param[in] network ネットワーク
  /// @param[in] fault_mgr 故障マネージャ
  virtual
  void
  set_network(const TpgNetwork& network,
	      FaultMgr& fault_mgr);

  /// @brief 故障にスキップマークをつける．
  virtual
  void
  set_skip(TpgFault* f);

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(TestVector* tv,
	TpgFault* f);

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] fault_list 対象の故障のリスト
  /// @param[in] det_faults このパタンで検出された故障のリスト
  virtual
  void
  sppfp(TestVector* tv,
	const vector<TpgFault*>& fault_list,
	vector<TpgFault*>& det_faults);

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] fault_list 対象の故障のリスト
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	const vector<TpgFault*>& fault_list,
	FsimOp& op);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 1ワード分の ppsfp
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
  void
  _ppsfp(const vector<TestVector*>& tv_array,
	 FsimOp& op);

  /// @brief FFR 内の故障シミュレーションを行う．
  PackedVal
  ffr_simulate(SimFFR* ffr);

  /// @brief イベントキューを用いてシミュレーションを行う．
  PackedVal
  eventq_simulate();

  /// @brief ffr 内の故障が検出可能か調べる．
  void
  fault_sweep(SimFFR* ffr,
	      vector<TpgFault*>& det_faults);


private:
  //////////////////////////////////////////////////////////////////////
  // TgNode と SimNode の対応関係を表すためのデータ構造
  //////////////////////////////////////////////////////////////////////

  struct EdgeMap {
    SimNode* mNode;
    ymuint32 mPos;
  };


private:

  /// @brief 現在保持している SimNode のネットワークを破棄する．
  /// 内部で clear_faults() を呼ぶ．
  void
  clear();

  /// @brief FsimFault を削除する．
  void
  clear_faults();

  /// @brief FsimFault を設定する．
  void
  set_faults(const vector<TpgFault*>& fault_list);

  /// @brief node に対応する SimNode を得る．
  SimNode*
  find_simnode(const TpgNode* node) const;

  /// @brief node の pos 番めの入力に対応する枝を得る．
  void
  find_simedge(const TpgNode* node,
	       ymuint pos,
	       SimNode*& simnode,
	       ymuint& ipos) const;

  /// @brief 外部入力ノードを作る．
  SimNode*
  make_input();

  /// @brief プリミティブに対応したノードを作る．
  /// @param[in] prim プリミティブ
  /// @param[in] inputs もとのノードの入力の SimNode
  /// @param[in] emap もとのノードの枝の対応関係を記録する配列
  /// @note inputs のサイズはノードの入力数 x 2
  SimNode*
  make_primitive(const TpgPrimitive* prim,
		 const vector<SimNode*>& inputs,
		 const vector<EdgeMap*>& emap);

  /// @brief logic ノードを作る．
  SimNode*
  make_node(tTgGateType type,
	    const vector<SimNode*>& inputs);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対象のネットワーク
  const TpgNetwork* mNetwork;

  // TpgNode の id をキーにして SimNode を入れる配列
  vector<SimNode*> mSimMap;

  // TpgNode の id と入力位置番号から枝の情報を取り出す配列
  vector<vector<EdgeMap> > mEdgeMap;

  // 全ての SimNode を納めた配列
  vector<SimNode*> mNodeArray;

  // 外部入力に対応する SimNode を納めた配列
  vector<SimNode*> mInputArray;

  // 外部出力に対応する SimNode を納めた配列
  vector<SimNode*> mOutputArray;

  // 入力からのトポロジカル順に並べた logic ノードの配列
  vector<SimNode*> mLogicArray;

  // FFR を納めた配列
  vector<SimFFR> mFFRArray;

  // イベントキュー
  EventQ mEventQ;

  // 値を消去する必要のあるノードを入れておく配列
  vector<SimNode*> mClearArray;

  // 故障シミュレーション用の故障の配列
  vector<FsimFault> mFsimFaults;

  // TpgFault::id() をキーとして FsimFault を格納する配列
  vector<FsimFault*> mFaultArray;

};

END_NAMESPACE_YM_SATPG_FSIM2

#endif // FSIMNEW2_H
