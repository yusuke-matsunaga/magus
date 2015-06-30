
/// @file pycell_CellLibrary.cc
/// @brief CellLibrary の Python 用ラッパ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/pycell.h"
#include "YmCell/CellLibrary.h"
#include "YmCell/CellDotlibReader.h"
#include "YmCell/CellMislibReader.h"
#include "YmCell/CellBus.h"
#include "YmCell/CellClass.h"
#include "YmCell/CellGroup.h"
#include "YmCell/Cell.h"
#include "YmUtils/pyutils.h"
#include "YmUtils/FileODO.h"
#include "YmUtils/FileIDO.h"


BEGIN_NAMESPACE_YM

/// @brief CellClass から CellClassObject を生成する．
/// @param[in] cell_class セルクラス
/// @param[in] py_library ライブラリ
extern
PyObject*
PyCellClass_FromCellClass(const CellClass* cell_class,
			  PyObject* group_list[]);

/// @brief CellGroup から CellGroupObject を生成する．
/// @param[in] group グループ
/// @param[in] py_library ライブラリ
extern
PyObject*
PyCellGroup_FromCellGroup(const CellGroup* group,
			  PyObject* cell_list[]);

/// @brief CellGroup に代表クラスを設定する．
extern
void
PyCellGroup_set_rep(PyObject* py_obj,
		    PyObject* rep_obj);

/// @brief Cell から PyObject を作る．
/// @param[in] cell Cell へのポインタ
/// @return cell を表す PyObject
extern
PyObject*
PyCellCell_FromCell(const Cell* cell);

/// @brief Cell にセルグループを設定する．
extern
void
PyCellCell_set_group(PyObject* py_obj,
		     PyObject* group_obj);

/// @brief CellPatGraph から PyObject を作る．
/// @param[in] pat_graph CellPatGraph へのポインタ
/// @return pat_graph を表す PyObject
extern
PyObject*
PyCellPatGraph_FromCellPatGraph(const CellPatGraph* pat_graph);


BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// Python 用の構造体定義
//////////////////////////////////////////////////////////////////////

// CellLibrary を表す型
struct CellLibraryObject
{
  // Python のお約束
  PyObject_HEAD

  // CellLibrary
  const CellLibrary* mLibrary;

  // Cell のリスト
  PyObject** mCellList;

  // セルグループの配列
  PyObject** mGroupList;

  // セルクラスの配列
  PyObject** mClassList;

  // パタンの配列
  PyObject** mPatList;

  // ノードの配列
  PyObject** mNodeList;

  // 枝の配列
  PyObject** mEdgeList;

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
  const CellLibrary* library = self->mLibrary;

