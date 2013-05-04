
/// @file pycell_CellLibrary.cc
/// @brief CellLibrary の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellDotlibReader.h"
#include "ym_cell/CellMislibReader.h"
#include "ym_cell/CellBus.h"
#include "ym_cell/Cell.h"
#include "ym_utils/pyutils.h"
#include "ym_utils/FileBinO.h"
#include "ym_utils/FileBinI.h"
#include "PyLibrary.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellLibrary を表す型
struct CellLibraryObject
{
  // Python のお約束
  PyObject_HEAD

  // PyLibrary
  PyLibrary* mLibrary;

};


//////////////////////////////////////////////////////////////////////
// Python 用のメソッド関数定義
//////////////////////////////////////////////////////////////////////

// CellLibraryObject の生成関数
CellLibraryObject*
CellLibrary_new(PyTypeObject* type)
{
  CellLibraryObject* self = PyObject_New(CellLibraryObject, type);
  if ( self == NULL ) {
    return NULL;
  }

  self->mLibrary = NULL;

  return self;
}

// CellLibraryObject を開放する関数
void
CellLibrary_dealloc(CellLibraryObject* self)
{
  delete self->mLibrary;

  PyObject_Del(self);
}

// 初期化関数
int
CellLibrary_init(CellLibraryObject* self,
		 PyObject* args)
{
  // args をパーズして初期化を行なう．
  // エラーが起きたらエラーメッセージをセットして -1 を返す．
  // 正常に終了したら 0 を返す．
  // 引数は
  // - (ファイル名 [, タイプ])
  // タイプは
  // - liberty
  // - mislib
  // - binary
  char* filename = NULL;
  const char* type = NULL;
  if ( !PyArg_ParseTuple(args, "s|s", &filename, &type) ) {
    return -1;
  }

  if ( type == NULL ) {
    type = "liberty";
  }
  if ( strcmp(type, "liberty") == 0 ) {
    CellDotlibReader read;

    const CellLibrary* library = read(filename);
    if ( library == NULL ) {
      PyErr_SetString(PyExc_ValueError, "Read error");
      return -1;
    }
    self->mLibrary = new PyLibrary(library);
  }
  else if ( strcmp(type, "mislib") == 0 ) {
    CellMislibReader read;

    const CellLibrary* library = read(filename);
    if ( library == NULL ) {
      PyErr_SetString(PyExc_ValueError, "Read error");
      return -1;
    }
    self->mLibrary = new PyLibrary(library);
  }
  else if ( strcmp(type, "binary") == 0 ) {
    FileBinI s(filename);
    CellLibrary* library = CellLibrary::new_obj();
    library->restore(s);
    self->mLibrary = new PyLibrary(library);
  }
  else {
    PyErr_SetString(PyExc_ValueError, "Illegal type string.");
    return -1;
  }

  return 0;
}

// name 関数
PyObject*
CellLibrary_name(CellLibraryObject* self,
		 PyObject* args)
{
  PyObject* result = self->mLibrary->name();

  Py_INCREF(result);
  return result;
}

// technology 関数
PyObject*
CellLibrary_technology(CellLibraryObject* self,
		       PyObject* args)
{
  PyObject* result = self->mLibrary->technology();

  Py_INCREF(result);
  return result;
}

// delay_model 関数
PyObject*
CellLibrary_delay_model(CellLibraryObject* self,
			PyObject* args)
{
  PyObject* result = self->mLibrary->delay_model();

  Py_INCREF(result);
  return result;
}

// bus_naming_style 関数
PyObject*
CellLibrary_bus_naming_style(CellLibraryObject* self,
			     PyObject* args)
{
  PyObject* result = self->mLibrary->bus_naming_style();

  Py_INCREF(result);
  return result;
}

// date 関数
PyObject*
CellLibrary_date(CellLibraryObject* self,
		 PyObject* args)
{
  PyObject* result = self->mLibrary->date();

  Py_INCREF(result);
  return result;
}

