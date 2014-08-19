#ifndef SRC_PT_LT_H
#define SRC_PT_LT_H

/// @file src/pt/Lt.h
/// @brief Lt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Constr.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class Lt Lt.h "Lt.h"
/// @brief '<' 演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class Lt :
  public Constr
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  Lt(const FileRegion& file_region,
     PtNode* lhs,
     PtNode* rhs);

  /// @brief デストラクタ
  virtual
  ~Lt();


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

#endif // SRC_PT_LT_H
