
/// @file src/pt/Assign.cc
/// @brief Assign の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Assign.h"
#include "PtMgr.h"


BEGIN_NAMESPACE_YM_BB

// @brief 新しい代入文を追加する．
void
PtMgr::new_assign(const FileRegion& file_region,
		  ymuint lhs_id,
		  PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Assign));
  Assign* assign = new (p) Assign(file_region, lhs_id, rhs);
  mAssignList.push_back(assign);
}

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs_id 左辺の変数番号
// @param[in] rhs 右辺式
Assign::Assign(const FileRegion& file_region,
	       ymuint lhs_id,
	       PtNode* rhs) :
  mFileRegion(file_region),
  mLhsId(lhs_id),
  mRhs(rhs)
{
}

// @brief デストラクタ
Assign::~Assign()
{
}

// @brief ファイル上の位置を得る．
FileRegion
Assign::file_region() const
{
  return mFileRegion;
}

// @brief 左辺の変数番号を得る．
ymuint
Assign::lhs_id() const
{
  return mLhsId;
}

// @brief 右辺式を得る．
PtNode*
Assign::rhs() const
{
  return mRhs;
}

END_NAMESPACE_YM_BB
