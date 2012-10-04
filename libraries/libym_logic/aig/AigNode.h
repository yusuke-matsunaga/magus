#ifndef AIGNODE_H
#define AIGNODE_H

/// @file AigNode.h
/// @brief AigNodeのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Aig.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
/// @class AigNode AigNode.h "AigNode.h"
/// @brief ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class AigNode
{
  friend class AigMgrImpl;

private:
  // このクラスは AigMgrImpl 内でしか生成／破壊できない．

  /// @brief コンストラクタ
  /// @param[in] id ノード番号
  AigNode(ymuint id);

  /// @brief デストラクタ
  ~AigNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 型とID番号に関する情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外部入力ノードのとき true を返す．
  bool
  is_input() const;

  /// @brief ANDノードのとき true を返す．
  bool
  is_and() const;

  /// @brief 通し番号を得る．
  ymuint
  node_id() const;

  /// @brief 外部入力ノードのときの入力番号を返す．
  /// @note is_input() の時のみ意味を持つ．
  VarId
  input_id() const;


public:
  //////////////////////////////////////////////////////////////////////
  // ファンインに関する情報を得る関数
  //////////////////////////////////////////////////////////////////////
#if 0
  /// @brief pos で指示されたファンインのノードを得る．
  /// @note pos は 0 か 1 でなければならない．
  AigNode*
  fanin_node(ymuint pos) const;

  /// @brief fanin0 のノードを得る．
  AigNode*
  fanin0_node() const;

  /// @brief fanin1 のノードを得る．
  AigNode*
  fanin1_node() const;
#endif

  /// @brief pos で指示されたファンインの極性を得る．
  /// @note pos は 0 か 1 でなければならない．
  bool
  fanin_inv(ymuint pos) const;

  /// @brief fanin0 の極性を得る．
  bool
  fanin0_inv() const;

  /// @brief fanin1 の極性を得る．
  bool
  fanin1_inv() const;

  /// @brief pos で指示されたファンインのハンドルを得る．
  /// @note pos は 0 か 1 でなければならない．
  Aig
  fanin(ymuint pos) const;

  /// @brief fanin0 のハンドルを得る．
  Aig
  fanin0() const;

  /// @brief fanin1 のハンドルを得る．
  Aig
  fanin1() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力タイプに設定する．
  /// @param[in] input_id 入力番号
  void
  set_input(VarId input_id);

  /// @brief ANDタイプに設定する．
  /// @param[in] fanin0, fanin1 ファンインのハンドル
  void
  set_and(Aig fanin0,
	  Aig fanin1);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 通し番号 + ノードタイプ
  ymuint32 mFlags;

  // ファンインの枝の配列
  Aig mFanins[2];

  // ハッシュ用のリンクポインタ
  AigNode* mLink;


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 型
  static
  const int kSftT = 0;

  // id 番号
  static
  const int kSftN = 1;

};


//////////////////////////////////////////////////////////////////////
// AigNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief 入力ノードのとき true を返す．
inline
bool
AigNode::is_input() const
{
  return ((mFlags >> kSftT) & 1U) == 1U;
}

// @brief ANDノードのとき true を返す．
inline
bool
AigNode::is_and() const
{
  return !is_input();
}

// 通し番号を得る．
inline
ymuint
AigNode::node_id() const
{
  return static_cast<ymuint>(mFlags >> kSftN);
}

// @brief 外部入力ノードのときの入力番号を返す．
// @note is_input() の時のみ意味を持つ．
inline
VarId
AigNode::input_id() const
{
  return VarId(mFanins[0].mPackedData);
}

#if 0
// pos で指示されたファンインのノードを得る．
// pos は 0 か 1 でなければならない．
inline
AigNode*
AigNode::fanin_node(ymuint pos) const
{
  return mFanins[pos & 1U].node();
}

// fanin0 のノードを得る．
inline
AigNode*
AigNode::fanin0_node() const
{
  return mFanins[0].node();
}

// fanin1 のノードを得る．
inline
AigNode*
AigNode::fanin1_node() const
{
  return mFanins[1].node();
}
#endif

// pos で指示されたファンインの極性を得る．
// pos は 0 か 1 でなければならない．
inline
bool
AigNode::fanin_inv(ymuint pos) const
{
  return mFanins[pos & 1U].inv();
}

// fanin0 の極性を得る．
inline
bool
AigNode::fanin0_inv() const
{
  return mFanins[0].inv();
}

// fanin1 の極性を得る．
inline
bool
AigNode::fanin1_inv() const
{
  return mFanins[1].inv();
}

// @brief pos で指示されたファンインのハンドルを得る．
// @note pos は 0 か 1 でなければならない．
inline
Aig
AigNode::fanin(ymuint pos) const
{
  return mFanins[pos & 1U];
}

// @brief fanin0 のハンドルを得る．
inline
Aig
AigNode::fanin0() const
{
  return mFanins[0];
}

// @brief fanin1 のハンドルを得る．
inline
Aig
AigNode::fanin1() const
{
  return mFanins[1];
}

END_NAMESPACE_YM_AIG

#endif // AIGNODE_H
