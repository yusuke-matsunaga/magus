#ifndef GBMPY_H
#define GBMPY_H

/// @file gbmpy.h
/// @brief Python 用の拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Python.h"
#include "ym_gbm/Gbm.h"


//////////////////////////////////////////////////////////////////////
// 名前空間の定義
//////////////////////////////////////////////////////////////////////

/// @brief gbmpy 用の名前空間の開始
#define BEGIN_NAMESPACE_GMBPY			\
  BEGIN_NAMESPACE_YM_GBM			\
  BEGIN_NAMESPACE(nsPy)

/// @brief gbmpy 用の名前空間の終了
#define END_NAMESPACE_GBMPY			\
  END_NAMESPACE(nsPy)				\
  END_NAMESPACE_YM_GBM


BEGIN_NAMESPACE_GBM

/// @brief エラー用のオブジェクト
extern
PyObject* ErrorObject;



//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

/// @brief Gbm を表す型
extern
PyTypeObject GbmType;

/// @brief GbmNode を表す型
extern
PyTypeObject GbmNodeType;

/// @brief GbmNodeHandle を表す型
extern
PyTypeObject GbmNodeHandleType;


//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

/// @brief GbmObject 用の型チェック
/// @param[in] obj Python オブジェクト
inline
bool
GbmObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &GbmType;
}

/// @brief GbmNode 用の型チェック
/// @param[in] obj Python オブジェクト
inline
bool
GbmNodeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &GbmNodeType;
}

/// @brief GbmNodeHandle 用の型チェック
/// @param[in] obj Python オブジェクト
inline
bool
GbmNodeHandleObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &GbmNodeHandleType;
}


//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

/// @brief PyObject から Gbm を取り出す．
/// @param[in] obj Python オブジェクト
/// @param[out] pgbm Gbm オブジェクトを格納するポインタ
/// @retval true 変換を成功した．
/// @retval false 変換が失敗した．obj が GbmObject ではなかった．
extern
bool
covn_from_pyobject(PyObject* obj,
		   Gbm* pgbm);

/// @brief PyObject から GbmNode を取り出す．
/// @param[in] obj Python オブジェクト
/// @param[out] Node GbmNode オブジェクトを格納するポインタ
/// @retval true 変換を成功した．
/// @retval false 変換が失敗した．obj が GbmNodeObject ではなかった．
extern
bool
covn_from_pyobject(PyObject* obj,
		   GbmNode* pgbm);

/// @brief PyObject から GbmNodeHandle を取り出す．
/// @param[in] obj Python オブジェクト
/// @param[out] handle GbmNodeHandle オブジェクトを変数
/// @retval true 変換を成功した．
/// @retval false 変換が失敗した．obj が GbmNodeHandleObject ではなかった．
extern
bool
covn_from_pyobject(PyObject* obj,
		   GbmNodeHandle& handle);


//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

/// @brief Gbm から PyObject を生成する．
/// @param[in] gbm Gbm オブジェクト
extern
PyObject*
conv_to_pyobject(Gbm& gbm);

/// @brief GbmNode から PyObject を生成する．
/// @param[in] node GbmNode
extern
PyObject*
conv_to_pyobject(GbmNode* node);

/// @brief GbmNodeHandle から PyObject を生成する．
/// @param[in] handle GbmNodeHandle
extern
PyObject*
conv_to_pyobject(GbmNodeHandle handle);

END_NAMESPACE_GBM

#endif // GBMPY_H
