#ifndef LCPAT2NODE_H
#define LCPAT2NODE_H

/// @file LcPat2Node.h
/// @brief LcPat2Node のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "LcPat2Handle.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcPat2Node LcPat2Node.h "LcPat2Node.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class LcPat2Node
{
  friend class LcPat2Mgr;

private:

  /// @brief コンストラクタ
  LcPat2Node();

  /// @brief デストラクタ
  ~LcPat2Node();


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

  /// @brief ファンイン数を返す．
  ymuint
  fanin_num() const;

  /// @brief AND/XOR の時にファンインのハンドルを返す．
  /// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
  LcPat2Handle
  fanin_handle(ymuint pos) const;

  /// @brief AND/XOR の時にファンインのノードを返す．
  /// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
  LcPat2Node*
  fanin(ymuint pos) const;

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(ymuint pos) const;

#if 0
private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力に設定する．
  /// @param[in] input_id 入力番号
  void
  set_input(ymuint input_id);

  /// @brief ANDに設定する．
  /// @param[in] fanin_num ファンイン数
  void
  set_and(ymuint fanin_num);

  /// @brief XORに設定する．
  /// @param[in] fanin_num ファンイン数
  void
  set_xor(ymuint fanin_num);
#endif

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
  //  + 入力番号 ( 30bit )
  // or
  //  + ファンイン数 (30bit )
  ymuint32 mType;

  // lock ビット
  bool mLocked;

  // ハッシュ表中のリンク
  LcPat2Node* mLink;

  // ファンインのノード＋極性の配列
  LcPat2Handle mFaninArray[1];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LcPat2Node::LcPat2Node() :
  mType(0U),
  mLocked(false)
{
}

// @brief デストラクタ
inline
LcPat2Node::~LcPat2Node()
{
}

// @brief ノード番号を返す．
inline
ymuint
LcPat2Node::id() const
{
  return mId;
}

// @brief 'lock' 状態を得る．
inline
bool
LcPat2Node::is_locked() const
{
  return mLocked;
}

// @brief 入力の時 true を返す．
inline
bool
LcPat2Node::is_input() const
{
  return ((mType & 3U) == kInput);
}

// @brief AND の時 true を返す．
inline
bool
LcPat2Node::is_and() const
{
  return ((mType & 3U) == kAnd);
}

// @brief XOR の時 true を返す．
inline
bool
LcPat2Node::is_xor() const
{
  return ((mType & 3U) == kXor);
}

// @brief 入力の時に入力番号を返す．
inline
ymuint
LcPat2Node::input_id() const
{
  if ( is_input() ) {
    return static_cast<ymuint>(mType >> 2);
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }
}

// @brief ファンイン数を返す．
inline
ymuint
LcPat2Node::fanin_num() const
{
  if ( is_input() ) {
    return 0;
  }
  else {
    return static_cast<ymuint>(mType >> 2);
  }
}

// @brief AND/XOR の時にファンインのハンドルを返す．
// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
inline
LcPat2Handle
LcPat2Node::fanin_handle(ymuint pos) const
{
  return mFaninArray[pos];
}

// @brief AND/XOR の時にファンインのノードを返す．
// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
inline
LcPat2Node*
LcPat2Node::fanin(ymuint pos) const
{
  return mFaninArray[pos].node();
}

// @brief AND/XOR の時にファンインの極性を返す．
// @param[in] pos 位置 ( 0 <= pos < fanin_num() )
// @retval true 反転あり
// @retval false 反転なし
inline
bool
LcPat2Node::fanin_inv(ymuint pos) const
{
  return mFaninArray[pos].inv();
}

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCPAT2NODE_H
