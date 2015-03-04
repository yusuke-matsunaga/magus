#ifndef IRGLOBALVARHANDLE_H
#define IRGLOBALVARHANDLE_H

/// @file IrGlobalVarHandle.h
/// @brief IrGlobalVarHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrIndexHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrGlobalVarHandle IrGlobalVarHandle.h "IrGlobalVarHandle.h"
/// @brief Var を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrGlobalVarHandle :
  public IrIndexHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] module_index モジュールインデックス
  /// @param[in] local_index ローカルインデックス
  IrGlobalVarHandle(ShString name,
		    const Type* value_type,
		    ymuint module_index,
		    ymuint local_index);

  /// @brief デストラクタ
  virtual
  ~IrGlobalVarHandle();


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

#endif // IRGLOBALVARHANDLE_H
