#ifndef YM_BDN_BDNNODE_H
#define YM_BDN_BDNNODE_H

/// @file ym_bdn/BdnNode.h
/// @brief BdnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_BDN

class BdnAuxData;

//////////////////////////////////////////////////////////////////////
/// @class BdnEdge BdnNode.h "ym_bdn/BdnNode.h"
/// @brief ノード間の枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa BdnNode BdNetwork
//////////////////////////////////////////////////////////////////////
class BdnEdge :
  public DlElem
{
  friend class BdnNode;
  friend class BdNetwork;

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  const BdnNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const BdnNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval NULL 定数0に接続している枝の場合
  BdnNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  BdnNode*
  to();

  /// @brief 出力側のノードの何番目の入力かを示す．
  ymuint
  pos() const;

  /// @brief 出力ノードに接続している時 true を返す．
  bool
  is_poedge() const;


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  // これらは friend class からしか呼ぶことができない．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  BdnEdge();

  /// @brief デストラクタ
  ~BdnEdge();

  /// @brief from ノードをセットする．
  void
  set_from(BdnNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(BdnNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  BdnNode* mFrom;

  // 出力側のノード + 入力位置(1bit)
  ympuint mToPos;

};


//////////////////////////////////////////////////////////////////////
/// @class BdnNode BdnNode.h <ym_bdn/BdnNode.h>
/// @brief ノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - 論理ノード
/// の 3種類がある．
///
/// ただし，入力ノードは以下の様に細分化される．
/// - 外部入力
/// - D-FF の出力
/// - ラッチの出力
///
/// 出力ノードは以下の様に細分化される．
/// - 外部出力
/// - D-FF のデータ入力
/// - D-FF のクロック
/// - D-FF のセット信号
/// - D-FF のリセット信号
/// - ラッチのデータ入力
/// - ラッチのイネーブル信号
///
/// 全てのノードはID番号をもつ．
/// 論理ノードは2つのファンインと機能コードを持つ．
/// 外部出力以外のノードは複数のファンアウトを持つ．
///
/// @sa BdnEdge BdNetwork
//////////////////////////////////////////////////////////////////////
class BdnNode :
  public DlElem
{
  friend class BdNetwork;

public:

  /// @brief ノードの型
  enum tType {
    /// @brief 外部入力ノード
    kINPUT        = 0,
    /// @brief D-FF の出力ノード
    kDFF_OUTPUT   = 1,
    /// @brief ラッチの出力ノード
    kLATCH_OUTPUT = 2,

    /// @brief 外部出力ノード
    kOUTPUT       = 8,
    /// @brief D-FF の入力ノード
    kDFF_INPUT    = 9,
    /// @brief D-FF のクロックノード
    kDFF_CLOCK    = 10,
    /// @brief D-FF のセットノード
    kDFF_SET      = 11,
    /// @brief D-FF のリセットノード
    kDFF_RESET    = 12,
    /// @brief ラッチの入力ノード
    kLATCH_INPUT  = 14,
    /// @brief ラッチのイネーブルノード
    kLATCH_ENABLE = 15,

    /// @brief 論理ノード
    kLOGIC        = 4
  };


protected:

  /// @brief コンストラクタ
  /// @param[in] data 補助的な情報
  BdnNode(BdnAuxData* data);

  /// @brief デストラクタ
  ~BdnNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に BdNetwork により自動的に割り振られる．
  /// @sa BdNetwork
  ymuint
  id() const;

  /// @brief ID を表す文字列の取得
  /// @note デバッグ時にしか意味を持たない
  string
  id_str() const;

  /// @brief タイプを得る．
  tType
  type() const;

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const;

  /// @brief 出力ノードの時に true を返す．
  bool
  is_output() const;

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトリストを得る．
  const BdnFanoutList&
  fanout_list() const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 入出力ノードの情報を取り出す関数
  /// @{

  /// @brief 関連するポートを返す．
  /// @note kINPUT および kOUTPUT の時に意味を持つ．
  /// @note それ以外では NULL を返す．
  const BdnPort*
  port() const;

  /// @brief ポート中の位置を返す．
  /// @note kINPUT および kOUTPUT の時に意味を持つ．
  /// @note それ以外では 0 を返す．
  ymuint
  port_bitpos() const;

  /// @brief 関連する D-FF を返す．
  /// @note D-FF に関連していない場合には NULL を返す．
  const BdnDff*
  dff() const;

  /// @brief 関連するラッチを返す．
  /// @note ラッチに関連していない場合には NULL を返す．
  const BdnLatch*
  latch() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 出力ノードの情報を取り出す関数
  /// @{

  /// @brief ファンインのノードを得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  const BdnNode*
  output_fanin() const;

  /// @brief ファンインのノードを得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  BdnNode*
  output_fanin();

  /// @brief ファンインの極性を得る．
  /// @note 出力ノードの場合のみ意味を持つ．
  bool
  output_fanin_inv() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理ノードの情報を取り出す関数
  /// @{

  /// @brief 機能コードを得る．
  /// @note 論理ノードの場合のみ意味を持つ．
  /// @note 機能コードは2入力の真理値表(4bit)
  ymuint32
  fcode() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin(ymuint pos) const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin(ymuint pos);

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  const BdnNode*
  fanin0() const;

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  BdnNode*
  fanin0();

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  const BdnNode*
  fanin1() const;

  /// @brief ファンイン1のノードを得る．
  /// @return 1番めのファンインのノード
  /// @note 該当するファンインがなければ NULL を返す．
  BdnNode*
  fanin1();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプを設定する．
  void
  set_type(tType type);

  /// @brief 出力ノードのファンインの極性を設定する．
  /// @param[in] inv 極性
  void
  set_inv(bool inv);

  /// @brief 機能コードを設定する．
  void
  set_fcode(ymuint fcode);

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 入力ノード: タイプ(4bit) + POマーク(1bit)
  // 出力ノード:     〃       + 出力極性(1bit)
  // 論理ノード:     〃       + POマーク(1bit) + 機能コード(4bit)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  BdnEdge mFanins[2];

  // ファンアウトの枝のリスト
  BdnFanoutList mFanoutList;

  // ハッシュ用のリンク
  BdnNode* mLink;

  // レベル
  ymuint32 mLevel;

  // 補助的な情報を持つオブジェクト
  BdnAuxData* mAuxData;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  const int kTypeShift = 0;
  static
  const int kPoShift = 4;
  static
  const int kInvShift = 4;
  static
  const int kFcodeShift = 5;

  static
  const ymuint32 kPoMask = 1U << kPoShift;
  static
  const ymuint32 kInvMask = 1U << kInvShift;

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス BdnEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
BdnEdge::BdnEdge() :
  mFrom(NULL),
  mToPos(0UL)
{
}

// デストラクタ
inline
BdnEdge::~BdnEdge()
{
}

// 入力側のノードを得る．
inline
const BdnNode*
BdnEdge::from() const
{
  return mFrom;
}

// 入力側のノードを得る．
inline
BdnNode*
BdnEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const BdnNode*
BdnEdge::to() const
{
  return reinterpret_cast<const BdnNode*>(mToPos & ~1UL);
}

// 出力側のノードを得る．
inline
BdnNode*
BdnEdge::to()
{
  return reinterpret_cast<BdnNode*>(mToPos & ~1UL);
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
BdnEdge::pos() const
{
  return static_cast<ymuint>(mToPos & 1UL);
}

// @brief from ノードをセットする．
inline
void
BdnEdge::set_from(BdnNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
BdnEdge::set_to(BdnNode* to,
		ymuint pos)
{
  mToPos = reinterpret_cast<ympuint>(to) | (pos & 1UL);
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
BdnEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス BdnNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
BdnNode::id() const
{
  return mId;
}

// タイプを得る．
inline
BdnNode::tType
BdnNode::type() const
{
  return static_cast<tType>(mFlags & 15U);
}

// 入力ノードの時に true を返す．
inline
bool
BdnNode::is_input() const
{
  // ちょっとトリッキー
  return (mFlags & 15U) < 4;
}

// 出力ノードの時に true を返す．
inline
bool
BdnNode::is_output() const
{
  // ちょっとトリッキー
  return static_cast<bool>((mFlags >> 3) & 1U);
}

// 論理ノードの時に true を返す．
inline
bool
BdnNode::is_logic() const
{
  return type() == kLOGIC;
}

// @brief ファンインのノードを得る．
// @note 出力ノードの場合のみ意味を持つ．
inline
const BdnNode*
BdnNode::output_fanin() const
{
  // 実はこれと同じ
  return fanin0();
}

// @brief ファンインのノードを得る．
// @note 出力ノードの場合のみ意味を持つ．
inline
BdnNode*
BdnNode::output_fanin()
{
  // 実はこれと同じ
  return fanin0();
}

// @brief ファンインの極性を得る．
// @note 出力ノードの場合のみ意味を持つ．
inline
bool
BdnNode::output_fanin_inv() const
{
  return static_cast<bool>((mFlags >> kInvShift) & 1U);
}

// @brief 機能コードを得る．
inline
ymuint
BdnNode::fcode() const
{
  return (mFlags >> kFcodeShift) & 0xf;
}

// @brief ファンインのノードを得る．
inline
const BdnNode*
BdnNode::fanin(ymuint pos) const
{
  return mFanins[pos & 1U].from();
}

// @brief ファンインのノードを得る．
inline
BdnNode*
BdnNode::fanin(ymuint pos)
{
  return mFanins[pos & 1U].from();
}

// @brief ファンイン0のノードを得る．
// @return 0番めのファンインのノード
inline
const BdnNode*
BdnNode::fanin0() const
{
  return mFanins[0].from();
}

// @brief ファンイン0のノードを得る．
// @return 0番めのファンインのノード
inline
BdnNode*
BdnNode::fanin0()
{
  return mFanins[0].from();
}

// @brief ファンイン1のノードを得る．
// @return 1番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
inline
const BdnNode*
BdnNode::fanin1() const
{
  return mFanins[1].from();
}

// @brief ファンイン1のノードを得る．
// @return 1番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
inline
BdnNode*
BdnNode::fanin1()
{
  return mFanins[1].from();
}

// ファンアウトリストを得る．
inline
const BdnFanoutList&
BdnNode::fanout_list() const
{
  return mFanoutList;
}

// ファンアウト数を得る．
inline
ymuint
BdnNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
BdnNode::pomark() const
{
  return !is_output() && static_cast<bool>((mFlags >> kPoShift) & 1U);
}

// @brief レベルを得る．
inline
ymuint
BdnNode::level() const
{
  return mLevel;
}

// タイプを設定する．
inline
void
BdnNode::set_type(tType type)
{
  mFlags &= ~15U;
  mFlags |= static_cast<ymuint32>(type);
}

// @brief 出力の極性を設定する．
// @param[in] inv 極性
inline
void
BdnNode::set_inv(bool inv)
{
  if ( inv ) {
    mFlags |= kInvMask;
  }
  else {
    mFlags &= ~kInvMask;
  }
}

// 機能コードを設定する．
inline
void
BdnNode::set_fcode(ymuint fcode)
{
  mFlags &= ~(15U << kFcodeShift);
  mFlags |= (fcode << kFcodeShift);
}

END_NAMESPACE_YM_BDN

#endif // YM_BDN_BDNNODE_H
