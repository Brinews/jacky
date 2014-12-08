#include "ASTree.hpp"
#include <string>

/////////////////////////////////////////////////////////////////////
// Copy constructor for srcML
//
srcML::srcML(const srcML & actual) {
    tree = new ASTree( * (actual.tree));
}

/////////////////////////////////////////////////////////////////////
// Constant time swap for srcML
//
void srcML::swap(srcML & b) {
    std::string t_header = header;
    header = b.header;
    b.header = t_header;

    ASTree * temp = tree;
    tree = b.tree;
    b.tree = temp;
}

/////////////////////////////////////////////////////////////////////
// Assignment for srcML
//
srcML & srcML::operator = (srcML rhs) {
    swap(rhs);
    return *this;
}

/////////////////////////////////////////////////////////////////////
// Reads in and constructs a srcML object.
//
std::istream & operator >> (std::istream & in , srcML & src) {
    char ch;
    if (! in .eof()) in >> ch;
    src.header = readUntil( in , '>');
    if (! in .eof()) in >> ch;
    if (src.tree) delete src.tree;
    src.tree = new ASTree(category, readUntil( in , '>'));
    src.tree->read( in );
    return in;
}


/////////////////////////////////////////////////////////////////////
// Prints out a srcML object
//
std::ostream & operator << (std::ostream & out, const srcML & src) {
    if (TAGS) out << "<" << src.header << ">" << std::endl;
    src.tree->print(out, 0);
    return out;
}

/////////////////////////////////////////////////////////////////////
//  Adds in the includes and profile variables
//
void srcML::mainHeader(std::vector < std::string > & profileNames) {
    tree->mainHeader(profileNames);
}

/////////////////////////////////////////////////////////////////////
//  Adds in the includes and profile variables
//
void srcML::fileHeader(std::string & profileNames) {
    tree->fileHeader(profileNames);
}


/////////////////////////////////////////////////////////////////////
// Adds in the report to the main.
//
void srcML::mainReport(std::vector < std::string > & profileNames) {
    tree->mainReport(profileNames);
}

/////////////////////////////////////////////////////////////////////
//  Inserts a function.count() into each function body.
//
void srcML::funcCount(const std::string & profilename) {
    tree->funcCount(profilename);
}



/////////////////////////////////////////////////////////////////////
// Inserts a filename.count() for each statement.
//
void srcML::lineCount(const std::string & profilename) {
    tree->lineCount(profilename);
}




/////////////////////////////////////////////////////////////////////
// Constructs a category, token, or whitespace node for the tree.
//
ASTree::ASTree(nodes t, const std::string & s) {
    nodeType = t;
    switch (nodeType) {
    case category:
        tag = s;
        closeTag = "/" + s;
        break;
    case token:
        text = unEscape(s);
        break;
    case whitespace:
        text = s;
        break;
    }
}

/////////////////////////////////////////////////////////////////////
// Copy Constructor for ASTree
//
ASTree::ASTree(const ASTree & actual) {
    //NEED TO IMPLEMENT	
    treeCopy( * this, actual);
}


/////////////////////////////////////////////////////////////////////
// Constant time swap for ASTree
//
void ASTree::swap(ASTree & b) {
    //NEED TO IMPLEMEN
	std::list<ASTree*> lst;
	lst=child;
	child=b.child;
	b.child=lst;    

	std::swap(nodeType, b.nodeType);
    std::swap(tag, b.tag);
    std::swap(closeTag, b.closeTag);
    std::swap(text, b.text);
}

/////////////////////////////////////////////////////////////////////
// Assignment for ASTree
//
ASTree & ASTree::operator = (ASTree rhs) {
    treeCopy( * this, rhs);
    return *this;
}




/////////////////////////////////////////////////////////////////////
// Returns an this->child[i] where (this->child[i]->tag == tagName)
//
ASTree * ASTree::getChild(std::string tagName) {
    std::list < ASTree * > ::iterator ptr = child.begin();
    while ((( * ptr)->tag != tagName) && (ptr != child.end())) {
        ++ptr;
    }
    return *ptr;
}



