#include "Trie.h"



PrefixTree::PrefixTree():empty(true)
{
    //creates a root node
    all_nodes.push_back(node('#'));
    //setting root to point to the front of the list
	root = &all_nodes.front();
    //setting traversal node to be root
    traversal_node = root;
}


PrefixTree::~PrefixTree()
{
}

void PrefixTree::insertWord(const QString & word)
{

	if (word.size() == 0)
		throw std::invalid_argument("Word is empty\n");
    //iterating the letters of word
    int w_index = 0;
    //tree is no longer empty
    empty = false;
    //the first parrent is always the root
	node * parrent = root;
    //iterator for the children of the parrent
    QMap<QChar, node*>::iterator it;


    while (w_index < word.size())
	{
        //finds the child containg the letter in w_index pos in word
		it = parrent->children.find(word[w_index]);
		
		//if there is no child
		if (it == parrent->children.end())
		{
                //the parrent becomes the created node
				parrent = createNode(word[w_index++], parrent);
		}
		else// there is a child
		{
			//update the parrent most searched children
            parrent->updateMostSearched(it.value());
			//the child becomes the parrent
            parrent = it.value();
			//increase word index
			w_index++;
		}

	}
    //sets the parrent as last letter of word
    parrent->isLastLetter = true;
}
void PrefixTree::ResetTraversal()
{
    //resets travesal node to be the root
    traversal_node = root;
}

std::vector<QString> PrefixTree::SuggestWords(QChar sym)
{
    //res will hold all suggested words
    std::vector<QString> res;
    //word will hold a single suggested word
    QString word;
    //finds the node containing sym
    traversal_node = travelForward(traversal_node, sym);

    if(traversal_node == nullptr)
    {
        //resets res
        res.clear();
        //returns empthy res
        return  res;
    }
    //adds the k msot searched words to res
    GetMostS(traversal_node, word, res,k);
	
    //retuns result
	return res;
}

bool PrefixTree::isEmpty() const
{
    return  empty;
}


void PrefixTree::GetMostS(node * ptr, QString word, std::vector<QString> & result, unsigned k) const
{
   if(ptr==nullptr)
       return;
    //adds the current symbol to the suggested word
    word.push_back(ptr->symbol);
    //if ptr is a node that holds last letter
    if (ptr->isLastLetter)
	{
        //word is added to the result
        result.push_back(word);
	}
    //the queue of children of ptr
	std::queue<node*> rQueue = ptr->GetKMostSearched(k);

	while (!rQueue.empty())
	{
        //k here is one so we get k results;
        GetMostS(rQueue.front(), word, result, k-rQueue.size());
        //removes the front of the queue
        rQueue.pop();
	}
}

PrefixTree::node* PrefixTree::createNode(QChar symbol, node * parrent)
{
    //check if there is a child that holds the symbol
    if(parrent->children.find(symbol)==parrent->children.end())
    {
        //add the node to the list of nodes
        all_nodes.push_back(node(symbol));
        //gets a pointer to that node
        node * ptr = &all_nodes.back();
        //adds the pointer to the parrent node
        parrent->children[symbol] = ptr;

        //updates the most searched children of the parrent
        parrent->updateMostSearched(ptr);
        //retunrs the node
        return ptr;
    }
    else//a child exists, there is notting to create
    {
        return  nullptr;
    }


}

PrefixTree::node * PrefixTree::travelForward( node * str_node, QChar symbol)
{

	if (str_node == nullptr)
        return  nullptr;
    //if str_node is a dummy node
    if (symbol == '#')
		return nullptr;

    //iterator for the children of str_node
    QMap<QChar, node*>::iterator it = str_node->children.find(symbol);

    //if str_node has no children
	if (it == str_node->children.end())
		return nullptr;

    //returns the child
    return it.value();
}



PrefixTree::node::node(QChar symbol):symbol(symbol), isLastLetter(false), value(0)
{
	;
}

void PrefixTree::node::updateMostSearched(const node * Searched)
{
	bool found = false;
    //itarates the mostSearched children
	for (std::pair<unsigned, node*>& p : mostSearched)
	{
        //if the child is found
		if (p.second == Searched)
		{
			found = true;
            //increase the times searched
			p.first++;
            //exits the loop
            break;
		}
	}
    //if the child is not found, adds the child to the most searched vector
	if (!found)
		mostSearched.push_back(std::pair<unsigned, node*>(const_cast<unsigned&>(Searched->value),const_cast<node*>(Searched)));
    //sorts mostSearched
	std::sort(mostSearched.begin(), mostSearched.end());
}

std::queue<PrefixTree::node*> PrefixTree::node::GetKMostSearched(size_t k)
{
    //the queue to be filled with the k most searched
	std::queue<node*> res;
    //iterating trough the most searched vector
	for (size_t i = 0; i < k && i < mostSearched.size(); ++i)
	{
		res.push(mostSearched[i].second);
	}

	return res;


}

void PrefixTree::SetNumberRes(int k)
{

    this->k = k;

}


void PrefixTree::loadFromFile(const QString& FileName)
{

    QFile inFile(FileName);

    QString word;
    //checks if the file can be opened in ReadOnly & text modes
    if(!inFile.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }
    //stream obj
    QTextStream in(&inFile);

        //while there is data in the stream
        while(!in.atEnd())
        {
            //resets the input word
            word.clear();
            //reads a word from the file/stream
            in>>word;
            //inserts the word in the prefix tree
            insertWord(word);
        }
    inFile.close();


}

