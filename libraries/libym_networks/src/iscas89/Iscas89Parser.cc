
/// @file Iscas89Parser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89Parser.h"
#include "Iscas89ParserImpl.h"


BEGIN_NAMESPACE_YM_ISCAS89

// @brief コンストラクタ
Iscas89Parser::Iscas89Parser()
{
  mRep = new Iscas89ParserImpl;
}

// @brief デストラクタ
Iscas89Parser::~Iscas89Parser()
{
  delete mRep;
}

// @brief 読み込みを行う．
bool
Iscas89Parser::read(const string& filename)
{
  return mRep->read(filename);
}

// @brief イベントハンドラの登録
void
Iscas89Parser::add_handler(Iscas89Handler* handler)
{
  mRep->add_handler(handler);
}

END_NAMESPACE_YM_ISCAS89
