#ifndef FGMGR2_H
#define FGMGR2_H

/// @file FgMgr2.h
/// @brief FgMgr2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FgMgrBase.h"
#include "YmUtils/USTime.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FgMgr2 FgMgr2.h "FgMgr2.h"
/// @brief fault group manager
///
/// 故障グループを管理するクラス
//////////////////////////////////////////////////////////////////////
class FgMgr2 :
  public FgMgrBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値 + 1
  /// @param[in] analyzer 故障解析器
  FgMgr2(ymuint max_node_id,
	 const FaultAnalyzer& analyzer);

  /// @brief デストラクタ
  virtual
  ~FgMgr2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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
		 vector<ymuint>& gid_list);

  /// @brief 追加できる既存グループを見つける．
  /// @param[in] fid 対象の故障番号
  /// @param[in] group_list 探索最小のグループ番号のリスト
  /// @param[in] fast 高速ヒューリスティック
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
	     bool fast,
	     bool first_hit,
	     vector<ymuint>& gid_list);

  /// @brief 既存のグループに故障を追加する．
  /// @param[in] gid グループ番号 ( 0 <= gid < group_num() )
  /// @param[in] fid 故障番号
  virtual
  void
  add_fault(ymuint gid,
	    ymuint fid);

  /// @brief 複数故障の検出検査回数
  ymuint
  mfault_num() const;

  /// @brief 複数故障の平均多重度
  double
  mfault_avg() const;

  /// @brief 複数故障の最大値
  ymuint
  mfault_max() const;

  /// @brief チェック回数
  ymuint
  check_count() const;

  /// @brief チェック時間
  USTime
  check_time() const;

  /// @brief 成功回数
  ymuint
  found_count() const;

  /// @brief 統計データをクリアする．
  void
  clear_count();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  ymuint mMnum;

  ymuint mFsum;

  ymuint mFmax;

  ymuint mCheckCount;

  ymuint mFoundCount;

  USTime mCheckTime;

};

END_NAMESPACE_YM_SATPG

#endif // FGMGR2_H