// revision 関数
PyObject*
CellLibrary_revision(CellLibraryObject* self,
		     PyObject* args)
{
  PyObject* result = self->mLibrary->revision();

  Py_INCREF(result);
  return result;
}

// comment 関数
PyObject*
CellLibrary_comment(CellLibraryObject* self,
		    PyObject* args)
{
  PyObject* result = self->mLibrary->comment();

  Py_INCREF(result);
  return result;
}

// time_unit 関数
PyObject*
CellLibrary_time_unit(CellLibraryObject* self,
		      PyObject* args)
{
  PyObject* result = self->mLibrary->time_unit();

  Py_INCREF(result);
  return result;
}

// voltage_unit 関数
PyObject*
CellLibrary_voltage_unit(CellLibraryObject* self,
			 PyObject* args)
{
  PyObject* result = self->mLibrary->voltage_unit();

  Py_INCREF(result);
  return result;
}

// current_unit 関数
PyObject*
CellLibrary_current_unit(CellLibraryObject* self,
			 PyObject* args)
{
  PyObject* result = self->mLibrary->current_unit();

  Py_INCREF(result);
  return result;
}

// pulling_resistance_unit 関数
PyObject*
CellLibrary_pulling_resistance_unit(CellLibraryObject* self,
				    PyObject* args)
{
  PyObject* result = self->mLibrary->pulling_resistance_unit();

  Py_INCREF(result);
  return result;
}

// capacitive_load_unit 関数
PyObject*
CellLibrary_capacitive_load_unit(CellLibraryObject* self,
				 PyObject* args)
{
  PyObject* result = self->mLibrary->capacitive_load_unit();

  Py_INCREF(result);
  return result;
}

// leakage_power_unit 関数
PyObject*
CellLibrary_leakage_power_unit(CellLibraryObject* self,
			       PyObject* args)
{
  PyObject* result = self->mLibrary->leakage_power_unit();

  Py_INCREF(result);
  return result;
}

// lu_table_template 関数
PyObject*
CellLibrary_lu_table_template(CellLibraryObject* self,
			      PyObject* args)
{
#if 0
  // 引数の形式は
  // - (str) テンプレート名
  char* name = NULL;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }

  const CellLutTemplate* lut_tmpl = self->mBody->lu_table_template(name);
  if ( lut_tmpl == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such lu_table_template");
    return NULL;
  }

  return self->mLibrary->get_CellLutTemplate(lut_tmpl);
#else
  return NULL;
#endif
}

// lut_table_template_list 関数
PyObject*
CellLibrary_lu_table_template_list(CellLibraryObject* self,
				   PyObject* args)
{
#if 0
  ymuint n = self->mBody->lu_table_template_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellLutTemplate* lut_tmpl = self->mBody->lu_table_template(i);
    PyObject* obj1 = self->mLibrary->get_CellLutTemplate(lut_tmpl);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
#else
  return NULL;
#endif
}

// bus_type 関数
PyObject*
CellLibrary_bus_type(CellLibraryObject* self,
		     PyObject* args)
{
#if 0
  char* name;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }

  const CellBusType* bus_type = self->mBody->bus_type(name);
  if ( bus_type != NULL ) {
#if 0
    return conv_to_pyobject(bus_type);
#else
    return NULL;
#endif
  }
  else {
    Py_INCREF(Py_None);
    return Py_None;
  }
#else
  return NULL;
#endif
}

// cell_num 関数
PyObject*
CellLibrary_cell_num(CellLibraryObject* self,
		     PyObject* args)
{
  return Py_BuildValue("I", self->mLibrary->library()->cell_num());
}

// cell 関数
PyObject*
CellLibrary_cell(CellLibraryObject* self,
		 PyObject* args)
{
  // 引数の形式は
  // - (int) セル番号
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "I", &pos) ) {
    return NULL;
  }

  PyObject* cell_obj = self->mLibrary->cell(pos);

  Py_INCREF(cell_obj);
  return cell_obj;
}

