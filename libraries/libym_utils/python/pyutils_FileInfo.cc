
/// @file pyutils_FileInfo.cc
/// @brief FileInfo の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileInfo.h"
#include "ym_utils/FileLoc.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileInfo を表す型
struct FileInfoObject
{
  // Python のお約束
  PyObject_HEAD

  // FileInfo の値
  FileInfo mFileInfo;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileInfoObject の生成関数
FileInfoObject*
FileInfo_new(PyTypeObject* type)
{
  FileInfoObject* self = PyObject_New(FileInfoObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileInfo を初期化する．
  self->mFileInfo = FileInfo();

  return self;
}

// FileInfoObject を開放する関数
void
FileInfo_dealloc(FileInfoObject* self)
{
  // FileInfo の開放は必要ない．

  PyObject_Del(self);
}

// 初期化関数
int
FileInfo_init(FileInfoObject* self,
	      PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (string)
  // - (string, FileLoc)
  char* filename = NULL;
  PyObject* obj1 = NULL;
  if ( !PyArg_ParseTuple(args, "|sO!",
			 &filename,
			 &PyFileLoc_Type, &obj1) ) {
    return -1;
  }

  if ( obj1 != NULL ) {
    assert_cond( filename != NULL, __FILE__, __LINE__);
    FileLoc parent_loc = PyFileLoc_AsFileLoc(obj1);
    self->mFileInfo = FileInfo(filename, parent_loc);
  }
  else if ( filename != NULL ) {
    self->mFileInfo = FileInfo(filename);
  }
  else {
    self->mFileInfo = FileInfo();
  }

  return 0;
}

// str 関数
PyObject*
FileInfo_str(FileInfoObject* self)
{
  ostringstream buf;
  buf << self->mFileInfo;
  return PyObject_FromString(buf.str());
}

// is_valid 関数
PyObject*
FileInfo_is_valid(FileInfoObject* self,
		  PyObject* args)
{
  return PyObject_FromBool(self->mFileInfo.is_valid());
}

// id 関数
PyObject*
FileInfo_id(FileInfoObject* self,
	    PyObject* args)
{
  return PyObject_FromYmuint32(self->mFileInfo.id());
}

// filename 関数
PyObject*
FileInfo_filename(FileInfoObject* self,
		  PyObject* args)
{
  return PyObject_FromString(self->mFileInfo.filename());
}

// parent_loc 関数
PyObject*
FileInfo_parent_loc(FileInfoObject* self,
		    PyObject* args)
{
  return PyFileLoc_FromFileLoc(self->mFileInfo.parent_loc());
}

// parent_loc_list 関数
PyObject*
FileInfo_parent_loc_list(FileInfoObject* self,
			 PyObject* args)
{
  // FileInfoObject のリストを返す．
  list<FileLoc> loc_list;
  self->mFileInfo.parent_loc_list(loc_list);
  ymuint n = loc_list.size();
  PyObject* ans_list = PyList_New(n);
  ymuint i = 0;
  for (list<FileLoc>::iterator p = loc_list.begin();
       p != loc_list.end(); ++ p, ++ i) {
    PyObject* obj = PyFileLoc_FromFileLoc(*p);
    PyList_SetItem(obj, i, obj);
  }
  return ans_list;
}


// FileInfoObject のメソッドテーブル
PyMethodDef FileInfo_methods[] = {
  {"is_valid", (PyCFunction)FileInfo_is_valid, METH_NOARGS,
   PyDoc_STR("return true if valid (NONE)")},
  {"id", (PyCFunction)FileInfo_id, METH_NOARGS,
   PyDoc_STR("return ID (NONE)")},
  {"filename", (PyCFunction)FileInfo_filename, METH_NOARGS,
   PyDoc_STR("return filename (NONE)")},
  {"parent_loc", (PyCFunction)FileInfo_parent_loc, METH_NOARGS,
   PyDoc_STR("return parent FileInfo (NONE)")},
  {"parent_loc_list", (PyCFunction)FileInfo_parent_loc_list, METH_NOARGS,
   PyDoc_STR("return list of parent FileInfo's (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// FileInfoObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFileInfo_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileInfo",             // tp_name
  sizeof(FileInfoObject),       // tp_basicsize
  0,                            // tp_itemsize
  /* methods */
  (destructor)FileInfo_dealloc, // tp_dealloc
  0,                            // tp_print
  0,                            // tp_getattr
  0,                            // tp_setattr
  0,                            // tp_compare
  0,                            // tp_repr
  0,                            // tp_as_number
  0,                            // tp_as_sequence
  0,                            // tp_as_mapping
  0,                            // tp_hash
  0,                            // tp_call
  (reprfunc)FileInfo_str,       // tp_str
  0,                            // tp_getattro
  0,                            // tp_setattro
  0,                            // tp_as_buffer
  Py_TPFLAGS_DEFAULT,           // tp_flags
  "file information",           // tp_doc
  0,                            // tp_traverse
  0,                            // tp_clear
  0,                            // tp_richcompare
  0,                            // tp_weaklistoffset
  0,                            // tp_iter
  0,                            // tp_iternext
  FileInfo_methods,             // tp_methods
  0,                            // tp_members
  0,                            // tp_getset
  0,                            // tp_base
  0,                            // tp_dict
  0,                            // tp_descr_get
  0,                            // tp_descr_set
  0,                            // tp_dictoffset
  (initproc)FileInfo_init,      // tp_init
  0,                            // tp_alloc
  (newfunc)FileInfo_new,        // tp_new
  0,                            // tp_free
  0,                            // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileInfo 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief FileInfo から PyObject を生成する．
// @param[in] obj FileInfo オブジェクト
PyObject*
PyFileInfo_FromFileInfo(const FileInfo& obj)
{
  FileInfoObject* py_obj = FileInfo_new(&PyFileInfo_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mFileInfo = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から FileInfo を取り出す．
// @param[in] py_obj Python オブジェクト
// @return FileInfo を返す．
// @note 変換が失敗したら TypeError を送出し，不正な値を返す．
FileInfo
PyFileInfo_AsFileInfo(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFileInfo_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.FileInfo is expected");
    return FileInfo();
  }

  // 強制的にキャスト
  FileInfoObject* fileinfo_obj = (FileInfoObject*)py_obj;

  return fileinfo_obj->mFileInfo;
}

// FileInfoObject 関係の初期化を行う．
void
FileInfoObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFileInfo_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録を行う．
  PyModule_AddObject(m, "FileInfo", (PyObject*)&PyFileInfo_Type);
}

END_NAMESPACE_YM
