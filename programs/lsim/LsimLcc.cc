
/// @file LsimLcc.cc
/// @brief LsimLcc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LsimLcc.h"
#include "YmNetworks/BdnNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LsimLcc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LsimLcc::LsimLcc()
{
}

// @brief デストラクタ
LsimLcc::~LsimLcc()
{
}

// @brief ネットワークをセットする．
// @param[in] bdn 対象のネットワーク
// @param[in] order_map 順序マップ
void
LsimLcc::set_network(const BdnMgr& bdn,
		     const unordered_map<string, ymuint>& order_map)
{
  vector<const BdnNode*> node_list;
  bdn.sort(node_list);
  ymuint n = bdn.max_node_id();

  cout << "/// @file lcc_main.cc" << endl
       << "/// @brief コンパイル法シミュレータのメイン関数" << endl
       << "/// @author Yusuke Matsunaga (松永 裕介)" << endl
       << "///" << endl
       << "/// Copyright (C) 2005-2011 Yusuke Matsunaga" << endl
       << "/// All rights reserved." << endl
       << endl
       << "#include <iostream>" << endl
       << endl
       << "void" << endl
       << "eval_logic(unsigned long ivals[]," << endl
       << "           unsigned long ovals[])" << endl
       << "{" << endl
       << "  unsigned long vals[" << n << "];" << endl;
  ymuint input_id = 0;
  const BdnNodeList& input_list = bdn.input_list();
  ymuint ni = input_list.size();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p, ++ input_id) {
    const BdnNode* node = *p;
    cout << "  vals[" << node->id() << "] = ivals[" << input_id << "];" << endl;
  }
  for (vector<const BdnNode*>::iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    const BdnNode* node = *p;
    cout << "  vals[" << node->id() << "] = ";

    if ( node->fanin_inv(0) ) {
      cout << "~";
    }
    cout << "vals[" << node->fanin(0)->id() << "]";

    if ( node->is_xor() ) {
      cout << " ^ ";
    }
    else {
      cout << " & ";
    }

    if ( node->fanin_inv(1) ) {
      cout << "~";
    }
    cout << "vals[" << node->fanin(1)->id() << "]";

    cout << ";" << endl;
  }

  const BdnNodeList& output_list = bdn.output_list();
  ymuint no = output_list.size();
  ymuint output_id = 0;
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p, ++ output_id) {
    const BdnNode* node = *p;
    const BdnNode* inode = node->output_fanin();
    cout << "  ovals[" << output_id << "] = ";
    if ( inode != NULL ) {
      if ( node->output_fanin_inv() ) {
	cout << "~";
      }
      cout << "vals[" << inode->id() << "]";
    }
    else {
      cout << "0UL";
    }
    cout << ";" << endl;
  }
  cout << "}" << endl
       << endl;

  cout << "int" << endl
       << "main(int argc," << endl
       << "const char** argv)"<< endl
       << "{" << endl
       << "  using namespace std;" << endl
       << endl
       << "  if ( argc != 2 ) {" << endl
       << "    cerr << \"Usage: lcc <loop-num>\" << endl;" << endl
       << "    return 1;" << endl
       << "  }" << endl
       << endl
       << "  int n = atoi(argv[1]);" << endl
       << "  unsigned long ivals[" << ni << "];" << endl
       << "  unsigned long ovals[" << no << "];" << endl
       << "  for (int i = 0; i < n; ++ i) {" << endl
       << "    for (int j = 0; j < " << ni << "; ++ j) {" << endl
       << "      ivals[j] = random();" << endl
       << "    }" << endl
       << "    eval_logic(ivals, ovals);" << endl
       << "  }" << endl
       << endl
       << "  return 0;" << endl
       << "}" << endl;
}

// @brief 論理シミュレーションを行う．
// @param[in] iv 入力ベクタ
// @param[out] ov 出力ベクタ
void
LsimLcc::eval(const vector<ymuint64>& iv,
	      vector<ymuint64>& ov)
{
}

END_NAMESPACE_YM