/////////////////////////////////////////////////////////////////////
// Returns the full name of a <name> node.
//
std::string ASTree::getName() const {
    std::string result;
    if (child.front()->tag != "name") {
        result = child.front()->text; //A simple name (e.g., main)
    } else { //A complex name (e.g., stack::push).
        result = child.front()->child.front()->text;
        result += "::";
        result += child.back()->child.front()->text;
    }
    return result;
}


ASTree::ASTree(nodes type, const std::string & strTag, const std::string & strText) {
    nodeType = type;
    tag = strTag;
    closeTag = strTag;
    text = strText;
}

/////////////////////////////////////////////////////////////////////
//  Adds in the includes and profile variables in a main file.
//

void ASTree::mainHeader(std::vector < std::string > & profileNames) {
    //NEED TO IMPLEMENT
    //Skip down a couple lines.
    std::list < ASTree * > ::iterator pit;
    pit = child.begin();
    while (( * pit)->tag != "function")
        pit++;
    //Add the include for profile.hpp	
    ASTree * node = new ASTree(whitespace, "\n");child.insert(pit, node);
	node = new ASTree(whitespace, "\n");child.insert(pit, node);
    child.insert(pit, getIncludeNode(""));
    //For each file profile name, add a new node with a profile declaration.    
    unsigned int i;
    for (i = 0; i < profileNames.size(); i++) {
        node = new ASTree(whitespace, "\n");child.insert(pit, node);       
		child.insert(pit, getProfilerNode(profileNames[i]));
    }
    node = new ASTree(whitespace, "\n");
    child.insert(pit, node);
}


/////////////////////////////////////////////////////////////////////
//  Adds in the includes and profile variables for non-main files
//
void ASTree::fileHeader(std::string & profileNames) {

    //NEED TO IMPLEMENT
    //Skip down a couple lines.
    std::list < ASTree * > ::iterator pit;
    pit = child.begin();
    while (( * pit)->tag != "function")
        pit++;
    //Add the include for profile.hpp
    ASTree * node = new ASTree(whitespace, "\n");
    child.insert(pit, node);
   
	child.insert(pit, getIncludeNode(""));
    // extern declaration.    

    node = new ASTree(whitespace, "\n");child.insert(pit, node);
    
	child.insert(pit, getDeclareNode(profileNames));

    node = new ASTree(whitespace, "\n");child.insert(pit, node);
    node = new ASTree(whitespace, "\n");child.insert(pit, node);
}


/////////////////////////////////////////////////////////////////////
// Adds in the report to the main.
// Assumes only one return at end of main body.
//
void ASTree::mainReport(std::vector < std::string > & profileNames) {
    //NEED TO IMPLEMENT    
    //Find the main - function with name of "main"
    std::list < ASTree * > ::iterator pit;
    pit = child.begin();
    while (pit != child.end()) {
        while (( * pit)->tag != "function" && pit != child.end())
            pit++;
        std::list < ASTree * > ::iterator pitFuncChild;
        pitFuncChild = ( * pit)->child.begin();
        while (( * pitFuncChild)->tag != "name" && pitFuncChild != ( * pit)->child.end())
            pitFuncChild++;
        if (pitFuncChild != ( * pit)->child.end() && ( * pitFuncChild)->child.size() > 0 && ( * ( * pitFuncChild)->child.begin())->text == "main")
            break;
        pit++;
    }
    
    std::list < ASTree * > ::iterator pitBlock;
    pitBlock = ( * pit)->child.end();
    while (pitBlock != ( * pit)->child.begin()) {
        pitBlock--;
        if (( * pitBlock)->tag == "block")
            break;
    }

    std::list < ASTree * > ::iterator pitReturn;
    pitReturn = ( * pitBlock)->child.end();
    while (pitReturn != ( * pitBlock)->child.begin()) {
        pitReturn--;
        if (( * pitReturn)->tag == "return")
            break;
    }
    //insert report
    unsigned int i;
    ASTree * node;
    for (i = 0; i < profileNames.size(); i++) {
        node = new ASTree(whitespace, "\n");( * pitBlock)->child.insert(pitReturn, node);        
		( * pitBlock)->child.insert(pitReturn, getStatiticsNode(profileNames[i]));
    }
    node = new ASTree(whitespace, "\n");
    ( * pitBlock)->child.insert(pitReturn, node);
}


