
/// @file BdnNode.cc
/// @brief BdnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/BdnNode.h"
#include "ym_networks/BdnPort.h"
#include "ym_networks/BdnDff.h"
#include "ym_networks/BdnLatch.h"
#include "ym_networks/BdnNodeHandle.h"
#include "ym_networks/BdnConstNodeHandle.h"
#include "BdnAuxData.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

//////////////////////////////////////////////////////////////////////
// クラス BdnPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdnPort::BdnPort()
{
}

// @brief デストラクタ
BdnPort::~BdnPort()
{
}

// @brief ビットごとの方向を得る．
// @param[out] iovect ビットごとの方向を収める配列
// @note iovect の値の意味は以下の通り
//  - 0 : なし
//  - 1 : 入力のみ
//  - 2 : 出力のみ
//  - 3 : 入力と出力
void
BdnPort::get_iovect(vector<ymuint>& iovect) const
{
  ymuint nb = bit_width();
  iovect.clear();
  iovect.resize(nb);
  for (ymuint i = 0; i < nb; ++ i) {
    ymuint val = 0U;
    if ( input(i) ) {
      val |= 1U;
    }
    if ( output(i) ) {
      val |= 2U;
    }
    iovect[i] = val;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス BdnDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdnDff::BdnDff()
{
}

// @brief デストラクタ
BdnDff::~BdnDff()
{
}


//////////////////////////////////////////////////////////////////////
// クラス BdnLatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BdnLatch::BdnLatch()
{
}

// @brief デストラクタ
BdnLatch::~BdnLatch()
{
}


///////////////////////////////////////////////////////////////////////
// クラス BdnNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
BdnNode::BdnNode() :
  mFlags(0U),
  mLevel(0)
{
  mFanins[0].set_to(this, 0);
  mFanins[1].set_to(this, 1);
}

// デストラクタ
BdnNode::~BdnNode()
{
}

// @brief ファンアウトに出力が含まれているか調べ pomark をセットする．
void
BdnNode::scan_po()
{
  mFlags &= ~kPoMask;
  const BdnFanoutList& folist = fanout_list();
  for (BdnFanoutList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    BdnEdge* e = *p;
    if ( e->is_poedge() ) {
      mFlags |= kPoMask;
      break;
    }
  }
}

// @brief 関連するポートを返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では NULL を返す．
const BdnPort*
BdnNode::port() const
{
  return mAuxData->port();
}

// @brief ポート中の位置を返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では 0 を返す．
ymuint
BdnNode::port_bitpos() const
{
  return mAuxData->port_bitpos();
}

// @brief 入出力ノードの場合に相方のノードを返す．
const BdnNode*
BdnNode::alt_node() const
{
  const BdnPort* port = mAuxData->port();
  if ( port ) {
    ymuint pos = port_bitpos();
    if ( is_input() ) {
      return port->output(pos);
    }
    else {
      return port->input(pos);
    }
  }
  return NULL;
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const BdnDff*
BdnNode::dff() const
{
  return mAuxData->dff();
}

// @brief 関連するラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const BdnLatch*
BdnNode::latch() const
{
  return mAuxData->latch();
}

// @brief ファンインのハンドルを得る．
BdnConstNodeHandle
BdnNode::output_fanin_handle() const
{
  return BdnConstNodeHandle(output_fanin(), output_fanin_inv());
}

// @brief ファンインのハンドルを得る．
BdnNodeHandle
BdnNode::output_fanin_handle()
{
  return BdnNodeHandle(output_fanin(), output_fanin_inv());
}

// @brief ファンインのハンドルを得る．
// @param[in] pos 入力番号(0 or 1)
// @return pos 番めのファンインのハンドルを返す．
BdnConstNodeHandle
BdnNode::fanin_handle(ymuint pos) const
{
  return BdnConstNodeHandle(fanin(pos), fanin_inv(pos));
}

// @brief ファンインのハンドルを得る．
// @param[in] pos 入力番号(0 or 1)
// @return pos 番めのファンインのハンドルを返す．
BdnNodeHandle
BdnNode::fanin_handle(ymuint pos)
{
  return BdnNodeHandle(fanin(pos), fanin_inv(pos));
}

// @brief ファンイン0のハンドルを得る．
BdnConstNodeHandle
BdnNode::fanin0_handle() const
{
  return BdnConstNodeHandle(fanin0(), fanin0_inv());
}

// @brief ファンイン0のハンドルを得る．
BdnNodeHandle
BdnNode::fanin0_handle()
{
  return BdnNodeHandle(fanin0(), fanin0_inv());
}

// @brief ファンイン0のハンドルを得る．
BdnConstNodeHandle
BdnNode::fanin1_handle() const
{
  return BdnConstNodeHandle(fanin1(), fanin1_inv());
}

// @brief ファンイン0のハンドルを得る．
BdnNodeHandle
BdnNode::fanin1_handle()
{
  return BdnNodeHandle(fanin1(), fanin1_inv());
}


//////////////////////////////////////////////////////////////////////
// クラス BdnAuxData
//////////////////////////////////////////////////////////////////////

// @brief 関連するポートを返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では NULL を返す．
const BdnPort*
BdnAuxData::port() const
{
  return NULL;
}

// @brief ポート中の位置を返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では 0 を返す．
ymuint
BdnAuxData::port_bitpos() const
{
  return 0;
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const BdnDff*
BdnAuxData::dff() const
{
  return NULL;
}

// @brief 関連するラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const BdnLatch*
BdnAuxData::latch() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス BdnPortData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port ポート
// @param[in] bitpos ビット位置
BdnPortData::BdnPortData(BdnPort* port,
			 ymuint bitpos) :
  mPort(port),
  mBitPos(bitpos)
{
}

// @brief デストラクタ
BdnPortData::~BdnPortData()
{
}

// @brief 関連するポートを返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では NULL を返す．
const BdnPort*
BdnPortData::port() const
{
  return mPort;
}

// @brief ポート中の位置を返す．
// @note kINPUT および kOUTPUT の時に意味を持つ．
// @note それ以外では 0 を返す．
ymuint
BdnPortData::port_bitpos() const
{
  return mBitPos;
}


//////////////////////////////////////////////////////////////////////
// クラス BdnDffData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff D-FF
BdnDffData::BdnDffData(BdnDff* dff) :
  mDff(dff)
{
}

// @brief デストラクタ
BdnDffData::~BdnDffData()
{
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const BdnDff*
BdnDffData::dff() const
{
  return mDff;
}


//////////////////////////////////////////////////////////////////////
// クラス BdnLatchData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] latch ラッチ
BdnLatchData::BdnLatchData(BdnLatch* latch) :
  mLatch(latch)
{
}

// @brief デストラクタ
BdnLatchData::~BdnLatchData()
{
}

// @brief 関連するラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const BdnLatch*
BdnLatchData::latch() const
{
  return mLatch;
}

END_NAMESPACE_YM_NETWORKS_BDN
