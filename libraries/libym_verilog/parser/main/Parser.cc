
/// @file libym_verilog/parser/Parser.cc
/// @brief Parser の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Parser.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Parser.h"
#include "Parser_item.h"

#include "PtiFactory.h"
#include "PuHierName.h"
#include "Lex.h"
#include "PtMgr.h"

#include "ym_verilog/pt/PtStmt.h"
#include "ym_verilog/pt/PtItem.h"


const int debug = 0;
#define dout cout


BEGIN_NAMESPACE_YM_VERILOG

#include "verilog_grammer.h"

// メモリリークのチェックを行うとき 1 にする．
const int check_memory_leak = 0;

//////////////////////////////////////////////////////////////////////
// Verilog-HDL のパーサークラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] msg_mgr メッセージマネージャ
// @param[in] ptmgr 読んだ結果のパース木を登録するマネージャ
// @param[in] alloc メモリアロケータ
// @param[in] ptifactory パース木の要素を生成するファクトリクラス
Parser::Parser(MsgMgr& msg_mgr,
	       PtMgr& ptmgr,
	       AllocBase& alloc,
	       PtiFactory& ptifactory) :
  mMsgMgr(msg_mgr),
  mPtMgr(ptmgr),
  mAlloc(alloc),
  mFactory(ptifactory),
  mLex(new Lex(msg_mgr, mPtMgr.mFdMgr)),
  mTmpAlloc(4096),
  mCellAlloc(sizeof(PtrList<void>::Cell), 1024),
  mPortList(mCellAlloc),
  mPortRefList(mCellAlloc),
  mParamPortHeadList(mCellAlloc),
  mModuleIOHeadList(mCellAlloc),
  mTfIOHeadList(mCellAlloc),
  mModuleParamHeadList(mCellAlloc),
  mTfParamHeadList(mCellAlloc),
  mModuleLparamHeadList(mCellAlloc),
  mTfLparamHeadList(mCellAlloc),
  mModuleDeclHeadList(mCellAlloc),
  mTfDeclHeadList(mCellAlloc),
  mModuleItemList(mCellAlloc),
  mIOItemList(mCellAlloc),
  mDeclItemList(mCellAlloc),
  mUdpEntryList(mCellAlloc),
  mUdpValueList(mCellAlloc),
  mDefParamList(mCellAlloc),
  mContAssignList(mCellAlloc),
  mInstList(mCellAlloc)
{
}

// @brief デストラクタ
Parser::~Parser()
{
  delete mLex;
}

// @brief ファイルを読み込む．
// @param[in] filename 読み込むファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウオッチャーのリスト
// @retval true 正常に終了した．
// @retval false エラーが起こった．
bool
Parser::read_file(const string& filename,
		  const SearchPathList& searchpath,
		  const list<VlLineWatcher*>& watcher_list)
{
  int yyparse(Parser&);

  lex().set_searchpath(searchpath);

  for (list<VlLineWatcher*>::const_iterator p = watcher_list.begin();
       p != watcher_list.end(); ++ p) {
    lex().reg_watcher(*p);
  }

  if ( !lex().open_file(filename) ) {
    ostringstream buf;
    buf << filename << " : No such file.";
    put_msg(__FILE__, __LINE__, FileRegion(),
	    kMsgFailure,
	    "VLPARSER",
	    buf.str());
    return false;
  }

  int stat = yyparse(*this);

  if ( check_memory_leak ) {
    dout << endl
	 << "Parser::mAlloc.allocated_size() = "
	 << mTmpAlloc.allocated_size() << endl
	 << "                     used_size()      = "
	 << mTmpAlloc.used_size() << endl
	 << "                     max_used_size()  = "
	 << mTmpAlloc.max_used_size() << endl;
  }
  return (stat == 0);
}

// @brief yylex とのインターフェイス
// @param[out] lvalp 値を格納する変数
// @param[out] llocp 位置情報を格納する変数
// @return 読み込んだトークンの id を返す．
int
Parser::yylex(YYSTYPE& lval,
	      FileRegion& lloc)
{
  int id = lex().get_token();

  switch ( id ) {
  case IDENTIFIER:
  case SYS_IDENTIFIER:
  case STRING:
  case UNUMBER:
  case UNUM_BIG:
    lval.strtype = mFactory.new_string(lex().cur_string());
    break;

  case UNUM_INT:
    lval.uinttype = lex().cur_uint();
    break;

  case RNUMBER:
    lval.dbltype = lex().cur_rnumber();
    break;

  default:
    break;
  }
  lloc = lex().cur_token_loc();
  return id;
}

