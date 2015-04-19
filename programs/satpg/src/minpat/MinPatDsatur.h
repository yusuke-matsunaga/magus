#ifndef MINPATDSATUR_H
#define MINPATDSATUR_H

/// @file MinPatDsatur.h
/// @brief MinPatDsatur のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "MinPatBase.h"
#include "FaultAnalyzer.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatDsatur MinPatDsatur.h "MinPatDsatur.h"
/// @brief 'Dsatur' っぽい MinPat
//////////////////////////////////////////////////////////////////////
class MinPatDsatur :
  public MinPatBase
{
public:

  /// @brief コンストラクタ
  MinPatDsatur();

  /// @brief デストラクタ
  ~MinPatDsatur();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化を行う．
  /// @param[in] network 対象のネットワーク
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] fault_list 検出された故障のリスト
  virtual
  void
  init(TpgNetwork& network,
       TvMgr& tvmgr,
       Fsim& fsim2,
       vector<TpgFault*>& fault_list);

  /// @brief 対象の全故障数を返す．
  virtual
  ymuint
  fault_num();

  /// @brief 最初の故障を選ぶ．
  virtual
  TpgFault*
  get_first_fault();

  /// @brief 次に処理すべき故障を選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] group_list 現在のグループリスト
  ///
  /// 故障が残っていなければ NULL を返す．
  virtual
  TpgFault*
  get_next_fault(FgMgr& fgmgr,
		 const vector<ymuint>& group_list);

  /// @brief 故障を追加するグループを選ぶ．
  /// @param[in] fgmgr 故障グループを管理するオブジェクト
  /// @param[in] fault 故障
  /// @param[in] group_list 現在のグループリスト
  ///
  /// グループが見つからなければ fgmgr.group_num() を返す．
  virtual
  ymuint
  find_group(FgMgr& fgmgr,
	     TpgFault* fault,
	     const vector<ymuint>& group_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障ごとの情報
  struct FaultStruct
  {
    // 故障
    TpgFault* mFault;

    // 検出パタン数
    ymuint mPatNum;

    // 選択済みマーク
    bool mSelected;

    // 衝突数
    ymuint mConflictNum;

    // 衝突したグループ番号のマップ
    vector<bool> mConflictMap;

    // ペンディング数
    ymuint mPendingNum;

    // ペンディングしているグループ番号のマップ
    vector<bool> mPendingMap;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 故障数
  ymuint mFaultNum;

  // 故障用の作業領域のリスト
  vector<FaultStruct> mFaultStructList;

  // 故障番号から FaultStruct の位置を引くためのマップ
  vector<ymuint> mFaultMap;

  // 未処理の故障数
  ymuint mRemainNum;

  // 前回更新されたグループ番号
  ymuint mPrevGid;

  ymuint mSimpleConfNum;
  ymuint mSatConfNum;
  ymuint mCompatNum;
};

END_NAMESPACE_YM_SATPG

#endif // MINPATDSATUR_H
