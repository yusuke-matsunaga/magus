
/// @file libym_mislib/CellMislibReader.cc
/// @brief CellMislibReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: CellMislibReader.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/CellMislibReader.h"
#include "MislibParser.h"
#include "MislibMgr.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
// MisLibPt オブジェクトを管理するクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellMislibReader::CellMislibReader()
{
  mParser = new nsMislib::MislibParser;
}

// @brief デストラクタ
CellMislibReader::~CellMislibReader()
{
  delete mParser;
}

// @brief mislib ファイルを読み込む
// @param[in] filename ファイル名
// @return 読み込んで作成したセルライブラリを返す．
// @note エラーが起きたら NULL を返す．
const CellLibrary*
CellMislibReader::read(const string& filename)
{
  using nsMislib::MislibPt;
  using nsMislib::MislibMgr;

  const MislibPt* root = mParser->read(filename);

  MislibMgr mgr;
  return mgr.gen_library(filename, root);
}

END_NAMESPACE_YM_CELL
