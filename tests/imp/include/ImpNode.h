#ifndef IMPNODE_H
#define IMPNODE_H

/// @file ImpNode.h
/// @brief ImpNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnMgr.h"
#include "ym_logic/Bool3.h"
#include "ImpInfo.h"
#include "ImpDst.h"
#include "ImpVal.h"


BEGIN_NAMESPACE_YM_NETWORKS

class ImpNode;
class ImpMgr;
class ImpRec;

//////////////////////////////////////////////////////////////////////
/// @class ImpNodeHandle ImpNode.h "ImpNode.h"
/// @brief ImpNode と極性の対を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpNodeHandle
{
public:

  /// @brief 空のコンストラクタ
  ImpNodeHandle();

  /// @brief 値を指定したコンストラクタ
  /// @param[in] node ノード
  /// @param[in] inv 反転属性 (true で反転)
  ImpNodeHandle(ImpNode* node,
		bool inv);

  /// @brief デストラクタ
  ~ImpNodeHandle();

  /// @brief 値をセットする．
  /// @param[in] node ノード
  /// @param[in] inv 反転属性 (true で反転)
  void
  set(ImpNode* node,
      bool inv);

  /// @brief 定数0を作る．
  static
  ImpNodeHandle
  make_zero();

  /// @brief 定数1を作る．
  static
  ImpNodeHandle
  make_one();


public:

  /// @brief 否定したハンドルを返す．
  ImpNodeHandle
  operator~() const;

  /// @brief ノードを得る．
  ImpNode*
  node() const;

  /// @brief 反転属性を得る．
  bool
  inv() const;

  /// @brief 定数0を表している時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1を表している時 true を返す．
  bool
  is_one() const;

  /// @brief 定数を表している時 true を返す．
  bool
  is_const() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を直接指定したコンストラクタ
  ImpNodeHandle(ympuint data);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポインタと極性をパックしたもの
  ympuint mBody;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpEdge ImpNode.h "ImpNode.h"
/// @brief ImpNode の間の枝を表すクラス
//////////////////////////////////////////////////////////////////////
class ImpEdge
{
public:

  /// @brief コンストラクタ
  ImpEdge();

  /// @brief デストラクタ
  ~ImpEdge();

  /// @brief 設定する．
  /// @param[in] src 入力元のハンドル
  /// @param[in] dst_node 出力先のノード
  /// @param[in] dst_pos 出力先のファンイン番号 ( 0 or 1 )
  void
  set(ImpNodeHandle src,
      ImpNode* dst_node,
      ymuint dst_pos);


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力元のハンドルを返す．
  ImpNodeHandle
  src() const;

  /// @brief 入力元のノードを返す．
  ImpNode*
  src_node() const;

  /// @brief 入力元の反転属性を返す．
  bool
  src_inv() const;

  /// @brief 出力先のノードを返す．
  ImpNode*
  dst_node() const;

  /// @brief 出力先のファンイン番号を返す．
  ymuint
  dst_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力元のハンドル
  ImpNodeHandle mSrc;

  // 出力先のノード + ファンイン番号
  ympuint mDstInfo;

};


//////////////////////////////////////////////////////////////////////
/// @class ImpNode ImpNode.h "ImpNode.h"
/// @brief StrImp で用いられるノード
//////////////////////////////////////////////////////////////////////
class ImpNode
{
  friend class ImpMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] handle0 ファンイン0のハンドル
  /// @param[in] handle1 ファンイン1のハンドル
  ImpNode(ImpNodeHandle handle0,
	  ImpNodeHandle handle1);

  /// @brief デストラクタ
  virtual
  ~ImpNode();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンアウト数を設定する．
  void
  set_nfo(ymuint n);


public:
  //////////////////////////////////////////////////////////////////////
  // 構造情報を取り出す関数
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

  /// @brief ファンイン0の枝を返す．
  const ImpEdge&
  fanin0() const;

  /// @brief ファンイン1の枝を返す．
  const ImpEdge&
  fanin1() const;

  /// @brief ファンアウトの枝のリストを返す．
  const vector<ImpEdge*>&
  fanout_list() const;


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
  // ラーニング用の含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を初期化する．
  virtual
  void
  clear() = 0;

  /// @brief 状態を返す．
  virtual
  ymuint32
  cur_state() const = 0;

  /// @brief 出力値を返す．
  virtual
  Bool3
  val() const = 0;

  /// @brief 定数0に縮退していたら true を返す．
  bool
  is_const0() const;

  /// @brief 定数1に縮退していたら true を返す．
  bool
  is_const1() const;

  /// @brief 定数に縮退していたら true を返す．
  bool
  is_const() const;

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
  ImpDst
  get_justification(ymuint pos) = 0;

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr,
	    ImpRec& rec) = 0;

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr,
	    ImpRec& rec) = 0;

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr,
	    ImpRec& rec) = 0;

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr,
	    ImpRec& rec) = 0;

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  bwd_imp0(ImpMgr& mgr,
	   ImpRec& rec) = 0;

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  /// @param[in] rec 含意を記録するオブジェクト
  virtual
  bool
  bwd_imp1(ImpMgr& mgr,
	   ImpRec& rec) = 0;