// cell_by_name 関数
PyObject*
CellLibrary_cell_by_name(CellLibraryObject* self,
			 PyObject* args)
{
  // 引数の形式は
  // - (str) セル名
  char* name = NULL;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }

  const Cell* cell = self->mLibrary->library()->cell(name);
  if ( cell == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such cell");
    return NULL;
  }

  PyObject* cell_obj = self->mLibrary->cell(cell->id());

  Py_INCREF(cell_obj);
  return cell_obj;
}

// cell_list 関数
PyObject*
CellLibrary_cell_list(CellLibraryObject* self,
		      PyObject* args)
{
  ymuint n = self->mLibrary->library()->cell_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = self->mLibrary->cell(i);
    Py_INCREF(obj1);
    PyList_SetItem(list_obj, i, obj1);
  }

  Py_INCREF(list_obj);
  return list_obj;
}

// group_list 関数
PyObject*
CellLibrary_group_list(CellLibraryObject* self,
		       PyObject* args)
{
#if 0
  ymuint n = self->mLibrary->library()->group_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellGroup* group = self->mLibrary->group(i);
    PyObject* obj1 = self->mLibrary->get_CellGroup(group);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
#else
  return NULL;
#endif
}

// npn_class_list 関数
PyObject*
CellLibrary_npn_class_list(CellLibraryObject* self,
			   PyObject* args)
{
#if 0
  ymuint n = self->mBody->npn_class_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellClass* npn_class = self->mBody->npn_class(i);
    PyObject* obj1 = self->mLibrary->get_CellClass(npn_class);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
#else
  return NULL;
#endif
}

// パタンを返す．
PyObject*
CellLibrary_pg_pat(CellLibraryObject* self,
		   PyObject*  args)
{
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "I", &pos) ) {
    return NULL;
  }

  PyObject* result = self->mLibrary->pg_pat(pos);

  Py_INCREF(result);
  return result;
}

// 枝の情報を返す．
PyObject*
CellLibrary_pg_edge(CellLibraryObject* self,
		    PyObject* args)
{
  ymuint pos = 0;
  if ( !PyArg_ParseTuple(args, "I", &pos) ) {
    return NULL;
  }

  PyObject* result = self->mLibrary->pg_edge(pos);

  Py_INCREF(result);
  return result;
}

// const0_func 関数
PyObject*
CellLibrary_const0_func(CellLibraryObject* self,
			PyObject* args)
{
#if 0
  const CellGroup* group = self->mBody->const0_func();
  return self->mLibrary->get_CellGroup(group);
#else
  return NULL;
#endif
}

