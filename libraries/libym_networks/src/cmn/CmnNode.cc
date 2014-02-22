
/// @file CmnNode.cc
/// @brief CmnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "networks/CmnNode.h"
#include "networks/CmnPort.h"
#include "networks/CmnEdge.h"

#include "CmnNodeInput.h"
#include "CmnNodePI.h"
#include "CmnNodeDffOut.h"
#include "CmnNodeLatchOut.h"

#include "CmnNodeOutput.h"
#include "CmnNodePO.h"
#include "CmnNodeDffIn.h"
#include "CmnNodeLatchIn.h"

#include "CmnNodeLogic.h"


BEGIN_NAMESPACE_YM_NETWORKS_CMN

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

// @brief 入力数を得る．
ymuint
CmnNode::fanin_num() const
{
  return 0;
}

// @brief ファンインのノードを得る．
// @param[in] pos 入力番号
// @return pos 番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
const CmnNode*
CmnNode::fanin(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
const CmnEdge*
CmnNode::fanin_edge(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
CmnEdge*
CmnNode::_fanin_edge(ymuint pos)
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief セルを得る．
const Cell*
CmnNode::cell() const
{
  assert_not_reached(__FILE__, __LINE__);
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
// @param[in] port 関連するポート
// @param[in] bitpos ポート中の位置
CmnNodePI::CmnNodePI(const CmnPort* port,
		     ymuint bitpos) :
  mPort(port),
  mBitPos(bitpos)
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
  return mPort->output(mBitPos);
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffOut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 親の D-FF
CmnNodeDffOut::CmnNodeDffOut(CmnDff* dff) :
  mDff(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffOut1::CmnNodeDffOut1(CmnDff* dff) :
  CmnNodeDffOut(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffOut2::CmnNodeDffOut2(CmnDff* dff) :
  CmnNodeDffOut(dff)
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
// @param[in] latch 親のラッチ
CmnNodeLatchOut::CmnNodeLatchOut(CmnLatch* latch) :
  mLatch(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchOut1::CmnNodeLatchOut1(CmnLatch* latch) :
  CmnNodeLatchOut(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchOut2::CmnNodeLatchOut2(CmnLatch* latch) :
  CmnNodeLatchOut(latch)
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
CmnNodeOutput::CmnNodeOutput()
{
  mFanin.set_to(this, 0);
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

// @brief ファンイン数を得る．
ymuint
CmnNodeOutput::fanin_num() const
{
  return 1;
}

// @brief ファンインのノードを得る．
// @param[in] pos 入力番号
// @return pos 番めのファンインのノード
// @note 該当するファンインがなければ NULL を返す．
const CmnNode*
CmnNodeOutput::fanin(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mFanin.from();
}

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
const CmnEdge*
CmnNodeOutput::fanin_edge(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mFanin;
}

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
CmnEdge*
CmnNodeOutput::_fanin_edge(ymuint pos)
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return &mFanin;
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodePO
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port 関連するポート
// @param[in] bitpos ポート中の位置
CmnNodePO::CmnNodePO(const CmnPort* port,
		     ymuint bitpos) :
  mPort(port),
  mBitPos(bitpos)
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
  return mPort->input(mBitPos);
}


//////////////////////////////////////////////////////////////////////
// クラス CmnNodeDffIn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dff 親の D-FF
CmnNodeDffIn::CmnNodeDffIn(CmnDff* dff) :
  mDff(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffData::CmnNodeDffData(CmnDff* dff) :
  CmnNodeDffIn(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffClock::CmnNodeDffClock(CmnDff* dff) :
  CmnNodeDffIn(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffClear::CmnNodeDffClear(CmnDff* dff) :
  CmnNodeDffIn(dff)
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
// @param[in] dff 親の D-FF
CmnNodeDffPreset::CmnNodeDffPreset(CmnDff* dff) :
  CmnNodeDffIn(dff)
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
// @param[in] latch 親のラッチ
CmnNodeLatchIn::CmnNodeLatchIn(CmnLatch* latch) :
  mLatch(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchData::CmnNodeLatchData(CmnLatch* latch) :
  CmnNodeLatchIn(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchEnable::CmnNodeLatchEnable(CmnLatch* latch) :
  CmnNodeLatchIn(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchClear::CmnNodeLatchClear(CmnLatch* latch) :
  CmnNodeLatchIn(latch)
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
// @param[in] latch 親のラッチ
CmnNodeLatchPreset::CmnNodeLatchPreset(CmnLatch* latch) :
  CmnNodeLatchIn(latch)
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
CmnNodeLogic::CmnNodeLogic(ymuint ni,
			   CmnEdge* fanins,
			   const Cell* cell) :
  mNi(ni),
  mFanins(fanins),
  mCell(cell)
{
  for (ymuint i = 0; i < ni; ++ i) {
    mFanins[i].set_to(this, i);
  }
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
CmnNodeLogic::fanin_num() const
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

// @brief ファンインの枝を得る．
// @param[in] pos 入力番号
// @return pos 番目の入力の枝
// @note 該当するファンインの枝がなければ NULL を返す．
CmnEdge*
CmnNodeLogic::_fanin_edge(ymuint pos)
{
  return &mFanins[pos];
}

// @brief セルを得る．
const Cell*
CmnNodeLogic::cell() const
{
  return mCell;
}

END_NAMESPACE_YM_NETWORKS_CMN
