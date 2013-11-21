#ifndef DOTLIBPARSER_H
#define DOTLIBPARSER_H

/// @file ym_dotlib/DotlibParser.h
/// @brief DotlibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

class DotlibParserImpl;

//////////////////////////////////////////////////////////////////////
/// @class DotlibParser DotlibParser.h "DotlibParser.h"
/// @brief Dotlib 用のパーサークラス
//////////////////////////////////////////////////////////////////////
class DotlibParser
{
public:

  /// @brief コンストラクタ
  DotlibParser();

  /// @brief デストラクタ
  /// @note 内部で確保したパース木は解放される．
  ~DotlibParser();


public:

  /// @brief ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] mgr DotlibNode を管理するオブジェクト
  /// @param[in] debug デバッグモード
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @return 読み込みが成功したら true を返す．
  /// @note パース木は mgr にセットされる．
  bool
  read_file(const string& filename,
	    DotlibMgr& mgr,
	    bool debug,
	    bool allow_no_semi = true);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサーの実体
  DotlibParserImpl* mImpl;

};

END_NAMESPACE_YM_DOTLIB

#endif // DOTLIBPARSER_H
