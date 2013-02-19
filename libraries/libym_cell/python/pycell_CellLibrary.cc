
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
#include "ym_utils/pyutils.h"
#include "ym_utils/FileBinO.h"
#include "ym_utils/FileBinI.h"
#include "PyLibrary.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// 文字列オブジェクト
//////////////////////////////////////////////////////////////////////

PyObject* Py_kTechCmos;
PyObject* Py_kTechFpga;
PyObject* Py_kDelayGenericCmos;
PyObject* Py_kDelayTableLookup;
PyObject* Py_kDelayPiecewiseCmos;
PyObject* Py_kDelayCmos2;
PyObject* Py_kDelayDcm;


//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellLibrary を表す型
struct CellLibraryObject
{
  // Python のお約束
  PyObject_HEAD

  // CellLibrary の本体
  const CellLibrary* mBody;

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

  // CellLibrary の生成を行なう．
  //self->mBody = CellLibrary::new_obj();
  self->mLibrary = new PyLibrary();

  return self;
}

// CellLibraryObject を開放する関数
void
CellLibrary_dealloc(CellLibraryObject* self)
{
  delete self->mLibrary;

  // CellLibrary の開放を行なう．
  delete self->mBody;

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

  return 0;
}

// name 関数
PyObject*
CellLibrary_name(CellLibraryObject* self,
		 PyObject* args)
{
  return PyObject_FromString(self->mBody->name());
}

// technology 関数
PyObject*
CellLibrary_technology(CellLibraryObject* self,
		       PyObject* args)
{
  PyObject* result = NULL;
  switch ( self->mBody->technology() ) {
  case CellLibrary::kTechCmos: result = Py_kTechCmos; break;
  case CellLibrary::kTechFpga: result = Py_kTechFpga; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  Py_INCREF(result);
  return result;
}

// delay_model 関数
PyObject*
CellLibrary_delay_model(CellLibraryObject* self,
			PyObject* args)
{
  PyObject* result = NULL;
  switch ( self->mBody->delay_model() ) {
  case kCellDelayGenericCmos:   result = Py_kDelayGenericCmos; break;
  case kCellDelayTableLookup:   result = Py_kDelayTableLookup; break;
  case kCellDelayPiecewiseCmos: result = Py_kDelayPiecewiseCmos; break;
  case kCellDelayCmos2:         result = Py_kDelayCmos2; break;
  case kCellDelayDcm:           result = Py_kDelayDcm; break;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  Py_INCREF(result);
  return result;
}

// bus_naming_style 関数
PyObject*
CellLibrary_bus_naming_style(CellLibraryObject* self,
			     PyObject* args)
{
  return PyObject_FromString(self->mBody->bus_naming_style());
}

// date 関数
PyObject*
CellLibrary_date(CellLibraryObject* self,
		 PyObject* args)
{
  return PyObject_FromString(self->mBody->date());
}

// revision 関数
PyObject*
CellLibrary_revision(CellLibraryObject* self,
		     PyObject* args)
{
  return PyObject_FromString(self->mBody->revision());
}

// comment 関数
PyObject*
CellLibrary_comment(CellLibraryObject* self,
		    PyObject* args)
{
  return PyObject_FromString(self->mBody->comment());
}

// time_unit 関数
PyObject*
CellLibrary_time_unit(CellLibraryObject* self,
		      PyObject* args)
{
  return PyObject_FromString(self->mBody->time_unit());
}

// voltage_unit 関数
PyObject*
CellLibrary_voltage_unit(CellLibraryObject* self,
			 PyObject* args)
{
  return PyObject_FromString(self->mBody->voltage_unit());
}

// current_unit 関数
PyObject*
CellLibrary_current_unit(CellLibraryObject* self,
			 PyObject* args)
{
  return PyObject_FromString(self->mBody->current_unit());
}

// pulling_resistance_unit 関数
PyObject*
CellLibrary_pulling_resistance_unit(CellLibraryObject* self,
				    PyObject* args)
{
  return PyObject_FromString(self->mBody->pulling_resistance_unit());
}

// capacitive_load_unit 関数
PyObject*
CellLibrary_capacitive_load_unit(CellLibraryObject* self,
				 PyObject* args)
{
  double unit_val = self->mBody->capacitive_load_unit();
  string unit_str = self->mBody->capacitive_load_unit_str();
  return Py_BuildValue("(ds)", unit_val, unit_str.c_str());
}

// leakage_power_unit 関数
PyObject*
CellLibrary_leakage_power_unit(CellLibraryObject* self,
			       PyObject* args)
{
  return PyObject_FromString(self->mBody->leakage_power_unit());
}

// lu_table_template 関数
PyObject*
CellLibrary_lu_table_template(CellLibraryObject* self,
			      PyObject* args)
{
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
}

// lut_table_template_list 関数
PyObject*
CellLibrary_lu_table_template_list(CellLibraryObject* self,
				   PyObject* args)
{
  ymuint n = self->mBody->lu_table_template_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellLutTemplate* lut_tmpl = self->mBody->lu_table_template(i);
    PyObject* obj1 = self->mLibrary->get_CellLutTemplate(lut_tmpl);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
}

// bus_type 関数
PyObject*
CellLibrary_bus_type(CellLibraryObject* self,
		     PyObject* args)
{
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
}

// cell 関数
PyObject*
CellLibrary_cell(CellLibraryObject* self,
		 PyObject* args)
{
  // 引数の形式は
  // - (str) セル名
  char* name = NULL;
  if ( !PyArg_ParseTuple(args, "s", &name) ) {
    return NULL;
  }

  const Cell* cell = self->mBody->cell(name);
  if ( cell == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such cell");
    return NULL;
  }

  return self->mLibrary->get_Cell(cell);
}

// cell_list 関数
PyObject*
CellLibrary_cell_list(CellLibraryObject* self,
		      PyObject* args)
{
  ymuint n = self->mBody->cell_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const Cell* cell = self->mBody->cell(i);
    PyObject* obj1 = self->mLibrary->get_Cell(cell);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
}

// group_list 関数
PyObject*
CellLibrary_group_list(CellLibraryObject* self,
		       PyObject* args)
{
  ymuint n = self->mBody->group_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellGroup* group = self->mBody->group(i);
    PyObject* obj1 = self->mLibrary->get_CellGroup(group);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
}

// npn_class_list 関数
PyObject*
CellLibrary_npn_class_list(CellLibraryObject* self,
			   PyObject* args)
{
  ymuint n = self->mBody->npn_class_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    const CellClass* npn_class = self->mBody->npn_class(i);
    PyObject* obj1 = self->mLibrary->get_CellClass(npn_class);
    PyList_SetItem(list_obj, i, obj1);
  }
  return list_obj;
}

// const0_func 関数
PyObject*
CellLibrary_const0_func(CellLibraryObject* self,
			PyObject* args)
{
  const CellGroup* group = self->mBody->const0_func();
  return self->mLibrary->get_CellGroup(group);
}

// read_dotlib 関数
PyObject*
CellLibrary_read_dotlib(PyTypeObject* type_obj,
			PyObject* args)
{
  char* filename;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return NULL;
  }

  CellLibraryObject* self = PyObject_New(CellLibraryObject, type_obj);
  if ( self == NULL ) {
    return NULL;
  }

  CellDotlibReader read;
  self->mBody = read(filename);

  return (PyObject*)self;
}

