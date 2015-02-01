#ifndef IRJUMP_H
#define IRJUMP_H

/// @file IrJump.h
/// @brief IrJump のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrJump IrJump.h "IrJump.h"
/// @brief jump/branch 系の命令を表すクラス
//////////////////////////////////////////////////////////////////////
class IrJump :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  IrJump(OpCode opcode);

  /// @brief デストラクタ
  virtual
  ~IrJump();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief opcode を返す．
  virtual
  OpCode
  opcode() const;

  /// @brief ジャンプ先のノードを得る．
  ///
  /// kOpJump, kOpBranchXXX のみ有効
  virtual
  IrNode*
  jump_addr() const;

  /// @brief ジャンプ先のノードを設定する．
  /// @param[in] node ジャンプ先のノード
  ///
  /// kOpJump, kOpBranchXXX のみ有効
  virtual
  void
  set_addr(IrNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  OpCode mOpCode;

  // ジャンプ先のノード
  IrNode* mJumpAddr;

};

END_NAMESPACE_YM_YMSL

#endif // IRJUMP_H
