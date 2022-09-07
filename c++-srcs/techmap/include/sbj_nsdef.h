#ifndef SBJ_NSDEF_H
#define SBJ_NSDEF_H

/// @file sbj_nsdef.h
/// @brief sbj パッケージに共通な定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "magus.h"


/// @brief sbj 用の名前空間の開始
#define BEGIN_NAMESPACE_SBJ \
BEGIN_NAMESPACE_MAGUS \
BEGIN_NAMESPACE(nsSbj)

/// @brief sbj 用の名前空間の終了
#define END_NAMESPACE_SBJ \
END_NAMESPACE(nsSbj) \
END_NAMESPACE_MAGUS


BEGIN_NAMESPACE_SBJ

enum class SbjNodeType;

class Bn2Sbj;

class SbjGraph;
class SbjPort;
class SbjDff;
class SbjLatch;
class SbjEdge;
class SbjNode;
class SbjHandle;
class SbjDumper;

END_NAMESPACE_SBJ

BEGIN_NAMESPACE_MAGUS

using nsSbj::Bn2Sbj;
using nsSbj::SbjGraph;
using nsSbj::SbjPort;
using nsSbj::SbjDff;
using nsSbj::SbjLatch;
using nsSbj::SbjEdge;
using nsSbj::SbjNode;
using nsSbj::SbjHandle;
using nsSbj::SbjDumper;

END_NAMESPACE_MAGUS

#endif // SBJ_NSDEF_H
