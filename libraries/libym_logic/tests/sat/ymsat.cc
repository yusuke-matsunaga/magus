
/// @file libym_sat/tests/ymsat.cc
/// @brief 簡単な SAT プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ymsat.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/DimacsParser.h"
#include "ym_logic/SatSolver.h"
#include "ym_logic/SatMsgHandlerImpl1.h"
#include "ym_logic/SatStats.h"

#include "ym_utils/StopWatch.h"


#if USE_ZSTREAM
#include "ym_utils/zstream.h"
#endif


BEGIN_NAMESPACE_YM
BEGIN_NONAMESPACE

//////////////////////////////////////////////////////////////////////
/// @class SatDimacsHandler ymsat.cc
/// @brief ymsat 用の DimacsHandler
//////////////////////////////////////////////////////////////////////
class SatDimacsHandler :
  public DimacsHandler
{
public:

  /// @brief コンストラクタ
  SatDimacsHandler(SatSolver& solver);

  /// @brief デストラクタ
  virtual
  ~SatDimacsHandler();


public:

  /// @brief 初期化
  virtual
  bool
  init();

  /// @brief p 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] nv 変数の数
  /// @param[in] nc 節の数
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_p(const FileRegion& loc,
	 ymuint nv,
	 ymuint nc);

  /// @brief clause 行の読込み
  /// @param[in] loc ファイル上の位置情報
  /// @param[in] lits リテラルの配列．最後の0は含まない
  /// @retval true 処理が成功した．
  /// @retval false エラーが起こった．
  virtual
  bool
  read_clause(const FileRegion& loc,
	      const vector<int>& lits);

  /// @brief 終了処理
  virtual
  bool
  end();

  /// @brief エラー終了時の処理
  virtual
  void
  error_exit();

  /// @brief 検証する．
  bool
  verify(const vector<Bool3>& model) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  SatSolver& mSolver;

  vector<vector<int> > clause_list;

};

#if 0
//////////////////////////////////////////////////////////////////////
// ymsat 用の DimacsMsgHandler
//////////////////////////////////////////////////////////////////////
class SatDimacsMsgHandler :
  public DimacsMsgHandler
{
public:

  /// @brief コンストラクタ
  SatDimacsMsgHandler();

  /// @brief デストラクタ
  virtual
  ~SatDimacsMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスは下記の仮想関数を上書きすること
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] lineno ファイル中の行番号
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  virtual
  void
  operator()(const char* src_file,
	     int src_line,
	     int lineno,
	     const string& label,
	     const string& body);
};
#endif


// @brief コンストラクタ
SatDimacsHandler::SatDimacsHandler(SatSolver& solver) :
  mSolver(solver)
{
}

// @brief デストラクタ
SatDimacsHandler::~SatDimacsHandler()
{
}

// @brief 初期化
bool
SatDimacsHandler::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_p(const FileRegion& loc,
			 ymuint nv,
			 ymuint nc)
{
  for (ymuint i = 0; i < nv; ++ i) {
    mSolver.new_var();
  }

  return true;
}

// @brief clause 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_clause(const FileRegion& loc,
			      const vector<int>& lits)
{
  vector<Literal> tmp;
  for (vector<int>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    int i = *p;
    VarId vid;
    tPol pol;
    if ( i > 0 ) {
      vid = VarId(i - 1);
      pol = kPolPosi;
    }
    else {
      vid = VarId(- i - 1);
      pol = kPolNega;
    }
    tmp.push_back(Literal(vid, pol));
  }
  mSolver.add_clause(tmp);

  clause_list.push_back(lits);

  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
SatDimacsHandler::end()
{
  return true;
}

// @brief エラー終了時の処理
void
SatDimacsHandler::error_exit()
{
}

// @brief 検証する．
bool
SatDimacsHandler::verify(const vector<Bool3>& model) const
{
  for (vector<vector<int> >::const_iterator p = clause_list.begin();
       p != clause_list.end(); ++ p) {
    const vector<int>& lits = *p;
    bool sat = false;
    for (vector<int>::const_iterator q = lits.begin();
	 q != lits.end(); ++ q) {
      int l = *q;
      if ( l > 0 ) {
	if ( model[l - 1] == kB3True ) {
	  sat = true;
	  break;
	}
      }
      else {
	if ( model[-l - 1] == kB3False ) {
	  sat = true;
	  break;
	}
      }
    }
    if ( !sat ) {
      const char* plus = "";
      for (vector<int>::const_iterator q = lits.begin();
	   q != lits.end(); ++ q) {
	int l = *q;
	cout << plus << l;
	plus = " + ";
      }
      cout << " is unsatisfied" << endl;
      return false;
    }
  }
  return true;
}


#if 0
// @brief コンストラクタ
SatDimacsMsgHandler::SatDimacsMsgHandler()
{
}

// @brief デストラクタ
SatDimacsMsgHandler::~SatDimacsMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] lineno ファイル中の行番号
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
SatDimacsMsgHandler::operator()(const char* src_file,
				int src_line,
				int lineno,
				const string& label,
				const string& body)
{
  cerr << "Line " << lineno << ": [" << label << "]: " << body << endl;
}
#endif

END_NONAMESPACE
END_NAMESPACE_YM

int
main(int argc,
     char** argv)
{
  using namespace std;
  using namespace nsYm;

  if ( argc != 2 ) {
    cerr << "USAGE : " << argv[0] << " cnf-file" << endl;
    return 2;
  }

#if 0
  ifstream s(argv[1]);
  if ( !s.is_open() ) {
    cerr << "Could not open " << argv[1] << endl;
    return 3;
  }
#endif
  try {
#if 0
#if USE_ZSTREAM
    izstream zs(s);
    istream& s1 = zs;
#else
    istream& s1 = s;
#endif
#endif
    SatSolver solver;

    DimacsParser parser;
    SatDimacsHandler handler(solver);
#if 0
    SatDimacsMsgHandler msg_handler;
#endif
    parser.add_handler(&handler);
#if 0
    parser.add_msg_handler(&msg_handler);
#endif

    if ( !parser.read(argv[1]) ) {
      cerr << "Error in reading " << argv[1] << endl;
      return 4;
    }

    SatMsgHandlerImpl1 satmsghandler(cout);
    solver.reg_msg_handler(&satmsghandler);
    solver.timer_on(true);

    StopWatch sw;
    sw.start();
    vector<Bool3> model;
    Bool3 ans = solver.solve(model);
    sw.stop();

    if ( ans == kB3True ) {
      handler.verify(model);
    }

    switch ( ans ) {
    case kB3True:  cout << "SAT" << endl; break;
    case kB3False: cout << "UNSAT" << endl; break;
    case kB3X:     cout << "UNKNOWN" << endl; break;
    }

    USTime ust = sw.time();
    cout << ust << endl;
  }
  catch ( AssertError x) {
    cout << x << endl;
  }

#if USE_ZSTREAM
  catch ( zlib_error x ) {
    cout << x.mMsg << endl;
  }
#endif

  return 0;
}
