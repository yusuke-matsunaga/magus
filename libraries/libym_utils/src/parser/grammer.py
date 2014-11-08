# coding=UTF-8

# 文脈自由文法を表すクラス
#
# - 文法記号は内部では0から始まる整数で表す．
#   ただし 0 番は実際には現れない末尾を示す記号($)
#   番号と文法記号の対応付けは m_TokenList で行う．
#   id 番目の文法記号は m_TokenList[id] でアクセス
#   できる．
#
# - 文法規則は (left, right) という2つ組で表す．
#   left は左辺の(非終端) 記号番号，right は
#   右辺の記号番号のタプル
#   文法規則も内部では0から始まる整数で表す．
#   m_RuleList に格納される．
#
# - LR(0) 項は (rule_id, pos) という2つ組で表す．
class Grammer :

    # コンストラクタ
    def __init__(self) :
        self.clear()

    # @brief 内容をクリアする．
    def clear(self) :
        self._DEBUG = False

        # 文法記号のリスト
        self._TokenList = []

        # 終端記号を表すフラグ
        self._TerminalList = []

        # 文法規則のリスト
        self._RuleList = []

        # 文法規則の左辺をキーにした右辺のリスト
        self._RuleArray = []

        # 0 番目の '$' は終端文字
        ret = self.add_token('$')
        assert ret == 0

        # 開始記号
        self._StartNode = self.add_token('_START_')

        # 開始規則
        self._StartRule = -1

        # 使われない終端記号
        self._DummyToken = -1

        # FIRST マップ
        self._FirstMap = []

        # FOLLOW マップ
        self._FollowMap = []

    # @brief 文法記号の追加
    # @param[in] token 追加するトークン文字列
    # @return トークン番号を返す．
    def add_token(self, token) :
        ret = len(self._TokenList)
        self._TokenList.append(token)
        self._TerminalList.append(True)
        self._RuleArray.append([])
        return ret


    # @brief 文法規則の追加
    # @param[in] left 左辺のトークン番号
    # @param[in] right 右辺式を表すトークン番号のタプル
    # @return 文法番号を返す．
    def add_rule(self, left, right) :
        assert left < len(self._TokenList)
        for token in right :
            assert token < len(self._TokenList)

        self._TerminalList[left] = False
        ret = len(self._RuleList)
        self._RuleList.append( (left, right) )
        self._RuleArray[left].append(ret)
        return ret


    # @brief 開始記号の設定
    # @param[in] start 開始記号のトークン番号
    #
    # この関数は clear() 後に一回しか呼べない．
    def set_start(self, start) :
        # 開始記号を右辺とする規則を追加する．
        self._StartRule = self.add_rule(self._StartNode, (start, ))

        # 使われない終端器号を追加する．
        self._DummyToken = self.add_token('#')

        # 左辺の文字数の最大値を求める．
        left_max = 0
        for rule in self._RuleList :
            (left, right) = rule
            tmp = self.id2token(left)
            tmp_len = len(tmp)
            if left_max < tmp_len :
                left_max = tmp_len
        # 表示用のフォーマットを作る．
        self._LeftFormat = "%%-%ds ::=" % left_max

        # FIRST/FOLLOW を計算する．
        self.calc_first()


    # @brief FIRST()/FOLLOW() を求める．
    def calc_first(self) :
        # m_FirstMap を [] で初期化する．
        self._FirstMap = []
        for token in self._TokenList :
            self._FirstMap.append([])

        # 終端記号の FIRST() は自分自身
        for token_id in range(0, len(self._TokenList)) :
            if self._TerminalList[token_id] :
                self._FirstMap[token_id].append(token_id)

        # 変更がなくなるまで以下のループを繰り返す．
        update = True
        while update :
            update = False
            for (left, right) in self._RuleList :
                flist = self.first(right)
                for token_id1 in flist :
                    if add_to_tokenlist(token_id1, self._FirstMap[left]) :
                        update = True

        # ソートしておく
        for token_id in range(0, len(self._TokenList)) :
            self._FirstMap[token_id].sort()

        # self._FollowMap を [] で初期化する．
        self._FollowMap = []
        for token_id in self._TokenList :
            self._FollowMap.append([])

        # 開始記号の FOLLOW は '$'
        self._FollowMap[self._StartNode].append(0)

        # 変化がなくなるまで以下の処理を繰り返す．
        update = True
        while update :
            update = False
            for (left, right) in self._RuleList :
                n = len(right)
                for i in range(0, n - 1) :
                    has_epsilon = False
                    for token_id in self._FirstMap[right[i + 1]] :
                        if token_id == -1 :
                            has_epsilon = True
                        else :
                            if add_to_tokenlist(token_id, self._FollowMap[right[i]]) :
                                update = True
                    if has_epsilon :
                        for token_id in self._FollowMap[left] :
                            if add_to_tokenlist(token_id, self._FollowMap[right[i]]) :
                                update = True
                for token_id in self._FollowMap[left] :
                    if add_to_tokenlist(token_id, self._FollowMap[right[n - 1]]) :
                        update = True

        # ソートしておく
        for token_id in range(0, len(self._TokenList)) :
            self._FollowMap[token_id].sort()


    # 記号列(トークン番号のリスト)に対する FIRST() を計算する．
    def first(self, token_list) :
        ans_list = []
        all_epsilon = True
        for token in token_list :
            has_epsilon = False
            for tmp_id in self._FirstMap[token] :
                if tmp_id == -1 :
                    has_epsilon = True
                else :
                    add_to_tokenlist(tmp_id, ans_list)
            if not has_epsilon :
                all_epsilon = False
                break
        if all_epsilon :
            ans_list.append(-1)
        return ans_list

    # @brief トークン番号からトークン文字列を得る．
    # @param[in] id トークン番号
    def id2token(self, id) :
        assert id < len(self._TokenList)
        return self._TokenList[id]

    # @brief 規則番号から規則を得る．
    # @param[in] id 規則番号
    def id2rule(self, id) :
        assert id < len(self._RuleList)
        return self._RuleList[id]

    # @brief 内容を表示する
    def print_rules(self) :
        rule_id = 0
        for rule in self._RuleList :
            (left, right) = rule
            line = "Rule (%d): " % rule_id
            rule_id += 1
            line += self._LeftFormat % self.id2token(left)
            for id in right :
                line += " ";
                line += self.id2token(id)
            print line
        print ""

    # @brief トークンの情報とFIRST/FOLLOW を表示する．
    def print_tokens(self) :
        for token_id in range(1, len(self._TokenList)) :
            print '%s:' % self.id2token(token_id)
            print '  FIRST: ',
            for tmp in self._FirstMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print '  FOLLOW: ',
            for tmp in self._FollowMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print ''


