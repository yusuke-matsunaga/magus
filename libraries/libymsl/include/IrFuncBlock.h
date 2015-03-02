#ifndef IRFUNCBLOCK_H
#define IRFUNCBLOCK_H

/// @file IrFuncBlock.h
/// @brief IrFuncBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrCodeBlock.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncBlock IrFuncBlock.h "IrFuncBlock.h"
/// @brief 関数の中間表現
//////////////////////////////////////////////////////////////////////
class IrFuncBlock :
  public IrCodeBlock
{
public:

  /// @brief コンストラクタ
  /// @param[in] arg_list 引数のリスト
  /// @param[in] arg_init_list 引数のデフォルト値のリスト
  /// @param[in] func_handle 関数のハンドル
  IrFuncBlock(const vector<IrHandle*>& arg_list,
	      const vector<IrNode*>& arg_init_list,
	      IrHandle* func_handle);

  /// @brief デストラクタ
  ~IrFuncBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数のハンドルを返す．
  IrHandle*
  func_handle();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 引数のリスト
  vector<IrHandle*> mArgList;

  // 引数のデフォルト値のリスト
  vector<IrNode*> mArgInitList;

  // 関数のハンドル
  IrHandle* mFuncHandle;

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCBLOCK_H
