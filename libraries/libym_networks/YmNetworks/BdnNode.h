#ifndef NETWORKS_BDNNODE_H
#define NETWORKS_BDNNODE_H

/// @file YmNetworks/BdnNode.h
/// @brief BdnNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

class BdnAuxData;

//////////////////////////////////////////////////////////////////////
/// @class BdnEdge BdnNode.h "YmNetworks/BdnNode.h"
/// @brief ノード間の枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa BdnNode BdnMgr
//////////////////////////////////////////////////////////////////////
class BdnEdge :
  public DlElem
{
  friend class BdnNode;
  friend class BdnMgrImpl;

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
/// @class BdnNode BdnNode.h <YmNetworks/BdnNode.h>
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
/// - D-FF のクリア信号
/// - D-FF のプリセット信号
/// - ラッチのデータ入力
/// - ラッチのイネーブル信号
/// - ラッチのクリア信号
/// - ラッチのプリセット信号
///
/// 入出力ノードは入力ノードと出力ノードの対で表す．
///
/// 全てのノードはID番号をもつ．
/// 論理ノードは2つのファンインと機能コードを持つ．
/// 外部出力以外のノードは複数のファンアウトを持つ．
///
/// @sa BdnEdge BdnMgr
//////////////////////////////////////////////////////////////////////
class BdnNode :
  public DlElem
{
  friend class BdnMgrImpl;

public:

  /// @brief ノードの型
  enum tType {
    /// @brief 論理ノード
    kLOGIC        = 0,
    /// @brief 外部入力ノード
    kINPUT        = 1,
    /// @brief 外部出力ノード
    kOUTPUT       = 2,
  };

  /// @brief 入力ノードのサブタイプ
  enum tInputType {
    /// @brief 外部入力
    kPRIMARY_INPUT  = 0,
    /// @brief D-FFの出力
    kDFF_OUTPUT     = 1,
    /// @brief ラッチの出力
    kLATCH_OUTPUT   = 2
  };

  /// @brief 出力ノードのサブタイプ
  enum tOutputType {
    /// @brief 外部出力
    kPRIMARY_OUTPUT = 0,
    /// @brief D-FFのデータ
    kDFF_DATA       = 1,
    /// @brief D-FFのクロック
    kDFF_CLOCK      = 2,
    /// @brief D-FFのクリア信号
    kDFF_CLEAR      = 3,
    /// @brief D-FFのプリセット信号
    kDFF_PRESET     = 4,
    /// @brief ラッチのデータ
    kLATCH_DATA     = 5,
    /// @brief ラッチのイネーブル
    kLATCH_ENABLE   = 6,
    /// @brief ラッチのクリア信号
    kLATCH_CLEAR    = 7,
    /// @brief ラッチのプリセット信号
    kLATCH_PRESET   = 8
  };


protected:

  /// @brief コンストラクタ
  BdnNode();

  /// @brief デストラクタ
  ~BdnNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に BdnMgr により自動的に割り振られる．
  /// @sa BdnMgr
  ymuint
  id() const;

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
  /// @name 入力ノードおよび出力ノードに関係した情報を取り出す関数
  /// @{

  /// @brief 入力ノードのサブタイプを得る．
  tInputType
  input_type() const;

  /// @brief 出力ノードのサブタイプを得る．
  tOutputType
  output_type() const;

  /// @brief 関連するポートを返す．
  /// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
  /// @note それ以外では NULL を返す．
  const BdnPort*
  port() const;

  /// @brief ポート中のビット位置を返す．
  /// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
  /// @note それ以外では 0 を返す．
  ymuint
  port_bitpos() const;

  /// @brief 入出力ノードの場合に相方のノードを返す．
  /// @note なければ NULL を返す．
  const BdnNode*
  alt_node() const;

  /// @brief 関連する D-FF を返す．
  /// @note D-FF に関連していない場合には NULL を返す．
  const BdnDff*
  dff() const;

  /// @brief 関連するラッチを返す．
  /// @note ラッチに関連していない場合には NULL を返す．
  const BdnLatch*
  latch() const;

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

  /// @brief ファンインのハンドルを得る．
  BdnConstNodeHandle
  output_fanin_handle() const;

  /// @brief ファンインのハンドルを得る．
  BdnNodeHandle
  output_fanin_handle();


  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理ノードの情報を取り出す関数
  /// @{

  /// @brief AND タイプのときに true を返す．
  bool
  is_and() const;

  /// @brief XOR タイプのときに true を返す．
  bool
  is_xor() const;

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

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号(0 or 1)
  bool
  fanin_inv(ymuint pos) const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドルを返す．
  BdnConstNodeHandle
  fanin_handle(ymuint pos) const;

  /// @brief ファンインのハンドルを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのハンドルを返す．
  BdnNodeHandle
  fanin_handle(ymuint pos);

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  const BdnNode*
  fanin0() const;

  /// @brief ファンイン0のハンドルを得る．
  BdnConstNodeHandle
  fanin0_handle() const;

  /// @brief ファンイン0のハンドルを得る．
  BdnNodeHandle
  fanin0_handle();

  /// @brief ファンイン0のノードを得る．
  /// @return 0番めのファンインのノード
  BdnNode*
  fanin0();

  /// @brief ファンイン0の反転属性を得る．
  bool
  fanin0_inv() const;

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

  /// @brief ファンイン1の反転属性を得る．
  bool
  fanin1_inv() const;

  /// @brief ファンイン1のハンドルを得る．
  BdnConstNodeHandle
  fanin1_handle() const;

  /// @brief ファンイン1のハンドルを得る．
  BdnNodeHandle
  fanin1_handle();

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 機能コードを得る．
  /// @note 論理ノードの場合のみ意味を持つ．
  /// @note 機能コードは以下の3ビットの情報を持つ．
  ///  - 0bit: ファンイン0の反転属性
  ///  - 1bit: ファンイン1の反転属性
  ///  - 2bit: XOR/AND フラグ(0: AND, 1:XOR)
  ymuint32
  _fcode() const;

  /// @brief 機能コードを設定する．
  /// @note 副作用で論理ノードタイプに設定される．
  void
  set_logic_type(ymuint fcode);

  /// @brief 入力タイプを設定する．
  /// @note 副作用で入力ノードタイプに設定される．
  void
  set_input_type(tInputType type);

  /// @brief 出力タイプを設定する．
  /// @note 副作用で出力ノードタイプに設定される．
  void
  set_output_type(tOutputType type);

  /// @brief 出力ノードのファンインの極性を設定する．
  /// @param[in] inv 極性
  void
  set_output_fanin_inv(bool inv);

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // 入力ノード  : タイプ(2bit) + POマーク(1bit) + サブタイプ(2bit)
  // 出力ノード  :     〃       + 出力極性(1bit) + サブタイプ(4bit)
  // 論理ノード  :     〃       + POマーク(1bit) + 機能コード(3bit)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  BdnEdge mFanins[2];

  // ファンアウトの枝のリスト
  BdnFanoutList mFanoutList;

  // ハッシュ用のリンク
  BdnNode* mLink;

  // レベル
  mutable
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
  const int kPoShift = 2;
  static
  const int kOInvShift = 2;
  static
  const int kIsubShift = 3;
  static
  const int kOsubShift = 3;
  static
  const int kFcodeShift = 3;

  static
  const ymuint32 kPoMask = 1U << kPoShift;
  static
  const ymuint32 kOInvMask = 1U << kOInvShift;

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
  return static_cast<tType>(mFlags & 3U);
}

// 入力ノードの時に true を返す．
inline
bool
BdnNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
BdnNode::is_output() const
{
  return type() == kOUTPUT;
}

// 論理ノードの時に true を返す．
inline
bool
BdnNode::is_logic() const
{
  return type() == kLOGIC;
}

// @brief 入力ノードのサブタイプを得る．
inline
BdnNode::tInputType
BdnNode::input_type() const
{
  return static_cast<tInputType>((mFlags >> kIsubShift) & 3U);
}

// @brief 出力ノードのサブタイプを得る．
inline
BdnNode::tOutputType
BdnNode::output_type() const
{
  return static_cast<tOutputType>((mFlags >> kOsubShift) & 0xFU);
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
  return static_cast<bool>((mFlags >> kOInvShift) & 1U);
}

// @brief 機能コードを得る．
inline
ymuint
BdnNode::_fcode() const
{
  return (mFlags >> kFcodeShift) & 07U;
}

// @brief AND タイプのときに true を返す．
inline
bool
BdnNode::is_and() const
{
  // 32(XOR) + 3(type)
  return ((mFlags & 35U) == 0U);
}

// @brief XOR タイプのときに true を返す．
inline
bool
BdnNode::is_xor() const
{
  // 32(XOR) + 3(type)
  return ((mFlags & 35U) == 32U);
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

// @brief ファンインの反転属性を得る．
// @param[in] pos 入力番号(0 or 1)
inline
bool
BdnNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mFlags >> (kFcodeShift + pos)) & 1U);
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

