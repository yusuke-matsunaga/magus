#ifndef RWTNODE_H
#define RWTNODE_H

/// @file RwtNode.h
/// @brief RwtNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RwtNode RwtNode.h "RwtNode.h"
/// @brief rewriting 用のクラス
//////////////////////////////////////////////////////////////////////
class RwtNode
{
  friend class RwtMgr;

public:

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 関数ベクタを得る．
  ymuint16
  func() const;

  /// @brief ボリュームを得る．
  ymuint
  volume() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief XOR フラグを得る．
  bool
  is_xor() const;

  /// @brief 左の子供を得る．
  RwtNode*
  child0() const;

  /// @brief 右の子供を得る．
  RwtNode*
  child1() const;

  /// @brief 左の子供の反転属性を得る．
  bool
  inv0() const;

  /// @brief 右の子供の反転属性を得る．
  bool
  inv1() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 関数ベクタ
  ymuint16 mFunc;

  // ボリューム
  ymuint8 mVolume;

  // レベル
  ymuint8 mLevel;

  // フラグ
  // 0: mChild0 の反転属性
  // 1: mChild1 の反転属性
  // 2: xor
  ymuint8 mFlags;

  // 左の子供
  RwtNode* mChild0;

  // 右の子供
  RwtNode* mChild1;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
RwtNode::id() const
{
  return mId;
}

// @brief 関数ベクタを得る．
inline
ymuint16
RwtNode::func() const
{
  return mFunc;
}

// @brief ボリュームを得る．
inline
ymuint
RwtNode::volume() const
{
  return mVolume;
}

// @brief レベルを得る．
inline
ymuint
RwtNode::level() const
{
  return mLevel;
}

// @brief XOR フラグを得る．
inline
bool
RwtNode::is_xor() const
{
  return static_cast<bool>((mFlags >> 2) & 1U);
}

// @brief 左の子供を得る．
inline
RwtNode*
RwtNode::child0() const
{
  return mChild0;
}

// @brief 右の子供を得る．
inline
RwtNode*
RwtNode::child1() const
{
  return mChild1;
}

// @brief 左の子供の反転属性を得る．
inline
bool
RwtNode::inv0() const
{
  return static_cast<bool>((mFlags >> 0) & 1U);
}

// @brief 右の子供の反転属性を得る．
inline
bool
RwtNode::inv1() const
{
  return static_cast<bool>((mFlags >> 1) & 1U);
}

END_NAMESPACE_YM

#endif // RWTNODE_H
