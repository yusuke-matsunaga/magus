
/// @file libym_networks/iscas89/Iscas89NetworkReader.cc
/// @brief Iscas89NetworkReader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89NetworkReader.h"
#include "Iscas89NetworkHandler.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// クラス Iscas89NetworkReader
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89NetworkReader::Iscas89NetworkReader() :
  mHandler(new Iscas89NetworkHandler)
{
  mParser.add_handler(mHandler);
}

// @brief デストラクタ
Iscas89NetworkReader::~Iscas89NetworkReader()
{
  // mHandler は Iscas89Parser が破壊する
}

// @brief ISCAS89 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[out] network 読み込んだ内容を設定するネットワーク
// @retval true 正常に読み込めた．
// @retval false 読み込み中にエラーが起こった．
bool
Iscas89NetworkReader::read(const string& filename,
			   Iscas89Network& network)
{
  mHandler->set_network(&network);

  bool stat = mParser.read(filename);
  if ( !stat ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_ISCAS89
