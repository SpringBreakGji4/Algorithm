#include <iostream>
using namespace std;

class Trie {
	public:
    	struct TrieNode{
        	bool isWord;
        	TrieNode *child[26];   
        	TrieNode(){
            	isWord = false;
            	for(int i=0 ; i<26 ; i++){
                	child[i] = NULL;
            	}
        	}
    	};

    	TrieNode *root;
    	Trie() {
        	root = new TrieNode();    
    	}
    
    	void insert(string word) {
        	TrieNode *n = root;
        	for(auto &c : word){
        	    int i = c - 'a';
        	    if(!n->child[i]){
        	        n->child[i] = new TrieNode();
        	    }
        	    n = n->child[i];
        	}
        	n->isWord = true;
    	}
    
    	bool search(string word) {
    	    TrieNode *n = root;
    	    for(auto &c : word){
    	        int i = c - 'a';
    	        if(!n->child[i]){
    	            return false;
    	        }
    	        n = n->child[i];
    	    }
    	    return n->isWord;
    	}
    
    	bool startsWith(string prefix) {
    	    TrieNode *n = root;
    	    for(auto &c : prefix){
    	        int i = c - 'a';
    	        if(!n->child[i]){
    	            return false;
    	        }
    	        n = n->child[i];
        	}
        	return true;
    	}
};

int main(){
	Trie* obj = new Trie();
	cout << "insert apple" << endl;
	obj->insert("apple");
	cout << "search apple ... ";
	if(obj->search("apple")){
		cout << "Found!"<<endl;
	}
	else{
		cout << "not found ..."<<endl;
	}
	cout << "search app ...";
	if(obj->search("app")){
		cout << "Found!"<<endl;
	}
	else{
		cout << "not found ..."<<endl;
	}

	cout << "find start with app ...";
	if(obj->startsWith("app")){
		cout << "Found!" << endl;
	}
	else{
		cout << "not found ..."<<endl;
	}

	cout << "insert app"<<endl;
	obj->insert("app");
	cout << "search app ...";
	if(obj->search("app")){
		cout << "Found!"<<endl;
	}
	else{
		cout << "not found ..."<<endl;
	}
	
	return 0;
}
