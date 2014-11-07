
/// @file Grammer_test.cc
/// @brief Grammer_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "../../src/misc/Grammer.h"
#include "../../src/misc/LR0Set.h"


BEGIN_NAMESPACE_YM

void
test1()
{
  Grammer g;

  Token* id = g.add_token("id");
  Token* plus = g.add_token("+");
  Token* times = g.add_token("*");
  Token* lpar = g.add_token("(");
  Token* rpar = g.add_token(")");
  Token* expr = g.add_token("expr");
  Token* term = g.add_token("term");
  Token* factor = g.add_token("factor");

  {
    vector<Token*> right;
    right.push_back(expr);
    right.push_back(plus);
    right.push_back(term);
    g.add_rule(expr, right);
  }
  {
    vector<Token*> right;
    right.push_back(term);
    g.add_rule(expr, right);
  }
  {
    vector<Token*> right;
    right.push_back(term);
    right.push_back(times);
    right.push_back(factor);
    g.add_rule(term, right);
  }
  {
    vector<Token*> right;
    right.push_back(factor);
    g.add_rule(term, right);
  }
  {
    vector<Token*> right;
    right.push_back(lpar);
    right.push_back(expr);
    right.push_back(rpar);
    g.add_rule(factor, right);
  }
  {
    vector<Token*> right;
    right.push_back(id);
    g.add_rule(factor, right);
  }
  g.set_start(expr);

  g.print_tokens(cout);

  g.print_rules(cout);

  LR0Set lr0set(&g);

  lr0set.print(cout);
}

void
Grammer_test(int argc,
	     char** argv)
{
  test1();

#if 0
  test2();

  test3();
#endif
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  YMTOOLS_NAMESPACE::Grammer_test(argc, argv);

  return 0;
}
