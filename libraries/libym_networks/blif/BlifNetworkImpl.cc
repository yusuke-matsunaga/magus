
/// @file BlifNetworkImpl.cc
/// @brief BlifNetworkImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BlifNetworkImpl.h"
#include "ym_networks/BlifNode.h"
#include "BlifParser.h"
#include "BlifNetworkHandler.h"


BEGIN_NAMESPACE_YM_NETWORKS_BLIF

//////////////////////////////////////////////////////////////////////
// BlifNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID 番号
BlifNode::BlifNode(ymuint32 id) :
  mId(id),
  mType(kUndef),
  mFaninNum(0),
  mFanins(NULL),
  mCubeNum(0),
  mCover(NULL)
{
}

// @brief デストラクタ
BlifNode::~BlifNode()
{
}


//////////////////////////////////////////////////////////////////////
// BlifNetworkImpl
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
  mName = string();
  mNodeArray.clear();
  mPIArray.clear();
  mPOArray.clear();
  mFFArray.clear();
  mLogicArray.clear();
}

// @brief ノードを取り出す．
// @param[in] id ID 番号
// @note なければ作成する．
BlifNode*
BlifNetworkImpl::get_node(ymuint32 id)
{
  ymuint32 n = mNodeArray.size();
  while ( n <= id ) {
    mNodeArray.push_back(NULL);
    ++ n;
  }

  BlifNode* node = mNodeArray[id];
  if ( node == NULL ) {
    void* p = mAlloc.get_memory(sizeof(BlifNode));
    node = new (p) BlifNode(id);
    mNodeArray[id] = node;
  }
  return node;
}

// @brief ノード名を設定する．
// @param[in] node 対象のノード
// @param[in] name 名前
void
BlifNetworkImpl::set_node_name(BlifNode* node,
			       const char* name)
{
  ymuint32 l = strlen(name);
  void* p = mAlloc.get_memory(l + 1);
  char* buff = new (p) char[l + 1];
  node->mName = buff;
  while ( (*buff ++ = *name ++) ) ;
}

// @brief ノードを外部入力に設定する．
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetworkImpl::set_input_type(BlifNode* node)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kInput;
  mPIArray.push_back(node);
  return true;
}

// @brief ノードを論理ノードに設定する．
// @param[in] ni ファンイン数
// @param[in] nc キューブ数
// @param[in] cover_pat 全キューブのパタンをつなげたもの
// @param[in] opat 出力のパタン
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetworkImpl::set_logic_type(BlifNode* node,
				ymuint32 ni,
				ymuint32 nc,
				const char* cover_pat,
				char opat)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLogic;
  alloc_fanin(node, ni);
  node->mCubeNum = nc;

  if ( ni > 0 ) {
    ymuint32 size = nc * ni;
    void* p = mAlloc.get_memory(size);
    char* buff = new (p) char[size];
    node->mCover = buff;
    while ( size -- > 0 ) {
      *buff ++ = *cover_pat ++;
    }
  }
  else {
    node->mCover = NULL;
  }
  node->mOpat = opat;

  mLogicArray.push_back(node);
  return true;
}

// @brief ノードをラッチノードに設定する．
// @param[in] rval リセット値 ( '0', '1', ' ' のいづれか )
// @note 既に他の型に設定されていたら false を返す．
bool
BlifNetworkImpl::set_latch_type(BlifNode* node,
				char rval)
{
  if ( node->mType != BlifNode::kUndef ) {
    return false;
  }
  node->mType = BlifNode::kLatch;
  alloc_fanin(node, 1);

  node->mOpat = rval;

  mFFArray.push_back(node);
  return true;
}

// @brief BlifNode のファンイン領域を確保する．
// @param[in] node 対象のノード
// @param[in] ni ファンイン数
void
BlifNetworkImpl::alloc_fanin(BlifNode* node,
			     ymuint32 ni)
{
  node->mFaninNum = ni;
  void* q = mAlloc.get_memory(sizeof(BlifNode*) * ni);
  node->mFanins = new (q) BlifNode*[ni];
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
  for (vector<BlifNode*>::const_iterator p = mPOArray.begin();
       p != mPOArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".outputs " << node->name() << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mFFArray.begin();
       p != mFFArray.end(); ++ p) {
    const BlifNode* node = *p;
    const BlifNode* inode = node->fanin(0);
    s << ".latch " << inode->name() << " " << node->name();
    if ( node->opat() != ' ' ) {
      s << node->opat();
    }
    s << endl;
  }
  for (vector<BlifNode*>::const_iterator p = mLogicArray.begin();
       p != mLogicArray.end(); ++ p) {
    const BlifNode* node = *p;
    s << ".names";
    ymuint32 ni = node->fanin_num();
    for (ymuint32 i = 0; i < ni; ++ i) {
      const BlifNode* inode = node->fanin(i);
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
  s << ".end" << endl;
}

END_NAMESPACE_YM_NETWORKS_BLIF
