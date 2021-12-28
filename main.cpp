#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <vector>

/**
  A resource-constrained scheduler program to minimize the latency.
  Two types of operations: addition and multiplication.
  An addition takes 1 time unit, and a multiplication takes 3 time units.
  */

using namespace std;

typedef struct node//double linked-list node structure
{

int number;//node's number
int latency;//node's operation time
int priority;//node's priority
int state;//node's state of scheduling
int ongoing;//ongoing node
string type;//node's operation type
vector<node*> predecessor;//node's predecessor. Use vector store predecessor's address
vector<node*> successor;//node's successor. Use vector store successor's address
node *nextptr;//next node address of Double linked-list
node *previousptr;//previous node address of Double linked-list

}node;

void get_file(vector<string> &, char *);//parser input file
node * push (node *, vector<string>, int);//push node's information into linked-list
void find_ps(node *, node *, vector<string>, int);//find node's predecessor and successor and push into linked-list
void list_l (node *, vector<string>, int, char *);//implementation of list algorithm
void priority_list (node *);//find priority of node
node * searching(node *,node *, int, int);//search position of linked-list

int main(int argc, char *argv[])
{

    vector<string> datafg;
    get_file(datafg, argv[1]);

    int number_node = atoi(datafg[2].c_str());

    node *firstptr = new node();//create first node of linked-list and use pointer point to first position
    node *lastptr = push(firstptr, datafg, number_node);//push node's information into linked-list and use pointer point to last position

    find_ps(firstptr, lastptr, datafg, number_node);
    priority_list(lastptr);
    list_l(firstptr, datafg, number_node, argv[2]);

    return 0;//end of the program
}

void get_file(vector<string> &dfg, char *argv)
{
    ifstream DFG (argv, ios::in);//parser in

    if(!DFG)
    {
       cerr << "File could not be opened" << endl;//print error message if the program fail to access file
       exit(1);
    }
    else
    {
       string infile;

       while(!DFG.eof())
       {
          getline(DFG,infile);
          infile = infile.substr(0,infile.find("/")-1);
          dfg.push_back(infile);
      }

    }

    int num_node = atoi(dfg[2].c_str());
    int k = 4, x = num_node + 3;

    for(int i = 4; i < num_node + 4; i++)
    {
         if(dfg[i].substr(dfg[i].find(" ")+1) == "i")
         {
             swap(dfg[i],dfg[k]);
             k++;
         }

    }

    for(int i = 4; i < num_node + 4; i++)
    {
         if(dfg[i].substr(dfg[i].find(" ")+1) == "o")
         {
                while(dfg[x].substr(dfg[x].find(" ")+1) == "o")
                {
                     x--;
                     if(x < i)
                        break;
                }

                if(x < i)
                   break;

                swap(dfg[i],dfg[x]);
                x--;
         }

    }
}

node * push (node *ptr, vector<string> dfg, int num_node)
{


    ptr->number = atoi(dfg[4].c_str());
    ptr->type = dfg[4].substr(dfg[4].find(" ")+1);
    ptr->nextptr = NULL;
    ptr->previousptr = NULL;

   for(int i = 1; i < num_node; i++)
   {
       node *previous = ptr;

       ptr->nextptr = new node();
       ptr = ptr->nextptr;

       ptr->previousptr = previous;
       ptr->number = atoi(dfg[4+i].c_str());
       ptr->type = dfg[4+i].substr(dfg[4+i].find(" ")+1);

       if(ptr->type == "+")
           ptr->latency = 1;
       else if(ptr->type == "*")
           ptr->latency = 3;

       ptr->nextptr = NULL;

   }

   return ptr;
}

void find_ps(node *ptr, node *lptr, vector<string> dfg, int num_node)
{
    int number;
    node *first = ptr;

    while(ptr != NULL)
    {

       for(int i = num_node + 4; i < dfg.size(); i++)
       {
            if(ptr->number == atoi(dfg[i].substr(dfg[i].find(" ")+1).c_str()))
            {
                number = atoi(dfg[i].substr(0,dfg[i].find(" ")).c_str());
                ptr->predecessor.push_back(searching(first, lptr, number, num_node));
            }
            if(ptr->number == atoi(dfg[i].substr(0,dfg[i].find(" ")).c_str()))
            {
                number = atoi(dfg[i].substr(dfg[i].find(" ")+1).c_str());
                ptr->successor.push_back(searching(first, lptr, number, num_node));
            }
       }

       ptr = ptr->nextptr;
    }
}

node * searching(node *ptr, node *lptr, int num, int num_node)
{
     if(num < num_node/2)
     {
        while(ptr != NULL)
        {
        if(num == ptr->number)
        {
            return ptr;
            break;
        }

          ptr = ptr->nextptr;
        }

     }
     else
     {
        while(lptr != NULL)
        {
        if(num == lptr->number)
        {
            return lptr;
            break;
        }

          lptr = lptr->previousptr;
        }

     }

}

