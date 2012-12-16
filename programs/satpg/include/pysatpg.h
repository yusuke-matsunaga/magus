#ifndef PYSATPG_H
#define PYSATPG_H

/// @file pysatpg.h
/// @brief satpg の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// 型を表すタイプオブジェクト
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief FileInfo を表す型
extern
PyTypeObject FileInfoType;

/// @brief FileLoc を表す型
extern
PyTypeObject FileLocType;

/// @brief FileRegion を表す型
extern
PyTypeObject FileRegionType;

/// @brief MsgType を表す型
extern
PyTypeObject MsgTypeType;

/// @brief kMsgError を表すオブジェクト
extern
PyObject* Py_kMsgError;

/// @brief kMsgWarning を表すオブジェクト
extern
PyObject* Py_kMsgWarning;

/// @brief kMsgFailure を表すオブジェクト
extern
PyObject* Py_kMsgFailure;

/// @brief kMsgInfo を表すオブジェクト
extern
PyObject* Py_kMsgInfo;

/// @brief kMsgDebug を表すオブジェクト
extern
PyObject* Py_kMsgDebug;

/// @brief MsgMgr を表す型
extern
PyTypeObject MsgMgrType;

/// @brief USTime を表す型
extern
PyTypeObject USTimeType;

/// @brief StopWatch を表す型
extern
PyTypeObject StopWatchType;

/// @brief MStopWatch を表す型
extern
PyTypeObject MStopWatchType;

/// @brief RandGen を表す型
extern
PyTypeObject RandGenType;

/// @brief RandPermGen を表す型
extern
PyTypeObject RandPermGenType;

/// @brief RandCombiGen を表す型
extern
PyTypeObject RandCombiGenType;

/// @brief MFSet を表す型
extern
PyTypeObject MFSetType;

/// @brief ItvlMgr を表す型
extern
PyTypeObject ItvlMgrType;

/// @brief NameMgr を表す型
extern
PyTypeObject NameMgrType;

/// @brief FileBinI を表す型
extern
PyTypeObject FileBinIType;

/// @brief FileBinO を表す型
extern
PyTypeObject FileBinOType;
#endif

//////////////////////////////////////////////////////////////////////
// 型をチェックする関数
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief FileInfoType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FileInfoType だった．
/// @retval false obj が他の型だった．
inline
bool
FileInfoObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &FileInfoType;
}

/// @brief FileLocType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FileLocType だった．
/// @retval false obj が他の型だった．
inline
bool
FileLocObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &FileLocType;
}

/// @brief FileRegionType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FileRegionType だった．
/// @retval false obj が他の型だった．
inline
bool
FileRegionObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &FileRegionType;
}

/// @brief MsgTypeType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が MsgTypeType だった．
/// @retval false obj が他の型だった．
inline
bool
MsgTypeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &MsgTypeType;
}

/// @brief MsgMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が MsgMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
MsgMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &MsgMgrType;
}

/// @brief USTimeType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が USTimeType だった．
/// @retval false obj が他の型だった．
inline
bool
USTimeObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &USTimeType;
}

/// @brief StopWatchType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が StopWatchType だった．
/// @retval false obj が他の型だった．
inline
bool
StopWatchObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &StopWatchType;
}

/// @brief MStopWatchType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が MStopWatchType だった．
/// @retval false obj が他の型だった．
inline
bool
MStopWatchObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &MStopWatchType;
}

/// @brief RandGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandGenType;
}

/// @brief RandPermGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandPermGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandPermGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandPermGenType;
}

/// @brief RandCombiGenType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が RandCombiGenType だった．
/// @retval false obj が他の型だった．
inline
bool
RandCombiGenObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &RandCombiGenType;
}

/// @brief MFSetType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が MFSetType だった．
/// @retval false obj が他の型だった．
inline
bool
MFSetObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &MFSetType;
}

/// @brief ItvlMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が ItvlMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
ItvlMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &ItvlMgrType;
}

/// @brief NameMgrType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が NameMgrType だった．
/// @retval false obj が他の型だった．
inline
bool
NameMgrObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &NameMgrType;
}