/////////////////////////////////////////////////////////////////////
// Adds in a line to count the number of times each function is executed.
//  Assumes no nested functions.
//
void ASTree::funcCount(const std::string & profileNames) {

    //NEED TO IMPLEMENT
    std::list < ASTree * > ::iterator pit;
    pit = child.begin();
    while (pit != child.end()) {
        while (pit != child.end() && ( * pit)->tag != "function")
            pit++;
        if (pit == child.end())
            break;
        ASTree * pFunc = * pit;
        
        std::list < ASTree * > ::iterator pitBlock;
        pitBlock = pFunc->child.begin();
        while (pitBlock != pFunc->child.end()) {
            if (( * pitBlock)->tag == "block")
                break;
            pitBlock++;
        }
        
        ASTree * pBlock = * pitBlock;
        std::list < ASTree * > ::iterator pitPar;
        pitPar = pBlock->child.begin();
        while (pitPar != pBlock->child.end()) {
            if (( * pitPar)->text == "{")
                break;
            pitPar++;
        }

        
        pitPar++;
        ASTree * node;        
		pBlock->child.insert(pitPar, getFunctionNode(pFunc,profileNames));
        node = new ASTree(whitespace, "\n");pBlock->child.insert(pitPar, node);

        pit++;
    }
}

/////////////////////////////////////////////////////////////////////
// Adds in a line to count the number of times each statement is executed.
//   No breaks, returns, throw etc.
//   Assumes all construts (for, while, if) have { }.
//

void ASTree::insertStmt(ASTree & tree, const std::string & profileNames) {
    std::list < ASTree * > ::iterator pit;
    pit = tree.child.end();
    while (pit != tree.child.begin()) {
        pit--;
        if (( * pit)->tag == "expr_stmt" || ( * pit)->tag == "decl_stmt" || ( * pit)->tag == "empty_stmt") {
            std::list < ASTree * > ::iterator pit2;
            pit2 = pit;
            pit2++;
            ASTree * node = new ASTree(category, "expr_stmt"); {
                ASTree * cd = new ASTree(category, "expr"); {
                    ASTree * cd1 = new ASTree(category, "name"); {
                        ASTree * cd2 = new ASTree(token, "", profileNames);
                        cd1->child.push_back(cd2);
                    }
                    cd->child.push_back(cd1);

                    cd1 = new ASTree(token, "", ".");
                    cd->child.push_back(cd1);

                    cd1 = new ASTree(category, "call"); {
                        ASTree * cd2 = new ASTree(category, "name"); {
                            ASTree * cd3 = new ASTree(token, "", "count");
                            cd2->child.push_back(cd3);
                        }
                        cd1->child.push_back(cd2);

                        cd2 = new ASTree(category, "argument_list"); {
                            ASTree * cd3 = new ASTree(token, "", "(");
                            cd2->child.push_back(cd3);

                            cd3 = new ASTree(category, "argument"); {
                                ASTree * cd4 = new ASTree(category, "expr"); {
                                    ASTree * cd5 = new ASTree(category, "name"); {
                                        ASTree * cd6 = new ASTree(token, "", "__LINE__");
                                        cd5->child.push_back(cd6);
                                    }
                                    cd4->child.push_back(cd5);
                                }
                                cd3->child.push_back(cd4);
                            }
                            cd2->child.push_back(cd3);

                            cd3 = new ASTree(token, "", ")");
                            cd2->child.push_back(cd3);
                        }
                        cd1->child.push_back(cd2);
                    }
                    cd->child.push_back(cd1);
                }
                node->child.push_back(cd);

                cd = new ASTree(token, "", ";");
                node->child.push_back(cd);
            }
            tree.child.insert(pit2, node);

            node = new ASTree(whitespace, "\n");
            tree.child.insert(pit2, node);
        } else if (( * pit)->child.size() > 0)
            insertStmt( * * pit, profileNames);
    }
}

