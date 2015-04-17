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

  /// @brief 故障を取り除く
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fault_list 故障リスト
  void
  delete_fault(ymuint gid,
	       const vector<TpgFault*>& fault_list);

  /// @brief 故障を支配しているグループを求める．
  /// @param[in] fault 故障
  ///
  /// 見つからない場合には group_num() を返す．
  ymuint
  find_dom_group(TpgFault* fault);

  /// @brief 単一の故障に対する支配関係をチェックする．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fault 故障
  /// @return gid のグループ内の故障で fault を支配しているものがあったら true を返す．
  bool
  check_fault_dominance(ymuint gid,
			TpgFault* fault);

  /// @brief 故障を追加することのできるグループを求める．
  /// @param[in] fault 故障
  ///
  /// 見つからない場合には group_num() を返す．
  ymuint
  find_group(TpgFault* fault);

  /// @brief 故障グループの圧縮を行う．
  /// @param[out] group_list グループ番号のリスト
  void
  compaction(vector<ymuint>& group_list);

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
    /// @brief 故障を追加する．
    void
    add_fault(TpgFault* fault,
	      const NodeValList& suf_list)
    {
      mFaultList.push_back(fault);
      mFaultSufList.push_back(suf_list);
      mSufList.merge(suf_list);
    }

    // グループ番号
    ymuint mId;

    // 故障リスト
    vector<TpgFault*> mFaultList;

    // 故障ごとの十分割当リスト
    vector<NodeValList> mFaultSufList;

    // 十分割当リスト
    NodeValList mSufList;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief phase-1
  /// @param[inout] group_list 選択されたグループ番号のリスト
  ///
  /// 他のグループに移動させることでグループを削除する．
  void
  phase1(vector<ymuint>& group_list);

  /// @brief phase-2
  /// @param[inout] group_list 選択されたグループ番号のリスト
  ///
  /// 要素数の少ないグループの故障を他のグループに移動する．
  void
  phase2(vector<ymuint>& group_list);


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
