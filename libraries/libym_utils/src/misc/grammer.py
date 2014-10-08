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
        self.m_DEBUG = False

        # 文法記号のリスト
        self.m_TokenList = []

        # 終端記号を表すフラグ
        self.m_TerminalList = []

        # 0 番目の '$' は終端文字
        ret = self.add_token('$')
        assert ret == 0

        # 文法規則のリスト
        self.m_RuleList = []

        # 開始記号
        self.m_StartNode = -1

        # 開始規則
        self.m_StartRule = -1

        # 状態リスト
        # 状態とは項の集合
        self.m_StateList = []

        # FIRST マップ
        self.m_FirstMap = []

        # FOLLOW マップ
        self.m_FollowMap = []

        # ACTION マップ
        self.m_ActionMap = []

    # @brief 文法記号の追加
    # @param[in] token 追加するトークン文字列
    # @return トークン番号を返す．
    def add_token(self, token) :
        ret = len(self.m_TokenList)
        self.m_TokenList.append(token)
        self.m_TerminalList.append(True)
        return ret

    # @brief 文法規則の追加
    # @param[in] left 左辺のトークン番号
    # @param[in] right 右辺式を表すトークン番号のタプル
    # @return 文法番号を返す．
    def add_rule(self, left, right) :
        assert left < len(self.m_TokenList)
        for token in right :
            assert token < len(self.m_TokenList)

        self.m_TerminalList[left] = False
        ret = len(self.m_RuleList)
        self.m_RuleList.append( (left, right) )
        return ret

    # @brief 開始記号の設定
    # @param[in] start 開始記号のトークン番号
    #
    # この関数は clear() 後に一回しか呼べない．
    def set_start(self, start) :
        assert self.m_StartNode == -1
        ss_id = self.add_token('_START_')
        self.m_StartNode = ss_id
        self.m_StartRule = self.add_rule(ss_id, (start, ))

        # 左辺の文字数の最大値を求める．
        left_max = 0
        for rule in self.m_RuleList :
            (left, right) = rule
            tmp = self.id2token(left)
            tmp_len = len(tmp)
            if left_max < tmp_len :
                left_max = tmp_len

        self.m_LeftFormat = "%%-%ds ::=" % left_max

        self.calc_first()

        self.LR0_items()

    # @brief 構文解析表を作る．
    def make_table(self) :
        for state in self.m_StateList :
            state_id = self.state2id(state)
            self.m_ActionMap.append({})
            for token_id in range(0, len(self.m_TokenList)) :
                if self.m_TerminalList[token_id] :
                    for (rule_id, pos) in state :
                        (left, right) = self.id2rule(rule_id)
                        if len(right) > pos and right[pos] == token_id :
                            next_state = self.next_state(state, token_id)
                            next_id = self.state2id(next_state)
                            assert next_id != -1
                            if not self.set_action(state_id, token_id, 'shift', next_id) :
                                print "Error: not an LR(0)"
                else :
                    next_state = self.next_state(state, token_id)
                    if len(next_state) > 0 :
                        next_id = self.state2id(next_state)
                        if not self.set_action(state_id, token_id, 'goto', next_id) :
                            print "Error: not an LR(0)"
            for (rule_id, pos) in state :
                (left, right) = self.id2rule(rule_id)
                if len(right) == pos :
                    if left == self.m_StartNode :
                        if not self.set_action(state_id, 0, 'accept', 0) :
                            print "Error: not an LR(0)"
                    else :
                        for token_id in self.m_FollowMap[left] :
                            if not self.set_action(state_id, token_id, 'reduce', rule_id) :
                                print "Error: not an LR(0)"

    # @brief ACTION 表の項目をセットする．
    # @param[in] state_id 状態番号
    # @param[in] token_id 記号番号
    # @param[in] action 動作
    # @param[in] action_id 動作に関連した番号
    #
    # 以前の設定内容と矛盾が生じた場合には False を返す．
    def set_action(self, state_id, token_id, action, action_id) :
        if self.m_DEBUG :
            print 'set_action(%d, %s, %s, %d)' % (state_id, self.id2token(token_id), action, action_id)
        if self.m_ActionMap[state_id].has_key(token_id) :
            if self.m_ActionMap[state_id][token_id] != (action, action_id) :
                return False
            else :
                return True
        else :
            self.m_ActionMap[state_id][token_id] = (action, action_id)
            return True

    # @brief FIRST()/FOLLOW() を求める．
    def calc_first(self) :
        # m_FirstMap を [] で初期化する．
        self.m_FirstMap = []
        for token in self.m_TokenList :
            self.m_FirstMap.append([])

        # 終端記号の FIRST() は自分自身
        for token_id in range(0, len(self.m_TokenList)) :
            if self.m_TerminalList[token_id] :
                self.m_FirstMap[token_id].append(token_id)

        # 変更がなくなるまで以下のループを繰り返す．
        update = True
        while update :
            update = False
            for (left, right) in self.m_RuleList :
                all_epsilon = True
                flist = []
                for node_id in right :
                    has_epsilon = False
                    for token_id1 in self.m_FirstMap[node_id] :
                        if token_id1 == -1 :
                            has_epsilon = True
                        else :
                            add_to_tokenlist(token_id1, flist)
                    if not has_epsilon :
                        all_epsilon = False
                        break
                for token_id1 in flist :
                    if add_to_tokenlist(token_id1, self.m_FirstMap[left]) :
                        update = True
                if all_epsilon :
                    if add_to_tokenlist(-1, self.m_FirstMap[left]) :
                        update = True
        # ソートしておく
        for token_id in range(0, len(self.m_TokenList)) :
            self.m_FirstMap[token_id].sort()

        # self.m_FollowMap を [] で初期化する．
        self.m_FollowMap = []
        for token_id in self.m_TokenList :
            self.m_FollowMap.append([])

        self.m_FollowMap[self.m_StartNode].append(0)
        update = True
        while update :
            update = False
            for (left, right) in self.m_RuleList :
                n = len(right)
                for i in range(0, n - 1) :
                    for token_id in self.m_FirstMap[right[i + 1]] :
                        if token_id == -1 :
                            has_epsilon = True
                        else :
                            if add_to_tokenlist(token_id, self.m_FollowMap[right[i]]) :
                                update = True
                    if has_epsilon :
                        for token_id in self.m_FollowMap[left] :
                            if add_to_tokenlist(token_id, self.m_FollowMap[right[i]]) :
                                update = True
                for token_id in self.m_FollowMap[left] :
                    if add_to_tokenlist(token_id, self.m_FollowMap[right[n - 1]]) :
                        update = True
        # ソートしておく
        for token_id in range(0, len(self.m_TokenList)) :
            self.m_FollowMap[token_id].sort()

    # @brief LR(0)正準集を作る．
    def LR0_items(self) :
        start_state = self.closure([(self.m_StartRule, 0)])

        self.m_StateList = []
        self.m_StateList.append(start_state)
        new_states = []
        new_states.append(start_state)
        while len(new_states) > 0 :
            cur_states = new_states
            new_states = []
            for state in cur_states :
                for token_id in range(0, len(self.m_TokenList)) :
                    next_state = self.next_state(state, token_id)
                    if len(next_state) > 0 and not next_state in self.m_StateList :
                        self.m_StateList.append(next_state)
                        new_states.append(next_state)

    # @brief 項集合の遷移先を求める．
    # @param[in] terms 入力の項集合
    def next_state(self, cur_state, token) :
        tmp_state = []
        for (rule_id, pos) in cur_state :
            (left, right) = self.id2rule(rule_id)
            if len(right) > pos and right[pos] == token :
                tmp_state.append( (rule_id, pos + 1) )
        return self.closure(tmp_state)

    # @brief 項の閉包演算を行う．
    # @param[in] terms 入力の項集合(リスト)
    # @return terms に対する閉包(項のリスト)を返す．
    def closure(self, terms) :
        ans_terms = list(terms)
        new_terms = list(terms)
        while len(new_terms) > 0 :
            cur_terms = new_terms
            new_terms = []
            for (rule_id, pos) in cur_terms :
                (left, right) = self.id2rule(rule_id)
                if len(right) > pos :
                    head = right[pos]
                    for rule1_id in range(0, len(self.m_RuleList)) :
                        (left1, right1) = self.id2rule(rule1_id)
                        if left1 == head :
                            term1 = (rule1_id, 0)
                            if not term1 in ans_terms :
                                ans_terms.append(term1)
                                new_terms.append(term1)
        ans_terms.sort()
        return ans_terms

    # @brief 内容を表示する
    def print_rules(self) :
        rule_id = 0
        for rule in self.m_RuleList :
            (left, right) = rule
            line = "Rule (%d): " % rule_id
            rule_id += 1
            line += self.m_LeftFormat % self.id2token(left)
            for id in right :
                line += " ";
                line += self.id2token(id)
            print line
        print ""

    # @brief 項集合を表示する．
    def print_terms(self, terms) :
        for (rule_id, pos) in terms :
            (left, right) = self.id2rule(rule_id)
            line = "  Rule (%d): " % rule_id
            line += self.m_LeftFormat % self.id2token(left)
            cur = 0
            for token_id in right :
                line += ' '
                if cur == pos :
                    line += '. '
                line += self.id2token(token_id)
                cur += 1
            if cur == pos :
                line += " ."
            print line

    # @brief 状態リストを表示する．
    def print_states(self) :
        for state in self.m_StateList :
            state_id = self.state2id(state)
            print 'State#%d:' % state_id
            print ''
            self.print_terms(state)
            print ""
            for key in self.m_ActionMap[state_id].keys() :
                (action, action_id) = self.m_ActionMap[state_id][key]
                token = self.id2token(key)
                print '  %s: %s %d' % (token, action, action_id)
            print ''

    # @brief FIRST/FOLLOW を表示する．
    def print_tokens(self) :
        for token_id in range(1, len(self.m_TokenList)) :
            print '%s:' % self.id2token(token_id)
            print '  FIRST: ',
            for tmp in self.m_FirstMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print '  FOLLOW: ',
            for tmp in self.m_FollowMap[token_id] :
                if tmp == -1 :
                    print '<>',
                else :
                    print self.id2token(tmp),
            print ''
            print ''

    # @brief 状態番号を返す．
    # @param[in] state 状態(項の集合)
    def state2id(self, state) :
        state_id = 0
        for state1 in self.m_StateList :
            if state1 == state :
                return state_id
            state_id += 1
        else :
            return -1

    # @brief トークン番号からトークン文字列を得る．
    # @param[in] id トークン番号
    def id2token(self, id) :
        assert id < len(self.m_TokenList)
        return self.m_TokenList[id]

    # @brief 規則番号から規則を得る．
    # @param[in] id 規則番号
    def id2rule(self, id) :
        assert id < len(self.m_RuleList)
        return self.m_RuleList[id]

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

    g.make_table()

    g.print_states()
