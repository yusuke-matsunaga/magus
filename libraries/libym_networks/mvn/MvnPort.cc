
/// @file libym_networks/MvnPort.cc
/// @brief MvnPort の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnPort.h"
#include "ym_networks/MvnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス MvnPortRef
//////////////////////////////////////////////////////////////////////


// @brief この実体のビット幅を返す．
ymuint
MvnPortRef::bit_width() const
{
  if ( has_bitselect() ) {
    return 1;
  }
  else if ( has_partselect() ) {
    return msb() - lsb() + 1;
  }
  else {
    const MvnNode* n = node();
    if ( n->type() == MvnNode::kInput ) {
      return n->output(0)->bit_width();
    }
    else if ( n->type() == MvnNode::kOutput ) {
      return n->input(0)->bit_width();
    }
    else if ( n->type() == MvnNode::kInout ) {
      return n->input(0)->bit_width();
    }
  }
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}


//////////////////////////////////////////////////////////////////////
// クラス MvnPort
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] np portref 数
MvnPort::MvnPort(const char* name,
		 ymuint np) :
  mName(name),
  mPortRefNum(np),
  mPortRefArray(new MvnPortRef[np])
{
}

// @brief デストラクタ
MvnPort::~MvnPort()
{
  delete [] mPortRefArray;
}

// @brief ビット幅を得る．
ymuint
MvnPort::bit_width() const
{
  ymuint ans = 0;
  for (ymuint i = 0; i < mPortRefNum; ++ i) {
    MvnPortRef& pr = mPortRefArray[i];
    ans += pr.bit_width();
  }
  return ans;
}

END_NAMESPACE_YM_NETWORKS
