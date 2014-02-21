
/// @file MislibPin.cc
/// @brief MislibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPin.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibPin
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibPin::MislibPin(const FileRegion& loc,
		     const MislibNode* name,
		     const MislibNode* phase,
		     const MislibNode* input_load,
		     const MislibNode* max_load,
		     const MislibNode* rise_block_delay,
		     const MislibNode* rise_fanout_delay,
		     const MislibNode* fall_block_delay,
		     const MislibNode* fall_fanout_delay) :
  MislibNodeImpl(loc),
  mName(name),
  mPhase(phase),
  mInputLoad(input_load),
  mMaxLoad(max_load),
  mRiseBlockDelay(rise_block_delay),
  mRiseFanoutDelay(rise_fanout_delay),
  mFallBlockDelay(fall_block_delay),
  mFallFanoutDelay(fall_fanout_delay)
{
  mNext = NULL;
}

// デストラクタ
MislibPin::~MislibPin()
{
}

// 種類を取り出す．
MislibNode::tType
MislibPin::type() const
{
  return kPin;
}

// ピン名を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::name() const
{
  return mName;
}

// 極性情報を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::phase() const
{
  return mPhase;
}

// 入力負荷を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::input_load() const
{
  return mInputLoad;
}

// 最大駆動負荷を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::max_load() const
{
  return mMaxLoad;
}

// 立ち上がり固定遅延を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::rise_block_delay() const
{
  return mRiseBlockDelay;
}

// 立ち上がり負荷依存遅延を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::rise_fanout_delay() const
{
  return mRiseFanoutDelay;
}

// 立ち下がり固定遅延を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::fall_block_delay() const
{
  return mFallBlockDelay;
}

// 立ち下がり負荷依存遅延を表すオブジェクトを取り出す．
const MislibNode*
MislibPin::fall_fanout_delay() const
{
  return mFallFanoutDelay;
}

// 次の要素を設定する．
void
MislibPin::set_next(MislibNodeImpl* pin)
{
  mNext = pin;
}

// 次の要素を取り出す．
const MislibNode*
MislibPin::next() const
{
  return mNext;
}

// 内容を出力する．
// デバッグ用
void
MislibPin::dump(ostream& s) const
{
  s << "<PIN>" << endl;
  dump_loc(s);

  s << "<NAME>" << endl;
  if ( name() ) {
    name()->dump(s);
  }
  else {
    s << "*" << endl;
  }
  s << "</NAME>" << endl;

  s << "<PHASE>" << endl;
  phase()->dump(s);
  s << "</PHASE>" << endl;

  s << "<INPUT_LOAD>" << endl;
  input_load()->dump(s);
  s << "</INPUT_LOAD>" << endl;

  s << "<MAX_LOAD>" << endl;
  max_load()->dump(s);
  s << "</MAX_LOAD>" << endl;

  s << "<RISE_BLOCK_DELAY>" << endl;
  rise_block_delay()->dump(s);
  s << "</RISE_BLOCK_DELAY>" << endl;

  s << "<RISE_FANOUT_DELAY>" << endl;
  rise_fanout_delay()->dump(s);
  s << "</RISE_FANOUT_DELAY>" << endl;

  s << "<FALL_BLOCK_DELAY>" << endl;
  fall_block_delay()->dump(s);
  s << "</FALL_BLOCK_DELAY>" << endl;

  s << "<FALLE_FANOUT_DELAY>" << endl;
  fall_fanout_delay()->dump(s);
  s << "</FALL_FANOUT_DELAY>" << endl;

  s << "</PIN>" << endl;
}

END_NAMESPACE_YM_MISLIB
