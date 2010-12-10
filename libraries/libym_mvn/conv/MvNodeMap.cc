
/// @file libym_mvn/conv/MvNodeMap.cc
/// @brief MvNodeMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvNodeMap.h"
#include "ym_sbj/SbjGraph.h"
#include "ym_mvn/MvMgr.h"
#include "ym_mvn/MvModule.h"
#include "ym_mvn/MvNode.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
MvNodeMap::MvNodeMap(ymuint n) :
  mArray(n)
{
}

// @brief デストラクタ
MvNodeMap::~MvNodeMap()
{
}

// @brief 登録する．(1ビット版)
// @param[in] mvnode MvNode
// @param[in] sbjhandle SbjHandle
void
MvNodeMap::put(const MvNode* mvnode,
	       SbjHandle sbjhandle)
{
  put(mvnode, 0, sbjhandle);
}

// @brief 登録する．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
// @param[in] sbjhandle SbjHandle
void
MvNodeMap::put(const MvNode* mvnode,
	       ymuint index,
	       SbjHandle sbjhandle)
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  vector<SbjHandle>& array = mArray[mvnode->id()];
  if ( array.size() != mvnode->output(0)->bit_width() ) {
    array.resize(mvnode->output(0)->bit_width());
  }
  array[index] = sbjhandle;
}

// @brief 探す．(1ビット版)
// @param[in] mvnode MvNode
SbjHandle
MvNodeMap::get(const MvNode* mvnode) const
{
  return get(mvnode, 0);
}

// @brief 探す．(ベクタ版)
// @param[in] mvnode MvNode
// @param[in] index ビット位置
SbjHandle
MvNodeMap::get(const MvNode* mvnode,
	       ymuint index) const
{
  assert_cond( mArray.size() > mvnode->id(), __FILE__, __LINE__);
  const vector<SbjHandle>& array = mArray[mvnode->id()];
  if ( array.empty() ) {
    return SbjHandle(NULL, false);
  }
  assert_cond( array.size() == mvnode->output(0)->bit_width(),
	       __FILE__, __LINE__);
  return array[index];
}


BEGIN_NONAMESPACE

void
dump_sbjhandle(ostream& s,
	       SbjHandle h)
{
  if ( h.is_const0() ) {
    s << "0";
  }
  else if ( h.is_const1() ) {
    s << "1";
  }
  else {
    if ( h.inv() ) {
      s << "~";
    }
    s << h.node()->id_str();
  }
}

END_NONAMESPACE

void
dump_mvnode_map(ostream& s,
		const MvMgr& mvmgr,
		const MvNodeMap& mvnode_map)
{
  ymuint n = mvmgr.max_node_id();
  for (ymuint i = 0; i < n; ++ i) {
    const MvNode* node = mvmgr.node(i);
    if ( node == NULL ) continue;
    const MvOutputPin* opin = node->output(0);
    ymuint bw = opin->bit_width();
    if ( bw == 1 ) {
      SbjHandle sbjhandle = mvnode_map.get(node);
      s << "// node" << node->id() << " : ";
      dump_sbjhandle(s, sbjhandle);
      s << endl;
    }
    else {
      for (ymuint i = 0; i < bw; ++ i) {
	SbjHandle sbjhandle = mvnode_map.get(node, i);
	s << "// node" << node->id() << " [" << i << "] : ";
	dump_sbjhandle(s, sbjhandle);
	s << endl;
      }
    }
  }
}

END_NAMESPACE_YM_MVN
