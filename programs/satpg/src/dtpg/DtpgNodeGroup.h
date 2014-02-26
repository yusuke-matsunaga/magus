#ifndef DTPGNODEGROUP_H
#define DTPGNODEGROUP_H

/// @file DtpgNodeGroup.h
/// @brief DtpgNodeGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

class DtpgNgEdge;

//////////////////////////////////////////////////////////////////////
/// @class DtpgNodeGroup DtpgNodeGroup.h "DtpgNodeGroup.h"
/// @brief ノードのかたまりを表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgNodeGroup
{
  friend class DtpgNgMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] node_list ノードのリスト
  DtpgNodeGroup(const vector<TpgNode*>& node_list);

  /// @brief デストラクタ
  ~DtpgNodeGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief ノードのリストを返す．
  const vector<TpgNode*>&
  node_list() const;

  /// @brief ノードリストのサイズを返す．
  ymuint
  size() const;

  /// @brief ファンインのリスト中に src_node があるかチェックする．
  bool
  check_fanin(DtpgNodeGroup* src_node) const;

  /// @brief ファンアウトのリスト中に dst_node があるかチェックする．
  bool
  check_fanout(DtpgNodeGroup* dst_node) const;

  /// @brief ファンインのリストを返す．
  const vector<DtpgNgEdge*>&
  fanin_list() const;

  /// @brief ファンインのリストを返す．
  vector<DtpgNgEdge*>&
  fanin_list();

  /// @brief ファンアウトのリストを返す．
  const vector<DtpgNgEdge*>&
  fanout_list() const;

  /// @brief ファンアウトのリストを返す．
  vector<DtpgNgEdge*>&
  fanout_list();

  /// @brief ファンインを削除する．
  void
  delete_fanin(DtpgNgEdge* edge);

  /// @brief ファンアウトを削除する．
  void
  delete_fanout(DtpgNgEdge* edge);

  /// @brief 内部状態のチェックを行う．
  void
  sanity_check() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // ノードのリスト
  vector<TpgNode*> mNodeList;

  // ファンインのリスト
  vector<DtpgNgEdge*> mFaninList;

  // ファンアウトのリスト
  vector<DtpgNgEdge*> mFanoutList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を返す．
inline
ymuint
DtpgNodeGroup::id() const
{
  return mId;
}

// @brief ノードのリストを返す．
inline
const vector<TpgNode*>&
DtpgNodeGroup::node_list() const
{
  return mNodeList;
}

// @brief ノードリストのサイズを返す．
inline
ymuint
DtpgNodeGroup::size() const
{
  return mNodeList.size();
}

// @brief ファンインのリストを返す．
inline
const vector<DtpgNgEdge*>&
DtpgNodeGroup::fanin_list() const
{
  return mFaninList;
}

// @brief ファンインのリストを返す．
inline
vector<DtpgNgEdge*>&
DtpgNodeGroup::fanin_list()
{
  return mFaninList;
}

// @brief ファンアウトのリストを返す．
inline
const vector<DtpgNgEdge*>&
DtpgNodeGroup::fanout_list() const
{
  return mFanoutList;
}

// @brief ファンアウトのリストを返す．
inline
vector<DtpgNgEdge*>&
DtpgNodeGroup::fanout_list()
{
  return mFanoutList;
}

END_NAMESPACE_YM_SATPG

#endif // DTPGNODEGROUP_H
