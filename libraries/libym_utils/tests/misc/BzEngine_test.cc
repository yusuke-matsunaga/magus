
// zstream のテスト


#include "io/BzEngine.h"
#include <fcntl.h>


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

string
BZ_ACTION(int action)
{
  switch ( action ) {
  case BZ_RUN:              return "BZ_RUN";
  case BZ_FLUSH:            return "BZ_FLUSH";
  case BZ_FINISH:           return "BZ_FINISH";
  default:
    {
      ostringstream buf;
      buf << "Undefined action: " << action;
      return buf.str();
    }
  }
  return string();
}

string
BZ_RET(int code)
{
  switch ( code ) {
  case BZ_OK:               return "BZ_OK";
  case BZ_RUN_OK:           return "BZ_RUN_OK";
  case BZ_FLUSH_OK:         return "BZ_FLUSH_OK";
  case BZ_FINISH_OK:        return "BZ_FINISH_OK";
  case BZ_STREAM_END:       return "BZ_STREAM_END";
  case BZ_SEQUENCE_ERROR:   return "BZ_SEQUENCE_ERROR";
  case BZ_PARAM_ERROR:      return "BZ_PARAM_ERROR";
  case BZ_MEM_ERROR:        return "BZ_MEM_ERROR";
  case BZ_DATA_ERROR:       return "BZ_DATA_ERROR";
  case BZ_DATA_ERROR_MAGIC: return "BZ_DATA_ERROR_MAGIC";
  case BZ_IO_ERROR:         return "BZ_IO_ERROR";
  case BZ_UNEXPECTED_EOF:   return "BZ_UNEXPECTED_EOF";
  case BZ_OUTBUFF_FULL:     return "BZ_OUTBUFF_FULL";
  case BZ_CONFIG_ERROR:     return "BZ_CONFIG_ERROR";
  default:
    {
      ostringstream buf;
      buf << "Undefined code: " << code;
      return buf.str();
    }
  }
  return string();
}

END_NONAMESPACE

int
compress(const char* infile,
	 const char* outfile)
{
  BzEngine bce;

  int i_fd = open(infile, O_RDONLY);
  if ( i_fd < 0 ) {
    cerr << "Could not open " << infile << endl;
    return -1;
  }

  int o_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if ( o_fd < 0 ) {
    cerr << "Could not create " << outfile << endl;
    return -1;
  }

  ymuint BUFF_SIZE = 4096;
  ymuint8 rbuff[BUFF_SIZE];
  ymuint8 wbuff[BUFF_SIZE];

  int ret = bce.compress_init(9, 0, 0);
  if ( ret != BZ_OK ) {
    cerr << "error in BzEngine::compress_init(9, 0, 0)" << endl;
    return -1;
  }

  int action = BZ_RUN;
  bce.set_outbuf(wbuff, BUFF_SIZE);
  do {
    ssize_t res = read(i_fd, rbuff, BUFF_SIZE);
    if ( res < 0 ) {
      cerr << "error in read()" << endl;
      return -1;
    }
    cerr << "read " << res << " bytes" << endl;

    if ( res == 0 ) {
      action = BZ_FINISH;
    }

    bce.set_inbuf(rbuff, static_cast<ymuint>(res));
    do {
      ret = bce.compress(action);
      cerr << "compress(" << BZ_ACTION(action) << ") => " << BZ_RET(ret) << endl;
      if ( ret == BZ_SEQUENCE_ERROR ) {
	cerr << "error in BzStream::compress(" << BZ_ACTION(action) << ")" << endl;
	return -1;
      }
      else if ( ret == BZ_RUN_OK ) {
	action = BZ_RUN;
      }
      else if ( ret == BZ_FLUSH_OK ) {
	action = BZ_FLUSH;
      }
      else if ( ret == BZ_FINISH_OK ) {
	action = BZ_FINISH;
      }

      if ( bce.avail_out() == 0 ) {
	res = write(o_fd, wbuff, BUFF_SIZE);
	if ( res < 0 ) {
	  cerr << "error in write()" << endl;
	  return -1;
	}
	cerr << "write " << res << " bytes" << endl;
	bce.set_outbuf(wbuff, BUFF_SIZE);
      }
    } while (bce.avail_in() > 0);
  } while ( ret != BZ_STREAM_END );
  ymuint64 wr = BUFF_SIZE - bce.avail_out();
  if ( wr > 0 ) {
    ssize_t res = write(o_fd, wbuff, wr);
    if ( res < 0 ) {
      cerr << "error in write()" << endl;
      return -1;
    }
    cerr << "write " << res << " bytes" << endl;
  }

  close(i_fd);
  close(o_fd);

  return 0;
}

int
decompress(const char* infile,
	   const char* outfile)
{
  BzEngine bde;

  int i_fd = open(infile, O_RDONLY);
  if ( i_fd < 0 ) {
    cerr << "Could not open " << infile << endl;
    return -1;
  }

  int o_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if ( o_fd < 0 ) {
    cerr << "Could not create " << outfile << endl;
    return -1;
  }

  const ymuint BUFF_SIZE = 4096;
  ymuint8 rbuff[BUFF_SIZE];
  ymuint8 wbuff[BUFF_SIZE];

  int ret = bde.decompress_init(0, 0);
  if ( ret != BZ_OK ) {
    cerr << "error in BzEngine::decompress_init(0, 0)" << endl;
    return -1;
  }

  bde.set_outbuf(wbuff, BUFF_SIZE);
  do {
    ssize_t res = read(i_fd, rbuff, 4096);
    if ( res < 0 ) {
      cerr << "error in read()" << endl;
      return -1;
    }
    bde.set_inbuf(rbuff, static_cast<ymint>(res));
    //assert(strm.avail_in != 0);
    do {
      ret = bde.decompress();
      if ( ret != BZ_OK && ret != BZ_STREAM_END ) {
	cerr << "error in BzStream::decompress()" << endl;
	return -1;
      }

      if ( bde.avail_out() == 0 ) {
	// 出力バッファの中身を処理
	res = write(o_fd, wbuff, BUFF_SIZE);
	if ( res < 0 ) {
	  cerr << "error in write()" << endl;
	  return -1;
	}
	bde.set_outbuf(wbuff, BUFF_SIZE);
      }
    } while (bde.avail_in() > 0);
  } while ( ret != BZ_STREAM_END );
  ymuint64 wr = BUFF_SIZE - bde.avail_out();
  if ( wr > 0 ) {
    ssize_t res = write(o_fd, wbuff, wr);
    if ( res < 0 ) {
      cerr << "error in write()" << endl;
      return -1;
    }
  }

  close(i_fd);
  close(o_fd);

  return 0;
}

END_NAMESPACE_YM


int
main(int argc,
     char** argv)
{
  using namespace nsYm;

  if ( argc != 4 ) {
    cerr << "usage: " << argv[0] << " c|d input-file output-file" << endl;
    return 250;
  }
  if ( strlen(argv[1]) != 1 || argv[1][0] != 'c' && argv[1][0] != 'd' ) {
    cerr << "argv[1] should be \"c\" or \"d\"" << endl;
    return 251;
  }
  try {
    if ( argv[1][0] == 'c' ) {
      // compress mode
      return compress(argv[2], argv[3]);
    }
    else if ( argv[1][0] == 'd' ) {
      // decompress mode
      return decompress(argv[2], argv[3]);
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }
  catch ( ... ) {
    cerr << "unknown exception" << endl;
    return 254;
  }

  return 0;
}
