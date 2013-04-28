#ifndef YM_UTILS_PYUTILS_H
#define YM_UTILS_PYUTILS_H

/// @file ym_utils/pyutils.h
/// @brief libym_utils の Python 用拡張モジュール
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ympython.h"
#include "ym_utils/msg_type.h"


BEGIN_NAMESPACE_YM

class FileInfo;
class FileLoc;
class FileRegion;
class CombiGen;
class PermGen;
class MultiCombiGen;
class MultiPermGen;
class MultiSetCombiGen;
class MultiSetPermGen;
class RandGen;
class RandCombiGen;
class RandPermGen;
class StopWatch;
class MStopWatch;
class USTime;
class MFSet;
class ItvlMgr;
class NameMgr;
class FileBinI;
class FileBinO;


//////////////////////////////////////////////////////////////////////
// PyFileInfo: ファイル情報を表す型
//////////////////////////////////////////////////////////////////////

/// @brief FileInfo を表すタイプオブジェクト
extern
PyTypeObject PyFileInfo_Type;

/// @brief PyFileInfo の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFileInfo型だった．
/// @retval false obj が他の型だった．
inline
bool
PyFileInfo_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFileInfo_Type;
}

/// @brief FileInfo から FileInfoObject を生成する．
/// @param[in] obj FileInfo オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyFileInfo_FromFileInfo(const FileInfo& obj);

/// @brief PyObject から FileInfo を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FileInfo を返す．
/// @note 変換が失敗したら TypeError を送出し，不正な値を返す．
extern
FileInfo
PyFileInfo_AsFileInfo(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFileLoc: ファイル位置を表す型
//////////////////////////////////////////////////////////////////////

/// @brief FileLoc を表すタイプオブジェクト
extern
PyTypeObject PyFileLoc_Type;

/// @brief PyFileLoc の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFileLoc_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFileLoc_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFileLoc_Type;
}

/// @brief FileLoc から FileLocObject を生成する．
/// @param[in] obj FileLoc オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyFileLoc_FromFileLoc(const FileLoc& obj);

/// @brief PyObject から FileLoc を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FileLoc を返す．
/// @note 変換が失敗したら TypeError を送出し，不正な値を返す．
extern
FileLoc
PyFileLoc_AsFileLoc(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFileRegion: ファイルの領域を表す型
//////////////////////////////////////////////////////////////////////

/// @brief FileRegion を表すタイプオブジェクト
extern
PyTypeObject PyFileRegion_Type;

/// @brief FileRegion の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFileRegion_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFileRegion_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFileRegion_Type;
}

/// @brief FileRegion から FileRegionObject を生成する．
/// @param[in] obj FileRegion オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyFileRegion_FromFileRegion(const FileRegion& obj);

/// @brief PyObject から FileRegion を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FileRegion を返す．
/// @note 変換が失敗したら TypeError を送出し，不正な値を返す．
extern
FileRegion
PyFileRegion_AsFileRegion(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMsgType: メッセージタイプを表す型
//////////////////////////////////////////////////////////////////////

/// @brief MsgType を表すタイプオブジェクト
extern
PyTypeObject PyMsgType_Type;

/// @brief PyMsgType の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMsgType_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMsgType_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMsgType_Type;
}

/// @brief tMsgType から PyObject を生成する．
/// @param[in] obj tMsgType オブジェクト
/// @return 変換した Python オブジェクトを返す．
extern
PyObject*
PyMsgType_FromMsgType(tMsgType obj);

/// @brief 文字列から PyObject を生成する．
/// @param[in] str 文字列
/// @return 変換した Python オブジェクトを返す．
/// @note str が不正な値の場合には ValueError を送出し，NULL を返す．
extern
PyObject*
PyMsgType_FromString(const char* str);

/// @brief long から PyObject を生成する．
/// @param[in] val メッセージタイプを表す値 ( 0 - 5 )
/// @return 変換した Python オブジェクトを返す．
/// @note val が不正な値の場合には ValueError を送出し，NULL を返す．
extern
PyObject*
PyMsgType_FromLong(ymlong val);

/// @brief PyObject から tMsgType を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return tMsgType を返す．
/// @note 変換が失敗したら TypeError を送出し，kMsgError を返す．
extern
tMsgType
PyMsgType_AsMsgType(PyObject* py_obj);

/// @brief kMsgError を表す定数オブジェクト
extern
PyObject* Py_kMsgError;

