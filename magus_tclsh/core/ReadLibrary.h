#ifndef READLIBRARY_H
#define READLIBRARY_H

/// @file ReadLibrary.h
/// @brief ReadLibrary のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadLibrary ReadLibrary.h "ReadLibrary.h"
/// @ingroup MagusGroup
/// @brief liberty 形式のセルライブラリの読み込みを行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadLibrary :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  ReadLibrary(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadLibrary();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] library 設定対象のライブラリ
  /// @return 読み込みが成功したら true を返す．
  virtual
  bool
  read_library(const string& filemae,
	       CellLibrary* library) = 0;

};

END_NAMESPACE_MAGUS

#endif // READLIBRARY_H
