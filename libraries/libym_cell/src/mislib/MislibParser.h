#ifndef MISLIBPARSER_H
#define MISLIBPARSER_H

/// @file MislibParser.h
/// @brief MislibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class MislibParser MislibParser.h "MislibParser.h"
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
  /// @param[in] mgr MislibNode を管理するクラス
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  read_file(const string& filename,
	    MislibMgr& mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うクラス
  MislibParserImpl* mImpl;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBPARSER_H