void priority_list (node *lptr)
{

    int n_p;

    node *current = lptr;

    while(current != NULL)
    {

        if(current->type == "o")
        {
            for(int i = 0; i < current->predecessor.size(); i++)
            {
                lptr = current->predecessor[i];
                lptr->priority = lptr->latency;
            }
        }

        else
          break;

        current = current->previousptr;
        lptr = current;
    }

    node *temp;
    int t = 0;

    while(lptr != NULL)
    {
        int counter = 0;

        for(int i = 0; i < lptr->successor.size(); i++)
        {
             temp = lptr->successor[i];
             n_p = lptr->latency + temp->priority;

             if(n_p > lptr->priority)
             {
                lptr->priority = n_p;
             }
        }

        if(lptr->previousptr->type == "i")
        {
            lptr = current;
            temp = lptr;

            while(temp->type != "i")
            {
                if(temp->priority == 0)
                {
                    counter++;
                }

                temp = temp->previousptr;
            }

            if(counter == 0)
            {
                if(t == 2)
                    break;
                t++;
            }
        }
        else
        {
            lptr = lptr->previousptr;
        }
    }

}

void list_l (node *ptr, vector<string> dfg, int num_node, char *argv)
{

    ofstream out (argv, ios::out);//parser out

    const int adder = atoi(dfg[0].c_str());
    const int multiplier = atoi(dfg[1].c_str());
    int p_num = 0;

    vector<node*>ready;
    vector<string> output;
    string scheduled_o, scheduled_a, scheduled_m;

    node *first = ptr;
    node *pre;
    node *s_node;

    int time = 0;

    while(true)
    {

         ready.clear();
         scheduled_o.clear();
         scheduled_a.clear();
         scheduled_m.clear();

         ptr = first;

         int duplicate_count = 0;

         while(ptr->type != "o")
         {

            if((ptr->state == 2 && time != ptr->ongoing) || ptr->state == 3)
            {
                if(ptr->state == 2)
                {
                    scheduled_o = scheduled_o + to_string(ptr->number) + " ";
                    duplicate_count++;
                }
            }
            else if((ptr->state == 2 && time == ptr->ongoing) || ptr->type == "i")
            {
                ptr->state = 3;
            }
            else
            {
                for(int i = 0; i < ptr->predecessor.size(); i++)
                {
                    pre = ptr->predecessor[i];

                    if(pre->state == 3)
                    {
                        ptr->state = 1;

                    }
                    else
                    {
                        ptr->state = 0;
                        break;
                    }
                }

                if(ptr->state == 1)
                {
                    ready.push_back(ptr);
                }
            }
            ptr = ptr->nextptr;
         }

        for(int i = 0; i < multiplier-duplicate_count; i++)
        {
           for(int j = 0; j < ready.size(); j++)
           {

              ptr = ready[j];

              if(ptr->type == "*" && p_num < ptr->priority && ptr->state == 1)
              {
                  p_num = ptr->priority;
                  s_node = ptr;
              }

           }

           if(p_num == 0)
           {
               break;
           }
           else
           {
              scheduled_m = scheduled_m + to_string(s_node->number) + " ";
              s_node->state = 2;
              s_node->ongoing = time + s_node->latency;
              p_num = 0;
           }
        }

        for(int i = 0; i < adder; i++)
        {
           for(int j = 0; j < ready.size(); j++)
           {
              ptr = ready[j];

              if(ptr->type == "+" && p_num < ptr->priority && ptr->state == 1)
              {
                  p_num = ptr->priority;
                  s_node = ptr;
              }

           }

           if(p_num == 0)
           {
               break;
           }
           else
           {
               scheduled_a = scheduled_a + to_string(s_node->number) + " ";
               s_node->state = 3;
               p_num = 0;
           }

        }

        ptr = first;
        int counter = 0;

        while(!ptr->successor.empty())//check if scheduling is done or not
        {
            if(ptr->predecessor.empty())
            {
                ptr = ptr->nextptr;
                continue;
            }

            if(ptr->state != 3)
            {
                counter++;
            }

            ptr = ptr->nextptr;
        }


        if(counter == 0)
            break;

        output.push_back(scheduled_o+scheduled_m+scheduled_a);
        time++;
     }

     for(int i = 0; i < output.size(); i++)
     {
         if(output[i] == "\0")
            time--;
     }

     //print scheduled information into output file
     out << time << endl;

     for(int i = 0; i < output.size(); i++)
     {
         if(i == output.size() - 1)
         {
            output[i].pop_back();
            out << output[i];
         }
         else if (output[i] != "\0")
         {
            output[i].pop_back();
            out << output[i] << endl;
         }
     }
}