// @brief ファンイン0の反転属性を得る．
inline
bool
BdnNode::fanin0_inv() const
{
  return static_cast<bool>((mFlags >> kFcodeShift) & 1U);
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

// @brief ファンイン1の反転属性を得る．
inline
bool
BdnNode::fanin1_inv() const
{
  return static_cast<bool>((mFlags >> (kFcodeShift + 1)) & 1U);
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

// @brief 機能コードを設定する．
// @note 副作用で論理ノードタイプに設定される．
inline
void
BdnNode::set_logic_type(ymuint fcode)
{
  mFlags = kLOGIC | (fcode << kFcodeShift);
}

// @brief 入力タイプを設定する．
// @note 副作用で入力ノードタイプに設定される．
inline
void
BdnNode::set_input_type(tInputType type)
{
  mFlags = kINPUT | (static_cast<ymuint>(type) << kIsubShift);
}

// @brief 出力タイプを設定する．
// @note 副作用で出力ノードタイプに設定される．
inline
void
BdnNode::set_output_type(tOutputType type)
{
  mFlags = kOUTPUT | (static_cast<ymuint>(type) << kOsubShift);
}

// @brief 出力の極性を設定する．
// @param[in] inv 極性
inline
void
BdnNode::set_output_fanin_inv(bool inv)
{
  if ( inv ) {
    mFlags |= kOInvMask;
  }
  else {
    mFlags &= ~kOInvMask;
  }
}

END_NAMESPACE_YM_NETWORKS_BDN

#endif // NETWORKS_BDNNODE_H
