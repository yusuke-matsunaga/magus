
/// @file libym_bdn/BdnNode.cc
/// @brief BdnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdNetwork.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnDff.h"


BEGIN_NAMESPACE_YM_BDN

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
  mLevel &= ~1U;
  const BdnFanoutList& folist = fanout_list();
  for (BdnFanoutList::const_iterator p = folist.begin();
       p != folist.end(); ++ p) {
    BdnEdge* e = *p;
    if ( e->is_poedge() ) {
      mLevel |= 1U;
      break;
    }
  }
}

// @brief ID を表す文字列の取得
string
BdnNode::id_str() const
{
  ostringstream buf;
  if ( is_input() ) {
    buf << "I" << id();
  }
  else if ( is_logic() ) {
    buf << "L" << id();
  }
  else if ( is_output() ) {
    buf << "O" << id();
  }
  else {
    buf << "X" << id();
  }
  return buf.str();
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

// @brief ID を表す文字列の取得
// @note デバッグ時にしか意味を持たない
string
BdnDff::id_str() const
{
  ostringstream buf;
  buf << "DFF" << id();
  return buf.str();
}


END_NAMESPACE_YM_BDN
