#ifndef SRC_PT_PTNEGOP_H
#define SRC_PT_PTNEGOP_H

/// @file src/pt/PtNegOp.h
/// @brief PtNegOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtUnaryOp.h"


BEGIN_NAMESPACE_YM_BB

//////////////////////////////////////////////////////////////////////
/// @class PtNegOp PtNegOp.h "PtNegOp.h"
/// @brief neg (反転)演算を表すノード
//////////////////////////////////////////////////////////////////////
class PtNegOp :
  public PtUnaryOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] file_region ファイル上の位置
  /// @param[in] opr1 オペランド
  PtNegOp(const FileRegion& file_region,
	  PtNode* opr1);

  /// @brief デストラクタ
  virtual
  ~PtNegOp();


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

#endif // SRC_PT_PTNEGOP_H
