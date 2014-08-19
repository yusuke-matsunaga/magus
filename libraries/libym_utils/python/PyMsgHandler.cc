
/// @file PyMsgHandler.cc
/// @brief PyMsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PyMsgHandler.h"
#include "ym_utils/pyutils.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] obj Python の関数オブジェクト
// @param[in] mask メッセージマスク
PyMsgHandler::PyMsgHandler(PyObject* obj,
			   ymuint32 mask) :
  MsgHandler(mask),
  mFuncObj(obj)
{
  assert_cond( PyCallable_Check(obj), __FILE__, __LINE__);
}

// @brief デストラクタ
PyMsgHandler::~PyMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
PyMsgHandler::put_msg(const char* src_file,
		      int src_line,
		      const FileRegion& loc,
		      tMsgType type,
		      const char* label,
		      const char* body)
{
  if ( mFuncObj == NULL ) {
    return;
  }

  PyObject* loc_obj = PyFileRegion_FromFileRegion(loc);
  PyObject* args = Py_BuildValue("(siOkss)",
				 src_file, src_line,
				 loc_obj, type,
				 label, body);
  PyObject* result = PyObject_CallObject(mFuncObj, args);
  Py_DECREF(args);

  if ( result == NULL ) {
    // コールバック中でエラーが起こった．
    // けどこの関数はエラーを送出できない．
    PyErr_Clear();
  }
  else {
    // どちらにせよ結果は使わない．
    Py_DECREF(result);
  }
}

END_NAMESPACE_YM