# @brief LR(0)正準集を求める．
def LR0_state_list(grammer) :
    start_state = LR0_closure(grammer, [(grammer._StartRule, 0)])

    state_list = []
    state_list.append(start_state)
    new_states = []
    new_states.append(start_state)
    while len(new_states) > 0 :
        cur_states = new_states
        new_states = []
        for state in cur_states :
            for token_id in range(0, len(grammer._TokenList)) :
                new_state = LR0_next_state(grammer, state, token_id)
                if len(new_state) == 0 :
                    continue
                if not new_state in state_list :
                    state_list.append(new_state)
                    new_states.append(new_state)

    return state_list


# @brief LR(0)正準集のカーネルを求める．
def LR0_kernel_list(grammer) :
    all_state_list = LR0_state_list(grammer)

    state_list = []
    for state in all_state_list :
        kernel = [(rule_id, pos) for (rule_id, pos) in state if pos > 0 or rule_id == grammer._StartRule]
        state_list.append(kernel)

    return state_list


# @brief LR(0)項集合の遷移先を求める．
# @param[in] terms 入力の項集合
def LR0_next_state(grammer, cur_state, token) :
    tmp_state = []
    for (rule_id, pos) in cur_state :
        (left, right) = grammer.id2rule(rule_id)
        if len(right) > pos and right[pos] == token :
            tmp_state.append( (rule_id, pos + 1) )
    return LR0_closure(grammer, tmp_state)


# @brief LR(0)項の閉包演算を行う．
# @param[in] terms 入力の項集合(リスト)
# @return terms に対する閉包(項のリスト)を返す．
def LR0_closure(grammer, terms) :
    ans_terms = list(terms)
    new_terms = list(terms)
    while len(new_terms) > 0 :
        cur_terms = new_terms
        new_terms = []
        for (rule_id, pos) in cur_terms :
            (left, right) = grammer.id2rule(rule_id)
            if len(right) > pos :
                head = right[pos]
                for rule1_id in grammer._RuleArray[head] :
                        term1 = (rule1_id, 0)
                        if not term1 in ans_terms :
                            ans_terms.append(term1)
                            new_terms.append(term1)
    ans_terms.sort()
    return ans_terms


