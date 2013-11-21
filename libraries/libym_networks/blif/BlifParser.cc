
/// @file BlifParser.cc
/// @brief BlibParser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BlifParser.h"
#include "IdCell.h"
#include "BlifHandler.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_utils/FileIDO.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

BEGIN_NONAMESPACE

// 文字列を 0/1 に変換する
// それ以外の文字なら -1 を返す．
int
str_to_01(const StrBuff& str)
{
  const char* strptr = str.c_str();
  if ( strptr[1] != '\0' ) {
    return -1;
  }
  if ( strptr[0] == '0' ) {
    return 0;
  }
  if ( strptr[0] == '1' ) {
    return 1;
  }
  return -1;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス BlifParser
//////////////////////////////////////////////////////////////////////


// @brief コンストラクタ
BlifParser::BlifParser()
{
  mScanner = NULL;
}

// @brief デストラクタ
BlifParser::~BlifParser()
{
  // 登録してあるハンドラを削除する．
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    delete handler;
  }
  delete mScanner;
}

// @brief 読み込みを行う．
bool
BlifParser::read(const string& filename,
		 const CellLibrary* cell_library)
{
  // ファイルをオープンする．
  FileIDO ido(filename);
  if ( !ido ) {
    // エラー
    ostringstream buf;
    buf << filename << " : No such file.";
    MsgMgr::put_msg(__FILE__, __LINE__, FileRegion(),
		    kMsgFailure, "BLIF_PARSER", buf.str());
    return false;
  }

  mCellLibrary = cell_library;

  // 初期化を行う．
  mScanner = new BlifScanner(ido);
  mIdHash.clear();
  mUngetToken = kTokenEOF;

  // 一つの .inputs/.outputs 文中のトークンの数
  ymuint n_token = 0;

  FileRegion error_loc;

  bool stat = true;
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    if ( !handler->init() ) {
      stat = false;
    }
  }
  if ( !stat ) {
    goto ST_ERROR_EXIT;
  }

  // ハードコーディングした状態遷移

 ST_INIT:
  // 読込開始
  {
    tToken tk= get_token(mLoc1);
    if ( tk == kTokenNL ) {
      // 読み飛ばす
      goto ST_INIT;
    }
    if ( tk == kTokenMODEL ) {
      // .model 文の開始
      goto ST_MODEL;
    }
    // それ以外はエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    mLoc1,
		    kMsgError,
		    "SYN01",
		    "No '.model' statement.");
    goto ST_ERROR_EXIT;
  }

 ST_MODEL:
  // .model 文の処理
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk != kTokenSTRING ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "SYN02",
		      "String expected after '.model'.");
      goto ST_ERROR_EXIT;
    }
    const char* name = mScanner->cur_string();
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->model(mLoc1, loc, name) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }

    if ( get_token(loc) != kTokenNL ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      loc,
		      kMsgError,
		      "SYN03",
		      "Newline expected.");
      goto ST_ERROR_EXIT;
    }

    goto ST_NEUTRAL;
  }

 ST_NEUTRAL:
  // 本体の処理
  {
    tToken tk= get_token(mLoc1);
    switch (tk) {
    case kTokenNL:
      goto ST_NEUTRAL;

    case kTokenEOF:
      goto ST_AFTER_EOF;

    case kTokenMODEL:
      MsgMgr::put_msg(__FILE__, __LINE__,
		      mLoc1,
		      kMsgError,
		      "SYN04",
		      "Multiple '.model' statements.");
      goto ST_ERROR_EXIT;

    case kTokenINPUTS:
      n_token = 0;
      goto ST_INPUTS;

    case kTokenOUTPUTS:
      n_token = 0;
      goto ST_OUTPUTS;

    case kTokenNAMES:
      mNameArray.clear();
      mNc = 0;
      mCoverPat.clear();
      mOpat = ' ';
      goto ST_NAMES;

    case kTokenGATE:
      if ( mCellLibrary == NULL ) {
	MsgMgr::put_msg(__FILE__, __LINE__,
			mLoc1,
			kMsgError,
			"NOCELL01",
			"No cell-library is specified.");
	goto ST_ERROR_EXIT;
      }
      goto ST_GATE;

    case kTokenLATCH:
      goto ST_LATCH;

    case kTokenEND:
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->end(mLoc1) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_AFTER_END;

    case kTokenEXDC:
      goto ST_EXDC;

    case kTokenWIRE_LOAD_SLOPE:
      goto ST_DUMMY_READ1;

    case kTokenWIRE:
      goto ST_DUMMY_READ1;

    case kTokenINPUT_ARRIVAL:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_INPUT_ARRIVAL:
      goto ST_DUMMY_READ1;

    case kTokenOUTPUT_REQUIRED:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_OUTPUT_REQUIRED:
      goto ST_DUMMY_READ1;

    case kTokenINPUT_DRIVE:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_INPUT_DRIVE:
      goto ST_DUMMY_READ1;

    case kTokenOUTPUT_LOAD:
      goto ST_DUMMY_READ1;

    case kTokenDEFAULT_OUTPUT_LOAD:
      goto ST_DUMMY_READ1;

    default:
      break;
    }
    error_loc = mLoc1;
    goto ST_SYNTAX_ERROR;
  }

 ST_INPUTS:
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenSTRING ) {
      const char* name = mScanner->cur_string();
      IdCell* cell = mIdHash.find(name, true);
      if ( cell->is_defined() ) {
	ostringstream buf;
	buf << name << ": Defined more than once. Previous definition is "
	    << cell->def_loc() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgError,
			"MLTDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      if ( cell->is_output() ) {
	ostringstream buf;
	buf << name << ": Defined as both input and output."
	    << " Previous difinition is " << cell->loc() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"MLTDEF02", buf.str().c_str());
      }
      cell->set_loc(loc);
      cell->set_defined();
      cell->set_input();
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->inputs_elem(cell->id()) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      ++ n_token;
      goto ST_INPUTS;
    }
    if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"SYN07", "Empty '.inputs' statement. Ignored.");
      }
      goto ST_NEUTRAL;
    }
    error_loc = loc;
    goto ST_SYNTAX_ERROR;
  }

 ST_OUTPUTS:
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenSTRING ) {
      const char* name = mScanner->cur_string();
      IdCell* cell = mIdHash.find(name, true);
      if ( cell->is_output() ) {
	ostringstream buf;
	buf << name << ": Defined more than once. Previous definition is "
	    << cell->loc();
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgError,
			"MLTDEF03", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      if ( cell->is_input() ) {
	ostringstream buf;
	buf << name << ": Defined as both input and output. "
	    << "Previous definition is "
	    << cell->loc() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"MLTDEF02", buf.str().c_str());
      }
      cell->set_loc(loc);
      cell->set_output();
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->outputs_elem(cell->id()) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      ++ n_token;
      goto ST_OUTPUTS;
    }
    if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgWarning,
			"SYN08", "Empty '.outputs' statement. Ignored.");
      }
      goto ST_NEUTRAL;
    }
    error_loc = loc;
    goto ST_SYNTAX_ERROR;
  }

 ST_NAMES:
  { // str* nl
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenSTRING ) {
      const char* name = mScanner->cur_string();
      IdCell* cell = mIdHash.find(name, true);
      cell->set_loc(loc);
      mNameArray.push_back(cell);
      goto ST_NAMES;
    }
    if ( tk == kTokenNL ) {
      ymuint n = mNameArray.size();
      if ( n == 0 ) {
	// 名前が1つもない場合
	MsgMgr::put_msg(__FILE__, __LINE__, loc,
			kMsgError,
			"SYN09",
			"Empty '.names' statement.");
	goto ST_ERROR_EXIT;
      }
      else if ( n == 1 ) {
	// 名前が1つしかない場合
	goto ST_NAMES0;
      }
      else {
	// 通常の場合
	goto ST_NAMES1;
      }
    }
    error_loc = loc;
    goto ST_SYNTAX_ERROR;
  }

 ST_NAMES0:
  { // str nl
    FileRegion loc1;
    tToken tk= get_token(loc1);
    if ( tk == kTokenSTRING ) {
      mName1 = mScanner->cur_string();
      char opat = mName1[0];
      if ( opat != '0' && opat != '1' ) {
	MsgMgr::put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"SYN15",
			"Illegal character in output cube.");
	goto ST_ERROR_EXIT;
      }
      if ( mOpat == ' ' ) {
	mOpat = opat;
      }
      else if ( mOpat != opat ) {
	MsgMgr::put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"SYN10",
			"Outpat pattern mismatch.");
	goto ST_ERROR_EXIT;
      }
      if ( get_token(loc1) == kTokenNL ) {
	++ mNc;
	goto ST_NAMES0;
      }
      MsgMgr::put_msg(__FILE__, __LINE__, loc1,
		      kMsgError,
		      "SYN14",
		      "Newline is expected.");
      goto ST_ERROR_EXIT;
    }
    else if ( tk == kTokenNL ) {
      // 空行はスキップ
      goto ST_NAMES0;
    }
    else {
      unget_token(tk, loc1);
      goto ST_NAMES_END;
    }
  }

 ST_NAMES1:
  { // str str nl
    FileRegion loc1;
    tToken tk= get_token(loc1);
    if ( tk == kTokenSTRING ) {
      mName1 = mScanner->cur_string();
      if ( mName1.size() != mNameArray.size() - 1 ) {
	MsgMgr::put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"SYN12",
			"Input pattern does not fit "
			"with the number of fanins.");
	goto ST_ERROR_EXIT;
      }
      ymuint n = mName1.size();
      mCoverPat.reserve(mCoverPat.size() + n);
      for (ymuint i = 0; i < n; ++ i) {
	char c = mName1[i];
	if ( c == '1' ) {
	  mCoverPat.put_char('1');
	}
	else if ( c == '0' ) {
	  mCoverPat.put_char('0');
	}
	else if ( c == '-' ) {
	  mCoverPat.put_char('-');
	}
	else {
	  MsgMgr::put_msg(__FILE__, __LINE__, loc1,
			  kMsgError,
			  "SYN11",
			  "Illegal character in input cube.");
	  goto ST_ERROR_EXIT;
	}
      }
      FileRegion loc2;
      tk = get_token(loc2);
      if ( tk == kTokenSTRING ) {
	char opat = mScanner->cur_string()[0];
	if ( opat != '0' && opat != '1' ) {
	  MsgMgr::put_msg(__FILE__, __LINE__, loc2,
			  kMsgError,
			  "SYN15",
			  "Illegal character in output cube.");
	  goto ST_ERROR_EXIT;
	}
	if ( mOpat == ' ' ) {
	  mOpat = opat;
	}
	else if ( mOpat != opat ) {
	  MsgMgr::put_msg(__FILE__, __LINE__, loc2,
			  kMsgError, "SYN10",
			  "Outpat pattern mismatch.");
	  goto ST_ERROR_EXIT;
	}
	if ( get_token(loc2) != kTokenNL ) {
	  MsgMgr::put_msg(__FILE__, __LINE__, loc2,
			  kMsgError, "SYN14",
			  "Newline is expected.");
	  goto ST_ERROR_EXIT;
	}
	++ mNc;
	goto ST_NAMES1;
      }
      MsgMgr::put_msg(__FILE__, __LINE__, loc1,
		      kMsgError, "SYN13",
		      "No output cube.");
      goto ST_ERROR_EXIT;
    }
    else if ( tk == kTokenNL ) {
      // 空行はスキップ
      goto ST_NAMES1;
    }
    else {
      unget_token(tk, loc1);
      goto ST_NAMES_END;
    }
  }

 ST_NAMES_END:
  {
    ymuint n = mNameArray.size();
    ymuint ni = n - 1;
    IdCell* cell = mNameArray[ni];
    if ( cell->is_defined() ) {
      // 二重定義
      ostringstream buf;
      buf << cell->str() << ": Defined more than once. "
	  << "Previsous Definition is " << cell->def_loc() << ".";
      MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
		      kMsgError,
		      "MLTDEF01", buf.str());
      goto ST_ERROR_EXIT;
    }
    cell->set_defined();
    ymuint oid = cell->id();
    mIdArray.clear();
    for (ymuint i = 0; i < ni ; ++ i) {
      mIdArray.push_back(mNameArray[i]->id());
    }
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->names(oid, mIdArray, mNc, mCoverPat.c_str(), mOpat) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }
    goto ST_NEUTRAL;
  }

 ST_GATE:
  { // str -> ST_GATE1
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk != kTokenSTRING ) {
      error_loc = loc;
      goto ST_GATE_SYNERROR;
    }
    const char* name = mScanner->cur_string();
    mCell = mCellLibrary->cell(name);
    if ( mCell == NULL ) {
      ostringstream buf;
      buf << name << ": No such cell.";
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgError,
		      "NOCELL02", buf.str());
      goto ST_ERROR_EXIT;
    }
    if ( !mCell->is_logic() ) {
      ostringstream buf;
      buf << name << " : Not a logic cell.";
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgError, "BNetBlifReader", buf.str());
      return false;
    }
    if ( mCell->output_num() != 1 ) {
      ostringstream buf;
      buf << name << " : Not a single output cell.";
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgError, "BNetBlifReader", buf.str());
      return false;
    }
    if ( mCell->has_tristate(0) ) {
      ostringstream buf;
      buf << name << " : Is a tri-state cell.";
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgError, "BNetBlifReader", buf.str());
      return false;
    }
    if ( mCell->inout_num() > 0 ) {
      ostringstream buf;
      buf << name << " : Has inout pins.";
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgError, "BNetBlifReader", buf.str());
      return false;
    }
    mNameArray.clear();
    mNameArray.resize(mCell->pin_num(), NULL);
    n_token = 0;
    goto ST_GATE1;
  }

 ST_GATE1:
  { // (str '=' str)* nl
    FileRegion loc1;
    tToken tk= get_token(loc1);
    if ( tk == kTokenSTRING ) {
      mName1 = mScanner->cur_string();
      const char* name1 = mName1.c_str();
      const CellPin* pin = mCell->pin(name1);
      if ( pin == NULL ) {
	ostringstream buf;
	buf << name1 << ": No such pin.";
	MsgMgr::put_msg(__FILE__, __LINE__, loc1,
			kMsgError,
			"NOPIN01", buf.str());
	goto ST_ERROR_EXIT;
      }
      FileRegion loc2;
      tk = get_token(loc2);
      if ( tk != kTokenEQ ) {
	error_loc = loc2;
	goto ST_GATE_SYNERROR;
      }
      tk = get_token(loc2);
      if ( tk != kTokenSTRING ) {
	error_loc = loc2;
	goto ST_GATE_SYNERROR;
      }
      const char* name2 = mScanner->cur_string();
      IdCell* cell = mIdHash.find(name2, true);
      cell->set_loc(loc2);

      if ( pin->is_output() ) {
	if ( cell->is_defined() ) {
	  // 二重定義
	  ostringstream buf;
	  buf << cell->str() << ": Defined more than once. "
	      << "Previous definition is " << cell->def_loc() << ".";
	  MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
			  kMsgError,
			  "MLTDEF01", buf.str());
	  goto ST_ERROR_EXIT;
	}
	cell->set_defined();
      }
      if ( mNameArray[pin->pin_id()] != NULL ) {
	ostringstream buf;
	buf << name2 << ": Appears more than once.";
	MsgMgr::put_msg(__FILE__, __LINE__, loc2,
			kMsgError,
			"MLTDEF02", buf.str());
	goto ST_ERROR_EXIT;
      }
      mNameArray[pin->pin_id()] = cell;
      ++ n_token;
      goto ST_GATE1;
    }
    else if ( tk == kTokenNL ) {
      if ( n_token == 0 ) {
	error_loc = loc1;
	goto ST_GATE_SYNERROR;
      }
      const CellPin* opin = mCell->output(0);
      ymuint onode_id = mNameArray[opin->pin_id()]->id();
      ymuint ni = mCell->input_num();
      mIdArray.clear();
      for (ymuint i = 0; i < ni; ++ i) {
	const CellPin* ipin = mCell->input(i);
	ymuint inode_id = mNameArray[ipin->pin_id()]->id();
	mIdArray.push_back(inode_id);
      }
      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->gate(onode_id, mIdArray, mCell) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_NEUTRAL;
    }
  }

 ST_GATE_SYNERROR:
  MsgMgr::put_msg(__FILE__, __LINE__, error_loc,
		  kMsgError,
		  "SYN16", "Syntax error in '.gate' statement.");
  goto ST_ERROR_EXIT;

 ST_LATCH:
  {
    FileRegion loc2;
    tToken tk= get_token(loc2);
    if ( tk == kTokenSTRING ) {
      const char* name1 = mScanner->cur_string();
      IdCell* cell1 = mIdHash.find(name1, true);
      cell1->set_loc(loc2);

      FileRegion loc3;
      tk = get_token(loc3);
      if ( tk != kTokenSTRING ) {
	error_loc = loc3;
	goto ST_LATCH_SYNERROR;
      }
      const char* name2 = mScanner->cur_string();
      IdCell* cell2 = mIdHash.find(name2, true);
      cell2->set_loc(loc3);

      if ( cell2->is_defined() ) {
	// 二重定義
	ostringstream buf;
	buf << cell2->str() << ": Defined more than once. "
	    << "Previsous Definition is " << cell2->def_loc() << ".";
	MsgMgr::put_msg(__FILE__, __LINE__, cell2->loc(),
			kMsgError,
			"MLTDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
      cell2->set_defined();

      FileRegion loc4;
      tk = get_token(loc4);
      char rval = ' ';
      if ( tk == kTokenSTRING ) {
	rval = mScanner->cur_string()[0];
	if ( rval != '0' && rval != '1' ) {
	  MsgMgr::put_msg(__FILE__, __LINE__, loc4,
			  kMsgError,
			  "SYN18",
			  "Illegal character for reset value.");
	  goto ST_ERROR_EXIT;
	}
	tk = get_token(loc4);
      }
      if ( tk != kTokenNL ) {
	error_loc = loc4;
	goto ST_LATCH_SYNERROR;
      }

      for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	   p != mHandlerList.end(); ++ p) {
	BlifHandler* handler = *p;
	if ( !handler->latch(cell2->id(), cell1->id(),
			     loc4, rval) ) {
	  stat = false;
	}
      }
      if ( !stat ) {
	goto ST_ERROR_EXIT;
      }
      goto ST_NEUTRAL;
    }
    else {
      error_loc = loc2;
      goto ST_LATCH_SYNERROR;
    }
  }

 ST_LATCH_SYNERROR:
  MsgMgr::put_msg(__FILE__, __LINE__, error_loc,
		  kMsgError,
		  "SYN17", "Syntax error in '.latch' statement.");
  goto ST_ERROR_EXIT;

 ST_AFTER_END:
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenEOF ) {
      goto ST_NORMAL_EXIT;
    }
    if ( tk != kTokenNL ) {
      MsgMgr::put_msg(__FILE__, __LINE__, loc,
		      kMsgWarning,
		      "SYN06",
		      "Statement after '.end' is ignored.");
    }
    goto ST_AFTER_END;
  }

 ST_EXDC:
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenEND ) {
      goto ST_NEUTRAL;
    }
    goto ST_EXDC;
  }

 ST_DUMMY_READ1:
  {
    FileRegion loc;
    tToken tk= get_token(loc);
    if ( tk == kTokenNL ) {
      goto ST_NEUTRAL;
    }
    goto ST_DUMMY_READ1;
  }

 ST_AFTER_EOF:
  {
    MsgMgr::put_msg(__FILE__, __LINE__, mLoc1,
		    kMsgWarning,
		    "SYN05",
		    "unexpected EOF. '.end' is assumed.");
    for (list<BlifHandler*>::iterator p = mHandlerList.begin();
	 p != mHandlerList.end(); ++ p) {
      BlifHandler* handler = *p;
      if ( !handler->end(mLoc1) ) {
	stat = false;
      }
    }
    if ( !stat ) {
      goto ST_ERROR_EXIT;
    }
    goto ST_NORMAL_EXIT;
  }

 ST_NORMAL_EXIT:
  {
    ymuint n = mIdHash.num();
    for (ymuint i = 0; i < n; ++ i) {
      IdCell* cell = mIdHash.cell(i);
      if ( !cell->is_defined() ) {
	ostringstream buf;
	buf << cell->str() << ": Undefined.";
	MsgMgr::put_msg(__FILE__, __LINE__, cell->loc(),
			kMsgError,
			"UNDEF01", buf.str().c_str());
	goto ST_ERROR_EXIT;
      }
    }
  }
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    handler->normal_exit();
  }
  delete mScanner;
  mScanner = NULL;

  return true;

 ST_SYNTAX_ERROR:
  MsgMgr::put_msg(__FILE__, __LINE__, error_loc,
		  kMsgError,
		  "SYN00",
		  "Syntax error.");
  // わざと次の行につづく
  // goto ST_ERROR_EXIT;

 ST_ERROR_EXIT:
  for (list<BlifHandler*>::iterator p = mHandlerList.begin();
       p != mHandlerList.end(); ++ p) {
    BlifHandler* handler = *p;
    handler->error_exit();
  }
  delete mScanner;
  mScanner = NULL;

  return false;
}

// @brief イベントハンドラの登録
void
BlifParser::add_handler(BlifHandler* handler)
{
  mHandlerList.push_back(handler);
  handler->mParser = this;
}

// @brief トークンを一つ読み出す．
// @param[out] loc トークンの位置を格納する変数
tToken
BlifParser::get_token(FileRegion& loc)
{
  if ( mUngetToken == kTokenEOF ) {
    return mScanner->read_token(loc);
  }
  // トークンバッファに値がある場合にはそれを返す．
  tToken tk = mUngetToken;
  mUngetToken = kTokenEOF;
  loc = mUngetTokenLoc;
  return tk;
}

END_NAMESPACE_YM_NETWORKS_BLIF
