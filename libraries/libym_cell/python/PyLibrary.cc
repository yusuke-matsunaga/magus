
/// @file PyLibrary.cc
/// @brief PyLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyLibrary.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"
#include "ym_cell/Cell.h"


BEGIN_NAMESPACE_YM

/// @brief CellLutTemplate から CellLutTemplateObject を生成する．
/// @param[in] lut_template LUTテンプレート
extern
PyObject*
PyCellLutTemplate_FromCellLutTemplate(const CellLutTemplate* lut_template);

/// @brief CellClass から CellClassObject を生成する．
/// @param[in] cell_class セルクラス
/// @param[in] py_library ライブラリ
extern
PyObject*
PyCellClass_FromCellClass(const CellClass* cell_class,
			  PyLibrary* py_library);

/// @brief CellGroup から CellGroupObject を生成する．
/// @param[in] group グループ
/// @param[in] py_library ライブラリ
extern
PyObject*
PyCellGroup_FromCellGroup(const CellGroup* group,
			  PyLibrary* py_library);

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


//////////////////////////////////////////////////////////////////////
// クラス PyLibrary;
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyLibrary::PyLibrary(const CellLibrary* library)
{
  mLibrary = library;

  mName = PyObject_FromString(library->name());
  switch ( library->technology() ) {
  case CellLibrary::kTechCmos: mTechnology = PyObject_FromString("cmos"); break;
  case CellLibrary::kTechFpga: mTechnology = PyObject_FromString("fpga"); break;
  }
  const char* dm_str = NULL;
  switch ( library->delay_model() ) {
  case kCellDelayGenericCmos:   dm_str = "generic_cmos"; break;
  case kCellDelayTableLookup:   dm_str = "table_lookup"; break;
  case kCellDelayPiecewiseCmos: dm_str = "piecewise_cmos"; break;
  case kCellDelayCmos2:         dm_str = "cmos2"; break;
  case kCellDelayDcm:           dm_str = "dcm"; break;
  }
  mDelayModel = PyObject_FromString(dm_str);
  mBusNamingStyle = PyObject_FromString(library->bus_naming_style());
  mDate = PyObject_FromString(library->date());
  mRevision = PyObject_FromString(library->revision());
  mComment = PyObject_FromString(library->comment());
  mTimeUnit = PyObject_FromString(library->time_unit());
  mVoltageUnit = PyObject_FromString(library->voltage_unit());
  mCurrentUnit = PyObject_FromString(library->current_unit());
  mPullingResistanceUnit = PyObject_FromString(library->pulling_resistance_unit());
  double unit_val = library->capacitive_load_unit();
  string unit_str = library->capacitive_load_unit_str();
  mCapacitiveLoadUnit = Py_BuildValue("(ds)", unit_val, unit_str.c_str());
  mLeakagePowerUnit = PyObject_FromString(library->leakage_power_unit());

  ymuint nlt = library->lu_table_template_num();
  mLutTemplateList = new PyObject*[nlt];
  for (ymuint i = 0; i < nlt; ++ i) {
    const CellLutTemplate* lut_template = library->lu_table_template(i);
    PyObject* obj = PyCellLutTemplate_FromCellLutTemplate(lut_template);
    mLutTemplateList[i] = obj;
  }

  ymuint nc = library->cell_num();
  mCellList = new PyObject*[nc];
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    PyObject* cell_obj = PyCellCell_FromCell(cell);
    mCellList[i] = cell_obj;
  }

  ymuint ng = library->group_num();
  mGroupList = new PyObject*[ng];
  for (ymuint i = 0; i < ng; ++ i) {
    const CellGroup* group = library->group(i);
    PyObject* group_obj = PyCellGroup_FromCellGroup(group, this);
    mGroupList[i] = group_obj;
  }

  ymuint nn = library->npn_class_num();
  mClassList = new PyObject*[nn];
  for (ymuint i = 0; i < nn; ++ i) {
    const CellClass* cell_class = library->npn_class(i);
    PyObject* class_obj = PyCellClass_FromCellClass(cell_class, this);
    mClassList[i] = class_obj;
  }

  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    const CellGroup* group = cell->cell_group();
    PyObject* py_obj = mCellList[i];
    PyObject* group_obj = mGroupList[group->id()];
    PyCellCell_set_group(py_obj, group_obj);
  }

  for (ymuint i = 0; i < ng; ++ i) {
    PyObject* py_obj = mGroupList[i];
    const CellGroup* group = library->group(i);
    const CellClass* rep = group->rep_class();
    PyObject* rep_obj = mClassList[rep->id()];
    PyCellGroup_set_rep(py_obj, rep_obj);
  }

  ymuint np = library->pg_pat_num();
  mPatList = new PyObject*[np];
  for (ymuint i = 0; i < np; ++ i) {
    const CellPatGraph& pat = library->pg_pat(i);
    PyObject* pat_obj = PyCellPatGraph_FromCellPatGraph(&pat);
    mPatList[i] = pat_obj;
  }

  kPatI = PyObject_FromString("INPUT");
  kPatA = PyObject_FromString("AND");
  kPatX = PyObject_FromString("XOR");

  ymuint npn = library->pg_node_num();
  mNodeList = new PyObject*[npn];
  for (ymuint i = 0; i < npn; ++ i) {
    tCellPatType pat_type = library->pg_node_type(i);
    PyObject* pat = NULL;
    switch ( pat_type ) {
    case kCellPatInput: pat = kPatI; break;
    case kCellPatAnd:   pat = kPatA; break;
    case kCellPatXor:   pat = kPatX; break;
    }
    ymuint id = 0;
    if ( pat_type == kCellPatInput ) {
      id = library->pg_input_id(i);
    }
    mNodeList[i] = Py_BuildValue("(OI)", pat, id);
  }

  ymuint ne = library->pg_edge_num();
  mEdgeList = new PyObject*[ne];
  for (ymuint i = 0; i < ne; ++ i) {
    ymuint from_id = library->pg_edge_from(i);
    ymuint to_id = library->pg_edge_to(i);
    ymuint pos = library->pg_edge_pos(i);
    bool inv = library->pg_edge_inv(i);
    PyObject* obj = Py_BuildValue("(IIII)", from_id, to_id, pos, inv);
    mEdgeList[i] = obj;
  }
}

