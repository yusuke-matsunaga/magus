#ifndef IRPRINTER_H
#define IRPRINTER_H

/// @file IrPrinter.h
/// @brief IrPrinter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrPrinter IrPrinter.h "IrPrinter.h"
/// @brief IrNode の内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class IrPrinter
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  IrPrinter(ostream& s);

  /// @brief デストラクタ
  ~IrPrinter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの内容を文として出力する．
  void
  print_statement(IrNode* node);

  /// @brief ノードの内容を式として出力する．
  void
  print_expr(IrNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};

END_NAMESPACE_YM_YMSL

#endif // IRPRINTER_H
