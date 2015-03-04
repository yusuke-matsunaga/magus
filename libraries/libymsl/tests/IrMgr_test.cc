
/// @file ir_test.cc
/// @brief ir_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "AstMgr.h"
#include "AstStatement.h"
#include "AstPrinter.h"
#include "IrMgr.h"
#include "IrToplevel.h"
#include "IrPrinter.h"

#include "YmUtils/FileIDO.h"
#include "YmUtils/StreamIDO.h"
#include "YmUtils/StringIDO.h"
#include "YmUtils/MsgHandler.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_YMSL

int
ir_test1(IDO& ido)
{
  StreamMsgHandler handler(&cout);
  MsgMgr::reg_handler(&handler);

  AstMgr ast_mgr;
  bool stat = ast_mgr.read_source(ido);
  if ( !stat ) {
    return 1;
  }

  AstPrinter printer(cout);

  AstStatement* ast_toplevel = ast_mgr.toplevel();
  printer.print_statement(ast_toplevel);

  IrMgr ir_mgr;
  IrToplevel ir_toplevel;
  bool stat2 = ir_mgr.elaborate(ast_toplevel, ShString("__main__"), &ir_toplevel);
  if ( !stat2 ) {
    return 2;
  }

  IrPrinter ir_printer(cout);
  ir_printer.print_code(ir_toplevel);

  return 0;
}

int
ir_test(int argc,
	    char** argv)
{
  if ( argc == 1 ) {
    const char* str =
      "var a:int = 0;"
      "var c:int = 0;"
      "for (a = 0; a < 10; a ++) {"
      "if c % 2 == 1 {"
      "c = c + a;"
      "c = f2(c);"
      "}"
      "else {"
      "var tmp: int = a + 1;"
      "c = f(c + tmp);"
      "}"
      "}"
      ""
      "function f(a:int):int"
      "{"
      "enum ktype {"
      " k1 = 2,"
      " k2 = k1 + 1"
      "}"
      "return a * ktype.k2;"
      "}"
      ""
      "function f2(a:float):float"
      "{"
      "return a / 2.0;"
      "}"
      ;

    StringIDO ido(str);
    return ir_test1(ido);
  }
  else {
    for (int i = 1; i < argc; ++ i) {
      FileIDO ido;
      if ( !ido.open(argv[i]) ) {
	cerr << argv[i] << ": no such file" << endl;
	return -1;
      }
      int stat = ir_test1(ido);
      if ( stat != 0 ) {
	return stat;
      }
    }
  }
  return 0;
}

END_NAMESPACE_YM_YMSL


int
main(int argc,
     char** argv)
{
  return nsYm::nsYmsl::ir_test(argc, argv);
}
