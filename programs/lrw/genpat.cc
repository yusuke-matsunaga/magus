
/// @file genpat.cc
/// @brief genpat のメインプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.

#include "GenPat.h"
#include "GenPat2.h"


BEGIN_NAMESPACE_YM

int
genpat(int argc,
       const char** argv)
{
  if ( argc == 2 && strcmp(argv[1], "2") == 0 ) {
    GenPat2 gp;

    gp();
  }
  else {
    GenPat gp;

    gp();
  }

  return 0;
}

END_NAMESPACE_YM

int
main(int argc,
     const char** argv)
{
  nsYm::genpat(argc, argv);
}
