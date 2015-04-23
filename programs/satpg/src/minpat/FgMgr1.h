#ifndef FGMGR1_H
#define FGMGR1_H

/// @file FgMgr1.h
/// @brief FgMgr1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr.h"
#include "NodeValList.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FgMgr1 FgMgr1.h "FgMgr1.h"
/// @brief fault group manager
///
/// 故障グループを管理するクラス
//////////////////////////////////////////////////////////////////////
class FgMgr1 :
  public FgMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値 + 1
  FgMgr1(ymuint max_node_id);

  /// @brief デストラクタ
  ~FgMgr1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  virtual
  void
  clear();

  /// @brief 現在のグループ数を返す．
  virtual
  ymuint
  group_num() const;

  /// @brief 新しいグループを作る．
  /// @return グループ番号を返す．
  virtual
  ymuint
  new_group();

  /// @brief グループを複製する．
  /// @param[in] src_gid 複製元のグループ番号
  /// @return 新しいグループ番号を返す．
  virtual
  ymuint
  duplicate_group(ymuint src_gid);

  /// @brief グループを置き換える．
  /// @param[in] old_gid 置き換え対象のグループ番号
  /// @param[in] new_gid 置き換えるグループ番号
  ///
  /// new_gid は削除される．
  virtual
  void
  replace_group(ymuint old_gid,
		ymuint new_gid);

  /// @brief グループを削除する．
  /// @param[in] gid グループ番号
  virtual
  void
  delete_group(ymuint gid);

  /// @brief 新たな条件なしで追加できる既存グループを見つける．
  /// @param[in] fault 対象の故障
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
  /// @param[out] gid_list 対象のグループ番号を収めるリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  /// gid_list は first_hit == true の時，意味を持たない．
  virtual
  ymuint
  find_dom_group(const TpgFault* fault,
		 const vector<ymuint>& group_list,
		 bool first_hit,
		 vector<ymuint>& gid_list);

  /// @brief 追加できる既存グループを見つける．
  /// @param[in] fault 対象の故障
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
  /// @param[out] gid_list 対象のグループ番号を収めるリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  /// gid_list は first_hit == true の時，意味を持たない．
  virtual
  ymuint
  find_group(const TpgFault* fault,
	     const vector<ymuint>& group_list,
	     bool first_hit,
	     vector<ymuint>& gid_list);

  /// @brief 既存のグループに故障を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fault 故障
  virtual
  void
  add_fault(ymuint gid,
	    const TpgFault* fault);

  /// @brief 故障を取り除く
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fault_list 故障リスト
  virtual
  void
  delete_fault(ymuint gid,
	       const vector<const TpgFault*>& fault_list);

  /// @brief グループの故障数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  ymuint
  fault_num(ymuint gid) const;

  /// @brief 故障リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const vector<const TpgFault*>&
  fault_list(ymuint gid) const;

  /// @brief 十分割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  sufficient_assignment(ymuint gid) const;

  /// @brief 必要割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  mandatory_assignment(ymuint gid) const;

  /// @brief 外部入力上の十分割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  pi_sufficient_assignment(ymuint gid) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障ごとの情報を表す構造体
  struct FaultData
  {
    // コンストラクタ
    FaultData(const TpgFault* fault,
	      bool single_cube,
	      const NodeValList& suf_list,
	      const NodeValList& pi_suf_list);

    // 故障
    const TpgFault* mFault;

    // single cube 条件
    bool mSingleCube;

    // 十分割当リスト
    NodeValList mSufList;

    // 外部入力上の十分割当リスト
    NodeValList mPiSufList;

  };

  // 故障グループを表す構造体
  struct FaultGroup
  {
    // コンストラクタ
    FaultGroup();

    /// @brief 故障を追加する．
    void
    add_fault(const TpgFault* fault,
	      const NodeValList& suf_list,
	      const NodeValList& pi_suf_list);

    // 故障数を返す．
    ymuint
    fault_num() const;

    // single cube でない故障数を返す．
    ymuint
    complex_fault_num() const;

    // 故障を返す．
    const TpgFault*
    fault(ymuint pos) const;

    // グループ番号
    ymuint mId;

    // 故障データのリスト
    vector<FaultData> mFaultDataList;

    // single cube でない条件を持つ故障数
    ymuint mCplxNum;

    // 十分割当リスト
    NodeValList mSufList;

    // 外部入力の十分割当リスト
    NodeValList mPiSufList;

    // 必要割当リスト
    NodeValList mMaList;

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

#endif // FGMGR1_H