void ASTree::lineCount(const std::string & profileNames) {
    insertStmt( * this, profileNames);
}


/////////////////////////////////////////////////////////////////////
// Read in and construct ASTree
// REQUIRES: '>' was previous charater read 
//           && this == new ASTree(category, "TagName")
//
//
std::istream & ASTree::read(std::istream & in ) {
    ASTree * subtree;
    std::string temp, Lws, Rws;
    char ch;
    if (! in .eof()) in .get(ch);
    while (! in .eof()) {
        if (ch == '<') { //Found a tag 
            temp = readUntil( in , '>');
            if (temp[0] == '/') {
                closeTag = temp;
                break; //Found close tag, stop recursion
            }
            subtree = new ASTree(category, temp); //New subtree
            subtree->read( in ); //Read it in
            in .get(ch);
            child.push_back(subtree); //Add it to child
        } else { //Found a token
            temp = std::string(1, ch) + readUntil( in , '<'); //Read it in.
            std::vector < std::string > tokenList = tokenize(temp);
            for (std::vector < std::string > ::const_iterator i = tokenList.begin(); i != tokenList.end(); ++i) {
                if (isspace(( * i)[0])) {
                    subtree = new ASTree(whitespace, * i);
                } else {
                    subtree = new ASTree(token, * i);
                }
                child.push_back(subtree);
            }
            ch = '<';
        }
    }
    return in;
}


/////////////////////////////////////////////////////////////////////
// Print an ASTree 
// REQUIRES: indent >= 0
//
std::ostream & ASTree::print(std::ostream & out, int indent) const {
    if (TAGS) out << std::setw(indent) << " ";
    if (TAGS) out << "<" << tag << ">" << std::endl;
    for (std::list < ASTree * > ::const_iterator i = child.begin(); i != child.end(); ++i) {
        switch (( * i)->nodeType) {
        case category:
            ( * i)->print(out, indent + 4);
            break;
        case token:
            //out << std::setw(indent) << " ";
            out << ( * i)->text; // << std::endl;
            break;
        case whitespace:
            out << ( * i)->text;
            break;
        }
    }
    if (TAGS) out << std::setw(indent) << " ";
    if (TAGS) out << "<" << closeTag << ">" << std::endl;
    return out;
}




/////////////////////////////////////////////////////////////////////
// Utilities
//

bool isStopTag(std::string tag) {
    if (tag == "decl_stmt") return true;
    if (tag == "argument_list") return true;
    if (tag == "init") return true;
    if (tag == "condition") return true;
    if (tag == "cpp:include") return true;
    if (tag == "comment type\"block\"") return true;
    if (tag == "comment type\"line\"") return true;
    if (tag == "macro") return true;

    return false;
}

/////////////////////////////////////////////////////////////////////
// Reads until a key is encountered.  Does not include ch.
// REQUIRES: in.open()
// ENSURES: RetVal[i] != key for all i.
//
std::string readUntil(std::istream & in , char key) {
    std::string result;
    char ch; in .get(ch);
    while (! in .eof() && (ch != key)) {
        result += ch; in .get(ch);
    }
    return result;
}

/////////////////////////////////////////////////////////////////////
// Converts escaped XML charaters back to charater form
// REQUIRES: s == "&lt;"
// ENSURES:  RetVal == "<"
//
std::string unEscape(std::string s) {
    unsigned pos = 0;
    while ((pos = s.find("&gt;")) != s.npos) {
        s.replace(pos, 4, ">");
    }
    while ((pos = s.find("&lt;")) != s.npos) {
        s.replace(pos, 4, "<");
    }
    while ((pos = s.find("&amp;")) != s.npos) {
        s.replace(pos, 5, "&");
    }
    return s;
}


