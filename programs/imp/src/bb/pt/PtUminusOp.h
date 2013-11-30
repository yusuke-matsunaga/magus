#ifndef SRC_PT_PTUMINUSOP_H
#define SRC_PT_PTUMINUSOP_H

/// @file src/pt/PtUminusOp.h
/// @brief PtUminusOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtUnaryOp.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtUminusOp PtUminusOp.h "PtUminusOp.h"
/// @brief 2の補数演算を表すノード
//////////////////////////////////////////////////////////////////////
class PtUminusOp :
  public PtUnaryOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] opr1 オペランド
  PtUminusOp(const FileRegion& file_region,
	     PtNode* opr1);

  /// @brief デストラクタ
  virtual
  ~PtUminusOp();


public:
  //////////////////////////////////////////////////////////////////////
  // PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  tType
  type() const;

  /// @brief 内容を表す文字列を返す．
  virtual
  string
  decompile() const;

  /// @brief 対応した AIG を作る．
  virtual
  void
  gen_aig(AigMgr& aigmgr,
	  const vector<Aig>& bvar_array,
	  ymuint bw,
	  vector<Aig>& out_array);

};

END_NAMESPACE_YM_BB

#endif // SRC_PT_PTUMINUSOP_H