  if ( library != NULL ) {
    ymuint nc = library->cell_num();
    for (ymuint i = 0; i < nc; ++ i) {
      Py_DECREF(self->mCellList[i]);
    }
    delete [] self->mCellList;

    ymuint ng = library->group_num();
    for (ymuint i = 0; i < ng; ++ i) {
      Py_DECREF(self->mGroupList[i]);
    }
    delete [] self->mGroupList;

    ymuint nn = library->npn_class_num();
    for (ymuint i = 0; i < nn; ++ i) {
      Py_DECREF(self->mClassList[i]);
    }
    delete [] self->mClassList;

    ymuint np = library->pg_pat_num();
    for (ymuint i = 0; i < np; ++ i) {
      Py_DECREF(self->mPatList[i]);
    }
    delete [] self->mPatList;

    ymuint npn = library->pg_node_num();
    for (ymuint i = 0; i < npn; ++ i) {
      Py_DECREF(self->mNodeList[i]);
    }
    delete [] self->mNodeList;

    ymuint ne = library->pg_edge_num();
    for (ymuint i = 0; i < ne; ++ i) {
      Py_DECREF(self->mEdgeList[i]);
    }
    delete [] self->mEdgeList;
  }

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
    self->mLibrary = library;
  }
  else if ( strcmp(type, "mislib") == 0 ) {
    CellMislibReader read;

    const CellLibrary* library = read(filename);
    if ( library == NULL ) {
      PyErr_SetString(PyExc_ValueError, "Read error");
      return -1;
    }
    self->mLibrary = library;
  }
  else if ( strcmp(type, "binary") == 0 ) {
    FileIDO s;
    if ( !s.open(filename) ) {
      PyErr_SetString(PyExc_ValueError, "Read error");
      return -1;
    }
    CellLibrary* library = CellLibrary::new_obj();
    library->restore(s);
    self->mLibrary = library;
  }
  else {
    PyErr_SetString(PyExc_ValueError, "Illegal type string.");
    return -1;
  }

  const CellLibrary* library = self->mLibrary;
  ASSERT_COND( library != NULL );

  ymuint nc = library->cell_num();
  self->mCellList = new PyObject*[nc];
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    PyObject* cell_obj = PyCellCell_FromCell(cell);
    self->mCellList[i] = cell_obj;
  }

  ymuint ng = library->group_num();
  self->mGroupList = new PyObject*[ng];
  for (ymuint i = 0; i < ng; ++ i) {
    const CellGroup* group = library->group(i);
    PyObject* group_obj = PyCellGroup_FromCellGroup(group, self->mCellList);
    self->mGroupList[i] = group_obj;
  }

  ymuint nn = library->npn_class_num();
  self->mClassList = new PyObject*[nn];
  for (ymuint i = 0; i < nn; ++ i) {
    const CellClass* cell_class = library->npn_class(i);
    PyObject* class_obj = PyCellClass_FromCellClass(cell_class, self->mGroupList);
    self->mClassList[i] = class_obj;
  }

  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    const CellGroup* group = cell->cell_group();
    PyObject* py_obj = self->mCellList[i];
    PyObject* group_obj = self->mGroupList[group->id()];
    PyCellCell_set_group(py_obj, group_obj);
  }

  for (ymuint i = 0; i < ng; ++ i) {
    PyObject* py_obj = self->mGroupList[i];
    const CellGroup* group = library->group(i);
    const CellClass* rep = group->rep_class();
    PyObject* rep_obj = self->mClassList[rep->id()];
    PyCellGroup_set_rep(py_obj, rep_obj);
  }

  ymuint np = library->pg_pat_num();
  self->mPatList = new PyObject*[np];
  for (ymuint i = 0; i < np; ++ i) {
    const CellPatGraph& pat = library->pg_pat(i);
    PyObject* pat_obj = PyCellPatGraph_FromCellPatGraph(&pat);
    self->mPatList[i] = pat_obj;
  }

  ymuint npn = library->pg_node_num();
  self->mNodeList = new PyObject*[npn];
  for (ymuint i = 0; i < npn; ++ i) {
    tCellPatType pat_type = library->pg_node_type(i);
    PyObject* pat = PyCellPatType_FromCellPatType(pat_type);
    ymuint id = 0;
    if ( pat_type == kCellPatInput ) {
      id = library->pg_input_id(i);
    }
    self->mNodeList[i] = Py_BuildValue("(OI)", pat, id);
  }

  ymuint ne = library->pg_edge_num();
  self->mEdgeList = new PyObject*[ne];
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint from_id = library->pg_edge_from(i);
    ymuint to_id = library->pg_edge_to(i);
    ymuint pos = library->pg_edge_pos(i);
    bool inv = library->pg_edge_inv(i);
    PyObject* obj = Py_BuildValue("(IIII)", from_id, to_id, pos, inv);
    self->mEdgeList[i] = obj;
  }

  return 0;
}

// name 関数
PyObject*
CellLibrary_name(CellLibraryObject* self,
		 PyObject* args)
{
  return PyObject_FromString(self->mLibrary->name());
}

// technology 関数
PyObject*
CellLibrary_technology(CellLibraryObject* self,
		       PyObject* args)
{
  return PyCellTechnology_FromCellTechnology(self->mLibrary->technology());
}

// delay_model 関数
PyObject*
CellLibrary_delay_model(CellLibraryObject* self,
			PyObject* args)
{
  return PyCellDelayModel_FromCellDelayModel(self->mLibrary->delay_model());
}

