
/// @file TestBlifHandler.cc
/// @brief TestBlifHandler の実装プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TestBlifHandler.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_utils/FileRegion.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

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
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 各識別子のID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 入力カバーを表す文字列
// @param[in] opat 出力の極性
// @retval true 処理が成功した．
// @retval false エラーが起こった．
// @note cover_pat は ni 個ごとに1行のパタンを表す．
// 各要素のとりうる値は '0', '1', '-' を表す．
// @note opat は '0' か '1' のどちらか
bool
TestBlifHandler::names(ymuint32 onode_id,
		       const vector<ymuint32>& inode_id_array,
		       ymuint32 nc,
		       const char* cover_pat,
		       char opat)
{
  (*mStreamPtr) << ".names" << endl;
  ymuint ni = inode_id_array.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint32 id = inode_id_array[i];
    (*mStreamPtr) << id2str(id) << endl
		  << "\t[" << id2loc(id) << "]" << endl;
  }
  (*mStreamPtr) << id2str(onode_id) << endl
		  << "\t[" << id2loc(onode_id) << "]" << endl;
  for (ymuint i = 0; i < nc; ++ i) {
    for (ymuint j = 0; j < ni; ++ j) {
      (*mStreamPtr) << cover_pat[i * ni + j];
    }
    if ( ni > 0 ) {
      (*mStreamPtr) << ' ';
    }
    (*mStreamPtr) << opat << endl;
  }
  return true;
}

// @brief .gate 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id_array 入力ノードのID番号の配列
// @param[in] cell セル
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::gate(ymuint32 onode_id,
		      const vector<ymuint32>& inode_id_array,
		      const Cell* cell)
{
  (*mStreamPtr) << ".gate " << cell->name() << " " << id2str(onode_id)
		<< "\t[" << id2loc(onode_id) << "]" << endl;
  ymuint ni = inode_id_array.size();
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint id = inode_id_array[i];
    (*mStreamPtr) << "  " << cell->input(i)->name() << " " << id2str(id)
		  << "\t[" << id2loc(id) << "]" << endl;
  }
  return true;
}

// @brief .latch 文の処理
// @param[in] onode_id 出力ノードのID番号
// @param[in] inode_id 入力ノードのID番号
// @param[in] loc4 リセット値の位置情報
// @param[in] rval リセット時の値('0'/'1') 未定義なら ' '
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
TestBlifHandler::latch(ymuint32 onode_id,
		       ymuint32 inode_id,
		       const FileRegion& loc4,
		       char rval)
{
  (*mStreamPtr) << ".latch "
		<< id2str(inode_id) << " "
		<< id2str(onode_id) << " " << rval << endl
		<< "\t[" << id2loc(inode_id) << "]" << endl
		<< "\t[" << id2loc(onode_id) << "]" << endl
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

END_NAMESPACE_YM_NETWORKS_BLIF