// read_mislib 関数
PyObject*
CellLibrary_read_mislib(PyTypeObject* type_obj,
			PyObject* args)
{
  char* filename;
  if ( !PyArg_ParseTuple(args, "s", &filename) ) {
    return NULL;
  }

  CellLibraryObject* self = PyObject_New(CellLibraryObject, type_obj);
  if ( self == NULL ) {
    return NULL;
  }

  CellMislibReader read;
  self->mBody = read(filename);

  return (PyObject*)self;
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

  self->mBody->dump(*bp);

  Py_INCREF(Py_None);
  return Py_None;
}

// restore 関数
PyObject*
CellLibrary_restore(PyTypeObject* type_obj,
		    PyObject* args)
{
  FileBinI* bp = parse_FileBinI(args);
  if ( bp == NULL ) {
    return NULL;
  }

  CellLibraryObject* self = PyObject_New(CellLibraryObject, type_obj);
  if ( self == NULL ) {
    return NULL;
  }

  CellLibrary* library = CellLibrary::new_obj();
  library->restore(*bp);

  self->mBody = library;

  return (PyObject*)self;
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
  {"cell", (PyCFunction)CellLibrary_cell, METH_VARARGS,
   PyDoc_STR("return cell (str)")},
  {"cell_list", (PyCFunction)CellLibrary_cell_list, METH_NOARGS,
   PyDoc_STR("return list of cell (NONE)")},
  {"group_list", (PyCFunction)CellLibrary_group_list, METH_NOARGS,
   PyDoc_STR("return list of cell group (NONE)")},
  {"npn_class_list", (PyCFunction)CellLibrary_npn_class_list, METH_NOARGS,
   PyDoc_STR("return list of cell class (NONE)")},
  {"read_dotlib", (PyCFunction)CellLibrary_read_dotlib, METH_STATIC | METH_VARARGS,
   PyDoc_STR("read '.lib'(liberty) file (str)")},
  {"read_mislib", (PyCFunction)CellLibrary_read_mislib, METH_STATIC | METH_VARARGS,
   PyDoc_STR("read mislib(genlib) file (str)")},
  {"dump", (PyCFunction)CellLibrary_dump, METH_VARARGS,
   PyDoc_STR("dump (FileBinO)")},
  {"restore", (PyCFunction)CellLibrary_restore, METH_STATIC | METH_VARARGS,
   PyDoc_STR("restore (FileBinI)")},
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
  "cell.CellLibrary",                 // tp_name
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
    PyErr_SetString(PyExc_TypeError, "cell.CellLibrary is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellLibraryObject* my_obj = (CellLibraryObject*)py_obj;

  return my_obj->mBody;
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
  PyModule_AddObject(m, "CellLibrary", (PyObject*)&PyCellLibrary_Type);

  Py_kTechCmos           = new_string("cmos");
  Py_kTechFpga           = new_string("fpga");
  Py_kDelayGenericCmos   = new_string("generic cmos");
  Py_kDelayTableLookup   = new_string("table lookup");
  Py_kDelayPiecewiseCmos = new_string("piecewise cmos");
  Py_kDelayCmos2         = new_string("cmos2");
  Py_kDelayDcm           = new_string("dcm");
}

END_NAMESPACE_YM
