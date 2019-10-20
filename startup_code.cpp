#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{
    
private:
    string Node_Name;  // Variable name
    vector<int> Children; // Children of a particular node - these are index of nodes in graph.
    vector<string> Parents; // Parents of a particular node- note these are names of parents
    int nvalues;  // Number of categories a variable represented by this node can take
    vector<string> values; // Categories of possible values
    vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    
public:
    // Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
    {
        Node_Name=name;
        
        nvalues=n;
        values=vals;
        
        
    }
    string get_name()
    {
        return Node_Name;
    }
    vector<int> get_children()
    {
        return Children;
    }
    vector<string> get_Parents()
    {
        return Parents;
    }
    vector<float> get_CPT()
    {
        return CPT;
    }
    int get_nvalues()
    {
        return nvalues;
    }
    vector<string> get_values()
    {
        return values;
    }
    void set_CPT(vector<float> new_CPT)
    {
        CPT.clear();
        CPT=new_CPT;
    }
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
    
    
    
};


// The whole network represted as a list of nodes
class network{
    
    list <Graph_Node> Pres_Graph;
    
public:
    int addNode(Graph_Node node)
    {
        Pres_Graph.push_back(node);
        return 0;
    }
    
    
    int netSize()
    {
        return Pres_Graph.size();
    }
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
    // get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt;
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
        
        cout<<"node not found\n";
        return listIt;
    }
    
    
};

network read_network()
{
    network Alarm;
    string line;
    int find=0;
    ifstream myfile;
    myfile.open("alarm.bif.txt");
    string temp;
    string name;
    vector<string> values;
    
    if(!myfile){
        cout<< "Unable to open file";
        exit(1);
    }
    while (! myfile.eof() )
    {
        stringstream ss;
        getline (myfile,line);
        
        
        ss.str(line);
        ss>>temp;
        
        
        if(temp.compare("variable")==0)
        {
            
            ss>>name;
            getline (myfile,line);
            
            stringstream ss2;
            ss2.str(line);
            for(int i=0;i<4;i++)
            {
                
                ss2>>temp;
                
                
            }
            values.clear();
            while(temp.compare("};")!=0)
            {
                values.push_back(temp);
                
                ss2>>temp;
            }
            Graph_Node new_node(name,values.size(),values);
            int pos=Alarm.addNode(new_node);
            
            
        }
        else if(temp.compare("probability")==0)
        {
            
            ss>>temp;
            ss>>temp;
            
            list<Graph_Node>::iterator listIt;
            list<Graph_Node>::iterator listIt1;
            listIt=Alarm.search_node(temp);
            int index=Alarm.get_index(temp);
            ss>>temp;
            values.clear();
            while(temp.compare(")")!=0)
            {
                listIt1=Alarm.search_node(temp);
                listIt1->add_child(index);
                values.push_back(temp);
                
                ss>>temp;
                
            }
            listIt->set_Parents(values);
            getline (myfile,line);
            stringstream ss2;
            
            ss2.str(line);
            ss2>> temp;
            
            ss2>> temp;
            
            vector<float> curr_CPT;
            string::size_type sz;
            while(temp.compare(";")!=0)
            {
                
                curr_CPT.push_back(atof(temp.c_str()));
                
                ss2>>temp;
                
                
                
            }
            
            listIt->set_CPT(curr_CPT);
            
            
        }
        else
        {
            
        }
        
        
        
        
        
    }
    
    if(find==1)
        myfile.close();
    
    
    return Alarm;
}


int sampling(vector<float> x){
    srand(time(0));
    float sum = 0;
    for(int i=0;i<x.size();i++){
        sum = sum + x[i];
    }
    if(sum ==0){
        return 0;
    }
    float nor = 1.0/sum;
    for(int i=0;i<x.size();i++){
        x[i] = nor*x[i]*1000;
    }
    int a = rand()%1000;
    //cout << a << " ";
    int check =0;
    for(int i=0;i<x.size();i++){
        check = check + x[i];
        if(a < check) {
            return i;
        }
    }
    return x.size() - 1;
}