// @brief 使用されているモジュール名を登録する．
// @param[in] name 登録する名前
void
Parser::reg_defname(const char* name)
{
  mPtMgr.reg_defname(name);
}

// @brief attribute instance を登録する．
void
Parser::reg_attrinst(PtBase* ptobj,
		     PtrList<PtAttrInst>* attr_list)
{
  if ( attr_list ) {
    // 未実装
  }
}

// 関数内で使えるステートメントかどうかのチェック
bool
Parser::check_function_statement(const PtStmt* stmt)
{
  switch ( stmt->type() ) {
  case kPtDisableStmt:
  case kPtSysEnableStmt:
  case kPtNullStmt:
    return true;

  case kPtAssignStmt:
    if ( stmt->control() == NULL) return true;
    break;

  case kPtIfStmt:
    if ( stmt->body() ) {
      if ( !check_function_statement(stmt->body()) ) {
	return false;
      }
    }
    if ( stmt->else_body() ) {
      if ( !check_function_statement(stmt->else_body()) ) {
	return false;
      }
    }
    return true;

  case kPtCaseStmt:
  case kPtCaseXStmt:
  case kPtCaseZStmt:
    for (ymuint i = 0; i < stmt->caseitem_num(); ++ i) {
      const PtCaseItem* item = stmt->caseitem(i);
      if ( !check_function_statement(item->body()) ) {
	return false;
      }
    }
    return true;

  case kPtForeverStmt:
  case kPtRepeatStmt:
  case kPtWhileStmt:
  case kPtForStmt:
    if ( stmt->init_stmt() ) {
      if ( !check_function_statement(stmt->init_stmt()) ) {
	return false;
      }
    }
    if ( stmt->next_stmt() ) {
      if ( !check_function_statement(stmt->next_stmt()) ) {
	return false;
      }
    }
    return check_function_statement(stmt->body());

  case kPtSeqBlockStmt:
  case kPtNamedSeqBlockStmt:
    for (ymuint i = 0; i < stmt->stmt_array().size(); ++ i) {
      const PtStmt* stmt1 = stmt->stmt_array()[i];
      if ( !check_function_statement(stmt1) ) {
	return false;
      }
    }
    return true;

  default:
    break;
  }
  ostringstream buf;
  buf << stmt->stmt_name()
      << " cannot be used in function declaration.";
  put_msg(__FILE__, __LINE__,
	  stmt->file_region(),
	  kMsgError,
	  "PARS",
	  buf.str());
  return false;
}

// default ラベルが2つ以上含まれていないかどうかのチェック
bool
Parser::check_default_label(const PtrList<PtCaseItem>* ci_list)
{
  ymuint n = 0;
  for (PtrList<PtCaseItem>::const_iterator p = ci_list->begin();
       p.is_valid(); ++ p) {
    const PtCaseItem* ci = *p;
    if ( ci->label_num() == 0 ) {
      ++ n;
      if ( n > 1 ) {
	put_msg(__FILE__, __LINE__,
		ci->file_region(),
		kMsgError,
		"PARS",
		" more than one 'default' labels.");
	return false;
      }
    }
  }
  return true;
}

// @brief 式のリストから配列を生成する．(multi_concat用)
// @param[in] pre_expr list の前に挿入する式
// @note 結果として list は削除される．
PtExprArray
Parser::ExprArray(PtExpr* pre_expr,
		  PtrList<PtExpr>* list)
{
  ymuint n = list->size();
  void* p = mAlloc.get_memory(sizeof(PtExpr*) * (n + 1));
  PtExpr** array = new (p) PtExpr*[n + 1];
  array[0] = pre_expr;
  ymuint i = 1;
  for (PtrList<PtExpr>::const_iterator p = list->begin();
       p.is_valid(); ++ p, ++ i) {
    array[i] = *p;
  }

  list->~PtrList<PtExpr>();
  mTmpAlloc.put_memory(sizeof(PtrList<PtExpr>), list);

  return PtExprArray(n + 1, array);
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] name 階層の最下位部分
PuHierName*
Parser::new_HierName(const char* head_name,
		     const char* name)
{
  PtNameBranch* nb = mFactory.new_NameBranch(head_name);
  void* p = mTmpAlloc.get_memory(sizeof(PuHierName));
  return new (p) PuHierName(mCellAlloc, nb, name);
}