// bus_naming_style 関数
PyObject*
CellLibrary_bus_naming_style(CellLibraryObject* self,
			     PyObject* args)
{
  return PyObject_FromString(self->mLibrary->bus_naming_style());
}

// date 関数
PyObject*
CellLibrary_date(CellLibraryObject* self,
		 PyObject* args)
{
  return PyObject_FromString(self->mLibrary->date());
}

// revision 関数
PyObject*
CellLibrary_revision(CellLibraryObject* self,
		     PyObject* args)
{
  return PyObject_FromString(self->mLibrary->revision());
}

// comment 関数
PyObject*
CellLibrary_comment(CellLibraryObject* self,
		    PyObject* args)
{
  return PyObject_FromString(self->mLibrary->comment());
}

// time_unit 関数
PyObject*
CellLibrary_time_unit(CellLibraryObject* self,
		      PyObject* args)
{
  return PyObject_FromString(self->mLibrary->time_unit());
}

// voltage_unit 関数
PyObject*
CellLibrary_voltage_unit(CellLibraryObject* self,
			 PyObject* args)
{
  return PyObject_FromString(self->mLibrary->voltage_unit());
}

// current_unit 関数
PyObject*
CellLibrary_current_unit(CellLibraryObject* self,
			 PyObject* args)
{
  return PyObject_FromString(self->mLibrary->current_unit());
}

// pulling_resistance_unit 関数
PyObject*
CellLibrary_pulling_resistance_unit(CellLibraryObject* self,
				    PyObject* args)
{
  return PyObject_FromString(self->mLibrary->pulling_resistance_unit());
}

// capacitive_load_unit 関数
PyObject*
CellLibrary_capacitive_load_unit(CellLibraryObject* self,
				 PyObject* args)
{
  double unit_val = self->mLibrary->capacitive_load_unit();
  string unit_str = self->mLibrary->capacitive_load_unit_str();
  return Py_BuildValue("(ds)", unit_val, unit_str.c_str());
}

// leakage_power_unit 関数
PyObject*
CellLibrary_leakage_power_unit(CellLibraryObject* self,
			       PyObject* args)
{
  return PyObject_FromString(self->mLibrary->leakage_power_unit());
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

// cell_list 関数
PyObject*
CellLibrary_cell_list(CellLibraryObject* self,
		      PyObject* args)
{
  ymuint nc = self->mLibrary->cell_num();
  PyObject* cell_list = PyList_New(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    PyObject* obj = self->mCellList[i];
    Py_INCREF(obj);
    PyList_SetItem(cell_list, i, obj);
  }

  return cell_list;
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

  const Cell* cell = self->mLibrary->cell(name);
  if ( cell == NULL ) {
    PyErr_SetString(PyExc_ValueError, "No such cell");
    return NULL;
  }

  PyObject* cell_obj = self->mCellList[cell->id()];

  Py_INCREF(cell_obj);
  return cell_obj;
}

// group_list 関数
PyObject*
CellLibrary_group_list(CellLibraryObject* self,
		       PyObject* args)
{
  ymuint n = self->mLibrary->group_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = self->mGroupList[i];
    Py_INCREF(obj1);
    PyList_SetItem(list_obj, i, obj1);
  }

  return list_obj;
}

// npn_class_list 関数
PyObject*
CellLibrary_npn_class_list(CellLibraryObject* self,
			   PyObject* args)
{
  ymuint n = self->mLibrary->npn_class_num();
  PyObject* list_obj = PyList_New(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = self->mClassList[i];
    Py_INCREF(obj1);
    PyList_SetItem(list_obj, i, obj1);
  }

  return list_obj;
}

// パタンの要素数を返す．
PyObject*
CellLibrary_pg_pat_num(CellLibraryObject* self,
		       PyObject*  args)
{
  return PyObject_FromYmuint32(self->mLibrary->pg_pat_num());
}

// パタンを返す．
PyObject*
CellLibrary_pg_pat(CellLibraryObject* self,
		   PyObject*  args)
{
  ymuint id = 0;
  if ( !PyArg_ParseTuple(args, "I", &id) ) {
    return NULL;
  }
  PyObject* result = self->mPatList[id];
  Py_INCREF(result);

  return result;
}