/// @brief FileBinIType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FileBinIType だった．
/// @retval false obj が他の型だった．
inline
bool
FileBinIObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &FileBinIType;
}

/// @brief FileBinOType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が FileBinOType だった．
/// @retval false obj が他の型だった．
inline
bool
FileBinOObject_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &FileBinOType;
}
#endif

//////////////////////////////////////////////////////////////////////
// PyObject からの型変換
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief PyObject から FileInfo を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj FileInfo を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が FileInfoObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   FileInfo& obj);

/// @brief PyObject から FileLoc を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj FileLoc を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が FileLocObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   FileLoc& obj);

/// @brief PyObject から FileRegion を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj FileRegion を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が FileRegionObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   FileRegion& obj);

/// @brief PyObject から tMsgType を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj MsgType を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が MsgTypeObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   tMsgType& obj);

/// @brief PyObject から USTime を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] obj USTime を格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が USTimeObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   USTime& obj);

/// @brief PyObject から StopWatch を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj StopWatch のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が StopWatchObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   StopWatch*& p_obj);

/// @brief PyObject から MStopWatch を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj MStopWatch のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が MStopWatchObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   MStopWatch*& p_obj);

/// @brief PyObject から RandGen を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj RandGen のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が RandGenObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   RandGen*& p_obj);

/// @brief PyObject から MFSet を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj MFSet のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した． py_obj が MFSetObject ではなかった．
extern
bool
conv_from_pyobject(PyObject* py_obj,
		   MFSet*& p_obj);

/// @brief PyObject から ItvlMgr を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj ItvlMgr のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が ItvlMgrObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   ItvlMgr*& p_obj);

/// @brief PyObject から NameMgr を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj NameMgr のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が NameMgrObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   NameMgr*& p_obj);

/// @brief PyObject から FileBinI を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj FileBinI のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が FileBinIObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   FileBinI*& p_obj);

/// @brief PyObject から FileBinO を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @param[out] p_obj FileBinO のポインタを格納する変数
/// @retval true 変換が成功した．
/// @retval false 変換が失敗した．py_obj が FileBinOObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   FileBinO*& p_obj);
#endif

//////////////////////////////////////////////////////////////////////
// PyObject への型変換
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief FileInfo から FileInfoObject を生成する．
/// @param[in] obj FileInfo オブジェクト
extern
PyObject*
FileInfo_FromFileInfo(const FileInfo& obj);

/// @brief FileLoc から FileLocObject を生成する．
/// @param[in] obj FileLoc オブジェクト
extern
PyObject*
FileLoc_FromFileLoc(const FileLoc& obj);

/// @brief FileRegion から FileRegionObject を生成する．
/// @param[in] obj FileRegion オブジェクト
extern
PyObject*
FileRegion_FromFileRegion(const FileRegion& obj);

/// @brief tMsgType から PyObject を生成する．
/// @param[in] obj tMsgType オブジェクト
extern
PyObject*
MsgType_FromMsgType(tMsgType obj);

/// @brief 文字列から MsgTypeObject を生成する．
extern
PyObject*
MsgType_FromString(const char* str);

/// @brief long から MsgTypeObject を生成する．
extern
PyObject*
MsgType_FromLong(ymlong val);

/// @brief USTime から USTimeObject を生成する．
/// @param[in] obj USTime オブジェクト
extern
PyObject*
USTime_FromUSTime(const USTime& obj);
#endif

//////////////////////////////////////////////////////////////////////
// 引数のパース用便利関数
//////////////////////////////////////////////////////////////////////

#if 0
/// @brief 引数をパースして FileBinO を取り出す．
/// @param[in] args 引数オブジェクト
/// @return FileBinO のポインタを返す．
/// @note エラーが起きたら NULL を返す．
extern
FileBinO*
parse_FileBinO(PyObject* args);

/// @brief 引数をパースして FileBinI を取り出す．
/// @param[in] args 引数オブジェクト
/// @return FileBinI のポインタを返す．
/// @note エラーが起きたら NULL を返す．
extern
FileBinI*
parse_FileBinI(PyObject* args);
#endif

END_NAMESPACE_YM_PYTHON


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
satpg_init();

END_EXTERN_C

#endif // PYSATPG_H
