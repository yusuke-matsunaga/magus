
/// @file BlifParser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParserImpl.h"
#include "BlifParser.h"


BEGIN_NAMESPACE_YM_BLIF

// @brief コンストラクタ
BlifParser::BlifParser()
{
  mRep = new BlifParserImpl;
}

// @brief デストラクタ
BlifParser::~BlifParser()
{
  delete mRep;
}

// @brief 読み込みを行う．
bool
BlifParser::read(const string& filename)
{
  return mRep->read(filename);
}

// @brief イベントハンドラの登録
void
BlifParser::add_handler(BlifHandler* handler)
{
  mRep->add_handler(handler);
}

END_NAMESPACE_YM_BLIF
