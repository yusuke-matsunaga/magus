#ifndef FGMGR1_H
#define FGMGR1_H

/// @file FgMgr1.h
/// @brief FgMgr1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgrBase.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FgMgr1 FgMgr1.h "FgMgr1.h"
/// @brief fault group manager
///
/// 故障グループを管理するクラス
//////////////////////////////////////////////////////////////////////
class FgMgr1 :
  public FgMgrBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値 + 1
  /// @param[in] analyzer 故障解析器
  FgMgr1(ymuint max_node_id,
	 const FaultAnalyzer& analyzer);

  /// @brief デストラクタ
  ~FgMgr1();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

#if 0
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
#endif

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

#if 0
  /// @brief グループの故障数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  ymuint
  fault_num(ymuint gid) const;

  /// @brief グループの故障を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] pos ( 0 <= pos < fault_num(gid) )
  virtual
  const TpgFault*
  fault(ymuint gid,
	ymuint pos) const;

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
#endif

#if 0
protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief 故障解析器を返す．
  const FaultAnalyzer&
  analyzer() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 故障グループを表す構造体
  class FaultGroup
  {
  public:

    /// @brief コンストラクタ
    /// @param[in] id ID番号
    FaultGroup(ymuint id);

    /// @brief デストラクタ
    ~FaultGroup();


  public:
    //////////////////////////////////////////////////////////////////////
    // 外部インターフェイス
    //////////////////////////////////////////////////////////////////////

    /// @brief ID番号を返す．
    ymuint
    id() const;

    /// @brief 故障数を返す．
    ymuint
    fault_num() const;

    /// @brief single cube でない故障数を返す．
    ymuint
    complex_fault_num() const;

    /// @brief 故障を返す．
    const TpgFault*
    fault(ymuint pos) const;

    /// @brief 十分割当を返す．
    const NodeValList&
    sufficient_assignment() const;

    /// @brief 必要割当を返す．
    const NodeValList&
    mandatory_assignment() const;

    /// @brief 外部入力上の十分割当を返す．
    const NodeValList&
    pi_sufficient_assignment() const;

    /// @brief ID番号以外の内容をコピーする
    void
    copy(const FaultGroup& dst);

    /// @brief ID番号をセットする．
    void
    set_id(ymuint id);

    /// @brief 故障を追加する．
    void
    add_fault(const TpgFault* fault,
	      bool single_cube,
	      const NodeValList& suf_list,
	      const NodeValList& ma_list,
	      const NodeValList& pi_suf_list);

    /// @brief 故障を削除する．
    void
    delete_faults(const vector<const TpgFault*>& fault_list);


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
		const NodeValList& ma_list,
		const NodeValList& pi_suf_list);

      // 故障
      const TpgFault* mFault;

      // single cube 条件
      bool mSingleCube;

      // 十分割当リスト
      NodeValList mSufList;

      // 必要割当リスト
      NodeValList mMaList;

      // 外部入力上の十分割当リスト
      NodeValList mPiSufList;

    };


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // グループ番号
    ymuint mId;

    // 故障データのリスト
    vector<FaultData> mFaultDataList;

    // single cube でない条件を持つ故障数
    ymuint mCplxNum;

    // 十分割当リスト
    NodeValList mSufList;

    // 必要割当リスト
    NodeValList mMaList;

    // 外部入力の十分割当リスト
    NodeValList mPiSufList;

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

  // 故障解析器
  const FaultAnalyzer& mAnalyzer;

  // 故障グループの配列
  vector<FaultGroup*> mGroupList;
#endif

};

END_NAMESPACE_YM_SATPG

#endif // FGMGR1_H