/////////////////////////////////////////////////////////////////////
// Given: s == "   a + c  "
// RetVal == {"   ", "a", " ", "+", "c", " "}  
//
std::vector < std::string > tokenize(const std::string & s) {
    std::vector < std::string > result;
    std::string temp = "";
    unsigned i = 0;
    while (i < s.length()) {
        while (isspace(s[i]) && (i < s.length())) {
            temp.push_back(s[i]);
            ++i;
        }
        if (temp != "") {
            result.push_back(temp);
            temp = "";
        }
        while (!isspace(s[i]) && (i < s.length())) {
            temp.push_back(s[i]);
            ++i;
        }
        if (temp != "") {
            result.push_back(temp);
            temp = "";
        }
    }
    return result;
}

void ASTree::treeCopy(ASTree & tree1, const ASTree & tree2) {
    tree1.nodeType = tree2.nodeType;
    tree1.tag = tree2.tag;
    tree1.closeTag = tree2.closeTag;
    tree1.text = tree2.text;

    tree1.child.clear();
    std::list < ASTree * > ::const_iterator pit;
    for (pit = tree2.child.begin(); pit != tree2.child.end(); pit++) {
        if ( * pit != NULL) {
            ASTree * ptr = new ASTree();
            treeCopy( * ptr, * * pit);
            tree1.child.push_back(ptr);
        } else
            tree1.child.push_back(NULL);
    }
}

ASTree* ASTree::getIncludeNode(std::string strname)
{
    ASTree* newnode = new ASTree(category, "cpp:include"); 
	{
        ASTree * cd = new ASTree(token, "", "#");
        newnode->child.push_back(cd);
        cd = new ASTree(category, "cpp:directive");
        cd->child.push_back(new ASTree(token, "", "include"));
        newnode->child.push_back(cd);
        cd = new ASTree(category, "cpp:file");
        cd->child.push_back(new ASTree(token, "", "\"profile.hpp\""));
        newnode->child.push_back(cd);
    }
	return newnode;
}
ASTree* ASTree::getProfilerNode(std::string strname)
{
	 ASTree* newnode = new ASTree(category, "decl_stmt"); {
            ASTree * cd = new ASTree(category, "decl"); {
                ASTree * cd1 = new ASTree(category, "type"); {
                    ASTree * cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "profile");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);

                cd1 = new ASTree(token, "", " ");
                cd->child.push_back(cd1);

                cd1 = new ASTree(category, "name"); {
                    ASTree * cd2 = new ASTree(token, "", strname);
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);
                cd1 = new ASTree(category, "argument_list"); {
                    ASTree * cd2 = new ASTree(token, "", "(");
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(category, "argument"); {
                        ASTree * cd3 = new ASTree(category, "expr"); {
							std::string newName = strname;
                            int j = newName.length() - 1;
                            while (newName[j] != '_' && j >= 0) j--;
                            if (j >= 0)
                                newName[j] = '.';
                            ASTree * cd4 = new ASTree(token, "", "\"" + newName + "\"");
                            cd3->child.push_back(cd4);
                        }
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(token, "", ")");
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);
            }
            newnode->child.push_back(cd);
            cd = new ASTree(token, "", ";");
            newnode->child.push_back(cd);
        }
	 return newnode;
}

