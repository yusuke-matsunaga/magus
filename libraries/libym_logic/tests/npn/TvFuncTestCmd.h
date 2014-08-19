#ifndef TVFUNCTESTCMD_H
#define TVFUNCTESTCMD_H

/// @file TvFuncTestCmd.h
/// @brief TvFuncTestCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "tclpp/TclCmd.h"
#include "npn/common.h"

#include "TvFuncTest.h"


BEGIN_NAMESPACE_YM_NPN

//////////////////////////////////////////////////////////////////////
/// TvFunc のメンバ関数をテストするコマンド
//////////////////////////////////////////////////////////////////////
class TvFuncTestCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  TvFuncTestCmd();

  /// @brief デストラクタ
  ~TvFuncTestCmd();


protected:

  /// @brief usage を出力する関数
  virtual
  void usage();

  /// @brief コマンドを実行する仮想関数
  virtual
  int cmd_proc(TclObjVector& objv);


private:

  // 関数を取り出す．
  int get_tvfunc(TclObj& obj,
		 TvFunc& func);

  // 入力番号を取り出す．
  int get_pos(TclObj& obj,
	      size_t ni,
	      unsigned int& pos);


private:

  TvFuncTest test;

};

END_NAMESPACE_YM_NPN

#endif // LIBYM_NPN_TESTS_TVFUNCTESTCMD_H
