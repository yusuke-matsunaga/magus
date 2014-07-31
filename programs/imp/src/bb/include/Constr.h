#ifndef INCLUDE_CONSTR_H
#define INCLUDE_CONSTR_H

/// @file include/Constr.h
/// @brief Constr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "YmUtils/FileRegion.h"
#include "YmLogic/AigMgr.h"
#include "YmLogic/Aig.h"


BEGIN_NAMESPACE_YM_BB

class PtNode;

//////////////////////////////////////////////////////////////////////
/// @class Constr Constr.h "Constr.h"
/// @brief 制約式を表す基底クラス
//////////////////////////////////////////////////////////////////////
class Constr
{
public:

  /// @brief 制約式の型
  enum tType {
    kEq,
    kLt,
    kLe,
    kGt,
    kGe
  };


public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  Constr(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief デストラクタ
  virtual
  ~Constr();


public:

  /// @brief ファイル上の位置を得る．
  FileRegion
  file_region() const;

  /// @brief 式の型を得る．
  virtual
  tType
  type() const = 0;

  /// @brief 左辺式を得る．
  PtNode*
  lhs() const;

  /// @brief 右辺式を得る．
  PtNode*
  rhs() const;

  /// @brief 対応した AIG を作る．
  virtual
  void
  gen_aig(AigMgr& aigmgr,
	  const vector<Aig>& bvar_array,
	  ymuint bw,
	  vector<Aig>& tmp_list) = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ファイル上の位置
  FileRegion mFileRegion;

  // 左辺式
  PtNode* mLhs;

  // 右辺式
  PtNode* mRhs;

};

END_NAMESPACE_YM_BB

#endif // INCLUDE_CONSTR_H
