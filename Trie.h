#pragma once
#include <list>
#include <QString>
#include <QMap>
#include <QTextStream>
#include <QFile>
#include <QSet>
#include <forward_list>
class PrefixTree
{
private:

    struct node
    {
        typedef QPair<unsigned, node*> pair;
        //how many times this node was reached
        int value;
        //the symbol that this node holds
        QChar symbol;
        //all the children of the node
        QHash<QChar,node*> children;
        //keeps a sorted vector of the children;
        QVector<pair> mostSearched;


        //ctor
        node(QChar symbol);

        //updates the value in child Searched
        void updateMostSearched(const node* Searched);
        //returns the k msot frequent nodes
        QVector<PrefixTree::node*> GetKMostSearched(int k);

        int getVal()const;

        void increaseVal();

        void decreaseVal();

        bool isLastLetter()const;

        void MarkAsLast();

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
    QSet<QString> SuggestWords(QChar sym);
    //loads from a txt file
    void loadFromFile(const QString& FileName);
    //sets the max number of suggested words
    void SetNumberRes(int k);
    //returns if tree is empty
    bool isEmpty() const;
private:
    //result contains k words that can be reached from ptr
    void GetMostS(node* ptr, QString word, QSet<QString>& result, unsigned k) const;
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
    std::forward_list<node> all_nodes;

    size_t k;

};


