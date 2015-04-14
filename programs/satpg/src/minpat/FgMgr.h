#ifndef FGMGR_H
#define FGMGR_H

/// @file FgMgr.h
/// @brief FgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "FaultInfo.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FgMgr FgMgr.h "FgMgr.h"
/// @brief fault group manager
///
/// 故障グループを管理するクラス
//////////////////////////////////////////////////////////////////////
class FgMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値 + 1
  FgMgr(ymuint max_node_id);

  /// @brief デストラクタ
  ~FgMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief 現在のグループ数を返す．
  ymuint
  group_num() const;

  /// @brief 新しいグループを作る．
  /// @return グループ番号を返す．
  ymuint
  new_group();

  /// @brief 既存のグループに故障を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fault 故障
  void
  add_fault(ymuint gid,
	    TpgFault* fault);

  /// @brief 故障リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  const vector<TpgFault*>&
  fault_list(ymuint gid) const;

  /// @brief 十分割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  const NodeValList&
  suf_list(ymuint gid) const;

  /// @brief テストパタンを作る．
  /// @param[in] gid グループ番号
  /// @param[in] network ネットワーク
  /// @param[in] tv テストベクタ
  void
  make_testvector(ymuint gid,
		  TpgNetwork& network,
		  TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障グループを表す構造体
  struct FaultGroup
  {
    // グループ番号
    ymuint mId;

    // 故障リスト
    vector<TpgFault*> mFaultList;

    // 十分割当リスト
    NodeValList mSufList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxNodeId;

  // 故障グループの配列
  vector<FaultGroup*> mGroupList;

};

END_NAMESPACE_YM_SATPG

#endif // FGMGR_H
