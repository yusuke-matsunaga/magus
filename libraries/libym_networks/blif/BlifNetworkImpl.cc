
/// @file BlifNetworkImpl.cc
/// @brief BlifNetworkImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNetworkImpl.h"
#include "BlifNodeImpl.h"
#include "BlifParser.h"
#include "BlifNetworkHandler.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
// クラス BlifNetworkImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BlifNetworkImpl::BlifNetworkImpl() :
  mAlloc(4096)
{
}

// @brief デストラクタ
BlifNetworkImpl::~BlifNetworkImpl()
{
}

// @brief 内容をクリアする．
void
BlifNetworkImpl::clear()
{
  mName = NULL;
  mNodeArray.clear();
  mPIArray.clear();
  mPOArray.clear();
  mFFArray.clear();
  mLogicArray.clear();
  mAlloc.destroy();
}

// @brief .model 名の設定
void
BlifNetworkImpl::set_model(const char* name)
{
  mName = alloc_string(name);
}

// @brief 外部入力ノードを生成する．
// @param[in] node_id ノードID
// @param[in] node_name ノード名
void
BlifNetworkImpl::new_input(ymuint32 node_id,
			   const char* node_name)
{
  const char* new_node_name = alloc_string(node_name);

  void* p = mAlloc.get_memory(sizeof(BlifInputNode));
  BlifNode* node = new (p) BlifInputNode(node_id, new_node_name);

  set_node(node_id, node);

  mPIArray.push_back(node);
}

// @brief 外部出力ノードの番号を登録する．
// @param[in] node_id ノードID
void
BlifNetworkImpl::new_output(ymuint32 node_id)
{
  mPOArray.push_back(node_id);
}

// @brief 論理ノードを生成する．
// @param[in] node_id ノードID
// @param[in] node_name ノード名
// @param[in] inode_id_array ファンインのID番号の配列
// @param[in] nc キューブ数
// @param[in] cover_pat 全キューブのパタンをつなげたもの
// @param[in] opat 出力のパタン
void
BlifNetworkImpl::new_logic(ymuint32 node_id,
			   const char* node_name,
			   const vector<ymuint32>& inode_id_array,
			   ymuint32 nc,
			   const char* cover_pat,
			   char opat)
{
  const char* new_node_name = alloc_string(node_name);

  ymuint ni = inode_id_array.size();
  ymuint32* fanins = NULL;
  if ( ni > 0 ) {
    void* q = mAlloc.get_memory(sizeof(ymuint32) * ni);
    fanins = new (q) ymuint32[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = inode_id_array[i];
    }
  }

  const char* new_cover_pat = NULL;
  if ( ni > 0 ) {
    new_cover_pat = alloc_string(cover_pat);
  }

  void* p = mAlloc.get_memory(sizeof(BlifLogicNode));
  BlifNode* node = new (p) BlifLogicNode(node_id, new_node_name, ni, fanins, nc, new_cover_pat, opat);

  set_node(node_id, node);

  mLogicArray.push_back(node);
}

// @brief ゲートノードを生成する．
// @param[in] node_id ノードID
// @param[in] node_name ノード名
// @param[in] inode_id_array ファンインのID番号の配列
// @param[in] cell セルへのポインタ
void
BlifNetworkImpl::new_gate(ymuint32 node_id,
			  const char* node_name,
			  const vector<ymuint32>& inode_id_array,
			  const Cell* cell)
{
  const char* new_node_name = alloc_string(node_name);

  ymuint ni = inode_id_array.size();
  ymuint32* fanins = NULL;
  if ( ni > 0 ) {
    void* q = mAlloc.get_memory(sizeof(ymuint32) * ni);
    fanins = new (q) ymuint32[ni];
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = inode_id_array[i];
    }
  }

  void* p = mAlloc.get_memory(sizeof(BlifGateNode));
  BlifNode* node = new (p) BlifGateNode(node_id, new_node_name, ni, fanins, cell);

  set_node(node_id, node);

  mGateArray.push_back(node);
}

