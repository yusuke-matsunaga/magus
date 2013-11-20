#ifndef PYSATMSGHANDLER_H
#define PYSATMSGHANDLER_H

/// @file PySatMsgHandler.h
/// @brief PySatMshgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/SatMsgHandler.h"
#include "ym_logic/pylogic.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PySatMsgHandler PySatMsgHanlder.h "PySatMsgHandler.h"
/// @brief Python のコードを呼び出す SatMsgHandler の継承クラス
//////////////////////////////////////////////////////////////////////
class PySatMsgHandler :
  public SatMsgHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] func_obj Python の関数オブジェクト
  PySatMsgHandler(PyObject* func_obj);

  /// @brief デストラクタ
  virtual
  ~PySatMsgHandler();


public:

  /// @brief 内部状態の出力を行う仮想関数
  /// @param[in] stats SAT ソルバの内部状態
  virtual
  void
  operator()(const SatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行う Python の関数オブジェクト
  PyObject* mFuncObj;

};

END_NAMESPACE_YM

#endif // PYSATMSGHANDLER_H
