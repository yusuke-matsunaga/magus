#ifndef YM_DOTLIB_DOTLIBPARSER_H
#define YM_DOTLIB_DOTLIBPARSER_H

/// @file ym_dotlib/DotlibParser.h
/// @brief DotlibParser のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: DotlibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

class DotlibParserImpl;

//////////////////////////////////////////////////////////////////////
/// Dotlib 用のパーサークラス
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
  /// @param[in] debug デバッグモード
  /// @param[in] allow_no_semi 行末のセミコロンなしを許すかどうか
  /// @return パース木の根のノードを返す．
  /// @note エラーが起きたら NULL を返す．
  const DotlibNode*
  read_file(const string& filename,
	    bool debug,
	    bool allow_no_semi = true);

  /// @brief 直前の read_file() で確保したパース木を解放する．
  void
  clear_node();

  /// @brief メモリ使用量のサマリを出力する．
  /// @param[in] s 出力先のストリーム
  void
  show_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パーサーの実体
  DotlibParserImpl* mImpl;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBPARSER_H
