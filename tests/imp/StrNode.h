#ifndef STRNODE_H
#define STRNODE_H

/// @file StrNode.h
/// @brief StrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_logic/Bool3.h"
#include "ImpInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS

class StrNode;
class ImpMgr;

//////////////////////////////////////////////////////////////////////
/// @class StrEdge StrNode.h "StrNode.h"
/// @brief StrNode の間の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class StrEdge
{
public:

  /// @brief コンストラクタ
  StrEdge();

  /// @brief デストラクタ
  ~StrEdge();

  /// @brief 設定する．
  /// @param[in] src_node 入力元のノード
  /// @param[in] inv 極性
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pos 出力先のファンイン番号(0 or 1)
  void
  set(StrNode* src_node,
      bool inv,
      StrNode* dst_node,
      ymuint dst_pos);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力元のノードを返す．
  StrNode*
  src_node() const;

  /// @brief 極性を返す．
  bool
  src_inv() const;

  /// @brief 出力先のノードを返す．
  StrNode*
  dst_node() const;

  /// @brief 出力先のファンイン番号を返す．
  ymuint
  dst_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力元のノード + 極性情報
  ympuint mSrcInfo;

  // 出力先のノード + ファンイン番号
  ympuint mDstInfo;

};


//////////////////////////////////////////////////////////////////////
/// @class StrNode StrNode.h "StrNode.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class StrNode
{
  friend class ImpMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] node0 ファンイン0のノード
  /// @param[in] inv0 ファンイン0の極性
  /// @param[in] node1 ファンイン1のノード
  /// @param[in] inv1 ファンイン1の極性
  StrNode(ymuint id,
	  StrNode* node0,
	  bool inv0,
	  StrNode* node1,
	  bool inv1);

  /// @brief デストラクタ
  virtual
  ~StrNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンアウト数を設定する．
  void
  set_nfo(ymuint n);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID 番号を返す．
  ymuint
  id() const;

  /// @brief 外部入力のときに true を返す．
  virtual
  bool
  is_input() const;

  /// @brief AND タイプのときに true を返す．
  virtual
  bool
  is_and() const;

  /// @brief XOR タイプのときに true を返す．
  virtual
  bool
  is_xor() const;

  /// @brief ファンイン0の枝を返す．
  const StrEdge&
  fanin0() const;

  /// @brief ファンイン1の枝を返す．
  const StrEdge&
  fanin1() const;

  /// @brief ファンアウトの枝のリストを返す．
  const vector<StrEdge*>&
  fanout_list() const;

  /// @brief 出力値を返す．
  virtual
  Bool3
  val() const = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // ビットベクタ関係の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ビットベクタ値を返す．
  ymuint64
  bitval() const;

  /// @brief ビットベクタ値を設定する．
  void
  set_bitval(ymuint64 bitval);

  /// @brief ビットベクタ値の計算を行なう．
  virtual
  void
  calc_bitval() = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  clear() = 0;

  /// @brief 状態を返す．
  virtual
  ymuint32
  cur_state() const = 0;

  /// @brief 状態を元にもどす．
  virtual
  void
  restore(ymuint32 val) = 0;

  /// @brief unjustified ノードの時 true を返す．
  virtual
  bool
  is_unjustified() const = 0;

  /// @brief justification パタン数を得る．
  virtual
  ymuint
  justification_num() = 0;

  /// @brief justification パタン を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < justification_num() )
  /// @return 値割り当て
  virtual
  ImpVal
  get_justification(ymuint pos) = 0;

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr,
	    vector<ImpVal>& imp_list) = 0;

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr,
	    vector<ImpVal>& imp_list) = 0;

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr,
	    vector<ImpVal>& imp_list) = 0;

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr,
	    vector<ImpVal>& imp_list) = 0;

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  bwd_imp0(ImpMgr& mgr,
	   vector<ImpVal>& imp_list) = 0;

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  /// @param[out] imp_list 含意の結果を格納するリスト
  virtual
  bool
  bwd_imp1(ImpMgr& mgr,
	   vector<ImpVal>& imp_list) = 0;;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // ファンインの枝
  StrEdge mFanins[2];

  // ファンアウトの枝のリスト
  vector<StrEdge*> mFanouts;

  // ビットベクタ値
  ymuint64 mBitVal;

  // スタックのレベル
  ymuint32 mStackLevel;

  // ImpMgr::mUnodeList 中の位置を示す反復子
  list<StrNode*>::iterator mListIter;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
StrEdge::StrEdge()
{
}

// @brief デストラクタ
inline
StrEdge::~StrEdge()
{
}

// @brief 設定する．
// @param[in] src_node 入力元のノード
// @param[in] inv 極性
// @param[in] dst_node 出力先のノード
// @param[in] dst_pos 出力先のファンイン番号(0 or 1)
inline
void
StrEdge::set(StrNode* src_node,
	     bool inv,
	     StrNode* dst_node,
	     ymuint dst_pos)
{
  mSrcInfo = reinterpret_cast<ympuint>(src_node) | static_cast<ympuint>(inv);
  mDstInfo = reinterpret_cast<ympuint>(dst_node) | (dst_pos & 1U);
}

// @brief 入力元のノードを返す．
inline
StrNode*
StrEdge::src_node() const
{
  return reinterpret_cast<StrNode*>(mSrcInfo & ~1UL);
}

// @brief 極性を返す．
inline
bool
StrEdge::src_inv() const
{
  return static_cast<bool>(mSrcInfo & 1UL);
}

// @brief 出力先のノードを返す．
inline
StrNode*
StrEdge::dst_node() const
{
  return reinterpret_cast<StrNode*>(mDstInfo & ~1UL);
}

// @brief 出力先のファンイン番号を返す．
inline
ymuint
StrEdge::dst_pos() const
{
  return static_cast<ymuint>(mDstInfo & 1UL);
}

// @brief ファンアウト数を設定する．
inline
void
StrNode::set_nfo(ymuint n)
{
  mFanouts.reserve(n);
}

// @brief ID 番号を返す．
inline
ymuint
StrNode::id() const
{
  return mId;
}

// @brief ファンイン0の枝を返す．
inline
const StrEdge&
StrNode::fanin0() const
{
  return mFanins[0];
}

// @brief ファンイン1の枝を返す．
inline
const StrEdge&
StrNode::fanin1() const
{
  return mFanins[1];
}

// @brief ファンアウトの枝のリストを返す．
inline
const vector<StrEdge*>&
StrNode::fanout_list() const
{
  return mFanouts;
}

// @brief ビットベクタ値を返す．
inline
ymuint64
StrNode::bitval() const
{
  return mBitVal;
}

// @brief ビットベクタ値を設定する．
inline
void
StrNode::set_bitval(ymuint64 bitval)
{
  mBitVal = bitval;
}

END_NAMESPACE_YM_NETWORKS

#endif // STRNODE_H