// @brief ラッチノードを生成する．
// @param[in] node_id ノードID
// @param[in] node_name ノード名
// @param[in] inode_id ファンインのID番号
// @param[in] rval リセット値 ( '0', '1', ' ' のいずれか )
void
BlifNetworkImpl::new_latch(ymuint32 node_id,
			   const char* node_name,
			   ymuint32 inode_id,
			   char rval)
{
  const char* new_node_name = alloc_string(node_name);

  void* p = mAlloc.get_memory(sizeof(BlifLatchNode));
  BlifNode* node = new (p) BlifLatchNode(node_id, new_node_name, inode_id, rval);

  set_node(node_id, node);

  mFFArray.push_back(node);
}

// @brief ノードをセットする．
// @param[in] node_id ノードID
// @param[in] node 対象のノード
void
BlifNetworkImpl::set_node(ymuint32 node_id,
			  BlifNode* node)
{
  while ( mNodeArray.size() <= node_id ) {
    mNodeArray.push_back(NULL);
  }

  mNodeArray[node_id] = node;
}

// @brief 文字列領域を確保する．
const char*
BlifNetworkImpl::alloc_string(const char* src_str)
{
  ymuint l = strlen(src_str);
  void* p = mAlloc.get_memory(l + 1);
  char* dst_str = new (p) char[l + 1];
  const char* dst_top = dst_str;
  while ( (*dst_str ++ = *src_str ++) ) ;
  return dst_top;
}

// @brief blif 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] cell_library セルライブラリ
// @retval true 正常に読み込めた
// @retval false 読み込み中にエラーが起こった．
bool
BlifNetworkImpl::read_blif(const string& filename,
			   const CellLibrary* cell_library)
{
  BlifParser parser;
  BlifNetworkHandler* handler = new BlifNetworkHandler;
  parser.add_handler(handler);

  handler->set(this);

  bool stat = parser.read(filename, cell_library);

  // handler は parser が解放してくれる．

  return stat;
}

// @brief 内容を blif 形式で出力する．
// @param[in] s 出力先のストリーム
void
BlifNetworkImpl::write_blif(ostream& s) const
{
  s << ".model " << name() << endl;
  for (vector<BlifNode*>::const_iterator p = mPIArray.begin();
       p != mPIArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".inputs " << node->name() << endl;
  }
  for (vector<ymuint32>::const_iterator p = mPOArray.begin();
       p != mPOArray.end(); ++ p) {
    const BlifNode* node = this->node(*p);
    s << ".outputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mFFArray.begin();
       p != mFFArray.end(); ++ p) {
    const BlifNode* node = *p;
    const BlifNode* inode = this->node(node->inode_id());
    s << ".latch " << inode->name() << " " << node->name();
    if ( node->reset_val() != ' ' ) {
      s << node->reset_val();
    }
    s << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mLogicArray.begin();
       p != mLogicArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".names";
    ymuint32 ni = node->fanin_num();
    for (ymuint32 i = 0; i < ni; ++ i) {
      const BlifNode* inode = this->node(node->fanin_id(i));
      s << " " << inode->name();
    }
    s << " " << node->name() << endl;
    ymuint32 nc = node->cube_num();
    if ( ni > 0 ) {
      for (ymuint32 c = 0; c < nc; ++ c) {
	for (ymuint32 i = 0; i < ni; ++ i) {
	  s << node->cube_pat(c, i);
	}
	s << " " << node->opat() << endl;
      }
    }
    else if ( nc == 1 ) {
      s << node->opat();
    }
  }
  for (vector<BlifNode*>::const_iterator p = mGateArray.begin();
       p != mGateArray.end(); ++ p) {
    const BlifNode* node = *p;
    ymuint32 ni = node->fanin_num();
    const Cell* cell = node->cell();
    assert_cond( ni == cell->input_num(), __FILE__, __LINE__);
    assert_cond( cell->output_num() == 1, __FILE__, __LINE__);
    assert_cond( cell->inout_num() == 0, __FILE__, __LINE__);
    s << ".gate " << cell->name();
    for (ymuint32 i = 0; i < ni; ++ i) {
      const CellPin* ipin = cell->input(i);
      const BlifNode* inode = this->node(node->fanin_id(i));
      s << " " << ipin->name() << "=" << inode->name();
    }
    const CellPin* opin = cell->output(0);
    s << " " << opin->name() << "=" << node->name() << endl;
  }
  s << ".end" << endl;
}

END_NAMESPACE_YM_NETWORKS_BLIF
