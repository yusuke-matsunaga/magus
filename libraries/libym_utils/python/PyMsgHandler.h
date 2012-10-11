#ifndef PYMSGHANDLER_H
#define PYMSGHANDLER_H

/// @file PyMsgHandler.h
/// @brief PyMsgHandler.h のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_utils/MsgHandler.h"


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
/// @class PyMsgHandler PyMsgHandler.h "PyMsgHandler.h"
/// @brief Python 拡張用の MsgHandler の継承クラス
//////////////////////////////////////////////////////////////////////
class PyMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] obj Python の関数オブジェクト
  /// @param[in] mask メッセージマスク
  PyMsgHandler(PyObject* obj,
	       ymuint32 mask = kMaskAll);

  /// @brief デストラクタ
  virtual
  ~PyMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler の継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] file_loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  tMsgType type,
	  const char* label,
	  const char* body);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージを扱う Python の関数本体
  PyObject* mFuncObj;

};

END_NAMESPACE_YM_PYTHON

#endif // PYMSGHANDLER_H
