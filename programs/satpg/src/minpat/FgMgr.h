#ifndef FGMGR_H
#define FGMGR_H

/// @file FgMgr.h
/// @brief FgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


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

  /// @brief デストラクタ
  virtual
  ~FgMgr() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  virtual
  void
  clear() = 0;

  /// @brief 現在のグループ数を返す．
  virtual
  ymuint
  group_num() const = 0;

  /// @brief 新しいグループを作る．
  /// @return グループ番号を返す．
  virtual
  ymuint
  new_group() = 0;

  /// @brief グループを複製する．
  /// @param[in] src_gid 複製元のグループ番号
  /// @return 新しいグループ番号を返す．
  virtual
  ymuint
  duplicate_group(ymuint src_gid) = 0;

  /// @brief グループを置き換える．
  /// @param[in] old_gid 置き換え対象のグループ番号
  /// @param[in] new_gid 置き換えるグループ番号
  ///
  /// new_gid は削除される．
  virtual
  void
  replace_group(ymuint old_gid,
		ymuint new_gid) = 0;

  /// @brief グループを削除する．
  /// @param[in] gid グループ番号
  virtual
  void
  delete_group(ymuint gid) = 0;

  /// @brief 新たな条件なしで追加できる既存グループを見つける．
  /// @param[in] fid 対象の故障番号
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
  /// @param[out] gid_list 対象のグループ番号を収めるリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  /// gid_list は first_hit == true の時，意味を持たない．
  virtual
  ymuint
  find_dom_group(ymuint fid,
		 const vector<ymuint>& group_list,
		 bool first_hit,
		 vector<ymuint>& gid_list) = 0;

  /// @brief 新たな条件なしで追加できる既存グループを見つける．
  /// @param[in] fid 対象の故障番号
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  ymuint
  find_dom_group(ymuint fid,
		 const vector<ymuint>& group_list);

  /// @brief 追加できる既存グループを見つける．
  /// @param[in] fid 対象の故障番号
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @param[in] first_hit 最初のグループのみを求めるとき true にするフラグ
  /// @param[out] gid_list 対象のグループ番号を収めるリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  /// gid_list は first_hit == true の時，意味を持たない．
  virtual
  ymuint
  find_group(ymuint fid,
	     const vector<ymuint>& group_list,
	     bool first_hit,
	     vector<ymuint>& gid_list) = 0;

  /// @brief 追加できる既存グループを見つける．
  /// @param[in] fid 対象の故障番号
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @return 最初のグループ番号を返す．
  ///
  /// 見つからない場合は group_num() を返す．
  ymuint
  find_group(ymuint fid,
	     const vector<ymuint>& group_list);

  /// @brief 既存のグループに故障を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fid 故障番号
  virtual
  void
  add_fault(ymuint gid,
	    ymuint fid) = 0;

  /// @brief 故障を取り除く
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fid_list 削除する故障番号のリスト
  virtual
  void
  delete_faults(ymuint gid,
		const vector<ymuint>& fid_list) = 0;

  /// @brief グループの故障数を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  ymuint
  fault_num(ymuint gid) const = 0;

  /// @brief グループの故障を返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] pos ( 0 <= pos < fault_num(gid) )
  /// @return 故障番号を返す．
  virtual
  ymuint
  fault_id(ymuint gid,
	   ymuint pos) const = 0;

  /// @brief 十分割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  sufficient_assignment(ymuint gid) const = 0;

  /// @brief 必要割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  mandatory_assignment(ymuint gid) const = 0;

  /// @brief 外部入力上の十分割当リストを返す．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  virtual
  const NodeValList&
  pi_sufficient_assignment(ymuint gid) const = 0;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 新たな条件なしで追加できる既存グループを見つける．
// @param[in] fid 対象の故障番号
// @param[in] group_list 探索最小のグループ番号のリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
inline
ymuint
FgMgr::find_dom_group(ymuint fid,
		      const vector<ymuint>& group_list)
{
  vector<ymuint> dummy;
  return find_dom_group(fid, group_list, true, dummy);
}

// @brief 追加できる既存グループを見つける．
// @param[in] fid 対象の故障番号
// @param[in] group_list 探索最小のグループ番号のリスト
// @return 最初のグループ番号を返す．
//
// 見つからない場合は group_num() を返す．
inline
ymuint
FgMgr::find_group(ymuint fid,
		  const vector<ymuint>& group_list)
{
  vector<ymuint> dummy;
  return find_group(fid, group_list, true, dummy);
}

END_NAMESPACE_YM_SATPG

#endif // FGMGR_H
