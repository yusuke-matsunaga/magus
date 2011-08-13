#ifndef LDPATNODE_H
#define LDPATNODE_H

/// @file LdPatNode.h
/// @brief LdPatNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

class LdPatMgr;

//////////////////////////////////////////////////////////////////////
/// @class LdPatNode LdPatNode.h "LdPatNode.h"
/// @brief パタン生成用のノードを表すクラス
//////////////////////////////////////////////////////////////////////
class LdPatNode
{
  friend class LdPatMgr;

private:

  /// @brief コンストラクタ
  LdPatNode();

  /// @brief デストラクタ
  ~LdPatNode();


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
  LdPatNode*
  fanin(ymuint pos) const;

  /// @brief AND/XOR の時にファンインの極性を返す．
  /// @param[in] pos 位置 ( 0 or 1 )
  /// @retval true 反転あり
  /// @retval false 反転なし
  bool
  fanin_inv(ymuint pos) const;


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

  // lock ビット
  bool mLocked;

  // ファンインのノード
  LdPatNode* mFanin[2];

  // ハッシュ表中のリンク
  LdPatNode* mLink;

};

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDPATNODE_H