vector<vector<string> > readFile(string fileName,int n){
    fstream infile;
    infile.open(fileName);
    if(!infile){
        cout<< "Unable to open file";
        exit(1);
    }
    string temp;
    vector<vector<string> > data;
    bool you = false;
    while(true){
        vector<string> tuple;
        for(int i=0;i<n;i++){
            if(infile>>temp){
                temp = temp.substr(1,temp.length()-2);
                tuple.push_back(temp);
            }
            else{
                you = true;
            }
        }
        if(you){
            break;
        }
        data.push_back(tuple);
    }
    infile.close();
    return data;
}



vector<vector<int> > random_CPT(network Alarm,vector<string>  input_data, int missing_index,int i,vector<vector<int> > CPT){
    Graph_Node node = (*(Alarm.get_nth_node(missing_index)));
    vector<string> values = node.get_values();
    int ra = (rand())%(values.size());
    input_data[missing_index] = values[ra].substr(1,values[ra].length()-2);
    //vector<vector<int> > CPT;
    vector<pair<string,string > > mapping;
    for(int j=0;j<input_data.size();j++){
        mapping.push_back(make_pair((*(Alarm.get_nth_node(j))).get_name(),input_data[j]));
    }
    for(int k=0;k<Alarm.netSize();k++){
        Graph_Node current = (*(Alarm.get_nth_node(k)));
        vector<string> all_parents = current.get_Parents();
        if(all_parents.size() == 0){
            vector<int > vecto(current.get_nvalues(),0);
            if(i==0){
                CPT.push_back(vecto);
            }
            vector<string> values = current.get_values();
            for(int qe = 0;qe < mapping.size();qe++){
                if(mapping[qe].first == current.get_name()){
                    for(int rt = 0;rt<values.size();rt++){
                        if(mapping[qe].second == values[rt].substr(1,values[rt].size()-2)){
                            CPT[k][rt] = CPT[k][rt] + 1;
                        }
                    }
                }
            }
            continue;
        }
        int cpt_i_length = ((*(Alarm.search_node(all_parents[all_parents.size()-1]))).get_nvalues());
        vector<int> for_binary;
        for_binary.push_back(1);
        for(int p=all_parents.size()-2;p>=0;p--){
            for_binary.push_back(cpt_i_length);
            cpt_i_length = cpt_i_length* ((*(Alarm.search_node(all_parents[p]))).get_nvalues());
        }
        for_binary.push_back(cpt_i_length);
        cpt_i_length = cpt_i_length * current.get_nvalues();
        vector<int> vect(cpt_i_length,0);
        int index_for_cpt = 0;
        for(int l=0;l<all_parents.size();l++){
            for(int m=0;m<mapping.size();m++){
                if(mapping[m].first == all_parents[l]){
                    vector<string> values = (*(Alarm.search_node(all_parents[l]))).get_values();
                    for(int q =0;q<values.size();q++){
                        if(mapping[m].second == values[q].substr(1,values[q].length()-2)){
                            int pwd = (for_binary.size()) - 2 - l;
                            index_for_cpt = index_for_cpt + q*for_binary[pwd];
                        }
                    }
                }
            }
        }
        for(int l=0;l<mapping.size();l++){
            string namese = current.get_name();
            if(namese == mapping[l].first){
                vector<string> values = current.get_values();
                for(int pl = 0;pl < values.size(); pl++){
                    if(values[pl].substr(1,values[pl].size()-2) == mapping[l].second){
                        int pwd  = (for_binary.size()) - 1;
                        index_for_cpt = index_for_cpt + pl * for_binary[pwd];
                    }
                }
            }
        }
        if(i==0){
            CPT.push_back(vect);
        }
        CPT[k][index_for_cpt] = CPT[k][index_for_cpt] + 1;
    }
    return CPT;
    
    //return ret_CPT;
}

