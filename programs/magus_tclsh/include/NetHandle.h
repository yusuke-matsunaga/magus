#ifndef MAGUS_NETHANDLE_H
#define MAGUS_NETHANDLE_H

/// @file include/NetHandle.h
/// @brief NetHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: NetHandle.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// ネットワークのオブジェクトを管理するクラス

#include "magus.h"
#include "ym_bnet/bnet_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"
#include "ym_sbj/SbjGraph.h"


BEGIN_NAMESPACE_MAGUS

class NetMgr;

//////////////////////////////////////////////////////////////////////
/// @class NetHandle NetHandle.h <NetHandle.h>
/// @brief ネットワークの情報を入れておく構造体
//////////////////////////////////////////////////////////////////////
class NetHandle
{
  friend class NetMgr;
private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  // NetMgr のみが生成/破壊を行う．
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] mgr 親のマネージャ
  /// @param[in] name 名前
  NetHandle(NetMgr* mgr,
	    const string& name);

  /// @brief デストラクタ
  ~NetHandle();


public:

  /// @brief ネットワーク名を得る．
  const string&
  name() const;

  /// @brief BNetwork を得る．
  BNetwork*
  bnetwork() const;

  /// @brief BdNetwork を得る．
  BdNetwork*
  bdn() const;

  /// @brief SbjGraph を得る．
  SbjGraph*
  sbjgraph() const;

  /// @brief ネットワークを複製する．
  void
  copy(const NetHandle& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ネットワーク名
  string mName;

  // BNetwork
  BNetwork* mBNetwork;

  // BdNetwork
  BdNetwork* mBdNetwork;

  // SbjGraph
  SbjGraph* mSbjGraph;

  // カレントネットワークスタックに積まれた回数
  int mCount;

  // ハッシュ表の次の要素を指すリンク
  NetHandle* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ネットワーク名を得る．
inline
const string&
NetHandle::name() const
{
  return mName;
}

// @brief BNetwork を得る．
inline
BNetwork*
NetHandle::bnetwork() const
{
  return mBNetwork;
}

// @brief BdNetwork を得る．
inline
BdNetwork*
NetHandle::bdn() const
{
  return mBdNetwork;
}

// @brief SbjGraph を得る．
inline
SbjGraph*
NetHandle::sbjgraph() const
{
  return mSbjGraph;
}

END_NAMESPACE_MAGUS

#endif // MAGUS_NETHANDLE_H
