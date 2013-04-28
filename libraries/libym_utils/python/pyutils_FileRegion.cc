
/// @file pyutils_FileRegion.cc
/// @brief FileRegion の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileRegion を表す型
struct FileRegionObject
{
  // Python のお約束
  PyObject_HEAD

  // FileRegion の値
  FileRegion mFileRegion;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileRegionObject の生成関数
FileRegionObject*
FileRegion_new(PyTypeObject* type)
{
  FileRegionObject* self = PyObject_New(FileRegionObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileRegion を初期化する．
  self->mFileRegion = FileRegion();

  return self;
}

// FileRegionObject を開放する関数
void
FileRegion_dealloc(FileRegionObject* self)
{
  // FileRegion の開放は必要ない．

  PyObject_Del(self);
}

// 初期化関数
int
FileRegion_init(FileRegionObject* self,
	      PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (FileInfo, uint, uint, uint, uint)
  // - (FileInfo, uint, uint, FileInfo, uint, uint)
  // - (FileLoc)
  // - (FileLoc, FileLoc)
  // - (FileRegion, FileRegion)
  ymuint n = PyTuple_GET_SIZE(args);
  if ( n == 0 ) {
    self->mFileRegion = FileRegion();
  }
  else if ( n == 1 ) {
    PyObject* obj1 = PyTuple_GET_ITEM(args, 0);
    if ( PyFileLoc_Check(obj1) ) {
      FileLoc loc = PyFileLoc_AsFileLoc(obj1);
      self->mFileRegion = FileRegion(loc);
    }
    else if ( PyFileRegion_Check(obj1) ) {
      FileRegion fr = PyFileRegion_AsFileRegion(obj1);
      self->mFileRegion = fr;
    }
    else {
      goto error;
    }
  }
  else if ( n == 2 ) {
    PyObject* obj1 = PyTuple_GET_ITEM(args, 0);
    PyObject* obj2 = PyTuple_GET_ITEM(args, 1);
    if ( PyFileLoc_Check(obj1) && PyFileLoc_Check(obj2) ) {
      FileLoc loc1 = PyFileLoc_AsFileLoc(obj1);
      FileLoc loc2 = PyFileLoc_AsFileLoc(obj2);
      self->mFileRegion = FileRegion(loc1, loc2);
    }
    else if ( PyFileRegion_Check(obj1) && PyFileRegion_Check(obj2) ) {
      FileRegion fr1 = PyFileRegion_AsFileRegion(obj1);
      FileRegion fr2 = PyFileRegion_AsFileRegion(obj2);
      self->mFileRegion = FileRegion(fr1, fr2);
    }
    else {
      goto error;
    }
  }
  else if ( n == 5 ) {
    PyObject* obj1;
    ymuint start_line;
    ymuint start_column;
    ymuint end_line;
    ymuint end_column;
    if ( !PyArg_ParseTuple(args, "O!IIII",
			   &PyFileInfo_Type, &obj1,
			   &start_line, &start_column,
			   &end_line, &end_column) ) {
      return -1;
    }
    FileInfo info = PyFileInfo_AsFileInfo(obj1);
    self->mFileRegion = FileRegion(info, start_line, start_column,
				   end_line, end_column);
  }
  else if ( n == 6 ) {
    PyObject* obj1;
    PyObject* obj2;
    ymuint start_line;
    ymuint start_column;
    ymuint end_line;
    ymuint end_column;
    if ( !PyArg_ParseTuple(args, "O!IIO!II",
			   &PyFileInfo_Type, &obj1,
			   &start_line, &start_column,
			   &PyFileInfo_Type, &obj2,
			   &end_line, &end_column) ) {
      return -1;
    }
    FileInfo info1 = PyFileInfo_AsFileInfo(obj1);
    FileInfo info2 = PyFileInfo_AsFileInfo(obj2);
    self->mFileRegion = FileRegion(info1, start_line, start_column,
				   info2, end_line, end_column);
  }
  else {
    goto error;
  }

  return 0;

 error:
  PyErr_SetString(PyExc_TypeError, "type error");

  return -1;
}

// str 関数
PyObject*
FileRegion_str(FileRegionObject* self)
{
  ostringstream buf;
  buf << self->mFileRegion;
  return PyObject_FromString(buf.str());
}

// is_valid 関数
PyObject*
FileRegion_is_valid(FileRegionObject* self,
		    PyObject* args)
{
  return PyObject_FromBool(self->mFileRegion.is_valid());
}

// start_loc 関数
PyObject*
FileRegion_start_loc(FileRegionObject* self,
		     PyObject* args)
{
  return PyFileLoc_FromFileLoc(self->mFileRegion.start_loc());
}

// start_file_info 関数
PyObject*
FileRegion_start_file_info(FileRegionObject* self,
			   PyObject* args)
{
  return PyFileInfo_FromFileInfo(self->mFileRegion.start_file_info());
}

// start_line 関数
PyObject*
FileRegion_start_line(FileRegionObject* self,
		      PyObject* args)
{
  return PyObject_FromYmuint32(self->mFileRegion.start_line());
}

// start_column 関数
PyObject*
FileRegion_start_column(FileRegionObject* self,
			PyObject* args)
{
  return PyObject_FromYmuint32(self->mFileRegion.start_column());
}

// end_loc 関数
PyObject*
FileRegion_end_loc(FileRegionObject* self,
		   PyObject* args)
{
  return PyFileLoc_FromFileLoc(self->mFileRegion.end_loc());
}

// end_file_info 関数
PyObject*
FileRegion_end_file_info(FileRegionObject* self,
			 PyObject* args)
{
  return PyFileInfo_FromFileInfo(self->mFileRegion.end_file_info());
}

// end_line 関数
PyObject*
FileRegion_end_line(FileRegionObject* self,
		    PyObject* args)
{
  return PyObject_FromYmuint32(self->mFileRegion.end_line());
}

// end_column 関数
PyObject*
FileRegion_end_column(FileRegionObject* self,
		      PyObject* args)
{
  return PyObject_FromYmuint32(self->mFileRegion.end_column());
}


//////////////////////////////////////////////////////////////////////
// FileRegionObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef FileRegion_methods[] = {
  {"is_valid", (PyCFunction)FileRegion_is_valid, METH_NOARGS,
   PyDoc_STR("return true if valid (NONE)")},
  {"start_loc", (PyCFunction)FileRegion_start_loc, METH_NOARGS,
   PyDoc_STR("return start location (NONE)")},
  {"start_file_info", (PyCFunction)FileRegion_start_file_info, METH_NOARGS,
   PyDoc_STR("return start file information (NONE)")},
  {"start_line", (PyCFunction)FileRegion_start_line, METH_NOARGS,
   PyDoc_STR("return start line number (NONE)")},
  {"start_column", (PyCFunction)FileRegion_start_column, METH_NOARGS,
   PyDoc_STR("return start column number (NONE)")},
  {"end_loc", (PyCFunction)FileRegion_end_loc, METH_NOARGS,
   PyDoc_STR("return start location (NONE)")},
  {"end_file_info", (PyCFunction)FileRegion_end_file_info, METH_NOARGS,
   PyDoc_STR("return start file information (NONE)")},
  {"end_line", (PyCFunction)FileRegion_end_line, METH_NOARGS,
   PyDoc_STR("return start line number (NONE)")},
  {"end_column", (PyCFunction)FileRegion_end_column, METH_NOARGS,
   PyDoc_STR("return start column number (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE



//////////////////////////////////////////////////////////////////////
// FileRegionObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyFileRegion_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileRegion",             // tp_name
  sizeof(FileRegionObject),       // tp_basicsize
  0,                              // tp_itemsize
  /* methods */
  (destructor)FileRegion_dealloc, // tp_dealloc
  0,                              // tp_print
  0,                              // tp_getattr
  0,                              // tp_setattr
  0,                              // tp_compare
  0,                              // tp_repr
  0,                              // tp_as_number
  0,                              // tp_as_sequence
  0,                              // tp_as_mapping
  0,                              // tp_hash
  0,                              // tp_call
  (reprfunc)FileRegion_str,       // tp_str
  0,                              // tp_getattro
  0,                              // tp_setattro
  0,                              // tp_as_buffer
  Py_TPFLAGS_DEFAULT,             // tp_flags
  "file region",                  // tp_doc
  0,                              // tp_traverse
  0,                              // tp_clear
  0,                              // tp_richcompare
  0,                              // tp_weaklistoffset
  0,                              // tp_iter
  0,                              // tp_iternext
  FileRegion_methods,             // tp_methods
  0,                              // tp_members
  0,                              // tp_getset
  0,                              // tp_base
  0,                              // tp_dict
  0,                              // tp_descr_get
  0,                              // tp_descr_set
  0,                              // tp_dictoffset
  (initproc)FileRegion_init,      // tp_init
  0,                              // tp_alloc
  (newfunc)FileRegion_new,        // tp_new
  0,                              // tp_free
  0,                              // tp_is_gc
};


//////////////////////////////////////////////////////////////////////
// PyObject と FileRegion 間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief FileRegion から FileRegionObject を生成する．
// @param[in] obj FileRegion オブジェクト
PyObject*
PyFileRegion_FromFileRegion(const FileRegion& obj)
{
  FileRegionObject* py_obj = FileRegion_new(&PyFileRegion_Type);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mFileRegion = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

// @brief PyObject から FileRegion を取り出す．
// @param[in] py_obj Python オブジェクト
// @return FileRegion を返す．
// @note 変換が失敗したら TypeError を送出し，不正な値を返す．
FileRegion
PyFileRegion_AsFileRegion(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyFileRegion_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "utils.FileRegion is expected");
    return FileRegion();
  }

  // 強制的にキャスト
  FileRegionObject* my_obj = (FileRegionObject*)py_obj;

  return my_obj->mFileRegion;
}

// FileRegionObject 関係の初期化を行う．
void
FileRegionObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyFileRegion_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "FileRegion", (PyObject*)&PyFileRegion_Type);
}

END_NAMESPACE_YM
