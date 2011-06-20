#ifndef LIBYM_MISLIB_MISLIBPARSERIMPL_H
#define LIBYM_MISLIB_MISLIBPARSERIMPL_H

/// @file libym_mislib/MislibParserImpl.h
/// @brief MislibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"

#include "MislibNodeImpl.h"
#include "MislibLex.h"


BEGIN_NAMESPACE_YM_MISLIB

class MislibMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class MislibParserImpl MislibParserImpl.h "MislibParserImpl.h"
/// @brief MislibParser の実装クラス
//////////////////////////////////////////////////////////////////////
class MislibParserImpl
{
public:

  /// @brief コンストラクタ
  MislibParserImpl();

  /// @brief デストラクタ
  ~MislibParserImpl();


public:

  /// @brief mislib ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] mgr MislibNode を管理するオブジェクト
  /// @return パース木の根のノードを返す．
  /// @note 読み込みが失敗したら false を返す．
  bool
  read_file(const string& filename,
	    MislibMgrImpl* mgr);


public:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(MislibNodeImpl*& lval,
       FileRegion& lloc);

  /// @brief エラーメッセージを出力する．
  void
  error(const FileRegion& loc,
	const char* msg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  MislibLex mLex;

  // MislibNode を管理するオブジェクト
  MislibMgrImpl* mMislibMgr;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPARSERIMPL_H
