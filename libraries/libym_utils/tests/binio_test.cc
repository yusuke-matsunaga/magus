
/// @file libym_utils/tests/binio_test.cc
/// @brief BinIO のテスト
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

ymuint8 data_8[] = {
  0U,
  1U,
  3U,
  127U,
  240U,
  255U
};
ymuint n_8 = sizeof(data_8) / sizeof(ymuint8);

ymuint16 data_16[] = {
  127U,
  255U,
  32767U,
  65535U
};
ymuint n_16 = sizeof(data_16) / sizeof(ymuint16);

ymuint32 data_32[] = {
  127UL,
  255UL,
  32727UL,
  65535UL,
  (1UL << 31) - 1UL,
  (1UL << 31) + 1UL
};
ymuint n_32 = sizeof(data_32) / sizeof(ymuint32);

ymuint64 data_64[] = {
  127ULL,
  255ULL,
  32727ULL,
  65535ULL,
  (1ULL << 31) - 1UL,
  (1ULL << 31) + 1UL,
  (1ULL << 63) - 1UL,
  (1ULL << 63) + 1UL
};
ymuint n_64 = sizeof(data_64) / sizeof(ymuint64);

END_NONAMESPACE

bool
binio_test()
{
  const char* datafile = "binio_test.bin";

  ofstream os;
  os.open(datafile, ios::binary);
  if ( !os ) {
    // エラー
    cerr << "Could not create " << datafile << endl;
    return false;
  }

  for (ymuint i = 0; i < n_8; ++ i) {
    BinIO::write_8(os, data_8[i]);
  }
  for (ymuint i = 0; i < n_16; ++ i) {
    BinIO::write_16(os, data_16[i]);
  }
  for (ymuint i = 0; i < n_32; ++ i) {
    BinIO::write_32(os, data_32[i]);
  }
  for (ymuint i = 0; i < n_64; ++ i) {
    BinIO::write_64(os, data_64[i]);
  }
  os.close();

  ifstream is;
  is.open(datafile, ios::binary);
  if ( !is ) {
    // エラー
    cerr << "Could not open " << datafile << endl;
    return false;
  }

  ymuint nerr = 0;
  for (ymuint i = 0; i < n_8; ++ i) {
    ymuint8 val = BinIO::read_8(is);
    if ( val != data_8[i] ) {
      cerr << "data mismatch at data_8[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_8[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_16; ++ i) {
    ymuint16 val = BinIO::read_16(is);
    if ( val != data_16[i] ) {
      cerr << "data mismatch at data_16[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_16[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_32; ++ i) {
    ymuint32 val = BinIO::read_32(is);
    if ( val != data_32[i] ) {
      cerr << "data mismatch at data_32[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_32[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  for (ymuint i = 0; i < n_64; ++ i) {
    ymuint64 val = BinIO::read_64(is);
    if ( val != data_64[i] ) {
      cerr << "data mismatch at data_64[" << i << "]" << endl
	   << "write data = " << static_cast<ymuint>(data_64[i]) << endl
	   << "read data  = " << static_cast<ymuint>(val) << endl;
      ++ nerr;
    }
  }
  is.close();

  if ( nerr ) {
    cerr << "Total " << nerr << "error";
    if ( nerr > 1 ) {
      cerr << "s";
    }
  }
  return nerr == 0;
}

bool
binio_test2()
{
  const char* datafile = "binio_test2.bin";

  {
    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      // エラー
      cerr << "Could not create " << datafile << endl;
      return false;
    }
    BinO bo(os);

    for (ymuint i = 0; i < n_8; ++ i) {
      bo << data_8[i];
    }
    for (ymuint i = 0; i < n_16; ++ i) {
      bo << data_16[i];
    }
    for (ymuint i = 0; i < n_32; ++ i) {
      bo << data_32[i];
    }
    for (ymuint i = 0; i < n_64; ++ i) {
      bo << data_64[i];
    }
  }

  ymuint nerr = 0;
  {
    ifstream is;
    is.open(datafile, ios::binary);
    if ( !is ) {
      // エラー
      cerr << "Could not open " << datafile << endl;
      return false;
    }
    BinI bi(is);

    for (ymuint i = 0; i < n_8; ++ i) {
      ymuint8 val;
      bi >> val;
      if ( val != data_8[i] ) {
	cerr << "data mismatch at data_8[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_8[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_16; ++ i) {
      ymuint16 val;
      bi >> val;
      if ( val != data_16[i] ) {
	cerr << "data mismatch at data_16[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_16[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_32; ++ i) {
      ymuint32 val;
      bi >> val;
      if ( val != data_32[i] ) {
	cerr << "data mismatch at data_32[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_32[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
    for (ymuint i = 0; i < n_64; ++ i) {
      ymuint64 val;
      bi >> val;
      if ( val != data_64[i] ) {
	cerr << "data mismatch at data_64[" << i << "]" << endl
	     << "write data = " << static_cast<ymuint>(data_64[i]) << endl
	     << "read data  = " << static_cast<ymuint>(val) << endl;
	++ nerr;
      }
    }
  }

  if ( nerr ) {
    cerr << "Total " << nerr << "error";
    if ( nerr > 1 ) {
      cerr << "s";
    }
  }
  return nerr == 0;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  bool stat = nsYm::binio_test();
  if ( !stat ) {
    return 1;
  }

  stat = nsYm::binio_test2();
  if ( !stat ) {
    return 2;
  }

  return 0;
}
