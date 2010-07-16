
/// @file libym_mvn/MvDff.cc
/// @brief MvDff の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvDff.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
// クラス MvDff
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff::MvDff(MvModule* module,
	     ymuint bit_width) :
  MvNode(module, 4, 1)
{
  set_bit_width(input(0), 1);
  set_bit_width(input(1), bit_width);
  set_bit_width(input(2), 1);
  set_bit_width(input(3), 1);
  set_bit_width(output(0), bit_width);
}

// @brief デストラクタ
MvDff::~MvDff()
{
}


//////////////////////////////////////////////////////////////////////
// クラス MvDff1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff1::MvDff1(MvModule* module,
	       ymuint bit_width) :
  MvDff(module, bit_width)
{
}

// @brief デストラクタ
MvDff1::~MvDff1()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff1::type() const
{
  return kDff1;
}


//////////////////////////////////////////////////////////////////////
// クラス MvDff2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] bit_width ビット幅
MvDff2::MvDff2(MvModule* module,
	       ymuint bit_width) :
  MvDff(module, bit_width)
{
}

// @brief デストラクタ
MvDff2::~MvDff2()
{
}

// @brief ノードの種類を得る．
MvNode::tType
MvDff2::type() const
{
  return kDff2;
}

END_NAMESPACE_YM_MVN
