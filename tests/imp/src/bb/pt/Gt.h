#ifndef SRC_PT_GT_H
#define SRC_PT_GT_H

/// @file src/pt/Gt.h
/// @brief Gt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Constr.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class Gt Gt.h "Gt.h"
/// @brief '>' を表すクラス
//////////////////////////////////////////////////////////////////////
class Gt :
  public Constr
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  Gt(const FileRegion& file_region,
     PtNode* lhs,
     PtNode* rhs);

  /// @brief デストラクタ
  virtual
  ~Gt();


public:

  /// @brief 式の型を得る．
  virtual
  tType
  type() const;

  /// @brief 対応した AIG を作る．
  virtual
  void
  gen_aig(AigMgr& aigmgr,
	  const vector<Aig>& bvar_array,
	  ymuint bw,
	  vector<Aig>& tmp_list);

};

END_NAMESPACE_YM_BB

#endif // SRC_PT_GT_H
