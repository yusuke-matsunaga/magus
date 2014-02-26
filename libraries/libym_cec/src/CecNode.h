#ifndef LIBYM_CEC_CECNODE_H
#define LIBYM_CEC_CECNODE_H

/// @file libym_cec/CecNode.h
/// @brief CecNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cec/cec_nsdef.h"
#include "CecHandle.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_CEC

//////////////////////////////////////////////////////////////////////
/// @class CecNode CecNode.h "CecNode.h"
/// @brief 組み合わせ回路の等価検証用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class CecNode
{
  friend class CecMgr;

public:

  /// @brief コンストラクタ
  CecNode();

  /// @brief デストラクタ
  ~CecNode();


public:
  //////////////////////////////////////////////////////////////////////
  // ID 番号に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CNF 上の変数番号を返す．
  VarId
  varid() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 入力ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 入力番号を返す．
  ymuint
  input_id() const;

  /// @brief 入力番号をセットする．
  void
  set_input(ymuint id);


public:
  //////////////////////////////////////////////////////////////////////
  // AND ノードの時のアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief AND の時に true を返す．
  bool
  is_and() const;

  /// @brief ファンインを得る．
  CecNode*
  fanin(ymuint pos) const;

  /// @brief 最初のファンインを得る．
  CecNode*
  fanin0() const;

  /// @brief 2番めのファンインを得る．
  CecNode*
  fanin1() const;

  /// @brief ファンインの極性を得る．
  bool
  fanin_inv(ymuint pos) const;

  /// @brief 最初のファンインの極性を得る．
  bool
  fanin0_inv() const;

  /// @brief 2番めのファンインの極性を得る．
  bool
  fanin1_inv() const;

  /// @brief ファンインのハンドルを得る．
  CecHandle
  fanin_handle(ymuint pos) const;

  /// @brief 最初のファンインのハンドルを得る．
  CecHandle
  fanin0_handle() const;

  /// @brief 2番めのファンインのハンドルを得る．
  CecHandle
  fanin1_handle() const;

  /// @brief ファンインをセットする．
  void
  set_fanin(CecHandle handle1,
	    CecHandle handle2);


public:
  //////////////////////////////////////////////////////////////////////
  // 等価グループに関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 削除済みのとき true を返す．
  bool
  check_dmark() const;

  /// @brief 要素数が2以上の等価候補グループの代表なら true を返す．
  bool
  check_rep() const;

  /// @brief 代表ノードを返す．
  CecNode*
  rep_node() const;

  /// @brief 代表ノードに対する極性を返す．
  bool
  rep_inv() const;

  /// @brief 代表ノードを返す．
  CecHandle
  rep_handle() const;

  /// @brief 次の等価候補ノードを得る．
  CecNode*
  next_eqnode();

  /// @brief 等価候補の代表にする．
  void
  set_rep(CecNode* rep_node,
	  bool inv);

  /// @brief 極性反転の印をつける．
  void
  set_rep_inv();

  /// @brief 等価候補ノードを追加する．
  void
  add_eqnode(CecNode* node);

  /// @brief 削除済みの印をつける．
  void
  set_dmark();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CNF 上の変数番号
  VarId mVarId;

  // ファンインのノード
  CecNode* mFanins[2];

  // 0/1マーク，極性などの情報をパックしたもの
  ymuint32 mFlags;

  // 構造ハッシュ用のリンクポインタ
  CecNode* mLink1;

  // 代表ノード情報
  CecNode* mRepNode;

  // 次の等価候補ノード
  CecNode* mEqLink;

  // 等価候補リストの末尾のノード
  CecNode* mEqTail;


private:
  //////////////////////////////////////////////////////////////////////
  // mFlags で用いるシフト定数
  //////////////////////////////////////////////////////////////////////

  // 入力フラグ
  static
  const int kSftI  = 0;

  // ファンイン0 の極性
  static
  const int kSftP0 = 1;

  // ファンイン1 の極性
  static
  const int kSftP1 = 2;

  // 0 になったことがあるかどうか
  static
  const int kSft0  = 3;

  // 1 になったことがあるかどうか
  static
  const int kSft1  = 4;

  // ハッシュパタンの極性
  static
  const int kSftH  = 5;

  // 等価グループ中の極性
  static
  const int kSftP  = 6;

  // 削除マーク
  static
  const int kSftD  = 7;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CecNode::CecNode() :
  mFlags(0),
  mRepNode(this),
  mEqLink(NULL),
  mEqTail(NULL)
{
  mFanins[0] = NULL;
  mFanins[1] = NULL;
}

// @brief デストラクタ
inline
CecNode::~CecNode()
{
}

// @brief CNF 上の変数番号を返す．
inline
VarId
CecNode::varid() const
{
  return mVarId;
}

// @brief 入力ノードの時に true を返す．
inline
bool
CecNode::is_input() const
{
  return static_cast<bool>((mFlags >> kSftI) & 1U);
}

// @brief 入力番号を返す．
inline
ymuint
CecNode::input_id() const
{
  return reinterpret_cast<ympuint>(mFanins[0]);
}

// @brief 入力番号をセットする．
inline
void
CecNode::set_input(ymuint id)
{
  mFlags |= (1U << kSftI);
  mFanins[0] = reinterpret_cast<CecNode*>(id);
}

// @brief AND の時に true を返す．
inline
bool
CecNode::is_and() const
{
  return !is_input();
}

// @brief ファンインを得る．
inline
CecNode*
CecNode::fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief 最初のファンインを得る．
inline
CecNode*
CecNode::fanin0() const
{
  return mFanins[0];
}

// @brief 2番めのファンインを得る．
inline
CecNode*
CecNode::fanin1() const
{
  return mFanins[1];
}

// @brief ファンインの極性を得る．
inline
bool
CecNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mFlags >> (kSftP0 + pos)) & 1U);
}

