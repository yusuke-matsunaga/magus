
/// @file recordcut.cc
/// @brief カットを記録するプログラム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_POPT
#include <popt.h>
#else
#error "<popt.h> not found."
#endif

#include "ym_networks/BdnMgr.h"
#include "ym_networks/BdnBlifReader.h"
#include "ym_networks/BdnIscas89Reader.h"
#include "ym_networks/BdnNode.h"

#include "TopDown.h"
#include "FuncRec.h"
#include "CutMgr.h"
#include "Cut.h"

#include "ym_utils/MsgMgr.h"
#include "ym_utils/MsgHandler.h"

#include "ym_utils/StopWatch.h"


BEGIN_NAMESPACE_YM_NETWORKS


//////////////////////////////////////////////////////////////////////
// カットを記録するためのクラス
//////////////////////////////////////////////////////////////////////
class RecOp :
  public EnumCutOp
{
public:

  /// @brief コンストラクタ
  RecOp(CutMgr& cut_mgr);


public:

  /// @brief 処理の最初に呼ばれる関数
  /// @param[in] sbjgraph 対象のサブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] mode カット列挙モード
  virtual
  void
  all_init(const BdnMgr& sbjgraph,
	   ymuint limit);

  /// @brief node を根とするカットを列挙する直前に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_init(const BdnNode* node);

  virtual
  void
  found_cut(const BdnNode* root,
	    ymuint ni,
	    const BdnNode** inputs);

  /// @brief node を根とするカットを列挙し終わった直後に呼ばれる関数
  /// @param[in] node 根のノード
  virtual
  void
  node_end(const BdnNode* node);

  /// @brief 処理の最後に呼ばれる関数
  virtual
  void
  all_end(const BdnMgr& sbjgraph,
	  ymuint limit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // カットを記録するオブジェクト
  CutMgr& mCutMgr;

  // 現在処理中のノード
  const BdnNode* mCurNode;

  // 現在処理中のノード番号
  ymuint32 mCurPos;

  // 現在のカット数
  ymuint32 mNcCur;

  // 全カット数
  ymuint32 mNcAll;

};

// @brief コンストラクタ
RecOp::RecOp(CutMgr& cut_mgr) :
  mCutMgr(cut_mgr)
{
}

// @brief 処理の最初に呼ばれる関数
// @param[in] sbjgraph 対象のサブジェクトグラフ
// @param[in] limit カットサイズ
// @param[in] mode カット列挙モード
void
RecOp::all_init(const BdnMgr& sbjgraph,
		ymuint limit)
{
  mCurPos = 0;
  mNcAll = 0;
}

// @brief node を根とするカットを列挙する直前に呼ばれる関数
// @param[in] node 根のノード
void
RecOp::node_init(const BdnNode* node)
{
  mNcCur = 0;
  mCurNode = node;
#if 1
  cout << "#" << mCurPos << ": Node#" << node->id() << endl;
#endif
}

void
RecOp::found_cut(const BdnNode* root,
		 ymuint ni,
		 const BdnNode** inputs)
{
  ++ mNcCur;

  if ( ni >= 5 ) {
    mCutMgr.new_cut(root, ni, inputs);
  }
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
RecOp::node_end(const BdnNode* node)
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
RecOp::all_end(const BdnMgr& sbjgraph,
	       ymuint limit)
{
  cout << "Total " << mNcAll << " cuts" << endl;
}


void
enumcut(const string& filename,
	bool blif,
	bool iscas89,
	ymuint cut_size)
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

  FuncRec op;
  TopDown enumcut;

  enumcut(network, cut_size, &op);

#if 0
  const list<Cut*>& cut_list = cut_mgr.cut_list();
  cout << "Total " << cut_list.size() << " unique cuts" << endl;

  const char* datafile = "cuts.dump";
  {
    ofstream os;
    os.open(datafile, ios::binary);
    if ( !os ) {
      cerr << "Could not create " << datafile << endl;
      return;
    }
    BinOStream bos(os);

    cut_mgr.dump(bos);
  }

  CutMgr cut_mgr2;
  {
    ifstream is;
    is.open(datafile, ios::binary);
    if ( !is ) {
      cerr << "Could not open " << datafile << endl;
      return;
    }
    BinIStream bis(is);
    cut_mgr2.restore(bis);
  }
  cout << "cut_mgr2:: total " << cut_mgr2.cut_list().size() << " unique cuts" << endl;
#endif
}

END_NAMESPACE_YM_NETWORKS


int
main(int argc,
     const char** argv)
{
  using namespace std;
  using namespace nsYm::nsNetworks;

  bool blif = false;
  bool iscas = false;
  int cut_size = 4;

  // オプション解析用のデータ
  const struct poptOption options[] = {
    // long-option
    // short-option
    // argument type
    // variable address
    // option tag
    // docstr
    // argstr
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
  enumcut(filename, blif, iscas, cut_size);

  return 0;
}