# @brief LR(0)状態リストを表示する．
def LR0_print_states(grammer, state_list) :
    state_id = 0
    for state in state_list :
        print 'State#%d:' % state_id
        print ''
        LR0_print_terms(grammer, state)
        state_id += 1


# @brief LR(0)項集合を表示する．
def LR0_print_terms(grammer, terms) :
    for (rule_id, pos) in terms :
        (left, right) = grammer.id2rule(rule_id)
        line = "  Rule#%d: " % rule_id
        line += grammer._LeftFormat % grammer.id2token(left)
        cur = 0
        for token_id in right :
            line += ' '
            if cur == pos :
                line += '. '
            line += grammer.id2token(token_id)
            cur += 1
        if cur == pos :
            line += " ."
        print line


# @brief LR(1)項集合の遷移先を求める．
# @param[in] terms 入力の項集合
def LR1_next_state(grammer, cur_state, token) :
    tmp_state = []
    for (rule_id, pos, token1) in cur_state :
        (left, right) = grammer.id2rule(rule_id)
        if len(right) > pos and right[pos] == token :
            tmp_state.append( (rule_id, pos + 1, token1) )
    return LR1_closure(grammer, tmp_state)


# @brief LR(1)項の閉包演算を行う．
# @param[in] terms 入力の項集合(リスト)
# @return terms に対する閉包(項のリスト)を返す．
def LR1_closure(grammer, terms) :
    ans_terms = list(terms)
    new_terms = list(terms)
    while len(new_terms) > 0 :
        cur_terms = new_terms
        new_terms = []
        for (rule_id, pos, token) in cur_terms :
            (left, right) = grammer.id2rule(rule_id)
            if len(right) > pos :
                head = right[pos]
                for rule1_id in grammer._RuleArray[head] :
                    rest = list(right[pos + 1:])
                    rest.append(token)
                    for token1 in grammer.first(rest) :
                        term1 = (rule1_id, 0, token1)
                        if not term1 in ans_terms :
                            ans_terms.append(term1)
                            new_terms.append(term1)
    ans_terms.sort()
    return ans_terms

# @brief LR(1)項集合を表示する．
def LR1_print_terms(grammer, terms) :
    for (rule_id, pos, token) in terms :
        (left, right) = grammer.id2rule(rule_id)
        line = "  Rule (%d): " % rule_id
        line += grammer._LeftFormat % grammer.id2token(left)
        cur = 0
        for token_id in right :
            line += ' '
            if cur == pos :
                line += '. '
            line += grammer.id2token(token_id)
            cur += 1
        if cur == pos :
            line += " ."
        line += ", %s" % grammer.id2token(token)
        print line

# @brief LALR(1)項集合を表示する．
def LALR1_print_terms(grammer, terms) :
    for (rule_id, pos, token_list) in terms :
        (left, right) = grammer.id2rule(rule_id)
        line = "  Rule (%d): " % rule_id
        line += grammer._LeftFormat % grammer.id2token(left)
        cur = 0
        for token_id in right :
            line += ' '
            if cur == pos :
                line += '. '
            line += grammer.id2token(token_id)
            cur += 1
        if cur == pos :
            line += " ."
        line += ", ["
        slash = ""
        for token in token_list :
            line += "%s%s" % (slash, grammer.id2token(token))
            slash = "/"
        line += "]"
        print line

# @brief トークンリストに重複なく追加する．
def add_to_tokenlist(token, token_list) :
    if not token in token_list :
        token_list.append(token)
        return True
    else :
        return False

if __name__ == '__main__' :
    # テストプログラム
    g = Grammer()

    id = g.add_token('id')
    plus = g.add_token('+')
    times = g.add_token('*')
    lpar = g.add_token('(')
    rpar = g.add_token(')')
    expr = g.add_token('expr')
    term = g.add_token('term')
    factor = g.add_token('factor')

    g.add_rule(expr, (expr, plus, term))
    g.add_rule(expr, (term, )) # (term) ではタプルにならない

    g.add_rule(term, (term, times, factor))
    g.add_rule(term, (factor, )) # (factor) ではタプルにならない

    g.add_rule(factor, (lpar, expr, rpar))
    g.add_rule(factor, (id, )) # (id) ではタプルにならない

    g.set_start(expr)

    g.print_rules()

    g.print_tokens()
