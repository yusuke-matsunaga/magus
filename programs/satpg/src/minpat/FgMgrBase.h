#ifndef FGMGRBASE_H
#define FGMGRBASE_H

/// @file FgMgrBase.h
/// @brief FgMgrBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgr.h"
#include "NodeValList.h"
#include "YmUtils/HashSet.h"


BEGIN_NAMESPACE_YM_SATPG

class FaultAnalyzer;
class FaultInfo;

//////////////////////////////////////////////////////////////////////
/// @class FgMgrBase FgMgrBase.h "FgMgrBase.h"
/// @brief fault group manager
///
/// 故障グループを管理するクラス
//////////////////////////////////////////////////////////////////////
class FgMgrBase :
  public FgMgr
{
protected:

  class FaultGroup;

public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値 + 1
  /// @param[in] analyzer 故障解析器
  FgMgrBase(ymuint max_node_id,
	    const FaultAnalyzer& analyzer);

  /// @brief デストラクタ
  ~FgMgrBase();


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
  /// @param[in] fid 故障番号
  /// @return グループ番号を返す．
  ///
  /// fid のみを要素に持つ．
  virtual
  ymuint
  new_group(ymuint fid);

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

  /// @brief 故障を取り除く
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fid_list 削除する故障番号のリスト
  virtual
  void
  delete_faults(ymuint gid,
		const vector<ymuint>& fid_list);

  /// @brief グループの故障数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  ymuint
  fault_num(ymuint gid) const;

  /// @brief グループの故障を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] pos ( 0 <= pos < fault_num(gid) )
  /// @return 故障番号を返す．
  virtual
  ymuint
  fault_id(ymuint gid,
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


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief 新しいグループを作る．
  /// @return グループ番号を返す．
  ymuint
  _new_group();

  /// @brief 既存のグループに故障を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fid 故障番号
  virtual
  void
  add_fault(ymuint gid,
	    ymuint fid) = 0;

  /// @brief 故障を返す．
  /// @param[in] fid 故障番号
  const TpgFault*
  fault(ymuint fid) const;

  /// @brief 故障の解析情報を返す．
  /// @param[in] fid 故障番号
  const FaultInfo&
  fault_info(ymuint fid) const;

  /// @brief 故障に関係するノード集合を返す．
  /// @param[in] fid 故障番号
  const NodeSet&
  node_set(ymuint fid) const;

  /// @brief 故障グループを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  FaultGroup*
  fault_group(ymuint gid);

  /// @brief 故障グループを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  const FaultGroup*
  fault_group(ymuint gid) const;

  /// @brief 両立キャッシュに登録する．
  void
  add_compat_cache(ymuint gid,
		   ymuint fid);

  /// @brief 両立キャッシュを調べる．
  bool
  check_compat_cache(ymuint gid,
		     ymuint fid);

  /// @brief 衝突キャッシュに登録する
  void
  add_conflict_cache(ymuint gid,
		     ymuint fid);

  /// @brief 衝突キャッシュを調べる．
  bool
  check_conflict_cache(ymuint gid,
		       ymuint fid);


protected:
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

    /// @brief グループID番号を返す．
    ymuint
    id() const;

    /// @brief 故障数を返す．
    ymuint
    fault_num() const;

    /// @brief single cube でない故障数を返す．
    ymuint
    complex_fault_num() const;

    /// @brief 故障番号を返す．
    ymuint
    fault_id(ymuint pos) const;

    /// @brief 十分割当を返す．
    const NodeValList&
    sufficient_assignment() const;

    /// @brief 必要割当を返す．
    const NodeValList&
    mandatory_assignment() const;

    /// @brief 外部入力上の十分割当を返す．
    const NodeValList&
    pi_sufficient_assignment() const;

    /// @brief 両立キャッシュに登録する．
    void
    add_compat_cache(ymuint fid);

    /// @brief 両立キャッシュを調べる．
    bool
    check_compat_cache(ymuint fid) const;

    /// @brief 衝突キャッシュに登録する．
    void
    add_conflict_cache(ymuint fid);

    /// @brief 衝突キャッシュを調べる．
    bool
    check_conflict_cache(ymuint fid) const;

    /// @brief ID番号以外の内容をコピーする
    void
    copy(const FaultGroup& dst);

    /// @brief ID番号をセットする．
    void
    set_id(ymuint id);

    /// @brief 故障を追加する．
    void
    add_fault(ymuint fid,
	      const NodeValList& suf_list,
	      const NodeValList& ma_list,
	      const NodeValList& pi_suf_list);

    /// @brief 故障を削除する．
    void
    delete_faults(const vector<ymuint>& fid_list);

    /// @brief 故障の十分割当リストを設定する．
    void
    set_suf_list(ymuint pos,
		 const NodeValList& suf_list,
		 const NodeValList& pi_suf_list);

    /// @brief 故障リストが変更された時の更新処理を行う．
    void
    update();


  private:
    //////////////////////////////////////////////////////////////////////
    // 内部で用いられるデータ構造
    //////////////////////////////////////////////////////////////////////

    // 故障ごとの情報を表す構造体
    struct FaultData
    {
      // コンストラクタ
      FaultData(ymuint fid,
		const NodeValList& suf_list,
		const NodeValList& ma_list,
		const NodeValList& pi_suf_list);

      // 故障番号
      ymuint mFaultId;

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

    // 両立する故障の集合
    HashSet<ymuint> mCompatCache;

    // 衝突する故障の集合
    HashSet<ymuint> mConflictCache;

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

};

END_NAMESPACE_YM_SATPG

#endif // FGMGRBASE_H