// @brief デストラクタ
PyLibrary::~PyLibrary()
{
  Py_DECREF(mName);
  Py_DECREF(mTechnology);
  Py_DECREF(mDelayModel);
  Py_DECREF(mBusNamingStyle);
  Py_DECREF(mDate);
  Py_DECREF(mRevision);
  Py_DECREF(mComment);
  Py_DECREF(mTimeUnit);
  Py_DECREF(mVoltageUnit);
  Py_DECREF(mCurrentUnit);
  Py_DECREF(mPullingResistanceUnit);
  Py_DECREF(mCapacitiveLoadUnit);
  Py_DECREF(mLeakagePowerUnit);

  ymuint nl = mLibrary->lu_table_template_num();
  for (ymuint i = 0; i < nl; ++ i) {
    Py_DECREF(mLutTemplateList[i]);
  }
  delete [] mLutTemplateList;

  ymuint n = mLibrary->cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    Py_DECREF(mCellList[i]);
  }
  delete [] mCellList;

  ymuint ng = mLibrary->group_num();
  for (ymuint i = 0; i < ng; ++ i) {
    Py_DECREF(mGroupList[i]);
  }
  delete [] mGroupList;

  ymuint nn = mLibrary->npn_class_num();
  for (ymuint i = 0; i < nn; ++ i) {
    Py_DECREF(mClassList[i]);
  }
  delete [] mClassList;

  ymuint np = mLibrary->pg_pat_num();
  for (ymuint i = 0; i < np; ++ i) {
    Py_DECREF(mPatList[i]);
  }
  delete [] mPatList;

  ymuint npn = mLibrary->pg_node_num();
  for (ymuint i = 0; i < npn; ++ i) {
    Py_DECREF(mNodeList[i]);
  }
  delete [] mNodeList;

  ymuint ne = mLibrary->pg_edge_num();
  for (ymuint i = 0; i < ne; ++ i) {
    Py_DECREF(mEdgeList[i]);
  }
  delete [] mEdgeList;

  Py_DECREF(kPatI);
  Py_DECREF(kPatA);
  Py_DECREF(kPatX);
}

// @brief 遅延テーブルのテンプレートを返す．
PyObject*
PyLibrary::lu_table_template(ymuint pos)
{
  assert_cond(pos < mLibrary->lu_table_template_num(), __FILE__, __LINE__);
  return mLutTemplateList[pos];
}

// @brief セルを返す．
PyObject*
PyLibrary::cell(ymuint pos)
{
  assert_cond( pos < mLibrary->cell_num(), __FILE__, __LINE__);
  return mCellList[pos];
}

// @brief セルグループを返す．
PyObject*
PyLibrary::cell_group(ymuint pos)
{
  assert_cond( pos < mLibrary->group_num(), __FILE__, __LINE__);
  return mGroupList[pos];
}

// @brief NPN同値クラスを返す．
PyObject*
PyLibrary::npn_class(ymuint pos)
{
  assert_cond( pos < mLibrary->npn_class_num(), __FILE__, __LINE__);
  return mClassList[pos];
}

// @brief パタンを返す．
PyObject*
PyLibrary::pg_pat(ymuint pos)
{
  assert_cond( pos < mLibrary->pg_pat_num(), __FILE__, __LINE__);
  return mPatList[pos];
}

// @brief ノードの情報を返す．
PyObject*
PyLibrary::pg_node(ymuint pos)
{
  assert_cond( pos < mLibrary->pg_node_num(), __FILE__, __LINE__);
  return mNodeList[pos];
}

// @brief 枝の情報を返す．
// @note (from, to, ipos, inv) のタプルを返す．
PyObject*
PyLibrary::pg_edge(ymuint pos)
{
  assert_cond( pos < mLibrary->pg_edge_num(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

END_NAMESPACE_YM