// @brief 階層名の生成
// @param[in] head_name 階層の上位部分
// @param[in] index インデックス
// @param[in] name 階層の最下位部分
PuHierName*
Parser::new_HierName(const char* head_name,
		     int index,
		     const char* name)
{
  PtNameBranch* nb = mFactory.new_NameBranch(head_name, index);
  void* p = mTmpAlloc.get_memory(sizeof(PuHierName));
  return new (p) PuHierName(mCellAlloc, nb, name);
}

// @brief 階層名の追加
// @aram[in] hname 階層名の上位部分
// @param[in] name 追加する名前
void
Parser::add_HierName(PuHierName* hname,
		     const char* name)
{
  PtNameBranch* nb = mFactory.new_NameBranch(hname->tail_name());
  hname->add(nb, name);
}

// @brief 階層名の追加
// @aram[in] hname 階層名の上位部分
// @param[in] index インデックス
// @param[in] name 追加する名前
void
Parser::add_HierName(PuHierName* hname,
		     int index,
		     const char* name)
{
  PtNameBranch* nb = mFactory.new_NameBranch(hname->tail_name(), index);
  hname->add(nb, name);
}

// @brief 階層名の取得
// @param[in] hname 階層名
// @param[out] 階層の上位部分の配列
// @return 階層の下位名
// @note この関数のなかで hname は削除される．
const char*
Parser::extract_HierName(PuHierName* hname,
			 PtNameBranchArray& nb_array)
{
  const char* ans = hname->mTailName;
  nb_array = hname->mNbList.to_array(mAlloc);

  hname->~PuHierName();
  mTmpAlloc.put_memory(sizeof(PuHierName), hname);
  if ( debug ) {
    dout << "delete_HierName (" << sizeof(PuHierName) << ")" << endl;
    dout << "  used_size() = " << mTmpAlloc.used_size() << endl << endl;
  }

  return ans;
}


//////////////////////////////////////////////////////////////////////
// generate 文の生成
//////////////////////////////////////////////////////////////////////

// @brief generate 文の生成
// @param[in] fr ファイル位置の情報
void
Parser::new_Generate(const FileRegion& fr,
		     PtrList<PtAttrInst>* ai_list)
{
  PtItem* item = mFactory.new_Generate(fr, get_decl_array(), get_item_array());
  reg_attrinst(item, ai_list);
  end_generate();
  add_item(item);
}

// @brief generate block 文の生成
// @param[in] fr ファイル位置の情報
void
Parser::new_GenBlock(const FileRegion& fr)
{
  PtItem* item = mFactory.new_GenBlock(fr, get_decl_array(), get_item_array());
  end_generate();
  add_item(item);
}

