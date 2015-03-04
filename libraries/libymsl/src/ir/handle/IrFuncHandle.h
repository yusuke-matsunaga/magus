#ifndef IRFUNCHANDLE_H
#define IRFUNCHANDLE_H

/// @file IrFuncHandle.h
/// @brief IrFuncHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFuncHandle IrFuncHandle.h "IrFuncHandle.h"
/// @brief Function を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrFuncHandle :
  public IrIndexHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 関数の型
  /// @param[in] module_index モジュールインデックス
  /// @param[in] local_index ローカルインデックス
  IrFuncHandle(ShString name,
	       const Type* type,
	       ymuint module_index,
	       ymuint local_index);

  /// @brief デストラクタ
  virtual
  ~IrFuncHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_YMSL

#endif // IRFUNCHANDLE_H
