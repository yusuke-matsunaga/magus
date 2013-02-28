#ifndef BACKTRACER_H
#define BACKTRACER_H

/// @file BackTracer.h
/// @brief BackTracer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BackTracer BackTracer.h "BackTracer.h"
/// @brief テストパタンを求めるためのバックトレースを行なうファンクター
//////////////////////////////////////////////////////////////////////
class BackTracer
{
public:

  /// @brief デストラクタ
  virtual
  ~BackTracer() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] val_list 求まったテストパタンを格納する配列
  void
  operator()(TpgNode* fnode,
	     const vector<TpgNode*>& output_list,
	     const vector<TpgNode*>& input_list,
	     vector<ymuint32>& val_list) = 0;

};


END_NAMESPACE_YM_SATPG

#endif // BACKTRACER_H
