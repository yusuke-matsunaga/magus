#ifndef LIBYM_MISLIB_MISLIBPARSER_H
#define LIBYM_MISLIB_MISLIBPARSER_H

/// @file libym_mislib/MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mislib/mislib_nsdef.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class MislibParser MislibParser.h "ym_mislib/MislibParser.h"
/// @brief Mislib ファイルのパーサー
//////////////////////////////////////////////////////////////////////
class MislibParser
{
public:

  /// @brief コンストラクタ
  MislibParser();

  /// @brief デストラクタ
  ~MislibParser();


public:

  /// @brief mislib ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @return パース木の根のノードを返す．
  /// @note 読み込みが失敗したら NULL を返す．
  const MislibNode*
  read(const string& filename);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  MislibParserImpl* mImpl;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPARSER_H
