
/// @file pyutils_FileLoc.cc
/// @brief FileLoc の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/pyutils.h"
#include "ym_utils/FileLoc.h"


BEGIN_NAMESPACE_YM_PYTHON

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// FileLoc を表す型
struct FileLocObject
{
  // Python のお約束
  PyObject_HEAD

  // FileLoc の値
  FileLoc mFileLoc;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// FileLocObject の生成関数
FileLocObject*
FileLoc_new(PyTypeObject* type)
{
  FileLocObject* self = PyObject_New(FileLocObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  // FileLoc を初期化する．
  self->mFileLoc = FileLoc();

  return self;
}

// FileLocObject を開放する関数
void
FileLoc_dealloc(FileLocObject* self)
{
  // FileLoc の開放は必要ない．

  PyObject_Del(self);
}

// 初期化関数
int
FileLoc_init(FileLocObject* self,
	      PyObject* args)
{
  // 引数の形式は
  // - ()
  // - (FileInfo, uint, uint)
  PyObject* obj1 = NULL;
  ymuint line = 0;
  ymuint column = 0;
  if ( !PyArg_ParseTuple(args, "|O!kk", &FileInfoType, &obj1, &line, &column) ) {
    return -1;
  }

  if ( obj1 != NULL ) {
    FileInfo file_info;
    if ( !conv_from_pyobject(obj1, file_info) ) {
      return -1;
    }
    self->mFileLoc = FileLoc(file_info, line, column);
  }
  else {
    self->mFileLoc = FileLoc();
  }

  return 0;
}

// str 関数
PyObject*
FileLoc_str(FileLocObject* self)
{
  ostringstream buf;
  buf << self->mFileLoc;
  return conv_to_pyobject(buf.str());
}

// is_valid 関数
PyObject*
FileLoc_is_valid(FileLocObject* self,
		  PyObject* args)
{
  return conv_to_pyobject(self->mFileLoc.is_valid());
}

// filename 関数
PyObject*
FileLoc_filename(FileLocObject* self,
		  PyObject* args)
{
  return conv_to_pyobject(self->mFileLoc.filename());
}

// parent_loc 関数
PyObject*
FileLoc_parent_loc(FileLocObject* self,
		    PyObject* args)
{
  return conv_to_pyobject(self->mFileLoc.parent_loc());
}

// parent_loc_list 関数
PyObject*
FileLoc_parent_loc_list(FileLocObject* self,
			 PyObject* args)
{
  // FileLocObject のリストを返す．
  list<FileLoc> loc_list;
  self->mFileLoc.parent_loc_list(loc_list);
  ymuint n = loc_list.size();
  PyObject* ans_list = PyList_New(n);
  ymuint i = 0;
  for (list<FileLoc>::iterator p = loc_list.begin();
       p != loc_list.end(); ++ p, ++ i) {
    PyObject* obj = conv_to_pyobject(*p);
    PyList_SetItem(obj, i, obj);
  }
  return ans_list;
}

// line 関数
PyObject*
FileLoc_line(FileLocObject* self,
	     PyObject* args)
{
  return conv_to_pyobject(self->mFileLoc.line());
}

// column 関数
PyObject*
FileLoc_column(FileLocObject* self,
	       PyObject* args)
{
  return conv_to_pyobject(self->mFileLoc.column());
}

// FileLocObject のメソッドテーブル
PyMethodDef FileLoc_methods[] = {
  {"is_valid", (PyCFunction)FileLoc_is_valid, METH_NOARGS,
   PyDoc_STR("return true if valid (NONE)")},
  {"filename", (PyCFunction)FileLoc_filename, METH_NOARGS,
   PyDoc_STR("return filename (NONE)")},
  {"parent_loc", (PyCFunction)FileLoc_parent_loc, METH_NOARGS,
   PyDoc_STR("return parent FileLoc (NONE)")},
  {"parent_loc_list", (PyCFunction)FileLoc_parent_loc_list, METH_NOARGS,
   PyDoc_STR("return list of parent FileLoc's (NONE)")},
  {"line", (PyCFunction)FileLoc_line, METH_NOARGS,
   PyDoc_STR("return line number (NONE)")},
  {"column", (PyCFunction)FileLoc_column, METH_NOARGS,
   PyDoc_STR("return column number (NONE)")},
  {NULL, NULL, 0, NULL}
};

END_NONAMESPACE


// FileLocObject 用のタイプオブジェクト
PyTypeObject FileLocType = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "utils.FileLoc",            /*tp_name*/
  sizeof(FileLocObject),      /*tp_basicsize*/
  0,                          /*tp_itemsize*/
  /* methods */
  (destructor)FileLoc_dealloc,    /*tp_dealloc*/
  0,                          /*tp_print*/
  0,                          /*tp_getattr*/
  0,                          /*tp_setattr*/
  0,                          /*tp_compare*/
  0,                          /*tp_repr*/
  0,                          /*tp_as_number*/
  0,                          /*tp_as_sequence*/
  0,                          /*tp_as_mapping*/
  0,                          /*tp_hash*/
  0,                          /*tp_call*/
  (reprfunc)FileLoc_str,     /*tp_str*/
  0,                          /*tp_getattro*/
  0,                          /*tp_setattro*/
  0,                          /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,         /*tp_flags*/
  "file location",            /*tp_doc*/
  0,                          /*tp_traverse*/
  0,                          /*tp_clear*/
  0,                          /*tp_richcompare*/
  0,                          /*tp_weaklistoffset*/
  0,                          /*tp_iter*/
  0,                          /*tp_iternext*/
  FileLoc_methods,           /*tp_methods*/
  0,                          /*tp_members*/
  0,                          /*tp_getset*/
  0,                          /*tp_base*/
  0,                          /*tp_dict*/
  0,                          /*tp_descr_get*/
  0,                          /*tp_descr_set*/
  0,                          /*tp_dictoffset*/
  (initproc)FileLoc_init,    /*tp_init*/
  0,                          /*tp_alloc*/
  (newfunc)FileLoc_new,      /*tp_new*/
  0,                          /*tp_free*/
  0,                          /*tp_is_gc*/
};

// @brief PyObject から FileLoc を取り出す．
// @param[in] py_obj Python オブジェクト
// @param[out] obj FileLoc を格納する変数
// @retval true 変換が成功した．
// @retval false 変換が失敗した．py_obj が FileLocObject ではなかった．
bool
conv_from_pyobject(PyObject* py_obj,
		   FileLoc& obj)
{
  // 型のチェック
  if ( !FileLocObject_Check(py_obj) ) {
    return false;
  }

  // 強制的にキャスト
  FileLocObject* fileloc_obj = (FileLocObject*)py_obj;

  obj = fileloc_obj->mFileLoc;

  return true;
}

// @brief FileLoc から PyObject を生成する．
// @param[in] obj FileLoc オブジェクト
PyObject*
conv_to_pyobject(const FileLoc& obj)
{
  FileLocObject* py_obj = FileLoc_new(&FileLocType);
  if ( py_obj == NULL ) {
    return NULL;
  }

  py_obj->mFileLoc = obj;

  Py_INCREF(py_obj);
  return (PyObject*)py_obj;
}

END_NAMESPACE_YM_PYTHON
