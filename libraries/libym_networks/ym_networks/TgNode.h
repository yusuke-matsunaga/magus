#ifndef YM_NETWORKS_TGNODE_H
#define YM_NETWORKS_TGNODE_H

/// @file ym_networks/TgNode.h
/// @brief TgNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"


BEGIN_NAMESPACE_YM_NETWORKS_TGNET

//////////////////////////////////////////////////////////////////////
/// @class TgEdge TgEdge.h "ym_networks/TgNode.h"
/// @brief ゲート間の接続を表すクラス
/// @note 具体的には
///  - destination のノードへのポインタ
///  - destination のノードにおける入力ピン番号
///  の情報を持つ．
//////////////////////////////////////////////////////////////////////
class TgEdge
{
  friend class TgNode;
  friend class TgNetwork;

private:

  // コンストラクタ
  TgEdge();

  // デストラクタ
  ~TgEdge();


public:

  /// @brief ファンアウト先のゲートを得る．
  const TgNode*
  to_node() const;

  /// @brief ファンアウト先の入力ピン番号を得る．
  ymuint
  to_ipos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファンアウト先のゲート
  TgNode* mTo;

  // ファンアウト先の入力ピン番号
  ymuint32 mIpos;

};


//////////////////////////////////////////////////////////////////////
/// @class TgNode TgNode.h "ym_networks/TgNode.h"
/// @brief ゲートを表すクラス
/// @note 保持している情報は
///  - タイプID (32以下は予約タイプ)
///  - ノードID (global ID)
///  - local ID (入力/出力/ロジック/ラッチごとに独立)
///  - 名前
///  - ファンインの枝の配列
///  - ファンアウトの枝のリスト
//////////////////////////////////////////////////////////////////////
class TgNode
{
  friend class TgNetwork;

private:
  // このクラスは TgNetwork 内でしか生成／破壊できない．

  // コンストラクタ
  TgNode(ymuint32 gid);

  // デストラクタ
  ~TgNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内部情報を得る関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのタイプを得る．
  tTgNodeType
  type() const;

  /// @brief 未定義タイプの時 true を返す．
  bool
  is_undef() const;

  /// @brief 外部入力タイプの時 true を返す．
  /// @note FF 出力もここに含まれる．
  bool
  is_input() const;

  /// @brief 外部出力タイプの時 true を返す．
  /// @note FF 入力もここに含まれる．
  bool
  is_output() const;

  /// @brief logic タイプの時 true を返す．
  bool
  is_logic() const;

  /// @brief 組み込み型でない logic タイプの時 true を返す．
  bool
  is_cplx_logic() const;

  /// @brief cplx_logic タイプの時の関数IDを返す．
  ymuint
  cplx_logic_id() const;

  /// @brief ffin タイプの時 true を返す．
  bool
  is_ffin() const;

  /// @brief ffout タイプの時 true を返す．
  bool
  is_ffout() const;

  /// @brief ノード全体の通し番号を得る．
  ymuint32
  gid() const;

  /// @brief 同じタイプのノード内での通し番号を得る．
  ymuint32
  lid() const;

  /// @brief 名前を得る．
  const char*
  name() const;

  /// @brief 入力数を得る．
  ymuint
  ni() const;

  /// @brief pos 番めの入力のノードを得る．
  const TgNode*
  fanin(ymuint pos) const;

  /// @brief ファンアウト数を得る．
  ymuint
  fanout_num() const;

  /// @brief ファンアウトの枝を得る．
  const TgEdge*
  fanout_edge(ymuint pos) const;

  /// @brief ファンアウト先のノードを得る．
  const TgNode*
  fanout(ymuint pos) const;

  /// @brief ファンアウト先のノードの入力位置を得る．
  ymuint
  fanout_ipos(ymuint pos) const;

  /// @brief ffin/ffout ノードの時に相方のノードを返す．
  TgNode*
  alt_node();

  /// @brief ffin/ffout ノードの時に相方のノードを返す．
  const TgNode*
  alt_node() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief タイプをセットする．
  /// @param[in] lid 論理ノードの通し番号
  /// @param[in] type 論理関数のタイプ
  /// @param[in] ni 入力数
  /// @param[in] aux_id kTgCplx 型の時の補助ID
  /// @param[in] p ファンインの配列用のメモリ領域
  void
  set_type(ymuint32 lid,
	   tTgNodeType type,
	   ymuint ni,
	   ymuint aux_id,
	   void* p);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイプ + 関数ID
  ymuint32 mTypeId;

