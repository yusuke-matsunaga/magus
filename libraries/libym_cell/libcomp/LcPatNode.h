#ifndef LCPATNODE_H
#define LCPATNODE_H

/// @file LcPatNode.h
/// @brief LcPatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

class LcPatMgr;

//////////////////////////////////////////////////////////////////////
/// @class LcPatNode LcPatNode.h "LcPatNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class LcPatNode
{
  friend class LcPatMgr;

private:

  /// @brief コンストラクタ
  LcPatNode();

  /// @brief デストラクタ
  ~LcPatNode();


public:

  /// @brief ノード番号を返す．
  ymuint
  id() const;

  /// @brief 'lock' 状態を得る．
  bool
  is_locked() const;

  /// @brief 入力の時 true を返す．
  bool
  is_input() const;

  /// @brief AND の時 true を返す．
  bool
  is_and() const;

  /// @brief XOR の時 true を返す．
  bool
  is_xor() const;

  /// @brief 入力の時に入力番号を返す．
  ymuint
  input_id() const;

  /// @brief AND/XOR の時にファンインのノードを返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  LcPatNode*
  fanin(ymuint pos) const;

  /// @brief AND/XOR の時にファンイン0のノードを返す．
  LcPatNode*
  fanin0() const;

  /// @brief AND/XOR の時にファンイン1のノードを返す．
  LcPatNode*
  fanin1() const;

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(ymuint pos) const;

  /// @brief AND/XOR の時にファンイン0の極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv0() const;

  /// @brief AND/XOR の時にファンイン1の極性を返す．
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv1() const;

  /// @brief シグネチャを返す．
  const string&
  signature() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に設定する．
  /// @param[in] input_id 入力番号
  void
  set_input(ymuint input_id);

  /// @brief ANDに設定する．
  /// @param[in] inv0, inv1 ファンインの反転属性
  void
  set_and(bool inv0,
	  bool inv1);

  /// @brief XORに設定する．
  void
  set_xor();

  /// @brief 'lock' する．
  /// @note ファンインに再帰する．
  void
  set_locked();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いる定数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの種類
  enum tType {
    kInput = 0,
    kAnd   = 1,
    kXor   = 2
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号
  ymuint32 mId;

  // ノードの種類 ( 2bit )
  //  + 入力番号 ( 30bit)
  // or
  //  + ファンインの極性 ( 1bit x 2)
  ymuint32 mType;

  // シグネチャ
  string mSignature;

  // lock ビット
  bool mLocked;

  // ファンインのノード
  LcPatNode* mFanin[2];

  // ハッシュ表中のリンク
  LcPatNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号を返す．
inline
ymuint
LcPatNode::id() const
{
  return mId;
}

// @brief 'lock' 状態を得る．
inline
bool
LcPatNode::is_locked() const
{
  return mLocked;
}

// @brief 入力の時 true を返す．
inline
bool
LcPatNode::is_input() const
{
  return ((mType & 3U) == kInput);
}

// @brief AND の時 true を返す．
inline
bool
LcPatNode::is_and() const
{
  return ((mType & 3U) == kAnd);
}

// @brief XOR の時 true を返す．
inline
bool
LcPatNode::is_xor() const
{
  return ((mType & 3U) == kXor);
}

// @brief 入力の時に入力番号を返す．
inline
ymuint
LcPatNode::input_id() const
{
  return mType >> 2;
}

// @brief AND/XOR の時にファンインのノードを返す．
// @param[in] pos 位置 ( 0 or 1 )
inline
LcPatNode*
LcPatNode::fanin(ymuint pos) const
{
  return mFanin[pos];
}

// @brief AND/XOR の時にファンイン0のノードを返す．
inline
LcPatNode*
LcPatNode::fanin0() const
{
  return mFanin[0];
}

// @brief AND/XOR の時にファンイン1のノードを返す．
inline
LcPatNode*
LcPatNode::fanin1() const
{
  return mFanin[1];
}

// @brief AND/XOR の時にファンインの極性を返す．
// @param[in] pos 位置 ( 0 or 1 )
// @retval true 反転あり
// @retval false 反転なし
inline
bool
LcPatNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mType >> (pos + 2)) & 1U);
}

// @brief AND/XOR の時にファンイン0の極性を返す．
// @retval true 反転あり
// @retval false 反転なし
inline
bool
LcPatNode::fanin_inv0() const
{
  return static_cast<bool>((mType >> 2) & 1U);
}

// @brief AND/XOR の時にファンイン1の極性を返す．
// @retval true 反転あり
// @retval false 反転なし
inline
bool
LcPatNode::fanin_inv1() const
{
  return static_cast<bool>((mType >> 3) & 1U);
}

// @brief シグネチャを返す．
inline
const string&
LcPatNode::signature() const
{
  return mSignature;
}

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPATNODE_H