// ノードの個数を返す．
PyObject*
CellLibrary_pg_node_num(CellLibraryObject* self,
			PyObject* args)
{
  return PyObject_FromYmuint32(self->mLibrary->pg_node_num());
}

// ノードの情報を返す．
PyObject*
CellLibrary_pg_node(CellLibraryObject* self,
		    PyObject* args)
{
  ymuint id = 0;
  if ( !PyArg_ParseTuple(args, "I", &id) ) {
    return NULL;
  }
  ymuint input_id = 0;
  if ( self->mLibrary->pg_node_type(id) == kCellPatInput ) {
    input_id = self->mLibrary->pg_input_id(id);
  }

  PyObject* type_obj = PyCellPatType_FromCellPatType(self->mLibrary->pg_node_type(id));
  return Py_BuildValue("(OI)", type_obj, input_id);
}

// 枝の個数を返す．
PyObject*
CellLibrary_pg_edge_num(CellLibraryObject* self,
			PyObject* args)
{
  return PyObject_FromYmuint32(self->mLibrary->pg_edge_num());
}

// 枝を返す．
PyObject*
CellLibrary_pg_edge(CellLibraryObject* self,
		    PyObject* args)
{
  ymuint id = 0;
  if ( !PyArg_ParseTuple(args, "I", &id) ) {
    return NULL;
  }
  PyObject* result = self->mEdgeList[id];
  Py_INCREF(result);

  return result;
}

// dump 関数
PyObject*
CellLibrary_dump(CellLibraryObject* self,
		 PyObject* args)
{
  FileODO* bp = parse_FileODO(args);
  if ( bp == NULL ) {
    return NULL;
  }

  self->mLibrary->dump(*bp);

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
  {"bus_type", (PyCFunction)CellLibrary_bus_type, METH_VARARGS,
   PyDoc_STR("return bus type of the library (NONE)")},
  {"cell_list", (PyCFunction)CellLibrary_cell_list, METH_NOARGS,
   PyDoc_STR("return list of cell (NONE)")},
  {"cell_by_name", (PyCFunction)CellLibrary_cell_by_name, METH_VARARGS,
   PyDoc_STR("return cell (str)")},
  {"cell_group_list", (PyCFunction)CellLibrary_group_list, METH_NOARGS,
   PyDoc_STR("return list of cell group (NONE)")},
  {"npn_class_list", (PyCFunction)CellLibrary_npn_class_list, METH_NOARGS,
   PyDoc_STR("return list of cell class (NONE)")},
  {"pg_pat_num", (PyCFunction)CellLibrary_pg_pat_num, METH_NOARGS,
   PyDoc_STR("return the number  of pattern graphs (NONE)")},
  {"pg_pat", (PyCFunction)CellLibrary_pg_pat, METH_VARARGS,
   PyDoc_STR("return pattern graph (int)")},
  {"pg_node_num", (PyCFunction)CellLibrary_pg_node_num, METH_NOARGS,
   PyDoc_STR("return the number of pattern nodes (NONE)")},
  {"pg_node", (PyCFunction)CellLibrary_pg_node, METH_VARARGS,
   PyDoc_STR("return pattenr node (int)")},
  {"pg_edge_num", (PyCFunction)CellLibrary_pg_edge_num, METH_NOARGS,
   PyDoc_STR("return the number of pattern edge (NONE)")},
  {"pg_edge", (PyCFunction)CellLibrary_pg_edge, METH_VARARGS,
   PyDoc_STR("return pattern edge infomation (int)")},
  {"dump", (PyCFunction)CellLibrary_dump, METH_VARARGS,
   PyDoc_STR("dump (FileODO)")},
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
  "cell_lib.CellLibrary",             // tp_name
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
    PyErr_SetString(PyExc_TypeError, "cell_lib.CellLibrary is expected");
    return NULL;
  }

  // 強制的にキャスト
  CellLibraryObject* my_obj = (CellLibraryObject*)py_obj;

  return my_obj->mLibrary;
}


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

}

END_NAMESPACE_YM
