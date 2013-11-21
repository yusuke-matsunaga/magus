#ifndef SRC_PT_LE_H
#define SRC_PT_LE_H

/// @file src/pt/Le.h
/// @brief Le のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Constr.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class Le Le.h "Le.h"
/// @brief '<=' を表すクラス
//////////////////////////////////////////////////////////////////////
class Le :
  public Constr
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] lhs 左辺式
  /// @param[in] rhs 右辺式
  Le(const FileRegion& file_region,
     PtNode* lhs,
     PtNode* rhs);

  /// @brief デストラクタ
  virtual
  ~Le();


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

#endif //