// @brief 最初のファンインの極性を得る．
inline
bool
CecNode::fanin0_inv() const
{
  return static_cast<bool>((mFlags >> kSftP0) & 1U);
}

// @brief 2番めのファンインの極性を得る．
inline
bool
CecNode::fanin1_inv() const
{
  return static_cast<bool>((mFlags >> kSftP1) & 1U);
}

// @brief ファンインのハンドルを得る．
inline
CecHandle
CecNode::fanin_handle(ymuint pos) const
{
  return CecHandle(fanin(pos), fanin_inv(pos));
}

// @brief 最初のファンインのハンドルを得る．
inline
CecHandle
CecNode::fanin0_handle() const
{
  return CecHandle(fanin0(), fanin0_inv());
}

// @brief 2番めのファンインのハンドルを得る．
inline
CecHandle
CecNode::fanin1_handle() const
{
  return CecHandle(fanin1(), fanin1_inv());
}

// @brief ファンインをセットする．
inline
void
CecNode::set_fanin(CecHandle handle1,
		   CecHandle handle2)
{
  mFanins[0] = handle1.node();
  mFanins[1] = handle2.node();
  if ( handle1.inv() ) {
    mFlags |= (1U << kSftP0);
  }
  else {
    mFlags &= ~(1U << kSftP0);
  }
  if ( handle2.inv() ) {
    mFlags |= (1U << kSftP1);
  }
  else {
    mFlags &= ~(1U << kSftP1);
  }
}

// @brief 削除済みのとき true を返す．
inline
bool
CecNode::check_dmark() const
{
  return static_cast<bool>((mFlags >> kSftD) & 1U);
}

// @brief 要素数が2以上の等価候補グループの代表なら true を返す．
inline
bool
CecNode::check_rep() const
{
  return mRepNode == this && mEqLink != NULL;
}

// @brief 代表ノードを返す．
inline
CecNode*
CecNode::rep_node() const
{
  return mRepNode;
}

// @brief 代表ノードに対する極性を返す．
inline
bool
CecNode::rep_inv() const
{
  return static_cast<bool>((mFlags >> kSftP) & 1U);
}

// @brief 代表ノードを返す．
inline
CecHandle
CecNode::rep_handle() const
{
  return CecHandle(mRepNode, rep_inv());
}

// @brief 次の等価候補ノードを得る．
inline
CecNode*
CecNode::next_eqnode()
{
  return mEqLink;
}

// @brief 代表ノードをセットする．
inline
void
CecNode::set_rep(CecNode* rep_node,
		 bool inv)
{
  mRepNode = rep_node;
  if ( inv ) {
    set_rep_inv();
  }
}

// @brief 等価候補ノードを追加する．
inline
void
CecNode::add_eqnode(CecNode* node)
{
  mEqTail->mEqLink = node;
  mEqTail = node;
  node->mRepNode = this;
  node->mEqLink = NULL;
}

// @brief 極性反転の印をつける．
inline
void
CecNode::set_rep_inv()
{
  mFlags |= (1U << kSftP);
}

// @brief 削除済みの印をつける．
inline
void
CecNode::set_dmark()
{
  mFlags |= (1U << kSftD);
}

END_NAMESPACE_YM_CEC

#endif // LIBYM_CEC_CECNODE_H
