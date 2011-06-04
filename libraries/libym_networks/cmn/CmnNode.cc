
/// @file libym_networks/cmn/CmnNode.cc
/// @brief CmnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: LutNetwork.cc 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/CmnNode.h"
#include "ym_networks/CmnEdge.h"

#include "CmnNodeInput.h"
#include "CmnNodePI.h"
#include "CmnNodeDffOut.h"
#include "CmnNodeLatchOut.h"

#include "CmnNodeOutput.h"
#include "CmnNodePO.h"
#include "CmnNodeDffIn.h"
#include "CmnNodeLatchIn.h"

#include "CmnNodeLogic.h"


BEGIN_NAMESPACE_YM_CMN

///////////////////////////////////////////////////////////////////////
// クラス CmnNode
///////////////////////////////////////////////////////////////////////

// コンストラクタ
CmnNode::CmnNode()
{
}

// デストラクタ
CmnNode::~CmnNode()
{
}

// @brief ID を表す文字列の取得
string
CmnNode::id_str() const
{
  ostringstream buf;
  buf << id();
  return buf.str();
}

// @brief 入力ノードの時に true を返す．
bool
CmnNode::is_input() const
{
  return false;
}

// @brief 出力ノードの時に true を返す．
bool
CmnNode::is_output() const
{
  return false;
}

