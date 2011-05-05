
/// @file libym_mislib/MislibPtImpl2.cc
/// @brief MislibPtImpl2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl2.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibPtImpl2.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibNot::MislibNot(const FileRegion& loc,
		     const MislibNode* child1) :
  MislibNodeImpl(loc),
  mChild1(child1)
{
}

// デストラクタ
MislibNot::~MislibNot()
{
}

// 種類を取り出す．
MislibNode::tType
MislibNot::type() const
{
  return kNot;
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibNot::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
const MislibNode*
MislibNot::child1() const
{
  return mChild1;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibNot::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return ~child1()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibNot::dump(ostream& s) const
{
  s << "<NOT>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "</NOT>" << endl;
}


//////////////////////////////////////////////////////////////////////
// 2項演算子を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibBop::MislibBop(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibNodeImpl(loc),
  mChild1(child1),
  mChild2(child2)
{
}

// デストラクタ
MislibBop::~MislibBop()
{
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibBop::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
const MislibNode*
MislibBop::child1() const
{
  return mChild1;
}

// 2番目の子供を取り出す．
const MislibNode*
MislibBop::child2() const
{
  return mChild2;
}


//////////////////////////////////////////////////////////////////////
// AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibAnd::MislibAnd(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibAnd::~MislibAnd()
{
}

// 種類を取り出す．
MislibNode::tType
MislibAnd::type() const
{
  return kAnd;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibAnd::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) & child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibAnd::dump(ostream& s) const
{
  s << "<AND>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</AND>" << endl;
}


//////////////////////////////////////////////////////////////////////
// OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibOr::MislibOr(const FileRegion& loc,
		   const MislibNode* child1,
		   const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibOr::~MislibOr()
{
}

// 種類を取り出す．
MislibNode::tType
MislibOr::type() const
{
  return kOr;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibOr::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) | child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibOr::dump(ostream& s) const
{
  s << "<OR>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</OR>" << endl;
}


//////////////////////////////////////////////////////////////////////
// XOR論理式を表すクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibXor::MislibXor(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibXor::~MislibXor()
{
}

// 種類を取り出す．
MislibNode::tType
MislibXor::type() const
{
  return kXor;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
LogExpr
MislibXor::to_expr(const hash_map<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) ^ child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibXor::dump(ostream& s) const
{
  s << "<XOR>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</XOR>" << endl;
}


//////////////////////////////////////////////////////////////////////
// ピンを表すクラス
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


//////////////////////////////////////////////////////////////////////
// ゲートを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc 位置情報
// @param[in] name 名前を表すパース木
// @param[in] area 面積を表すパース木
// @param[in] opin_name 出力ピン名を表すパース木
// @param[in] opin_expr 出力の論理式を表すパース木
// @param[in] ipin_list 入力ピンのリストを表すパース木
MislibGate::MislibGate(const FileRegion& loc,
		       const MislibNode* name,
		       const MislibNode* area,
		       const MislibNode* opin_name,
		       const MislibNode* opin_expr,
		       const MislibNode* ipin_list) :
  MislibNodeImpl(loc),
  mName(name),
  mArea(area),
  mOpinName(opin_name),
  mOpinExpr(opin_expr),
  mIpinList(ipin_list),
  mNext(NULL)
{
}

// @brief デストラクタ
MislibGate::~MislibGate()
{
}

// @brief 種類を取り出す．
tType
MislibGate::type() const
{
  return kGate;
}

// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
const MislibNode*
MislibGate::name() const
{
  return mName;
}

// @brief 面積を表すオブジェクトを返す．
const MislibNode*
MislibGate::area() const
{
  return mArea;
}

// @brief 出力ピン名を表すオブジェクトを返す．
const MislibNode*
MislibGate::opin_name() const
{
  return mOpinName;
}

// @brief 出力の論理式を表すオブジェクトを返す．
const MislibNode*
MislibGate::opin_expr() const
{
  return mOpinExpr;
}

// @brief 入力ピンのリストを表すオブジェクトを返す．
const MislibNode*
MislibGate::ipin_list() const
{
  return mIpinList;
}

// 次の要素を設定する．
void
MislibGate::set_next(MislibNodeImpl* next)
{
  mNext = next;
}

// 次の要素を取り出す．
const MislibNode*
MislibGate::next() const
{
  return mNext;
}

// @brief 内容を出力する．
void
MislibGate::dump(ostream& s) const
{
  s << "<GATE>" << endl;
  dump_loc(s);

  s << "<NAME>" << endl;
  name()->dump(s);
  s << "</NAME>" << endl;

  s << "<AREA>" << endl;
  area()->dump(s);
  s << "</AREA>" << endl;

  s << "<OPIN_NAME>" << endl;
  opin_name()->dump(s);
  s << "</OPIN_NAME>" << endl;

  s << "<OPIN_EXPR>" << endl;
  opin_expr()->dump(s);
  s << "</OPIN_EXPR>" << endl;

  s << "<IPIN_LIST>" << endl;
  ipin_list()->dump(s);
  s << "</IPIN_LIST>" << endl;

  s << "</GATE>" << endl;
}

END_NAMESPACE_YM_MISLIB