// @brief 名前付き generate block 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] name 名前
void
Parser::new_GenBlock(const FileRegion& fr,
		     const char* name)
{
  PtItem* item = mFactory.new_GenBlock(fr, name,
				       get_decl_array(), get_item_array());
  end_generate();
  add_item(item);
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
void
Parser::new_GenIf(const FileRegion& fr,
		  PtExpr* cond)
{
  PtItem* item = mFactory.new_GenIf(fr, cond,
				    get_then_decl_array(),
				    get_then_item_array(),
				    PtDeclHeadArray(),
				    PtItemArray());
  end_genif();
  add_item(item);
}

// @brief generate if 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] cond 条件を表す式
void
Parser::new_GenIfElse(const FileRegion& fr,
		      PtExpr* cond)
{
  PtItem* item = mFactory.new_GenIf(fr, cond,
				    get_then_decl_array(),
				    get_then_item_array(),
				    get_else_decl_array(),
				    get_else_item_array());
  end_genif();
  add_item(item);
}

// @brief generate-if 文の then 節の宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_then_decl_array()
{
  flush_declitem_list(*mCurDeclHeadList);
  return mCurThenDeclHeadList->to_array(mAlloc);
}

// @brief generate-if 文の else 節の宣言リストを配列に変換する．
inline
PtDeclHeadArray
Parser::get_else_decl_array()
{
  flush_declitem_list(*mCurDeclHeadList);
  return mCurElseDeclHeadList->to_array(mAlloc);
}

// @brief generate-if 文の then 節の item リストを配列に変換する．
inline
PtItemArray
Parser::get_then_item_array()
{
  return mCurThenItemList->to_array(mAlloc);
}

// @brief generate-if 文の else 節の item リストを配列に変換する．
inline
PtItemArray
Parser::get_else_item_array()
{
  return mCurElseItemList->to_array(mAlloc);
}

// @brief generate case 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] expr 選択式
// @param[in] item_list generate case item のリスト
void
Parser::new_GenCase(const FileRegion& fr,
		    PtExpr* expr,
		    PtrList<PtGenCaseItem>* item_list)
{
  add_item( mFactory.new_GenCase(fr, expr, to_array(item_list)) );
}

// @brief generate case の要素の生成
// @param[in] fr ファイル位置の情報
// @param[in] label_list 比較式のリスト
// @return 生成された generate case item
PtGenCaseItem*
Parser::new_GenCaseItem(const FileRegion& fr,
			PtrList<PtExpr>* label_list)
{
  PtGenCaseItem* item = mFactory.new_GenCaseItem(fr,
						 to_array(label_list),
						 get_decl_array(),
						 get_item_array());
  end_generate();
  return item;
}

// @brief generate for 文の生成
// @param[in] fr ファイル位置の情報
// @param[in] loop_var ループ変数
// @param[in] init_expr 初期化式
// @param[in] cond ループ条件式
// @param[in] inc_var 増加式の左辺の変数
// @param[in] inc_expr 増加式
// @param[in] block_name ブロック名
// @param[in] decl_array 宣言のリスト
// @param[in] item_array 要素のリスト
void
Parser::new_GenFor(const FileRegion& fr,
		   const char* loop_var,
		   PtExpr* init_expr,
		   PtExpr* cond,
		   const char* inc_var,
		   PtExpr* inc_expr,
		   const char* block_name)
{
  if ( strcmp(loop_var, inc_var) != 0 ) {
    end_generate();
    ostringstream buf;
    buf << "Lhs of the increment statement ("
	<< inc_var
	<< ") does not match with Lhs of the initial statement ("
	<< loop_var
	<< ")";
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "PARSER",
	    buf.str());
  }

  PtItem* item = mFactory.new_GenFor(fr, loop_var,
				     init_expr, cond, inc_expr,
				     block_name,
				     get_decl_array(),
				     get_item_array());
  add_item(item);
  end_generate();
}

// @brief モジュール定義の開始
// - port list の初期化
// - paramport list の初期化
// - iohead list の初期化
// - paramhead list の初期化
// - localparamhead list の初期化
// - declhead list の初期化
// - item list の初期化
// を行う．
void
Parser::init_module()
{
  mPortList.clear();
  mParamPortHeadList.clear();

  mCurIOHeadList = &mModuleIOHeadList;
  mCurParamHeadList = &mModuleParamHeadList;
  mCurLparamHeadList = &mModuleLparamHeadList;
  mCurDeclHeadList = &mModuleDeclHeadList;
  mCurItemList = &mModuleItemList;

  mCurIOHeadList->clear();
  mIOItemList.clear();

  mCurParamHeadList->clear();

  mCurLparamHeadList->clear();

  mCurDeclHeadList->clear();
  mDeclItemList.clear();

  mCurItemList->clear();
}

// @brief モジュール定義の終了
void
Parser::end_module()
{
  // 今のところやる事はない．
}

// @brief UDP定義の開始
// - port list の初期化
// - iohead list の初期化
// - declhead list の初期化
// - UDP entry list の初期化
// を行う．
void
Parser::init_udp()
{
  mPortList.clear();
  mCurIOHeadList = &mModuleIOHeadList;
  mCurDeclHeadList = &mModuleDeclHeadList;


  mCurIOHeadList->clear();
  mIOItemList.clear();

  mCurDeclHeadList->clear();
  mDeclItemList.clear();

  mUdpEntryList.clear();
}

// @brief UDP 定義の終了
void
Parser::end_udp()
{
  // 今のところやる事はない．
}