// @brief 論理ノードの時に true を返す．
bool
CmnNode::is_logic() const
{
  return false;
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNode::input_type() const
{
  assert_not_reached(__FILE__, __LINE__);
  return kPRIMARY_INPUT;
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNode::output_type() const
{
  assert_not_reached(__FILE__, __LINE__);
  return kPRIMARY_OUTPUT;
}

// @brief 関連するポートを返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では NULL を返す．
const CmnPort*
CmnNode::port() const
{
  return NULL;
}

// @brief ポート中のビット位置を返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では 0 を返す．
ymuint
CmnNode::port_bitpos() const
{
  return 0;
}

// @brief 入出力ノードの場合に相方のノードを返す．
// @note なければ NULL を返す．
const CmnNode*
CmnNode::alt_node() const
{
  return NULL;
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const CmnDff*
CmnNode::dff() const
{
  return NULL;
}

// @brief 関連するラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const CmnLatch*
CmnNode::latch() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeInput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeInput::CmnNodeInput()
{
}

// @brief デストラクタ
CmnNodeInput::~CmnNodeInput()
{
}

// @brief タイプを得る．
CmnNode::tType
CmnNodeInput::type() const
{
  return kINPUT;
}

// @brief 入力ノードの時に true を返す．
bool
CmnNodeInput::is_input() const
{
  return true;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodePI
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodePI::CmnNodePI() :
  mPort(NULL),
  mBitPos(0),
  mAltNode(NULL)
{
}

// @brief デストラクタ
CmnNodePI::~CmnNodePI()
{
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNodePI::input_type() const
{
  return kPRIMARY_INPUT;
}

// @brief 関連するポートを返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では NULL を返す．
const CmnPort*
CmnNodePI::port() const
{
  return mPort;
}

// @brief ポート中のビット位置を返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では 0 を返す．
ymuint
CmnNodePI::port_bitpos() const
{
  return mBitPos;
}

// @brief 入出力ノードの場合に相方のノードを返す．
// @note なければ NULL を返す．
const CmnNode*
CmnNodePI::alt_node() const
{
  return mAltNode;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffOut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffOut::CmnNodeDffOut() :
  mDff(NULL)
{
}

// @brief デストラクタ
CmnNodeDffOut::~CmnNodeDffOut()
{
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const CmnDff*
CmnNodeDffOut::dff() const
{
  return mDff;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffOut1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffOut1::CmnNodeDffOut1()
{
}

// @brief デストラクタ
CmnNodeDffOut1::~CmnNodeDffOut1()
{
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNodeDffOut1::input_type() const
{
  return kDFF_OUTPUT1;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffOut2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffOut2::CmnNodeDffOut2()
{
}

// @brief デストラクタ
CmnNodeDffOut2::~CmnNodeDffOut2()
{
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNodeDffOut2::input_type() const
{
  return kDFF_OUTPUT2;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchOut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchOut::CmnNodeLatchOut() :
  mLatch(NULL)
{
}

// @brief デストラクタ
CmnNodeLatchOut::~CmnNodeLatchOut()
{
}

// @brief 関連するラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const CmnLatch*
CmnNodeLatchOut::latch() const
{
  return mLatch;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchOut1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchOut1::CmnNodeLatchOut1()
{
}

// @brief デストラクタ
CmnNodeLatchOut1::~CmnNodeLatchOut1()
{
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNodeLatchOut1::input_type() const
{
  return kLATCH_OUTPUT1;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchOut2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchOut2::CmnNodeLatchOut2()
{
}

// @brief デストラクタ
CmnNodeLatchOut2::~CmnNodeLatchOut2()
{
}

// @brief 入力ノードのサブタイプを得る．
CmnNode::tInputType
CmnNodeLatchOut2::input_type() const
{
  return kLATCH_OUTPUT2;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeOutput
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeOutput::CmnNodeOutput() :
  mFanin(NULL)
{
}

// @brief デストラクタ
CmnNodeOutput::~CmnNodeOutput()
{
}

// @brief タイプを得る．
CmnNode::tType
CmnNodeOutput::type() const
{
  return kOUTPUT;
}

// @brief 出力ノードの時に true を返す．
bool
CmnNodeOutput::is_output() const
{
  return true;
}

// @brief ファンインのノードを得る．
// @note 出力ノードの場合のみ意味を持つ．
const CmnNode*
CmnNodeOutput::output_fanin() const
{
  return mFanin;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodePO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodePO::CmnNodePO() :
  mPort(NULL),
  mBitPos(0),
  mAltNode(NULL)
{
}

// @brief デストラクタ
CmnNodePO::~CmnNodePO()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodePO::output_type() const
{
  return kPRIMARY_OUTPUT;
}

// @brief 関連するポートを返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では NULL を返す．
const CmnPort*
CmnNodePO::port() const
{
  return mPort;
}

// @brief ポート中のビット位置を返す．
// @note kPRIMARY_INPUT および kPRIMARY_OUTPUT の時のみ意味を持つ．
// @note それ以外では 0 を返す．
ymuint
CmnNodePO::port_bitpos() const
{
  return mBitPos;
}

// @brief 入出力ノードの場合に相方のノードを返す．
// @note なければ NULL を返す．
const CmnNode*
CmnNodePO::alt_node() const
{
  return mAltNode;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffIn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffIn::CmnNodeDffIn() :
  mDff(NULL)
{
}

// @brief デストラクタ
CmnNodeDffIn::~CmnNodeDffIn()
{
}

// @brief 関連する D-FF を返す．
// @note D-FF に関連していない場合には NULL を返す．
const CmnDff*
CmnNodeDffIn::dff() const
{
  return mDff;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffData::CmnNodeDffData()
{
}

// @brief デストラクタ
CmnNodeDffData::~CmnNodeDffData()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeDffData::output_type() const
{
  return kDFF_DATA;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffClock
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffClock::CmnNodeDffClock()
{
}

// @brief デストラクタ
CmnNodeDffClock::~CmnNodeDffClock()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeDffClock::output_type() const
{
  return kDFF_CLOCK;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffClear
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffClear::CmnNodeDffClear()
{
}

// @brief デストラクタ
CmnNodeDffClear::~CmnNodeDffClear()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeDffClear::output_type() const
{
  return kDFF_CLEAR;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffPreset
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeDffPreset::CmnNodeDffPreset()
{
}

// @brief デストラクタ
CmnNodeDffPreset::~CmnNodeDffPreset()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeDffPreset::output_type() const
{
  return kDFF_PRESET;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchIn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchIn::CmnNodeLatchIn() :
  mLatch(NULL)
{
}

// @brief デストラクタ
CmnNodeLatchIn::~CmnNodeLatchIn()
{
}

// @brief 関連する ラッチを返す．
// @note ラッチに関連していない場合には NULL を返す．
const CmnLatch*
CmnNodeLatchIn::latch() const
{
  return mLatch;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchData::CmnNodeLatchData()
{
}

// @brief デストラクタ
CmnNodeLatchData::~CmnNodeLatchData()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeLatchData::output_type() const
{
  return kLATCH_DATA;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchEnable
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchEnable::CmnNodeLatchEnable()
{
}

// @brief デストラクタ
CmnNodeLatchEnable::~CmnNodeLatchEnable()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeLatchEnable::output_type() const
{
  return kLATCH_ENABLE;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchClear
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchClear::CmnNodeLatchClear()
{
}

// @brief デストラクタ
CmnNodeLatchClear::~CmnNodeLatchClear()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeLatchClear::output_type() const
{
  return kLATCH_CLEAR;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLatchPreset
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLatchPreset::CmnNodeLatchPreset()
{
}

// @brief デストラクタ
CmnNodeLatchPreset::~CmnNodeLatchPreset()
{
}

// @brief 出力ノードのサブタイプを得る．
CmnNode::tOutputType
CmnNodeLatchPreset::output_type() const
{
  return kLATCH_PRESET;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeLogic
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CmnNodeLogic::CmnNodeLogic() :
  mNi(0),
  mFanins(NULL),
  mCell(NULL)
{
}

// @brief デストラクタ
CmnNodeLogic::~CmnNodeLogic()
{
}

// @brief タイプを得る．
CmnNode::tType
CmnNodeLogic::type() const
{
  return kLOGIC;
}

// @brief 論理ノードの時に true を返す．
bool
CmnNodeLogic::is_logic() const
{
  return true;
}

// @brief 入力数を得る．
ymuint
CmnNodeLogic::ni() const
{
  return mNi;
}

// @brief ファンインのノードを得る．
// @param[in] pos 入力番号
// @return pos 番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
const CmnNode*
CmnNodeLogic::fanin(ymuint pos) const
{
  return mFanins[pos].from();
}

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
const CmnEdge*
CmnNodeLogic::fanin_edge(ymuint pos) const
{
  return &mFanins[pos];
}

// @brief セルを得る．
const Cell*
CmnNodeLogic::cell() const
{
  return mCell;
}

END_NAMESPACE_YM_CMN
