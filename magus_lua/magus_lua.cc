
/// @file magus_lua.cc
/// @brief magus_lua の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include <unistd.h> // for getopt()
#include <libgen.h> // for basename()
#include "LuaMagus.h"


BEGIN_NONAMESPACE

void
usage(
  char** argv
)
{
  using namespace std;

  cerr << "Usage: " << basename(argv[0])
       << " <script-file>" << endl;
}

END_NONAMESPACE


int
main(
  int argc,
  char** argv
)
{
  using namespace std;
  using namespace MAGUS_NAMESPACE;

  if ( argc < 2 ) {
    usage(argv);
    return 1;
  }

  LuaMagus lua;

  lua.L_openlibs();
  lua.open_Magus();

  // 引数を lua のグローバル変数 "arg" にセットする．
  lua.create_table(argc, 0);
  for ( int i = 0; i < argc; ++ i ) {
    lua.push_string(argv[i]);
    lua.set_table(-2, i);
  }
  lua.set_global("arg");

  string init_file = argv[1];
  if ( init_file != string{} && lua.L_dofile(init_file.c_str()) ) {
    cerr << lua.to_string(-1) << endl;
    return 1;
  }

  return 0;
}