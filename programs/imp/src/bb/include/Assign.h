#ifndef INCLUDE_ASSIGN_H
#define INCLUDE_ASSIGN_H

/// @file include/Assign.h
/// @brief Assign のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "PtNode.h"
#include "utils/FileRegion.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class Assign Assign.h "Assign.h"
/// @brief 代入文を表すクラス
//////////////////////////////////////////////////////////////////////
class Assign
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] lhs_id 左辺の変数番号
  /// @param[in] rhs 右辺式
  Assign(const FileRegion& file_region,
	 ymuint lhs_id,
	 PtNode* rhs);

  /// @brief デストラクタ
  ~Assign();


public:

  /// @brief ファイル上の位置を得る．
  FileRegion
  file_region() const;

  /// @brief 左辺の変数番号を得る．
  ymuint
  lhs_id() const;

  /// @brief 右辺式を得る．
  PtNode*
  rhs() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // 左辺の変数番号
  ymuint32 mLhsId;

  // 右辺式
  PtNode* mRhs;

};

END_NAMESPACE_YM_BB

#endif // INCLUDE_ASSIGN_H
