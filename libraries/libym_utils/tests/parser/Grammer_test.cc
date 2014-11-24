
/// @file Grammer_test.cc
/// @brief Grammer_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "../../src/parser/Grammer.h"
#include "../../src/parser/LR0Set.h"
#include "../../src/parser/LALR1Set.h"


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

  LALR1Set lr0set(&g);

  lr0set.print(cout);
}

void
test2()
{
  Grammer g;

  Token* id = g.add_token("id");
  Token* eq = g.add_token("=");
  Token* star = g.add_token("*");

  Token* S = g.add_token("S");
  Token* L = g.add_token("L");
  Token* R = g.add_token("R");

  {
    vector<Token*> right;
    right.push_back(L);
    right.push_back(eq);
    right.push_back(R);
    g.add_rule(S, right);
  }
  {
    vector<Token*> right;
    right.push_back(R);
    g.add_rule(S, right);
  }
  {
    vector<Token*> right;
    right.push_back(star);
    right.push_back(R);
    g.add_rule(L, right);
  }
  {
    vector<Token*> right;
    right.push_back(id);
    g.add_rule(L, right);
  }
  {
    vector<Token*> right;
    right.push_back(L);
    g.add_rule(R, right);
  }

  g.set_start(S);

  g.print_tokens(cout);

  g.print_rules(cout);

  LALR1Set lr0set(&g);

  lr0set.print(cout);
}

void
test3()
{
  Grammer g;

  Token* id = g.add_token("id");
  Token* plus = g.add_token("+", 1, kLeftAssoc);
  Token* times = g.add_token("*", 2, kLeftAssoc);
  Token* lpar = g.add_token("(");
  Token* rpar = g.add_token(")");

  Token* expr = g.add_token("expr");

  {
    vector<Token*> right;
    right.push_back(id);
    g.add_rule(expr, right);
  }
  {
    vector<Token*> right;
    right.push_back(expr);
    right.push_back(plus);
    right.push_back(expr);
    g.add_rule(expr, right);
  }
  {
    vector<Token*> right;
    right.push_back(expr);
    right.push_back(times);
    right.push_back(expr);
    g.add_rule(expr, right);
  }
  {
    vector<Token*> right;
    right.push_back(lpar);
    right.push_back(expr);
    right.push_back(rpar);
    g.add_rule(expr, right);
  }

  g.set_start(expr);

  g.print_tokens(cout);

  g.print_rules(cout);

  LALR1Set lr0set(&g);

  lr0set.print(cout);
}

void
Grammer_test(int argc,
	     char** argv)
{
#if 0
  test1();
#endif

#if 0
  test2();
#endif

#if 1
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
