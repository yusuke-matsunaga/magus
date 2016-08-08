#ifndef SBJNODE_H
#define SBJNODE_H

/// @file SbjNode.h
/// @brief SbjNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "sbj_nsdef.h"

//#include "ym/ym_logic.h"
//#include "ym/SimpleAlloc.h"
//#include "ym/FragAlloc.h"
//#include "ym/ItvlMgr.h"


BEGIN_NAMESPACE_YM_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SbjEdge SbjNode.h "SbjNode.h"
/// @brief サブジェクトグラフの枝を表すクラス
///
/// - 入力側のノード
/// - 出力側のノード
/// - 出力側のノードの何番目(0 or 1)の入力か
/// の情報を持つ．
/// @sa SbjNode SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjEdge
{
  friend class SbjNode;
  friend class SbjGraph;

public:

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval nullptr 定数0に接続している枝の場合
  const SbjNode*
  from() const;

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  const SbjNode*
  to() const;

  /// @brief 入力側のノードを得る．
  /// @retval 入力側のノード 通常の枝の場合
  /// @retval nullptr 定数0に接続している枝の場合
  SbjNode*
  from();

  /// @brief 出力側のノードを得る．
  /// @return 出力側のノードを返す．
  SbjNode*
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
  SbjEdge();

  /// @brief デストラクタ
  ~SbjEdge();

  /// @brief from ノードをセットする．
  void
  set_from(SbjNode* from);

  /// @brief to ノードをセットする．
  void
  set_to(SbjNode* to,
	 ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ変数
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  SbjNode* mFrom;

  // 出力側のノード
  SbjNode* mTo;

  // 入力位置
  ymuint32 mIpos;

};


//////////////////////////////////////////////////////////////////////
/// @class SbjNode SbjGraph.h "SbjGraph.h"
/// @brief サブジェクトグラフを構成するプリミティブノードを表すクラス
///
/// ノードの種類は
/// - 入力ノード
/// - 出力ノード
/// - 論理ノード
/// - DFFノード
/// の 4種類がある．
///
/// 論理ノードの場合，常に2つのファンインを持つ．
/// ノードの論理タイプが AND と XOR の2種類があり，
/// さらに2つの入力の極性がある．
/// ただし，XOR タイプの場合には入力は反転させない．
/// @sa SbjEdge SbjGraph
//////////////////////////////////////////////////////////////////////
class SbjNode
{
  friend class SbjGraph;

public:
  /// @brief ノードの型
  enum tType {
    /// @brief 入力ノード
    kINPUT  = 0,
    /// @brief 出力ノード
    kOUTPUT = 1,
    /// @brief 論理ノード
    kLOGIC  = 2,
  };


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  // SbjGraph のみが呼び出せる．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  SbjNode();

  /// @brief デストラクタ
  ~SbjNode();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 構造に関する情報を取り出す関数
  /// @{

  /// @brief ID 番号の取得
  /// @return ID 番号を返す．
  /// @note ID 番号はノードの生成時に SbjGraph により自動的に割り振られる．
  /// @sa SbjGraph
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

  /// @brief AND ノードの時に true を返す．
  bool
  is_and() const;

  /// @brief XOR ノードの時に true を返す．
  bool
  is_xor() const;

  /// @brief サブID (入力／出力番号)を得る．
  /// @note 入力ノード/出力ノードの場合のみ意味を持つ．
  ymuint
  subid() const;

  /// @brief 出力ノードの極性を返す．
  /// @return 反転していたら true を返す．
  bool
  output_inv() const;

  /// @brief 機能コードを得る．
  ///
  /// 論理ノードの場合のみ意味を持つ．
  /// 機能コードの意味は以下の通り
  ///  - 000 : a & b
  ///  - 001 : a' & b
  ///  - 010 : a & b'
  ///  - 011 : a' & b'
  ///  - 100 : a ^ b
  /// このうち，000 〜 011 が is_and() で true を返す．
  /// 100 が is_xor() で true を返す．
  ymuint
  fcode() const;

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  ///
  /// 該当するファンインがなければ nullptr を返す．
  const SbjNode*
  fanin(ymuint pos) const;

  /// @brief ファンインの反転属性を得る．
  /// @param[in] pos 入力番号 (0 or 1)
  bool
  fanin_inv(ymuint pos) const;

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  ///
  /// 該当するファンインの枝がなければ nullptr を返す．
  const SbjEdge*
  fanin_edge(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトの枝を返す．
  /// @param[in] pos 位置番号
  const SbjEdge*
  fanout_edge(ymuint pos) const;

  /// @brief 出力ノードにファンアウトしているとき true を返す．
  bool
  pomark() const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内容の設定に用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンインのノードを得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番めのファンインのノード
  /// @note 該当するファンインがなければ nullptr を返す．
  SbjNode*
  fanin(ymuint pos);

  /// @brief ファンインの枝を得る．
  /// @param[in] pos 入力番号(0 or 1)
  /// @return pos 番目の入力の枝
  /// @note 該当するファンインの枝がなければ nullptr を返す．
  SbjEdge*
  fanin_edge(ymuint pos);

  /// @brief タイプを入力に設定する．
  void
  set_input(ymuint subid);

  /// @brief タイプを出力に設定する．
  /// @param[in] inv 反転属性
  void
  set_output(ymuint subid,
	     bool inv);

  /// @brief タイプを論理に設定する．
  void
  set_logic(ymuint fcode);

  /// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
  void
  scan_po();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;

  // タイプ (+ 入力/出力番号/機能コード)
  ymuint32 mFlags;

  // ファンインの枝(そのもの)の配列
  // 要素数は通常2だが DFF の場合は4
  SbjEdge* mFanins;

  // ファンアウトの枝のポインタ配列
  vector<SbjEdge*> mFanoutList;

  // 作業用のマーク
  ymuint32 mMark;

  // レベル
  ymuint32 mLevel;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mFlags の演算で用いる定数
  static
  const int kTypeShift = 0;
  static
  const int kOinvShift = 2;
  static
  const int kSubidShift = 3;
  static
  const int kFcodeShift = 2;
  static
  const int kDDinvShift = 2;
  static
  const int kDCinvShift = 3;
  static
  const int kDSinvShift = 4;
  static
  const int kDRinvShift = 5;

  static
  const ymuint kTypeMask = 3U;
  static
  const ymuint kFcodeMask = 7U;

  // mMark の演算で用いる定数
  static
  const int kPoShift = 0;

  static
  const ymuint kPoMask = (1U << kPoShift);

};


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス SbjEdge
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
SbjEdge::SbjEdge() :
  mFrom(nullptr),
  mTo(nullptr),
  mIpos(0U)
{
}

// デストラクタ
inline
SbjEdge::~SbjEdge()
{
}

// 入力側のノードを得る．
inline
const SbjNode*
SbjEdge::from() const
{
  return mFrom;
}

// 出力側のノードを得る．
inline
const SbjNode*
SbjEdge::to() const
{
  return mTo;
}

// 入力側のノードを得る．
inline
SbjNode*
SbjEdge::from()
{
  return mFrom;
}

// 出力側のノードを得る．
inline
SbjNode*
SbjEdge::to()
{
  return mTo;
}

// 出力側のノードの何番目の入力かを示す．
inline
ymuint
SbjEdge::pos() const
{
  return mIpos;
}

// @brief from ノードをセットする．
inline
void
SbjEdge::set_from(SbjNode* from)
{
  mFrom = from;
}

// @brief to ノードをセットする．
inline
void
SbjEdge::set_to(SbjNode* to,
		ymuint pos)
{
  mTo = to;
  mIpos = pos;
}

// @brief 出力ノードに接続している時 true を返す．
inline
bool
SbjEdge::is_poedge() const
{
  return to()->is_output();
}


//////////////////////////////////////////////////////////////////////
// クラス SbjNode
//////////////////////////////////////////////////////////////////////

// ID 番号を得る．
inline
ymuint
SbjNode::id() const
{
  return mId;
}

// タイプを入力に設定する．
inline
void
SbjNode::set_input(ymuint subid)
{
  mFlags = static_cast<ymuint>(kINPUT) | (subid << kSubidShift);
}

// タイプを出力に設定する．
inline
void
SbjNode::set_output(ymuint subid,
		    bool inv)
{
  mFlags = static_cast<ymuint>(kOUTPUT) |
    (subid << kSubidShift) |
    (inv << kOinvShift);
}

// タイプを論理に設定する．
inline
void
SbjNode::set_logic(ymuint fcode)
{
  mFlags = static_cast<ymuint>(kLOGIC) | (fcode << kFcodeShift);
}

// タイプを得る．
inline
SbjNode::tType
SbjNode::type() const
{
  return static_cast<tType>(mFlags & kTypeMask);
}

// 入力ノードの時に true を返す．
inline
bool
SbjNode::is_input() const
{
  return type() == kINPUT;
}

// 出力ノードの時に true を返す．
inline
bool
SbjNode::is_output() const
{
  return type() == kOUTPUT;
}

// 論理ノードの時に true を返す．
inline
bool
SbjNode::is_logic() const
{
  return type() == kLOGIC;
}

// @brief AND ノードの時に true を返す．
inline
bool
SbjNode::is_and() const
{
  // 注意! encoding が変わったら変更すること．
  return (mFlags & 0x13U) == 0x02U;
}

// @brief XOR ノードの時に true を返す．
inline
bool
SbjNode::is_xor() const
{
  // 注意! encoding が変わったら変更すること．
  return (mFlags & 0x13U) == 0x12U;
}

// @brief サブID (入力／出力番号)を得る．
inline
ymuint
SbjNode::subid() const
{
  return mFlags >> kSubidShift;
}

// @brief 出力ノードの極性を得る．
inline
bool
SbjNode::output_inv() const
{
  return static_cast<bool>((mFlags >> kOinvShift) & 1U);
}

// @brief 機能コードを得る．
inline
ymuint
SbjNode::fcode() const
{
  return (mFlags >> kFcodeShift) & kFcodeMask;
}

// @brief ファンインのノードを得る．
inline
const SbjNode*
SbjNode::fanin(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインのノードを得る．
inline
SbjNode*
SbjNode::fanin(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return mFanins[pos].from();
}

// @brief ファンインの反転属性を得る．
// @param[in] pos 入力番号 (0 or 1)
inline
bool
SbjNode::fanin_inv(ymuint pos) const
{
  return static_cast<bool>((mFlags >> (pos + kFcodeShift)) & 1U);
}

// ファンインの枝を得る．
inline
const SbjEdge*
SbjNode::fanin_edge(ymuint pos) const
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンインの枝を得る．
inline
SbjEdge*
SbjNode::fanin_edge(ymuint pos)
{
  // pos の範囲チェックはしていない！！！
  return &mFanins[pos];
}

// ファンアウト数を得る．
inline
ymuint
SbjNode::fanout_num() const
{
  return mFanoutList.size();
}

// @brief ファンアウトの枝を返す．
// @param[in] pos 位置番号
inline
const SbjEdge*
SbjNode::fanout_edge(ymuint pos) const
{
  ASSERT_COND( pos < fanout_num() );
  return mFanoutList[pos];
}

// @brief 出力ノードにファンアウトしているとき true を返す．
inline
bool
SbjNode::pomark() const
{
  return static_cast<bool>((mMark >> kPoShift) & 1U);
}

// @brief レベルを得る．
inline
ymuint
SbjNode::level() const
{
  return mLevel;
}

END_NAMESPACE_YM_SBJ

#endif // SBJGRAPH_H
