#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_InputBox_textChanged(const QString &arg1)
{
    //holds the size of the last input
    static size_t lastSize=0;
    //holds the word inputed
    static QString iWord;
    unsigned user_sug_size=0;

    //if there is no input, reset the traversal node
    if(arg1.size()==0)
    {
        tree.ResetTraversal();
    }
    else
    {

        //a word has been entered
        if(arg1.back() ==' ' && lastSize<arg1.size())
        {
            if(iWord.size()!=0)
            {


            //inserts the word into the tree
            user_input_tree.insertWord(iWord);
            //resets traversal node for next word
            tree.ResetTraversal();

            user_input_tree.ResetTraversal();
            }

            //cleans
            iWord.clear();

        }
        else
        {
            //if there was a new letter added to the input
            if(lastSize<arg1.size())
            {
                //add the letter to the word
                iWord.push_back(arg1.back());
                //a vector of all suggested words
                std::vector<QString> res;
                //a vector of all user input suggested input;
                std::vector<QString> user_res;
                //this is the text that is going to be displayed
                QString output;

                //if there are user inputed words
                if(!user_input_tree.isEmpty())
                {
                   user_res = user_input_tree.SuggestWords(arg1.back());
                }


                //if there are words in the tree
                if(!tree.isEmpty())
                {

                   res = tree.SuggestWords(arg1.back());


                if(res.size()==0 && user_res.size()==0)
                {
                    ;//if there are no suggested words, do nothing
                }
                else
                {
                    //will hold words from res
                    QString word;

                    user_sug_size = user_res.size();
                    //iterate user_res and add words to the input

                    for(size_t i = 0; i< ui->NumberOfSuggestions->value() && i<user_res.size();++i)
                    {
                        word = user_res[i];

                        word.remove(0,1);
                        //separate words with  a new line
                        output+=arg1+word+'\n';
                    }



                    //adding words from res to output
                    for(size_t i = user_sug_size;i<res.size()&&i<=ui->NumberOfSuggestions->value();++i)
                    {
                       word = res[i];
                       //removes the first letter, so there is no doubling of letters
                       word.remove(0,1);
                       //adds the word to the output
                       output += arg1+word+'\n';
                    }
                    //outputing the text
                    ui->OutputBox->setPlainText(output);
                }
             }
           }
        }
    }
    //the size of the input
    lastSize = arg1.size();
}

void MainWindow::on_LoadFromFileButton_clicked()
{


    QString FileIn = QFileDialog::getOpenFileName(this,tr("Open text"),"Computer" ,tr("Text file (*.txt)"));

    tree.loadFromFile(FileIn);

    ui->LoadedFile->setCheckState(Qt::CheckState::Checked);
    //allows input
    ui->InputBox->setReadOnly(false);
    //sets the number of results the tree will show
    tree.SetNumberRes(ui->NumberOfSuggestions->value());

    user_input_tree.SetNumberRes(ui->NumberOfSuggestions->value());

    ui->InputBox->setPlaceholderText("Start typing...");
}

void MainWindow::on_NumberOfSuggestions_valueChanged(int arg1)
{
    tree.SetNumberRes(arg1);
    user_input_tree.SetNumberRes(arg1);
}


void MainWindow::on_InputBox_returnPressed()
{
    tree.insertWord(ui->InputBox->text());
}

void MainWindow::on_ExitButton_clicked()
{
    this->close();
}