  // 全体での通し番号
  ymuint32 mGid;

  // 同タイプ内での通し番号
  ymuint32 mLid;

  // 名前
  const char* mName;

  // 入力数
  ymuint32 mNi;

  // ファンインのノードの配列
  TgNode** mFanins;

  // ファンアウト数
  ymuint32 mFanoutNum;

  // ファンアウトの枝の配列
  TgEdge* mFanouts;

  // ffin/ffout ノードの時に相方のノードを保持する．
  TgNode* mAltNode;

};


//////////////////////////////////////////////////////////////////////
// TgEdge のインライン関数
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
TgEdge::TgEdge() :
  mTo(NULL),
  mIpos(0)
{
}

// デストラクタ
inline
TgEdge::~TgEdge()
{
}

// @brief ファンアウト先のゲートを得る．
inline
const TgNode*
TgEdge::to_node() const
{
  return mTo;
}

// @brief ファンアウト先の入力ピン番号を得る．
inline
ymuint
TgEdge::to_ipos() const
{
  return mIpos;
}


//////////////////////////////////////////////////////////////////////
// TgNode のインライン関数
//////////////////////////////////////////////////////////////////////

// @brief タイプ id を得る．
inline
tTgNodeType
TgNode::type() const
{
  return static_cast<tTgNodeType>(mTypeId & 63U);
}

// @brief 未定義タイプの時 true を返す．
inline
bool
TgNode::is_undef() const
{
  return type() == kTgUndef;
}

// @brief 外部入力タイプの時 true を返す．
inline
bool
TgNode::is_input() const
{
  return type() == kTgInput;
}

// @brief 外部出力タイプの時 true を返す．
inline
bool
TgNode::is_output() const
{
  return type() == kTgOutput;
}

// @brief logic タイプの時 true を返す．
inline
bool
TgNode::is_logic() const
{
  return (static_cast<ymuint>(type()) & kTgLogic) == kTgLogic;
}

// @brief 組み込み型でない logic タイプの時 true を返す．
inline
bool
TgNode::is_cplx_logic() const
{
  return type() == kTgCplx;
}

// @brief ffin タイプの時 true を返す．
inline
bool
TgNode::is_ffin() const
{
  return type() == kTgOutput && mAltNode != NULL;
}

// @brief ffout タイプの時 true を返す．
inline
bool
TgNode::is_ffout() const
{
  return type() == kTgInput && mAltNode != NULL;
}

// @brief ノード全体での通し番号を得る．
inline
ymuint32
TgNode::gid() const
{
  return mGid;
}

// @brief 同じタイプのノード内での通し番号を得る．
inline
ymuint32
TgNode::lid() const
{
  return mLid;
}

// @brief 名前を得る．
inline
const char*
TgNode::name() const
{
  return mName;
}

// @brief 入力数を得る．
inline
ymuint
TgNode::ni() const
{
  return mNi;
}

// @brief pos 番めの入力のノードを得る．
inline
const TgNode*
TgNode::fanin(ymuint pos) const
{
  return mFanins[pos];
}

// @brief ファンアウト数を得る．
inline
ymuint
TgNode::fanout_num() const
{
  return mFanoutNum;
}

// @brief ファンアウトの枝を得る．
inline
const TgEdge*
TgNode::fanout_edge(ymuint pos) const
{
  return &mFanouts[pos];
}

// @brief ファンアウト先のノードを得る．
inline
const TgNode*
TgNode::fanout(ymuint pos) const
{
  return mFanouts[pos].mTo;
}

// @brief ファンアウト先のノードの入力位置を得る．
inline
ymuint
TgNode::fanout_ipos(ymuint pos) const
{
  return mFanouts[pos].mIpos;
}

// @brief ffin/ffout ノードの時に相方のノードを返す．
inline
TgNode*
TgNode::alt_node()
{
  return mAltNode;
}

// @brief ffin/ffout ノードの時に相方のノードを返す．
inline
const TgNode*
TgNode::alt_node() const
{
  return mAltNode;
}

END_NAMESPACE_YM_NETWORKS_TGNET

#endif // YM_NETWORKS_TGNODE_H
