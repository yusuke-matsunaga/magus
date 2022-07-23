
/// @file equiv_test.cc
/// @brief equiv_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/BnNetwork.h"
#include "EquivMgr.h"


BEGIN_NAMESPACE_MAGUS

BnNetwork
read_network(
  const string& filename
)
{
  auto pos = filename.find_last_of(".");
  ASSERT_COND( pos != string::npos );
  auto ext = filename.substr(pos + 1);
  BnNetwork network;
  if ( ext == "aag" ) {
    network = BnNetwork::read_aag(filename);
  }
  else if ( ext == "aig" ) {
    network = BnNetwork::read_aig(filename);
  }
  else if ( ext == "blif" ) {
    network = BnNetwork::read_blif(filename);
  }
  else if ( ext == "bench" ) {
    network = BnNetwork::read_iscas89(filename);
  }
  else {
    cerr << "Unknown filetype: " << ext << endl;
    return BnNetwork{};
  }
  return network;
}

int
equiv_test(
  int argc,
  char** argv
)
{
  if ( argc != 3 ) {
    return 1;
  }

  auto network1 = read_network(argv[1]);
  auto network2 = read_network(argv[2]);

  EquivMgr mgr;
  auto result = mgr.check(network1, network2);
  switch ( result.result() ) {
  case SatBool3::True:
    cout << "OK" << endl;
    break;
  case SatBool3::False:
    cout << "NG" << endl;
    {
      auto& oresults = result.output_results();
      SizeType n = oresults.size();
      for ( SizeType i = 0; i < n; ++ i ) {
	cout << " O#" << i << ": ";
	switch ( oresults[i] ) {
	case SatBool3::True:
	  cout << "OK";
	  break;
	case SatBool3::False:
	  cout << "NG";
	  break;
	case SatBool3::X:
	  cout << "Abort";
	  break;
	}
	cout << endl;
      }
    }
    break;
  case SatBool3::X:
    cout << "Abort" << endl;
    break;
  }

  return 0;
}

END_NAMESPACE_MAGUS


int
main(
  int argc,
  char** argv
)
{
  return MAGUS_NAMESPACE::equiv_test(argc, argv);
}