ASTree* ASTree::getDeclareNode(std::string strname)
{
	ASTree* newnode = new ASTree(category, "decl_stmt"); {
        ASTree * cd = new ASTree(category, "decl"); {
            ASTree * cd1 = new ASTree(category, "type"); {
                ASTree * cd2 = new ASTree(category, "name"); {
                    ASTree * cd3 = new ASTree(token, "", "extern");
                    cd2->child.push_back(cd3);
                }
                cd1->child.push_back(cd2);

                cd2 = new ASTree(token, "", " ");
                cd1->child.push_back(cd2);

                cd2 = new ASTree(category, "name"); {
                    ASTree * cd3 = new ASTree(token, "", "profile");
                    cd2->child.push_back(cd3);
                }
                cd1->child.push_back(cd2);
            }
            cd->child.push_back(cd1);

            cd1 = new ASTree(token, "", " ");
            cd->child.push_back(cd1);

            cd1 = new ASTree(category, "name"); {
                ASTree * cd2 = new ASTree(token, "", strname);
                cd1->child.push_back(cd2);
            }
            cd->child.push_back(cd1);
        }
        newnode->child.push_back(cd);
        cd = new ASTree(token, "", ";");
        newnode->child.push_back(cd);
    }
	return newnode;
}

ASTree* ASTree::getStatiticsNode(std::string strname)
{
	ASTree* newnode = new ASTree(category, "expr_stmt"); {
            ASTree * cd = new ASTree(category, "expr"); {
                ASTree * cd1 = new ASTree(category, "name"); {
                    ASTree * cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "std");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(token, "", "::");
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "cout");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);

                cd1 = new ASTree(token, "", " << ");
                cd->child.push_back(cd1);

                cd1 = new ASTree(category, "name"); {
                    ASTree * cd2 = new ASTree(token, "", strname);
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);

                cd1 = new ASTree(token, "", " << ");
                cd->child.push_back(cd1);

                cd1 = new ASTree(category, "name"); {
                    ASTree * cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "std");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(token, "", "::");
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "endl");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);
            }
            newnode->child.push_back(cd);
            cd = new ASTree(token, "", ";");
            newnode->child.push_back(cd);
        }
	return newnode;
}

ASTree* ASTree::getFunctionNode(ASTree*p,std::string strname)
{
	ASTree* newnode = new ASTree(category, "expr_stmt"); {
            ASTree * cd = new ASTree(category, "expr"); {
                ASTree * cd1 = new ASTree(category, "name"); {
                    ASTree * cd2 = new ASTree(token, "", strname);
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);

                cd1 = new ASTree(token, "", ".");
                cd->child.push_back(cd1);

                cd1 = new ASTree(category, "call"); {
                    ASTree * cd2 = new ASTree(category, "name"); {
                        ASTree * cd3 = new ASTree(token, "", "count");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);

                    cd2 = new ASTree(category, "argument_list"); {
                        ASTree * cd3 = new ASTree(token, "", "(");
                        cd2->child.push_back(cd3);

                        cd3 = new ASTree(category, "argument"); {
                            ASTree * cd4 = new ASTree(category, "expr"); {
                                ASTree * cd5 = new ASTree(category, "name"); {
                                    ASTree * cd6 = new ASTree(token, "", "__LINE__");
                                    cd5->child.push_back(cd6);
                                }
                                cd4->child.push_back(cd5);
                            }
                            cd3->child.push_back(cd4);
                        }
                        cd2->child.push_back(cd3);

                        cd3 = new ASTree(token, "", ",");
                        cd2->child.push_back(cd3);

                        cd3 = new ASTree(category, "argument"); {
                            ASTree * cd4 = new ASTree(category, "expr"); {
                                
                                std::string funcName;
                                std::list < ASTree * > ::iterator pitFuncName = p->child.begin();
                                while (( * pitFuncName)->tag != "name")
                                    pitFuncName++;
                                funcName = ( * ( * pitFuncName)->child.begin())->text;
                                ASTree * cd5 = new ASTree(token, "", "\"" + funcName + "\"");
                                cd4->child.push_back(cd5);
                            }
                            cd3->child.push_back(cd4);
                        }
                        cd2->child.push_back(cd3);

                        cd3 = new ASTree(token, "", ")");
                        cd2->child.push_back(cd3);
                    }
                    cd1->child.push_back(cd2);
                }
                cd->child.push_back(cd1);
            }
            newnode->child.push_back(cd);

            cd = new ASTree(token, "", ";");
            newnode->child.push_back(cd);
        }
	return newnode;
}
