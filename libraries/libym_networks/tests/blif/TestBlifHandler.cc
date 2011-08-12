
/// @file TestBlifHandler.cc
/// @brief TestBlifHandler の実装プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TestBlifHandler.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_BLIF

// @brief コンストラクタ
TestBlifHandler::TestBlifHandler(ostream* streamptr) :
  mStreamPtr(streamptr)
{
}

// @brief デストラクタ
TestBlifHandler::~TestBlifHandler()
{
}

// @brief 初期化
bool
TestBlifHandler::init()
{
  return true;
}

// @brief .model 文の開始
bool
TestBlifHandler::model(const FileRegion& loc1,
		       const FileRegion& loc2,
		       const char* name)
{
  (*mStreamPtr) << ".model " << name << endl
		<< "\t[" << loc1 << "]" << endl
		<< "\t[" << loc2 << "]" << endl;
  return true;
}

// @brief .inputs 文中の文字列の処理
bool
TestBlifHandler::inputs_elem(ymuint32 name_id)
{
  (*mStreamPtr) << "  inputs elem: " << id2str(name_id) << endl
		<< "\t[" << id2loc(name_id) << "]" << endl;
  return true;
}

// @brief .outputs 文中の文字列の処理
bool
TestBlifHandler::outputs_elem(ymuint32 name_id)
{
  (*mStreamPtr) << "  outputs elem: " << id2str(name_id) << endl
		<< "\t[" << id2loc(name_id) << "]" << endl;
  return true;
}

// @brief .names 文の処理
bool
TestBlifHandler::names(const vector<ymuint32>& name_id_array,
		       ymuint32 nc,
		       const char* cover_pat,
		       char opat)
{
  (*mStreamPtr) << ".names" << endl;
  size_t n = name_id_array.size();
  for (size_t i = 0; i < n; ++ i) {
    ymuint32 id = name_id_array[i];
    (*mStreamPtr) << id2str(id) << endl
		  << "\t[" << id2loc(id) << "]" << endl;
  }
  size_t ni = n - 1;
  for (size_t i = 0; i < nc; ++ i) {
    for (size_t j = 0; j < ni; ++ j) {
      (*mStreamPtr) << cover_pat[i * ni + j];
    }
    if ( ni > 0 ) {
      (*mStreamPtr) << ' ';
    }
    (*mStreamPtr) << opat << endl;
  }
  return true;
}

// @brief .gate 文の開始
bool
TestBlifHandler::gate_begin(const FileRegion& loc1,
			    const FileRegion& loc2,
			    const char* name)
{
  (*mStreamPtr) << ".gate " << name << " begin" << endl
		<< "\t[" << loc1 << "]" << endl
		<< "\t[" << loc2 << "]" << endl;
  return true;
}

// @brief .gate 文中のピン割り当ての処理
bool
TestBlifHandler::gate_assign(const FileRegion& loc1,
			     const char* f_name,
			     const FileRegion& loc2,
			     const char* a_name)
{
  (*mStreamPtr) << "  .gate assign: "
		<< f_name << " = " << a_name << endl
		<< "\t[" << loc1 << "]" << endl
		<< "\t[" << loc2 << "]" << endl;
  return true;
}

// @brief .gate 文の終了
bool
TestBlifHandler::gate_end()
{
  (*mStreamPtr) << ".gate end" << endl << endl;
  return true;
}

// @brief .latch 文の処理
bool
TestBlifHandler::latch(ymuint32 name1_id,
		       ymuint32 name2_id,
		       const FileRegion& loc4,
		       char rval)
{
  (*mStreamPtr) << ".latch "
		<< id2str(name1_id) << " "
		<< id2str(name2_id) << " " << rval << endl
		<< "\t[" << id2loc(name1_id) << "]" << endl
		<< "\t[" << id2loc(name2_id) << "]" << endl
		<< "\t[" << loc4 << "]" << endl;
  return true;
}

// @brief .end 文の処理
bool
TestBlifHandler::end(const FileRegion& loc)
{
  (*mStreamPtr) << ".end" << endl
		<< "\t[" << loc << "]" << endl;
  return true;
}

// @brief 通常終了時の処理
void
TestBlifHandler::normal_exit()
{
  (*mStreamPtr) << "normal_exit" << endl;
}

// @brief エラー終了時の処理
void
TestBlifHandler::error_exit()
{
  (*mStreamPtr) << "error_exit" << endl;
}

END_NAMESPACE_YM_BLIF