public:
  //////////////////////////////////////////////////////////////////////
  // 含意を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ファンイン0を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd0_imp0(ImpMgr& mgr) = 0;

  /// @brief ファンイン0を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd0_imp1(ImpMgr& mgr) = 0;

  /// @brief ファンイン1を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd1_imp0(ImpMgr& mgr) = 0;

  /// @brief ファンイン1を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  fwd1_imp1(ImpMgr& mgr) = 0;

  /// @brief 出力を0にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  bwd_imp0(ImpMgr& mgr) = 0;

  /// @brief 出力を1にする．
  /// @param[in] mgr ImMgr
  virtual
  bool
  bwd_imp1(ImpMgr& mgr) = 0;

  /// @brief 定数に設定する．
  /// @param[in] mgr ImMgr
  /// @param[in] val 値
  void
  set_const(ImpMgr& mgr,
	    ymuint val);

  /// @brief 定数伝搬を行なう．
  /// @param[in] mgr ImMgr
  /// @param[in] val 値
  /// @param[in] ipos 入力位置
  virtual
  void
  prop_const(ImpMgr& mgr,
	     ymuint val,
	     ymuint ipos) = 0;

  /// @brief 0の間接含意を行う．
  /// @param[in] mgr ImMgr
  bool
  ind_imp0(ImpMgr& mgr);

  /// @brief 1の間接含意を行う．
  /// @param[in] mgr ImMgr
  bool
  ind_imp1(ImpMgr& mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // ファンインの枝
  ImpEdge mFanins[2];

  // ファンアウトの枝のリスト
  vector<ImpEdge*> mFanouts;

  // 定数縮退の情報
  // 0: なし
  // 1: 0縮退
  // 2: 1縮退
  ymuint8 mConstState;

  // ビットベクタ値
  ymuint64 mBitVal;

  // スタックのレベル
  ymuint32 mStackLevel;

  // ImpMgr::mUnodeList 中の位置を示す反復子
  list<ImpNode*>::iterator mListIter;

  // 間接含意のリスト(直接含意も含む)
  vector<ImpDst> mImpList[2];

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
ImpNodeHandle::ImpNodeHandle()
{
  mBody = 0UL;
}

// @brief 値を指定したコンストラクタ
// @param[in] node ノード
// @param[in] inv 反転属性 (true で反転)
inline
ImpNodeHandle::ImpNodeHandle(ImpNode* node,
			     bool inv)
{
  set(node, inv);
}

// @brief 内容を直接指定したコンストラクタ
inline
ImpNodeHandle::ImpNodeHandle(ympuint data) :
  mBody(data)
{
}

// @brief デストラクタ
inline
ImpNodeHandle::~ImpNodeHandle()
{
}

// @brief 値をセットする．
// @param[in] node ノード
// @param[in] inv 反転属性 (true で反転)
inline
void
ImpNodeHandle::set(ImpNode* node,
		   bool inv)
{
  mBody = reinterpret_cast<ympuint>(node) | inv;
}

// @brief 定数0を作る．
inline
ImpNodeHandle
ImpNodeHandle::make_zero()
{
  return ImpNodeHandle(0ULL);
}

// @brief 定数1を作る．
inline
ImpNodeHandle
ImpNodeHandle::make_one()
{
  return ImpNodeHandle(1ULL);
}

// @brief 否定したハンドルを返す．
inline
ImpNodeHandle
ImpNodeHandle::operator~() const
{
  return ImpNodeHandle(mBody ^ 1ULL);
}

// @brief ノードを得る．
inline
ImpNode*
ImpNodeHandle::node() const
{
  return reinterpret_cast<ImpNode*>(mBody & ~1ULL);
}

// @brief 反転属性を得る．
inline
bool
ImpNodeHandle::inv() const
{
  return static_cast<bool>(mBody & 1ULL);
}

// @brief 定数0を表している時 true を返す．
inline
bool
ImpNodeHandle::is_zero() const
{
  return mBody == 0ULL;
}

// @brief 定数1を表している時 true を返す．
inline
bool
ImpNodeHandle::is_one() const
{
  return mBody == 1ULL;
}

// @brief 定数を表している時 true を返す．
inline
bool
ImpNodeHandle::is_const() const
{
  return (mBody & ~0ULL) == 0ULL;
}

// @brief コンストラクタ
inline
ImpEdge::ImpEdge()
{
}

// @brief デストラクタ
inline
ImpEdge::~ImpEdge()
{
}

// @brief 設定する．
// @param[in] src 入力元のハンドル
// @param[in] dst_node 出力先のノード
// @param[in] dst_pos 出力先のファンイン番号(0 or 1)
inline
void
ImpEdge::set(ImpNodeHandle src,
	     ImpNode* dst_node,
	     ymuint dst_pos)
{
  mSrc = src;
  mDstInfo = reinterpret_cast<ympuint>(dst_node) | (dst_pos & 1U);
}

// @brief 入力元のハンドルを返す．
inline
ImpNodeHandle
ImpEdge::src() const
{
  return mSrc;
}

// @brief 入力元のノードを返す．
inline
ImpNode*
ImpEdge::src_node() const
{
  return mSrc.node();
}

// @brief 極性を返す．
inline
bool
ImpEdge::src_inv() const
{
  return mSrc.inv();
}

// @brief 出力先のノードを返す．
inline
ImpNode*
ImpEdge::dst_node() const
{
  return reinterpret_cast<ImpNode*>(mDstInfo & ~1UL);
}

// @brief 出力先のファンイン番号を返す．
inline
ymuint
ImpEdge::dst_pos() const
{
  return static_cast<ymuint>(mDstInfo & 1UL);
}

// @brief ファンアウト数を設定する．
inline
void
ImpNode::set_nfo(ymuint n)
{
  mFanouts.reserve(n);
}

// @brief ID 番号を返す．
inline
ymuint
ImpNode::id() const
{
  return mId;
}

// @brief ファンイン0の枝を返す．
inline
const ImpEdge&
ImpNode::fanin0() const
{
  return mFanins[0];
}

// @brief ファンイン1の枝を返す．
inline
const ImpEdge&
ImpNode::fanin1() const
{
  return mFanins[1];
}

// @brief ファンアウトの枝のリストを返す．
inline
const vector<ImpEdge*>&
ImpNode::fanout_list() const
{
  return mFanouts;
}

// @brief ビットベクタ値を返す．
inline
ymuint64
ImpNode::bitval() const
{
  return mBitVal;
}

// @brief ビットベクタ値を設定する．
inline
void
ImpNode::set_bitval(ymuint64 bitval)
{
  mBitVal = bitval;
}

// @brief 定数0に縮退していたら true を返す．
inline
bool
ImpNode::is_const0() const
{
  return mConstState == 1;
}

// @brief 定数1に縮退していたら true を返す．
inline
bool
ImpNode::is_const1() const
{
  return mConstState == 2;
}

// @brief 定数に縮退していたら true を返す．
inline
bool
ImpNode::is_const() const
{
  return mConstState != 0;
}

END_NAMESPACE_YM_NETWORKS

#endif // IMPNODE_H