vector<vector<int> > find_new_CPT(network Alarm,vector<vector<float> > CPT_old,vector<string> input_data, int missing_index, int i, vector<vector<int> > CPT){
    Graph_Node current = (*(Alarm.get_nth_node(missing_index)));
    vector<string> all_parents = current.get_Parents();
    vector<pair<string,string> > mapping;
    for(int j=0;j<input_data.size();j++){
        for(int k=0;k<all_parents.size();k++){
            if(all_parents[k] == (*(Alarm.get_nth_node(j))).get_name()){
                mapping.push_back(make_pair(all_parents[k],input_data[j]));
            }
        }
    }
    if(all_parents.size() ==0 ){
        int index = sampling(CPT_old[missing_index]);
        input_data[missing_index] = current.get_values()[index];
    }
    else{
        int cpt_i_length = (*(Alarm.search_node(all_parents[all_parents.size()-1]))).get_nvalues();
        vector<int> for_binary;
        for_binary.push_back(1);
        for(int p=all_parents.size()-2;p>=0;p--){
            for_binary.push_back(cpt_i_length);
            cpt_i_length = cpt_i_length* (*(Alarm.search_node(all_parents[p]))).get_nvalues();
        }
        for_binary.push_back(cpt_i_length);
        cpt_i_length = cpt_i_length * current.get_nvalues();
        int index_for_cpt = 0;
        for(int l=0;l<all_parents.size();l++){
            for(int m=0;m<mapping.size();m++){
                if(mapping[m].first == all_parents[l]){
                    vector<string> values = (*(Alarm.search_node(all_parents[l]))).get_values();
                    for(int q =0;q<values.size();q++){
                        if(mapping[m].second == values[q].substr(1,values[q].length()-2)){
                            int pwd = (for_binary.size()) - 2 - l;
                            index_for_cpt = index_for_cpt + q*for_binary[pwd];
                        }
                    }
                }
            }
        }
        vector<string> current_values = current.get_values();
        vector<int> current_childern = current.get_children();
        vector<float> probs;
        for(int j=0;j<current.get_nvalues();j++){
            input_data[missing_index] = current.get_values()[j];
            float prob = 1;
            for(int k=0;k<current_childern.size();k++){
                Graph_Node current_child = (*(Alarm.get_nth_node(current_childern[k])));
                vector<string> current_child_parents = current_child.get_Parents();
                int cpt_i_length1 = (*(Alarm.search_node(current_child_parents[current_child_parents.size()-1]))).get_nvalues();
                vector<int> for_binary1;
                for_binary1.push_back(1);
                for(int p=current_child_parents.size()-2;p>=0;p--){
                    for_binary1.push_back(cpt_i_length1);
                    cpt_i_length1 = cpt_i_length1* (*(Alarm.search_node(current_child_parents[p]))).get_nvalues();
                }
                for_binary1.push_back(cpt_i_length1);
                cpt_i_length1 = cpt_i_length1 * current_child.get_nvalues();
                int index_for_cpt1 = 0;
                for(int l=0;l<current_child_parents.size();l++){
                    for(int m=0;m<mapping.size();m++){
                        if(mapping[m].first == current_child_parents[l]){
                            vector<string> values1 = (*(Alarm.search_node(current_child_parents[l]))).get_values();
                            for(int q =0;q<values1.size();q++){
                                if(mapping[m].second == values1[q].substr(1,values1[q].length()-2)){
                                    int pwd = (for_binary1.size()) - 2 - l;
                                    index_for_cpt1 = index_for_cpt1 + q*for_binary1[pwd];
                                }
                            }
                        }
                    }
                }
                for(int l=0;l<mapping.size();l++){
                    string namese = current_child.get_name();
                    if(namese == mapping[l].first){
                        vector<string> values1 = current_child.get_values();
                        for(int pl = 0;pl < values1.size(); pl++){
                            if(values1[pl].substr(1,values1[pl].size()-2) == mapping[l].second){
                                int pwd  = (for_binary1.size()) - 1;
                                index_for_cpt1 = index_for_cpt1 + pl * for_binary1[pwd];
                            }
                        }
                    }
                }
                for(int l=0;l<Alarm.netSize();l++){
                    if(current_child.get_name() == (*(Alarm.get_nth_node(l))).get_name()){
                        prob = (float)prob * (float)CPT_old[l][index_for_cpt1];
                    }
                }
            }
            prob = (float)prob * (float)CPT_old[missing_index][index_for_cpt + j*(for_binary[for_binary.size()-1])];
            probs.push_back(prob);
        }
        int best = sampling(probs);
        input_data[missing_index] = current.get_values()[best];
    }
    
    //return CPT;
    Graph_Node node = (*(Alarm.get_nth_node(missing_index)));
    vector<string> values = node.get_values();
    for(int k=0;k<Alarm.netSize();k++){
        Graph_Node current = (*(Alarm.get_nth_node(k)));
        vector<string> all_parents = current.get_Parents();
        if(all_parents.size() == 0){
            vector<int > vecto(current.get_nvalues(),0);
            vector<string> values = current.get_values();
            for(int qe = 0;qe < mapping.size();qe++){
                if(mapping[qe].first == current.get_name()){
                    for(int rt = 0;rt<values.size();rt++){
                        if(mapping[qe].second == values[rt].substr(1,values[rt].size()-2)){
                            CPT[k][rt] = CPT[k][rt] + 1;
                        }
                    }
                }
            }
            continue;
        }
        int cpt_i_length = ((*(Alarm.search_node(all_parents[all_parents.size()-1]))).get_nvalues());
        vector<int> for_binary;
        for_binary.push_back(1);
        for(int p=all_parents.size()-2;p>=0;p--){
            for_binary.push_back(cpt_i_length);
            cpt_i_length = cpt_i_length* ((*(Alarm.search_node(all_parents[p]))).get_nvalues());
        }
        for_binary.push_back(cpt_i_length);
        cpt_i_length = cpt_i_length * current.get_nvalues();
        vector<int> vect(cpt_i_length,0);
        int index_for_cpt = 0;
        for(int l=0;l<all_parents.size();l++){
            for(int m=0;m<mapping.size();m++){
                if(mapping[m].first == all_parents[l]){
                    vector<string> values = (*(Alarm.search_node(all_parents[l]))).get_values();
                    for(int q =0;q<values.size();q++){
                        if(mapping[m].second == values[q].substr(1,values[q].length()-2)){
                            int pwd = (for_binary.size()) - 2 - l;
                            index_for_cpt = index_for_cpt + q*for_binary[pwd];
                        }
                    }
                }
            }
        }
        for(int l=0;l<mapping.size();l++){
            string namese = current.get_name();
            if(namese == mapping[l].first){
                vector<string> values = current.get_values();
                for(int pl = 0;pl < values.size(); pl++){
                    if(values[pl].substr(1,values[pl].size()-2) == mapping[l].second){
                        int pwd  = (for_binary.size()) - 1;
                        index_for_cpt = index_for_cpt + pl * for_binary[pwd];
                    }
                }
            }
        }
        CPT[k][index_for_cpt] = CPT[k][index_for_cpt] + 1;
        
    }
    return CPT;
    
}