/// @brief kMsgWarning を表す定数オブジェクト
extern
PyObject* Py_kMsgWarning;

/// @brief kMsgFailure を表す定数オブジェクト
extern
PyObject* Py_kMsgFailure;

/// @brief kMsgInfo を表す定数オブジェクト
extern
PyObject* Py_kMsgInfo;

/// @brief kMsgDebug を表す定数オブジェクト
extern
PyObject* Py_kMsgDebug;


//////////////////////////////////////////////////////////////////////
// PyMsgMgr: MsgMgr を表す型
//////////////////////////////////////////////////////////////////////

/// @brief MsgMgr を表すタイプオブジェクト
extern
PyTypeObject PyMsgMgr_Type;

/// @brief PyMsgMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMsgMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMsgMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMsgMgr_Type;
}


//////////////////////////////////////////////////////////////////////
// PyUSTime: USTime を表す型
//////////////////////////////////////////////////////////////////////

/// @brief USTime を表すタイプオブジェクト
extern
PyTypeObject PyUSTime_Type;

/// @brief PyUSTime の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyUSTime_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyUSTime_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyUSTime_Type;
}

/// @brief USTime から USTimeObject を生成する．
/// @param[in] obj USTime オブジェクト
extern
PyObject*
PyUSTime_FromUSTime(const USTime& obj);

/// @brief PyObject から USTime を取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return USTime を返す．
/// @note 変換が失敗したら TypeError を送出し，適当な値を返す．
extern
USTime
PyUSTime_AsUSTime(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyStopWatch: StopWatch を表す型
//////////////////////////////////////////////////////////////////////

/// @brief StopWatch を表すタイプオブジェクト
extern
PyTypeObject PyStopWatch_Type;

/// @brief PyStopWatch の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyStopWatch_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyStopWatch_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyStopWatch_Type;
}

/// @brief PyObject から StopWatch へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return StopWatch へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
StopWatch*
PyStopWatch_AsStopWatchPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMStopWatch: MStopWatch を表す型
//////////////////////////////////////////////////////////////////////

/// @brief MStopWatch を表すタイプオブジェクト
extern
PyTypeObject PyMStopWatch_Type;

/// @brief PyMStopWatch の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMStopWatch_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMStopWatch_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMStopWatch_Type;
}

/// @brief PyObject から MStopWatch へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MStopWatch へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MStopWatch*
PyMStopWatch_AsMStopWatchPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyRandGen: 乱数生成器を表す型
//////////////////////////////////////////////////////////////////////

/// @brief RandGen を表すタイプオブジェクト
extern
PyTypeObject PyRandGen_Type;

/// @brief PyRandGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyRandGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyRandGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyRandGen_Type;
}

/// @brief PyObject から RandGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return RandGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
RandGen*
PyRandGen_AsRandGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyRandPermGen: ランダムな順列発生器を表す型
//////////////////////////////////////////////////////////////////////

/// @brief RandPermGen を表すタイプオブジェクト
extern
PyTypeObject PyRandPermGen_Type;

/// @brief PyRandPermGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyRandPermGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyRandPermGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyRandPermGen_Type;
}


//////////////////////////////////////////////////////////////////////
// PyRandCombiGen: ランダムな組み合わせ発生器を表す型
//////////////////////////////////////////////////////////////////////

/// @brief RandCombiGen を表すタイプオブジェクト
extern
PyTypeObject PyRandCombiGen_Type;

/// @brief PyRandCombiGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyRandCombiGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyRandCombiGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyRandCombiGen_Type;
}


//////////////////////////////////////////////////////////////////////
// PyMFSet: Merge-Find-Set を表す型
//////////////////////////////////////////////////////////////////////

/// @brief MFSet を表すタイプオブジェクト
extern
PyTypeObject PyMFSet_Type;

/// @brief PyMFSet の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMFSet_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMFSet_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMFSet_Type;
}

/// @brief PyObject から MFSet へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MFSet へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MFSet*
PyMFSet_AsMFSetPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyItvlMgr: 区間を管理する型
//////////////////////////////////////////////////////////////////////

/// @brief ItvlMgr を表す型
extern
PyTypeObject PyItvlMgr_Type;

/// @brief PyItvlMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyItvlMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyItvlMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyItvlMgr_Type;
}

/// @brief PyObject から ItvlMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return ItvlMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
ItvlMgr*
PyItvlMgr_AsItvlMgrPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyNameMgr: 名前を管理する型
//////////////////////////////////////////////////////////////////////

