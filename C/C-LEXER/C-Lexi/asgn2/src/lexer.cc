/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

// Lexer modified for FIRST & FOLLOW project

string reserved[] = { "END_OF_FILE", "ARROW", "HASH", "DOUBLEHASH", "ID", "ERROR" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

Token LexicalAnalyzer::ScanId()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '-':
            input.GetChar(c);
            if (c == '>') {
                tmp.token_type = ARROW;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = ERROR;
            }
            return tmp;
        case '#':
            input.GetChar(c);
            if (c == '#') {
                tmp.token_type = DOUBLEHASH;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = HASH;
            }
            return tmp;
        default:
            if (isalpha(c)) {
                input.UngetChar(c);
                return ScanId();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}

// task 1
void LexicalAnalyzer::ParseGrammer() 
{
    SymbolType stype = NONTERM;
    int ruleNo = 0;

    std::vector<Token> ru;
    Token token = GetToken();

    while (token.token_type != END_OF_FILE &&
            token.token_type != DOUBLEHASH) {

        if (token.token_type == ARROW) {
            stype = TERM;
        } else if (token.token_type == HASH) {
            stype = NONTERM;
            rules.push_back(ru);
            ru.clear();
            ruleNo++;
        } else if (token.token_type == ID) {
            token.symbol_type = stype;
            ru.push_back(token);

            if (stype == NONTERM) {
                if (nonterms.find(token.lexeme) == nonterms.end()) {
                    nonterms.insert(keyType(token.lexeme, ruleNo));
                }
            }
        }

        token = GetToken();
    }
}

void LexicalAnalyzer::MarkSymbols()
{
    // print terminals
    for (int i = 0; i < rules.size(); i++) {
        for (int j = 1; j < rules[i].size(); j++) {
            if (rules[i][j].symbol_type == TERM
                && nonterms.find(rules[i][j].lexeme) == nonterms.end()
                && terms.find(rules[i][j].lexeme) == terms.end()) {

                term_seq.push_back(rules[i][j].lexeme);
                terms.insert(keyType(rules[i][j].lexeme, i));
            }
        }
    }

    std::map<std::string, int> temp_nonterms;

    // print non-terminals
    for (int i = 0; i < rules.size(); i++) {
        for (int j = 0; j < rules[i].size(); j++) {
            if (rules[i][j].symbol_type == NONTERM
                    || terms.find(rules[i][j].lexeme) == terms.end()) {
                if (temp_nonterms.find(rules[i][j].lexeme) == temp_nonterms.end()) {
                    rules[i][j].symbol_type = NONTERM;

                    nonterm_seq.push_back(rules[i][j].lexeme);
                    temp_nonterms.insert(keyType(rules[i][j].lexeme, 1));
                }
            }
        }
    }
}

void LexicalAnalyzer::PrintSymbols()
{
    bool firstE = false;
    for (int i = 0; i < term_seq.size(); i++) {
        if (firstE) cout << " ";
        cout << term_seq[i];
        firstE = true;
    }
    for (int i = 0; i < nonterm_seq.size(); i++) {
        if (firstE) cout << " ";
        cout << nonterm_seq[i];
    }
    cout << endl;
}

// task 2
void LexicalAnalyzer::MarkUseful()
{
    // init
    for (int i = 0; i < rules.size(); i++) {
        rule_useless.push_back(true);
        if (nonterm_useless.find(rules[i][0].lexeme) == 
                nonterm_useless.end()) {
            nonterm_useless.insert(useType(rules[i][0].lexeme, true));
        }
    }

    std::map<std::string, bool>::iterator itr;
    
    std::string start = rules[0][0].lexeme;

    // find all useful non-terms
    for (int k = 0; k < rules.size(); k++) {
        for (int i = 0; i < rules.size(); i++) {
            bool useful = true;
            for (int j = 1; j < rules[i].size(); j++) {
                if (nonterm_useless.find(rules[i][j].lexeme) != 
                        nonterm_useless.end()) {
                    rules[i][j].symbol_type = NONTERM;

                    useful = (useful & 
                                (!nonterm_useless[rules[i][j].lexeme]));

                    if (useful == false) break;
                }
            }

            if (useful == true) { // all depends are useful
                nonterm_useless[rules[i][0].lexeme] = false;
            }
        }
    }

    /* // for debug
    for (itr = nonterm_useless.begin(); itr != nonterm_useless.end();
            itr++) {
        cout << (*itr).first << " = " << (*itr).second << endl;
    }
    */

    // find all useful rules
    UpdateRuleUse(start);
}

void LexicalAnalyzer::UpdateRuleUse(std::string start)
{
    for (int i = 0; i < rules.size(); i++) { // for each rule
        if (rules[i][0].lexeme == start && rule_useless[i] == true) {
            bool rule_useful = true;
            for (int j = 1; j < rules[i].size(); j++) {
                if (rules[i][j].symbol_type == NONTERM
                     && nonterm_useless[rules[i][j].lexeme] == true) {
                    rule_useful = false; // exist useless non-term
                    break;
                }
            }

            if (rule_useful == true) { // update all right non-term rule
                rule_useless[i] = false;
                for (int j = 1; j < rules[i].size(); j++) {
                    if (rules[i][j].symbol_type == NONTERM)
                        UpdateRuleUse(rules[i][j].lexeme);
                }
            }
        }
    }
}

void LexicalAnalyzer::PrintUsefulRule()
{
    for (int i = 0; i < rules.size(); i++) {
        if (rule_useless[i] == false) {
            cout << rules[i][0].lexeme << " ->";
            if (rules[i].size() > 1) {
                for (int j = 1; j < rules[i].size(); j++) {
                    cout << " " << rules[i][j].lexeme;
                }
            } else {
                cout << " #";
            }
            cout << endl;
        }
    }
}

void LexicalAnalyzer::MarkFirst()
{
    std::vector<std::string> vec;
    // non-terms' first is void
    for (int i = 0; i < nonterm_seq.size(); i++) {
        first.insert(vecType(nonterm_seq[i], vec));
    }

    // terms' first is self
    for (int i = 0; i < term_seq.size(); i++) {
        vec.clear();
        vec.push_back(term_seq[i]);
        first.insert(vecType(term_seq[i], vec));
    }

    vec.clear();
    vec.push_back("#");
    first.insert(vecType("#", vec)); // epislon first set

    bool notEnd = true;
    while (notEnd) {
        notEnd = false;
        // if has change, notEnd = true
        int adds = 0;
        
        for (int i = 0; i < nonterm_seq.size(); i++) {
            // for each nonterm A, get its first
            for (int j = 0; j < rules.size(); j++) {
                // for each valid A->X
                if (rules[j][0].lexeme == nonterm_seq[i]) { 
                    bool con = true;
                    int k = 1;
                    while (con && k < rules[j].size()) {
                        adds = AddFirsts(nonterm_seq[i], 
                                        rules[j][k].lexeme, false);
                        if (adds > 0) notEnd = true;

                        if (!ContainsFirst(rules[j][k].lexeme, "#")) {
                            con = false;
                        }
                        k++;
                    }

                    if (con) {
                        if (!ContainsFirst(nonterm_seq[i], "#")) {
                            // not contain then add
                            adds = AddFirsts(nonterm_seq[i], "#", true);
                            if (adds > 0) notEnd = true;
                        }
                    }
                }
            }
        }
    }
}

int LexicalAnalyzer::AddFirsts(std::string key1, std::string key2, 
        bool withEpislon)
{
    int adds = 0;

    std::vector<std::string> &fs = first[key2];
    for (int i = 0; i < fs.size(); i++) {
        if ((withEpislon || fs[i] != "#") 
                && !ContainsFirst(key1, fs[i])) {
            // add it
            first[key1].push_back(fs[i]);
            adds++;
        }
    }

    return adds;
}

bool LexicalAnalyzer::ContainsFirst(std::string key, std::string ft)
{
    bool contains = false;

    for (int i = 0; i < first[key].size(); i++) {
        if (first[key][i] == ft) {
            contains = true;
            break;
        }
    }

    return contains;
}

void LexicalAnalyzer::PrintFirstSet()
{
    std::vector<std::string> terms(term_seq);
    terms.insert(terms.begin(), "#");

    for (int i = 0; i < nonterm_seq.size(); i++) {
        cout << "FIRST(" << nonterm_seq[i] << ") = { ";
        std::vector<std::string> &f = first[nonterm_seq[i]];

        bool flag = false;
        for (int k = 0; k < terms.size(); k++) {
            for (int j = 0; j < f.size(); j++) {
                if (f[j] == terms[k]) {
                    if (flag) {
                        cout << ", ";
                    }
                    cout << f[j];
                    flag = true;
                    break;
                }
            }
        }
        cout << " }\n";
    }
}

void LexicalAnalyzer::MarkFellow()
{
    std::vector<std::string> vec;
    // non-terms' fellow is void
    for (int i = 1; i < nonterm_seq.size(); i++) {
        fellow.insert(vecType(nonterm_seq[i], vec));
    }

    vec.clear();
    vec.push_back("$");
    fellow.insert(vecType(nonterm_seq[0], vec)); // $ in first fellow set

    bool notEnd = true;
    while (notEnd) {
        notEnd = false;
        // if has change, notEnd = true
        int adds = 0;
        
        // for each production, update the fellow sets
        for (int j = 0; j < rules.size(); j++) {
            // for each valid A->X
            for (int i = 1; i < rules[j].size(); i++) {
                if (nonterms.find(rules[j][i].lexeme) != nonterms.end()){ 
                    adds = AddFellows(rules[j][i].lexeme, j, i+1);
                    if (adds > 0) notEnd = true;
                }

                if (ContainsEpsilon(j, i+1)) {
                    adds = CopyFellows(rules[j][i].lexeme, 
                                fellow[rules[j][0].lexeme]);
                    if (adds > 0) notEnd = true;
                }
            }
        }
    }
}

bool LexicalAnalyzer::ContainsEpsilon(int row, int col)
{
    std::vector<std::string> firsts;

    for (int i = col; i < rules[row].size(); i++) {
        if (!ContainsFirst(rules[row][i].lexeme, "#")) return false;
    }

    return true;
}


std::vector<std::string> LexicalAnalyzer::SetFirst(int row, int col)
{
    std::vector<std::string> firsts;

    for (int i = col; i < rules[row].size(); i++) {
        firsts.insert(firsts.begin(), first[rules[row][i].lexeme].begin(),
                first[rules[row][i].lexeme].end());
        if (!ContainsFirst(rules[row][i].lexeme, "#")) break;
    }

    return firsts;
}

int LexicalAnalyzer::AddFellows(std::string key, int row, int col)
{
    int adds = 0;
    std::vector<std::string> vec = SetFirst(row, col);
    adds += CopyFellows(key, vec);

    return adds;
}

int LexicalAnalyzer::CopyFellows(std::string key, 
        std::vector<std::string> fws)
{
    int adds = 0;

    for (int i = 0; i < fws.size(); i++) {
        if (fws[i] != "#") {
            bool found = false;
            for (int j = 0; j < fellow[key].size(); j++) {
                if (fws[i] == fellow[key][j]) {
                    found = true;
                    break;
                }
            }
            if (found == false) { // add it
                fellow[key].push_back(fws[i]);
                adds++;
            }
        }
    }

    return adds;
}

void LexicalAnalyzer::PrintFellowSet()
{
    std::vector<std::string> terms(term_seq);
    terms.insert(terms.begin(), "$");

    for (int i = 0; i < nonterm_seq.size(); i++) {
        cout << "FOLLOW(" << nonterm_seq[i] << ") = { ";
        std::vector<std::string> &f = fellow[nonterm_seq[i]];

        bool flag = false;
        for (int k = 0; k < terms.size(); k++) {
            for (int j = 0; j < f.size(); j++) {
                if (f[j] == terms[k]) {
                    if (flag) {
                        cout << ", ";
                    }
                    cout << f[j];
                    flag = true;
                    break;
                }
            }
        }
        cout << " }\n";
    }
}

int LexicalAnalyzer::FindIndex(std::vector<std::string> &list, std::string term)
{
    for (int i = 0; i < list.size(); i++) {
        if (term == list[i]) return i;
    }

    return -1;
}

void LexicalAnalyzer::MarkPredictive()
{
    predictive = true;

    std::vector<std::string> terms(term_seq);
    terms.push_back("$");

    for (int i = 0; i < nonterm_seq.size(); i++) {
        std::vector<int> v;
        v.clear();
        for (int j = 0; j < terms.size(); j++) {
            v.push_back(-1);
        }
        ptable.push_back(v);
    }

    //cout << ptable[0][1] << endl;

    int m;
    for (int i = 0; i < nonterm_seq.size(); i++) {
        for (int j = 0; j < rules.size(); j++) {
            if (nonterm_seq[i] == rules[i][0].lexeme) {
                if (rules[i].size() > 1) {
                    bool foundE = false;
                    //cout << first[rules[i][1].lexeme].size() << "KK\n";
                    for (int k =0; k < first[rules[i][1].lexeme].size()
                            ;k++) {
                        if (first[rules[i][1].lexeme][k] != "#") {
                            m = FindIndex(terms, 
                                    first[rules[i][1].lexeme][k]);
                            if (ptable[i][m] == -1 || ptable[i][m] == j) {
                                //cout << first[rules[i][1].lexeme][k] << "! "<<j<<"\n";
                                ptable[i][m] = j;
                            } else {
                                //cout << first[rules[i][1].lexeme][k] << " ";
                                //cout << i << "-" << m << endl;
                                //cout << ptable[i][m] << endl;
                                predictive = false;
                                return;
                            }
                        } else {
                            foundE = true;
                        }
                    }

                    if (foundE) {
                        for (int k = 0; k < 
                                fellow[rules[i][0].lexeme].size();
                                k++) {
                            m = FindIndex(terms, 
                                    fellow[rules[i][0].lexeme][k]);
                            if (ptable[i][m] == -1) {
                                ptable[i][m] = j;
                            } else {
                                //cout << i << "+" << m << endl;
                                predictive = false;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void LexicalAnalyzer::PrintPredictive()
{
    if (predictive) cout << "YES\n";
    else cout << "NO\n";
}
