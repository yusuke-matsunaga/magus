
/// @file tests/enumcut/enumcut.cc
/// @brief カットの列挙プログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnBlifReader.h"
#include "YmNetworks/BdnIscas89Reader.h"
#include "YmNetworks/BdnNode.h"

#include "TopDown.h"
#include "BottomUp.h"
#include "ZddImp.h"
#include "ZddImp2.h"
#include "ZddTopDown.h"

#include "YmLogic/ZddMgr.h"

#include "YmUtils/PoptMainApp.h"
#include "YmUtils/MsgMgr.h"
#include "YmUtils/MsgHandler.h"

#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM

class SimpleOp :
  public EnumCutOp2
{
public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(BdnNode* node);

  virtual
  void
  found_cut(BdnNode* root,
	    ymuint ni,
	    BdnNode** inputs);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在処理中のノード
  BdnNode* mCurNode;

  // 現在処理中のノード番号
  ymuint32 mCurPos;

  // 現在のカット数
  ymuint32 mNcCur;

  // 全カット数
  ymuint32 mNcAll;

};

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
SimpleOp::all_init(BdnMgr& sbjgraph,
		   ymuint limit)
{
  mCurPos = 0;
  mNcAll = 0;
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
SimpleOp::node_init(BdnNode* node)
{
  mNcCur = 0;
  mCurNode = node;
#if 1
  cout << "#" << mCurPos << ": Node#" << node->id() << endl;
#endif
}

void
SimpleOp::found_cut(BdnNode* root,
		    ymuint ni,
		    BdnNode** inputs)
{
  ++ mNcCur;

#if 0
  cout << "found_cut(" << root->id() << ", {";
  for (ymuint i = 0; i < ni; ++ i) {
    cout << " " << inputs[i]->id();
  }
  cout << "}" << endl;
#endif
}

// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
// @param[in] node 根のノード
void
SimpleOp::node_end(BdnNode* node)
{
  assert_cond( node == mCurNode, __FILE__, __LINE__);
  ++ mCurPos;
  mNcAll += mNcCur;

#if 1
  cout << "    " << mNcCur << " cuts" << endl
       << endl;
#endif
}

// @brief 処理の最後に呼ばれる関数
void
SimpleOp::all_end(BdnMgr& sbjgraph,
		  ymuint limit)
{
  cout << "Total " << mNcAll << " cuts" << endl;
}


void
enumcut(const string& filename,
	bool blif,
	bool iscas89,
	ymuint cut_size,
	const string& method_str)
{
  MsgHandler* msg_handler = new StreamMsgHandler(&cerr);
  MsgMgr::reg_handler(msg_handler);

  BdnMgr network;

  if ( blif ) {
    BdnBlifReader read;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }
  else {
    BdnIscas89Reader read;
    if ( !read(filename, network) ) {
      cerr << "Error in reading " << filename << endl;
      return;
    }
  }

  SimpleOp op;

  if ( method_str == "bottom_up" ) {
    BottomUp enumcut;

    enumcut(network, cut_size, &op);
  }
#if 0
  else if ( method_str == "top_down" ) {
    TopDown enumcut;

    enumcut(network, cut_size, &op);
  }
#endif
  else if ( method_str == "zdd" ) {
    ZddMgr mgr("zddmgr");
    ZddImp enumcut(mgr);

    enumcut(network, cut_size);
  }
  else if ( method_str == "zdd2" ) {
    ZddMgr mgr("zddmgr");
    ZddImp2 enumcut(mgr);

    enumcut(network, cut_size);
  }
  else if ( method_str == "zdd_top_down" ) {
    ZddMgr mgr("zddmgr");
    ZddTopDown enumcut(mgr);

    enumcut(network, cut_size);
  }
  else {
    cerr << "Unknown method: " << method_str << endl;
    return;
  }
}

END_NAMESPACE_YM


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm;

  string method = "bottom_up";
  bool blif = false;
  bool iscas = false;
  int cut_size = 4;

  PoptMainApp popt;

  PoptStr popt_method("method", 'm', "specify evaluation method", "bottom_up|top_down|zdd");
  PoptNone popt_blif("blif", 0, "blif mode");
  PoptNone popt_iscas89("iscas89", 0, "iscas89 mode");
  PoptInt popt_cutsize("cut_size", 'c', "specify cut size", NULL);

  popt.add_option(&popt_method);
  popt.add_option(&popt_blif);
  popt.add_option(&popt_iscas89);
  popt.add_option(&popt_cutsize);

  popt.set_other_option_help("<file-name> ...");

  tPoptStat stat = popt.parse_options(argc, argv, 0);
  if ( stat == kPoptAbort ) {
    return -1;
  }

  if ( popt_method.is_specified() ) {
    method = popt_method.val();
  }
  if ( popt_blif.is_specified() ) {
    blif = true;
  }
  else if ( popt_iscas89.is_specified() ) {
    iscas = true;
  }
  else {
    // fall back
    blif = true;
  }

  if ( popt_cutsize.is_specified() ) {
    cut_size = popt_cutsize.val();
  }

  // 残りの引数はファイル名とみなす．
  vector<string> file_list;
  ymuint n_files = popt.get_args(file_list);
  if ( n_files == 0 ) {
    popt.print_usage(stderr, 0);
    return 1;
  }

  for (vector<string>::iterator p = file_list.begin();
       p != file_list.end(); ++ p) {
    string filename = *p;
    enumcut(filename, blif, iscas, cut_size, method);
  }

#if 0
  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
    { "method", 'm', POPT_ARG_STRING, &method_str, 0,
      "specify evaluation method", "bottom_up|top_down|zdd" },

    { "blif", '\0', POPT_ARG_NONE, NULL, 0x100,
      "blif mode", NULL },

    { "iscas89", '\0', POPT_ARG_NONE, NULL, 0x101,
      "iscas89 mode", NULL },

    { "cut_size", 'c', POPT_ARG_INT, &cut_size, 0,
      "specify cut size", NULL },

    POPT_AUTOHELP

    { NULL, '\0', 0, NULL, 0, NULL, NULL }
  };

  // オプション解析用のコンテキストを生成する．
  poptContext popt_context = poptGetContext(NULL, argc, argv, options, 0);
  poptSetOtherOptionHelp(popt_context, "[OPTIONS]* <file-name> ...");

  // オプション解析行う．
  for ( ; ; ) {
    int rc = poptGetNextOpt(popt_context);
    if ( rc == -1 ) {
      break;
    }
    if ( rc < -1 ) {
      // エラーが起きた．
      fprintf(stderr, "%s: %s\n",
	      poptBadOption(popt_context, POPT_BADOPTION_NOALIAS),
	      poptStrerror(rc));
      return 1;
    }
    if ( rc == 0x100 ) {
      blif = true;
    }
    else if ( rc == 0x101 ) {
      iscas = true;
    }
  }

  if ( !blif && !iscas ) {
    blif = true;
  }

  // 残りの引数はファイル名とみなす．
  const char* str = poptGetArg(popt_context);
  if ( str == NULL ) {
    fprintf(stderr, "No filename.\n");
    return 2;
  }

  string filename(str);
  enumcut(filename, blif, iscas, cut_size, method_str);
#endif

  return 0;
}
