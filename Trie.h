#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <list>
#include <fstream>
#include <iostream>
#include <fstream>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QFile>
class PrefixTree
{
private:

	struct node
	{
		typedef std::pair<unsigned, node*> pair;
        //holds if node is the last letter of a word
        bool isLastLetter;
        //the symbol that this node holds
        QChar symbol;
        //all the children of the node
        QMap<QChar,node*> children;
		//keeps a sorted vector of the children;
		std::vector<pair> mostSearched;

		//how many times this node was reached
		unsigned value;
		//ctor
        node(QChar symbol);

		//updates the value in child Searched
		void updateMostSearched(const node* Searched);
		//returns the k msot frequent nodes
		std::queue<node*> GetKMostSearched(size_t k);
		

	};
public:
	PrefixTree();
	~PrefixTree();

public:
    //resets traveral node to be the root
    void ResetTraversal();
    //inserts a word into the tree
    void insertWord(const QString& word);
    //returns a string containing all suggested words
    std::vector<QString> SuggestWords(QChar sym);
    //loads from a txt file
    void loadFromFile(const QString& FileName);
    //sets the max number of suggested words
    void SetNumberRes(int k);
    //returns if tree is empty
    bool isEmpty() const;
private:
    //result contains k words that can be reached from ptr
    void GetMostS(node* ptr, QString word, std::vector<QString>& result, unsigned k) const;
    //creates a node that holds symbol
    node* createNode(QChar symbol, node* parrent);
    //returns the child of str_node that contains symbol
    //if there is no child returns nullptr
    node* travelForward( node* str_node, QChar symbol);

private:
    //the root of the prefix tree
	node* root;
    //node for traversal of the prefix tree
    node* traversal_node;
    //storage of all nodes, in a list so pointers & iterators won't break when adding nodes
    std::list<node> all_nodes;

    bool empty;

    size_t k;

};


