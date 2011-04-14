
/// @file libym_bdn/BdnBlifWriter.cc
/// @brief ブーリアンネットワークの内容を出力する関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BdnBlifWriter.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdnBlifWriter.h"
#include "ym_bdn/BdnMgr.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnDff.h"
#include "ym_utils/NameMgr.h"


BEGIN_NAMESPACE_YM_BDN


// @brief コンストラクタ
BdnBlifWriter::BdnBlifWriter()
{
}

// @brief デストラクタ
BdnBlifWriter::~BdnBlifWriter()
{
}

// ブーリアンネットワークをblif形式で表示
void
BdnBlifWriter::operator()(ostream& s,
			  const BdnMgr& network)
{
  // 名前を管理するオブジェクト
  NameMgr name_mgr("[", "]");

  ymuint n = network.max_node_id();

  // ID 番号をキーにして名前を入れる配列
  vector<string> name_array(n);

  // 外部入力，外部出力の名前を登録
  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    string name = name_mgr.new_name(true);
    BdnNode* node = *p;
    name_array[node->id()] = name;
  }
  const BdnNodeList& output_list = network.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    string name = name_mgr.new_name(true);
    BdnNode* node = *p;
    name_array[node->id()] = name;
  }

  // 論理ノードに名前をつける．
  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    if ( node->fanout_num() == 1 &&
	 node->pomark() ) {
      BdnFanoutList::const_iterator p = node->fanout_list().begin();
      const BdnNode* onode = (*p)->to();
      if ( !onode->output_fanin_inv() ) {
	// 外部出力と同じ名前にする．
	name_array[node->id()] = name_array[onode->id()];
	continue;
      }
    }
    string name = name_mgr.new_name(true);
    name_array[node->id()] = name;
  }

  // ファイルに出力する．
  s << ".model " << network.name() << endl;
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << ".inputs " << name_array[node->id()] << endl;
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << ".outputs " << name_array[node->id()] << endl;
  }

  const BdnDffList& dff_list = network.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* node = dff->output();
    const BdnNode* inode = dff->input()->output_fanin();
    bool oinv = dff->input()->output_fanin_inv();
    string iname;
    if ( inode ) {
      iname = name_array[inode->id()];
      if ( oinv ) {
	string name = name_mgr.new_name(true);
	s << ".names " << iname
	  << " " << name << endl
	  << "0 1" << endl;
	iname = name;
      }
    }
    else {
      iname = name_mgr.new_name(true);
      s << ".names " << iname << endl;
      if ( oinv ) {
	s << "0" << endl;
      }
      else {
	s << "1" << endl;
      }
    }
    s << ".latch " << iname << " " << name_array[node->id()];
    if ( dff->set()->output_fanin() ) {
      s << " 1";
    }
    else if ( dff->reset()->output_fanin() ) {
      s << " 0";
    }
    s << endl;
  }

  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.begin(); ++ p) {
    const BdnNode* node = *p;
    const BdnNode* inode = node->output_fanin();
    bool oinv = node->output_fanin_inv();
    if ( inode == 0 ) {
      s << ".names " << name_array[node->id()] << endl;
      if ( oinv ) {
	s << "1" << endl;
      }
      else {
	s << "0" << endl;
      }
      s << endl;
    }
    else if ( name_array[inode->id()] != name_array[node->id()] ) {
      s << ".names " << name_array[inode->id()]
	<< " " << name_array[node->id()] << endl;
      if ( oinv ) {
	s << "0 1" << endl;
      }
      else {
	s << "1 1" << endl;
      }
      s << endl;
    }
  }

  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    s << ".names " << name_array[node->fanin(0)->id()]
      << " " << name_array[node->fanin(1)->id()]
      << " " << name_array[node->id()] << endl;
    if ( node->is_xor() ) {
      s << "00 1" << endl
	<< "11 1" << endl;
    }
    else {
      char f0 = node->fanin0_inv() ? '0' : '1';
      char f1 = node->fanin1_inv() ? '0' : '1';
      s << f0 << f1 << " 1" << endl;
    }
    s << endl;
  }
  s << ".end" << endl;
}

END_NAMESPACE_YM_BDN
