
/// @file tseconv.cc
/// @brief tseconev の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif

#include "igf_nsdef.h"
#include "ym_utils/PoptMainApp.h"


BEGIN_NAMESPACE_YM_IGF

int
tseconv(int argc,
	const char** argv)
{

  PoptMainApp app;

  // number オプション
  PoptNone popt_number("number", 'n',
		       "number mode");
  // digit オプション
  PoptNone popt_digit("digit", 'd',
		      "digit mode");
  // ascii オプション
  PoptNone popt_ascii("ascc", 'a',
		      "ascii mode");

  app.add_option(&popt_number);
  app.add_option(&popt_digit);
  app.add_option(&popt_ascii);

  app.set_other_option_help("<filename>");

  // オプション解析を行う．
  tPoptStat stat = app.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  // オプション解析行う．
  bool number_mode = popt_number.is_specified();
  bool digit_mode = popt_digit.is_specified();
  bool ascii_mode = popt_ascii.is_specified();
  if ( number_mode ) {
    if ( digit_mode || ascii_mode ) {
      cerr << "--digit and/or --ascii cannot be specified with --number." << endl;
      digit_mode = false;
      ascii_mode = false;
    }
  }
  else if ( digit_mode ) {
    if ( ascii_mode ) {
      cerr << "--ascii cannot be specified with --digit." << endl;
      ascii_mode = false;
    }
  }
  else if ( ascii_mode ) {
    ;
  }
  else {
    // none of the options are specified
    number_mode = true;
  }

  vector<string> args;
  ymuint n_args = app.get_args(args);
  if ( n_args != 1 ) {
    app.usage(2);
    return -1;
  }

  ifstream ifs(args[0].c_str());
  if ( !ifs ) {
    cerr << args[0] << " : No such file" << endl;
    return -1;
  }

  ymuint line = 1;
  vector<string> line_buf;
  string buf;
  while ( getline(ifs, buf) ) {
    if ( buf.size() != 4 ) {
      cerr << "Illegal format string at line " << line << endl;
      return -1;
    }
    ymuint val[4];
    for (ymuint i = 0; i < 4; ++ i) {
      char c = buf[i];
      if ( c < '0' || c > '9' ) {
	cerr << "Illegal character '" << c << "' at line " << line << endl;
	return -1;
      }
      val[i] = static_cast<ymuint>(c - '0');
    }
    ostringstream os;
    if ( number_mode ) {
      ymuint32 num = ((val[0] * 10 + val[1]) * 10 + val[2]) * 10 + val[3];
      for (ymuint i = 0; i < 14; ++ i) {
	ymuint32 mask = (1U << (13 - i));
	if ( num & mask ) {
	  os << '1';
	}
	else {
	  os << '0';
	}
      }
    }
    else if ( digit_mode ) {
      for (ymuint i = 0; i < 4; ++ i) {
	for (ymuint j = 4; j > 0; -- j) {
	  if ( val[i] & (1 << (j - 1)) ) {
	    os << '1';
	  }
	  else {
	    os << '0';
	  }
	}
      }
    }
    else if ( ascii_mode ) {
      for (ymuint i = 0; i < 4; ++ i) {
	ymuint aval = val[i] + '0';
	for (ymuint j = 7; j > 0; -- j) {
	  if ( aval & (1 << (j - 1)) ) {
	    os << '1';
	  }
	  else {
	    os << '0';
	  }
	}
      }
    }
    line_buf.push_back(os.str());
    ++ line;
  }

  ymuint n = 0;
  if ( number_mode ) {
    n = 14;
  }
  if ( digit_mode ) {
    n = 16;
  }
  else if ( ascii_mode ) {
    n = 28;
  }
  cout << n << " " << (line - 1) << endl;
  for (vector<string>::iterator p = line_buf.begin();
       p != line_buf.end(); ++ p) {
    cout << *p << endl;
  }

  return 0;
}

END_NAMESPACE_YM_IGF

int
main(int argc,
     const char** argv)
{
  using nsYm::nsIgf::tseconv;

  return tseconv(argc, argv);
}