// dump 関数
PyObject*
CellLibrary_dump(CellLibraryObject* self,
		 PyObject* args)
{
  FileBinO* bp = parse_FileBinO(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mLibrary->library()->dump(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}


//////////////////////////////////////////////////////////////////////
// CellLibraryObject のメソッドテーブル
//////////////////////////////////////////////////////////////////////
PyMethodDef CellLibrary_methods[] = {
  // PyMethodDef のフィールド
  //   char*       ml_name;
  //   PyCFunction ml_meth;
  //   int         ml_flags;
  //   char*       ml_doc;
  // ml_flags で使用可能なマクロは以下の通り
  //  - METH_VARARGS
  //  - METH_KEYWORDS
  //  - METH_NOARGS
  //  - METH_O
  //  - METH_OLDARGS (obsolete)
  //  - METH_CLASS
  //  - METH_STATIC
  //  - METH_COEXIST
  {"name", (PyCFunction)CellLibrary_name, METH_NOARGS,
   PyDoc_STR("return name of the library (NONE)")},
  {"technology", (PyCFunction)CellLibrary_technology, METH_NOARGS,
   PyDoc_STR("return technology of the library (NONE)")},
  {"delay_model", (PyCFunction)CellLibrary_delay_model, METH_NOARGS,
   PyDoc_STR("return delay model of the library (NONE)")},
  {"bus_naming_style", (PyCFunction)CellLibrary_bus_naming_style, METH_NOARGS,
   PyDoc_STR("return bus naming style of the library (NONE)")},
  {"date", (PyCFunction)CellLibrary_date, METH_NOARGS,
   PyDoc_STR("return date of the library (NONE)")},
  {"revision", (PyCFunction)CellLibrary_revision, METH_NOARGS,
   PyDoc_STR("return revision of the library (NONE)")},
  {"comment", (PyCFunction)CellLibrary_comment, METH_NOARGS,
   PyDoc_STR("return comment of the library (NONE)")},
  {"time_unit", (PyCFunction)CellLibrary_time_unit, METH_NOARGS,
   PyDoc_STR("return time unit of the library (NONE)")},
  {"voltage_unit", (PyCFunction)CellLibrary_voltage_unit, METH_NOARGS,
   PyDoc_STR("return voltage unit of the library (NONE)")},
  {"current_unit", (PyCFunction)CellLibrary_current_unit, METH_NOARGS,
   PyDoc_STR("return current unit of the library (NONE)")},
  {"pulling_resistance_unit", (PyCFunction)CellLibrary_pulling_resistance_unit,
   METH_NOARGS,
   PyDoc_STR("return resistance unit of the library (NONE)")},
  {"capacitive_load_unit", (PyCFunction)CellLibrary_capacitive_load_unit,
   METH_NOARGS,
   PyDoc_STR("return load unit of the library (NONE)")},
  {"leakage_power_unit", (PyCFunction)CellLibrary_leakage_power_unit, METH_NOARGS,
   PyDoc_STR("return power unit of the library (NONE)")},
  {"lu_table_template", (PyCFunction)CellLibrary_lu_table_template, METH_VARARGS,
   PyDoc_STR("return lu_table_template (str)")},
  {"lu_table_template_lsit", (PyCFunction)CellLibrary_lu_table_template_list,
   METH_NOARGS,
   PyDoc_STR("return list of lu_table_template (NONE)")},
  {"bus_type", (PyCFunction)CellLibrary_bus_type, METH_VARARGS,
   PyDoc_STR("return bus type of the library (NONE)")},
  {"cell_num", (PyCFunction)CellLibrary_cell_num, METH_NOARGS,
   PyDoc_STR("return cell number")},
  {"cell", (PyCFunction)CellLibrary_cell, METH_VARARGS,
   PyDoc_STR("return cell (int)")},
  {"cell_by_name", (PyCFunction)CellLibrary_cell_by_name, METH_VARARGS,
   PyDoc_STR("return cell (str)")},
  {"cell_list", (PyCFunction)CellLibrary_cell_list, METH_NOARGS,
   PyDoc_STR("return list of cell (NONE)")},
  {"group_list", (PyCFunction)CellLibrary_group_list, METH_NOARGS,
   PyDoc_STR("return list of cell group (NONE)")},
  {"npn_class_list", (PyCFunction)CellLibrary_npn_class_list, METH_NOARGS,
   PyDoc_STR("return list of cell class (NONE)")},
  {"pg_pat", (PyCFunction)CellLibrary_pg_pat, METH_VARARGS,
   PyDoc_STR("return pattern graph (int)")},
  {"pg_edge", (PyCFunction)CellLibrary_pg_edge, METH_VARARGS,
   PyDoc_STR("return edge infomation (int)")},
  {"dump", (PyCFunction)CellLibrary_dump, METH_VARARGS,
   PyDoc_STR("dump (FileBinO)")},
  {NULL, NULL, 0, NULL} // end-marker
};

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// CellLibraryObject 用のタイプオブジェクト
//////////////////////////////////////////////////////////////////////
PyTypeObject PyCellLibrary_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  "cell_lib.Library",                     // tp_name
  sizeof(CellLibraryObject),          // tp_basicsize
  (int)0,                             // tp_itemsize

  // Methods to implement standard operations

  (destructor)CellLibrary_dealloc,    // tp_dealloc
  (printfunc)0,                       // tp_print
  (getattrfunc)0,                     // tp_getattr
  (setattrfunc)0,                     // tp_setattr
  (cmpfunc)0,                         // tp_compare
  (reprfunc)0,                        // tp_repr

  // Method suites for standard classes
  0,                                  // tp_as_number
  0,                                  // tp_as_sequence
  0,                                  // tp_as_mapping

  // More standard operations (here for binary compatibility)
  (hashfunc)0,                        // tp_hash
  (ternaryfunc)0,                     // tp_call
  (reprfunc)0,                        // tp_str
  (getattrofunc)0,                    // tp_getattro
  (setattrofunc)0,                    // tp_setattro

  // Functions to access object as input/output buffer
  0,                                  // tp_as_buffer

  // Flags to define presence of optional/expanded features
  Py_TPFLAGS_DEFAULT,                 // tp_flags

  // Documentation string
  "Cell Library",                     // tp_doc

  // Assigned meaning in release 2.0

  // call function for all accesible objects
  (traverseproc)0,                    // tp_traverse

  // delete references to contained objects
  (inquiry)0,                         // tp_clear

  // Assigned meaning in release 2.1

  // rich comparisons
  (richcmpfunc)0,                     // tp_richcompare

  // weak reference enabler
  (long)0,                            // tp_weaklistoffset

  // Added in release 2.2

  // Iterators
  (getiterfunc)0,                     // tp_iter
  (iternextfunc)0,                    // tp_iternext

  // Attribute descriptor and subclassing stuff
  CellLibrary_methods,                // tp_methods
  0,                                  // tp_members
  0,                                  // tp_getset
  (struct _typeobject*)0,             // tp_base
  (PyObject*)0,                       // tp_dict
  (descrgetfunc)0,                    // tp_descr_get
  (descrsetfunc)0,                    // tp_descr_set
  (long)0,                            // tp_dictoffset
  (initproc)CellLibrary_init,         // tp_init
  (allocfunc)0,                       // tp_alloc
  (newfunc)CellLibrary_new,           // tp_new
  (freefunc)0,                        // tp_free
  (inquiry)0,                         // tp_is_gc

  (PyObject*)0,                       // tp_bases
  (PyObject*)0,                       // tp_mro (method resolution order)
  (PyObject*)0,                       // tp_cache
  (PyObject*)0,                       // tp_subclasses
  (PyObject*)0                        // tp_weaklist
};


//////////////////////////////////////////////////////////////////////
// PyObject と CellLibrary の間の変換関数
//////////////////////////////////////////////////////////////////////

// @brief PyObject から CellLibrary へのポインタを取り出す．
// @param[in] py_obj Python オブジェクト
// @return CellLibrary へのポインタを返す．
// @note 変換が失敗したら TypeError を送出し，NULL を返す．
const CellLibrary*
PyCellLibrary_AsCellLibraryPtr(PyObject* py_obj)
{
  // 型のチェック
  if ( !PyCellLibrary_Check(py_obj) ) {
    PyErr_SetString(PyExc_TypeError, "cell_lib.Library is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellLibraryObject* my_obj = (CellLibraryObject*)py_obj;

  return my_obj->mLibrary->library();
}


BEGIN_NONAMESPACE

inline
PyObject*
new_string(const char* str)
{
  PyObject* py_obj = PyString_InternFromString(str);
  Py_INCREF(py_obj);
  return py_obj;
}

END_NONAMESPACE


// CellLibraryObject 関係の初期化を行う．
void
CellLibraryObject_init(PyObject* m)
{
  // タイプオブジェクトの初期化
  if ( PyType_Ready(&PyCellLibrary_Type) < 0 ) {
    return;
  }

  // タイプオブジェクトの登録
  PyModule_AddObject(m, "Library", (PyObject*)&PyCellLibrary_Type);
}

END_NAMESPACE_YM