/// @brief NameMgr を表すタイプオブジェクト
extern
PyTypeObject PyNameMgr_Type;

/// @brief PyNameMgr の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyNameMgr_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyNameMgr_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyNameMgr_Type;
}

/// @brief PyObject から NameMgr へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return NameMgr へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
NameMgr*
PyNameMgr_AsNameMgrPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFileBinI: バイナリ形式の入力ファイルオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief FileBinI を表す型
extern
PyTypeObject PyFileBinI_Type;

/// @brief PyFileBinI の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFileBinI_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFileBinI_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFileBinI_Type;
}

/// @brief PyObject から FileBinI へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FileBinI へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
FileBinI*
PyFileBinI_AsFileBinIPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyFileBinO: バイナリ形式の出力ファイルオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief FileBinO を表す型
extern
PyTypeObject PyFileBinO_Type;

/// @brief PyFileBinO の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyFileBinO_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyFileBinO_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyFileBinO_Type;
}

/// @brief PyObject から FileBinO へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return FileBinO へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
FileBinO*
PyFileBinO_AsFileBinOPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyCombiGen: 組み合わせを列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief CombiGen を表す型
extern
PyTypeObject PyCombiGen_Type;

/// @brief PyCombiGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyCombiGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyCombiGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyCombiGen_Type;
}

/// @brief PyObject から CombiGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return CombiGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
CombiGen*
PyCombiGen_AsCombiGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyPermGen: 組み合わせを列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief PermGen を表す型
extern
PyTypeObject PyPermGen_Type;

/// @brief PyPermGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyPermGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyPermGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyPermGen_Type;
}

/// @brief PyObject から PermGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return PermGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
PermGen*
PyPermGen_AsPermGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMultiCombiGen: 組み合わせを列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief MultiCombiGen を表す型
extern
PyTypeObject PyMultiCombiGen_Type;

/// @brief PyMultiCombiGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMultiCombiGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMultiCombiGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMultiCombiGen_Type;
}

/// @brief PyObject から MultiCombiGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MultiCombiGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MultiCombiGen*
PyMultiCombiGen_AsMultiCombiGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMultiPermGen: 順列を列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief MultiPermGen を表す型
extern
PyTypeObject PyMultiPermGen_Type;

/// @brief PyMultiPermGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMultiPermGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMultiPermGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMultiPermGen_Type;
}

/// @brief PyObject から MultiPermGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MultiPermGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MultiPermGen*
PyMultiPermGen_AsMultiPermGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMultiSetCombiGen: 重複を許した集合の組み合わせを列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief MultiSetCombiGen を表す型
extern
PyTypeObject PyMultiSetCombiGen_Type;

/// @brief PyMultiSetCombiGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMultiSetCombiGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMultiSetCombiGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMultiSetCombiGen_Type;
}

/// @brief PyObject から MultiSetCombiGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MultiSetCombiGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MultiSetCombiGen*
PyMultiSetCombiGen_AsMultiSetCombiGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// PyMultiSetPermGen: 重複を許した集合の順列を列挙するオブジェクトを表す型
//////////////////////////////////////////////////////////////////////

/// @brief MultiSetPermGen を表す型
extern
PyTypeObject PyMultiSetPermGen_Type;

/// @brief PyMultiSetPermGen の型チェック
/// @param[in] obj Python オブジェクト
/// @retval true obj が PyMultiSetPermGen_Type だった．
/// @retval false obj が他の型だった．
inline
bool
PyMultiSetPermGen_Check(PyObject* obj)
{
  return Py_TYPE(obj) == &PyMultiSetPermGen_Type;
}

/// @brief PyObject から MultiSetPermGen へのポインタを取り出す．
/// @param[in] py_obj Python オブジェクト
/// @return MultiSetPermGen へのポインタを返す．
/// @note 変換が失敗したら TypeError を送出し，NULL を返す．
extern
MultiSetPermGen*
PyMultiSetPermGen_AsMultiSetPermGenPtr(PyObject* py_obj);


//////////////////////////////////////////////////////////////////////
// 引数のパース用便利関数
//////////////////////////////////////////////////////////////////////

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


END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// 初期化関数
//////////////////////////////////////////////////////////////////////

BEGIN_EXTERN_C

void
utils_init();

END_EXTERN_C

#endif // YM_UTILS_PYUTILS_H