// @brief IO宣言の終わり
void
Parser::end_io()
{
  if ( !mIOItemList.empty() ) {
    assert_cond( !mCurIOHeadList->empty(), __FILE__, __LINE__);
    PtiIOHead* last = mCurIOHeadList->back();
    last->set_elem(mIOItemList.to_array(mAlloc));
  }
}

// @brief parameter port 宣言の終わり
void
Parser::end_paramport()
{
  flush_declitem_list(mParamPortHeadList);
}

// @param parameter 宣言の終わり
void
Parser::end_param()
{
  flush_declitem_list(*mCurParamHeadList);
}

// @param localparam 宣言の終わり
void
Parser::end_localparam()
{
  flush_declitem_list(*mCurLparamHeadList);
}

// @param 宣言の終わり
void
Parser::end_decl()
{
  flush_declitem_list(*mCurDeclHeadList);
}

// @brief 宣言要素リストを最後の宣言ヘッダにセットする．
void
Parser::flush_declitem_list(PtrList<PtiDeclHead, PtDeclHead>& head_list)
{
  if ( !mDeclItemList.empty() ) {
    assert_cond( !head_list.empty(), __FILE__, __LINE__);
    PtiDeclHead* last = head_list.back();
    last->set_elem(mDeclItemList.to_array(mAlloc));
  }
}

// @brief task/function 定義の開始
// - iohead list の初期化
// - paramhead list の初期化
// - localparamhead list の初期化
// - declhead list の初期化
// を行う．
void
Parser::init_tf()
{
  push_iohead_list(&mTfIOHeadList);
  push_paramhead_list(&mTfParamHeadList, &mTfLparamHeadList);
  push_declhead_list(&mTfDeclHeadList);

  mCurIOHeadList->clear();
  mIOItemList.clear();

  mCurParamHeadList->clear();
  mCurLparamHeadList->clear();

  mCurDeclHeadList->clear();
  mDeclItemList.clear();
}

// @brief task/function 定義の終了
void
Parser::end_tf()
{
  pop_iohead_list();
  pop_paramhead_list();
  pop_declhead_list(false);
}

// @brief generate block の開始
void
Parser::init_generate()
{
  push_declhead_list(NULL);
  push_item_list(NULL);
}

// @brief generate block の終了
void
Parser::end_generate()
{
  pop_declhead_list(true);
  pop_item_list(true);
}

// @brief generate-if の then 節の開始
void
Parser::init_genif()
{
  push_declhead_list(NULL);
  push_item_list(NULL);

  mThenDeclHeadListStack.push_back(mCurThenDeclHeadList);
  mThenItemListStack.push_back(mCurThenItemList);

  mCurThenDeclHeadList = mCurDeclHeadList;
  mCurThenItemList = mCurItemList;
}

// @brief generate-if の else 節の開始
void
Parser::init_else()
{
  pop_declhead_list(true);
  pop_item_list(true);

  push_declhead_list(NULL);
  push_item_list(NULL);

  mElseDeclHeadListStack.push_back(mCurElseDeclHeadList);
  mElseItemListStack.push_back(mCurElseItemList);

  mCurElseDeclHeadList = mCurDeclHeadList;
  mCurElseItemList = mCurItemList;
}

// @brief generate-if の終了
void
Parser::end_genif()
{
  pop_declhead_list(true);
  pop_item_list(true);

  mCurThenDeclHeadList = mThenDeclHeadListStack.back();
  mThenDeclHeadListStack.pop_back();

  mCurThenItemList = mThenItemListStack.back();
  mThenItemListStack.pop_back();

  mCurElseDeclHeadList = mElseDeclHeadListStack.back();
  mElseDeclHeadListStack.pop_back();

  mCurElseItemList = mElseItemListStack.back();
  mElseItemListStack.pop_back();
}

// @brief block-statment の開始
void
Parser::init_block()
{
  push_declhead_list(NULL);
}

// @brief block-statement の終了
void
Parser::end_block()
{
  pop_declhead_list(true);
}

// @brief defparam リストを初期化する．
void
Parser::init_defparam()
{
  mDefParamList.clear();
}

// @brief contassign リストを初期化する．
void
Parser::init_contassign()
{
  mContAssignList.clear();
}

// @brief instance リストを初期化する．
void
Parser::init_inst()
{
  mInstList.clear();
}

END_NAMESPACE_YM_VERILOG