void writeFile(string inputfile,string outfile, network Alarm, vector<vector<float> > CPT){
    ifstream infile;
    string a = inputfile;
    infile.open(a);
    if(!infile){
        cout << "Unable to load file!";
        exit(1);
    }
    int count = 0;
    string temp ;
    bool x  = false;
    ofstream myfile(outfile);
    while(infile>> temp){
        //temp = temp.substr(0,temp.length()-2);
        if(temp == "variable"){
            x = true;
            myfile << "\n";
        }
        if(x){
            if(temp == "probability"){
                myfile << "\n";
            }
            if(temp == "table" || temp == "type" || temp == "property"){
                myfile << "\n";
                myfile << "\t";
            }
            if(temp == "// "){
                myfile << "\n";
            }
            if(temp == "}"){
                myfile << "\n";
            }
            if(temp == to_string(-1)){
                for(int i=0;i<CPT[count].size();i++){
                    myfile << CPT[count][i] << " ";
                    infile >> temp;
                    //temp = temp.substr(0,temp.length()-2);
                }
                count =  count + 1;
            }
            myfile << temp << " " ;
        }
    }
    myfile.close();
}



int main()
{
    network Alarm;
    Alarm=read_network();
    vector<Graph_Node> Baysian_network;
    for(int i=0;i<Alarm.netSize();i++){
        Baysian_network.push_back((*(Alarm.get_nth_node(i))));
    }
    vector<vector<string> > we;
    we = readFile("records.dat",Baysian_network.size());
    vector<pair<int,int> > missing_index;
    for(int i=0;i<we.size();i++){
        for(int j=0;j<we[i].size();j++){
            if(we[i][j] == "?"){
                missing_index.push_back(make_pair(i,j));
            }
        }
    }
    vector<vector<int> > CPT;
    for(int i=0;i<we.size();i++){
        CPT = random_CPT(Alarm,we[i],missing_index[i].second,i,CPT);
    }
    for(int i=0;i<CPT.size();i++){
        for(int j=0;j<CPT[i].size();j++){
            CPT[i][j] = CPT[i][j]*1000 +1;
        }
    }
    vector<vector<float> > ret_CPT1;
    //vector<vector<float> > cpt =  find_new_CPT(Alarm,random_CPT(Alarm,we,missing_index),we,missing_index);
    for(int ky =0;ky<5;ky++){
        vector<vector<float> > ret_CPT;
        for(int i=0;i<CPT.size();i++){
            vector<float> x_f(CPT[i].size(),0.0);
            ret_CPT.push_back(x_f);
        }
        for(int i=0;i<CPT.size();i++){
            int nvalues = (*(Alarm.get_nth_node(i))).get_nvalues();
            for(int j=0;j<CPT[i].size()/nvalues;j++){
                int sum = 0;
                int count = j;
                while(count<CPT[i].size()){
                    sum = sum + CPT[i][count];
                    count =  count + (CPT[i].size()/nvalues);
                }
                count = j;
                while(count<CPT[i].size()){
                    if(sum == 0){
                        ret_CPT[i][count] = float(0);
                        count = count + (CPT[i].size()/nvalues);
                        continue;
                    }
                    float fl = float(CPT[i][count])/float(sum);
                    ret_CPT[i][count] = fl;
                    count = count + (CPT[i].size()/nvalues);
                }
            }
        }
        for(int i=0;i<CPT.size();i++){
            for(int j=0;j<CPT[i].size();j++){
                //CPT[i][j] =0;
            }
        }
        for(int i=0;i<we.size();i++){
            CPT = find_new_CPT(Alarm,ret_CPT,we[i],missing_index[i].second,i,CPT);
        }
        for(int i=0;i<CPT.size();i++){
            for(int j=0;j<CPT[i].size();j++){
                //CPT[i][j] =CPT[i][j] *1000 + 1;
            }
        }
    }
    vector<vector<float> > ret_CPT;
    for(int i=0;i<CPT.size();i++){
        vector<float> x_f(CPT[i].size(),0.0);
        ret_CPT.push_back(x_f);
    }
    for(int i=0;i<CPT.size();i++){
        int nvalues = (*(Alarm.get_nth_node(i))).get_nvalues();
        for(int j=0;j<CPT[i].size()/nvalues;j++){
            int sum = 0;
            int count = j;
            while(count<CPT[i].size()){
                sum = sum + CPT[i][count];
                count =  count + (CPT[i].size()/nvalues);
            }
            count = j;
            while(count<CPT[i].size()){
                if(sum == 0){
                    ret_CPT[i][count] = float(0);
                    count = count + (CPT[i].size()/nvalues);
                    continue;
                }
                float fl = float(CPT[i][count])/float(sum);
                ret_CPT[i][count] = fl;
                count = count + (CPT[i].size()/nvalues);
            }
        }
    }
    for(int i=0;i<CPT.size();i++){
        for(int j=0;j<CPT[i].size();j++){
            //cout << ret_CPT[i][j] << " ";
            //cout << CPT[i][j] << " ";
        }
        //cout << endl;
    }
    writeFile("alarm.bif.txt","solved_alarm.bif",Alarm,ret_CPT);
    
    return 0;
}
