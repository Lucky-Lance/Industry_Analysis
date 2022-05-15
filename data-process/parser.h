#ifndef _PARSER_H_
#define _PARSER_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <optional>
#include <tuple>
#include <assert.h>
#include <set>
#include <filesystem>
#include <iomanip>
using namespace std;
#define DEBUG printf("%s:%d\n", __FILE__, __LINE__);

ostream& operator<<(ostream& os, const vector<string>& vct){
    for(auto iter=vct.cbegin();iter!=vct.cend();iter++){
        os << *iter << ' ';
    }
    return os;
}
vector<string> split(const string& str, char c){
    // "a,b,c,de,,f," => ["a", "b", "c", "de", "", "f", ""]
    vector<string> result;
    int i=0, j=i;
    for(;j<str.size(); j++){
        if(str[j] == c){
            result.emplace_back(move(str.substr(i, j-i)));
            i = j+1;
        }
        else if(str[j] == '"'){
            for(j++; j < str.size() && str[j] != '"'; j++){}
        }
    }
    result.emplace_back(move(str.substr(i, j-i)));
    return result;
}
optional<char> stringToChar(const string& str){
    // " 'A'" => 'A'
    for(int i=0; i<str.size(); i++){
        if(str[i] == '\''){
            assert(i+2 < str.size());
            assert(str[i+2] == '\'');
            return str[i+1];
        }
    }
    return {};
}
vector<char> industryParser(const string& str){
    vector<char> result;
    int stackDipth = 0;
    int prev_i;
    for(int i=0; i<str.size(); i++){
        switch(str[i]){
            case '[': {
                prev_i = i+1;
                stackDipth += 1;
            }
            break;
            case ']': {
                assert(stackDipth>0);
                vector<string> vectorChar = split(str.substr(prev_i, i-prev_i), ',');
                for(const string& s: vectorChar){
                    optional<char> c = stringToChar(s);
                    if(c.has_value()){
                        result.emplace_back(c.value());
                    }
                }
                stackDipth--;
            }
            break;
            default:{

            }
            break;
        }
    }
    return result;
}
ostream& outputIndustry(ostream& os, const vector<char>& industry){
    if(industry.empty()){
        os << "[]";
        return os;
    }
    os << "\"[";
    for(int i=0; i<industry.size(); i++){
        os << ' ' << '\'' << industry[i] << '\'';
        if(i<industry.size()-1){
            os << ',';
        }
    }
    os << "]\"";
    return os;
}
tuple<string, string> splitLast(const string& str, char c){
    // "a_b_c_d_e" => ("a_b_c_d", "e")
    for(int i=str.size()-1; i>=0; i--){
        if(str[i] == c){
            return make_tuple(str.substr(0, i),
                              str.substr(i+1, str.size()-i));
        }
    }
    DEBUG
    cerr << "splitLast() failed!\nstr = \""<< str << "\", c = " << c << endl;
    exit(1);
}
struct Hash{
    uint64_t hash1, hash2;
    string str;
    Hash():hash1(0U), hash2(0U){}
    Hash(const string& s):str(s){
        hash1 = hash2 = 0U;
        for(char c: s){
            hash1 = ((hash1 * maxLen + transform(c))%mod1);
            hash2 = ((hash2 * maxLen + transform(c))%mod2);
        }
    }
    constexpr static const uint64_t mod1 = (1ULL<<56)+1ULL, mod2 = (1ULL<<56)+3ULL;
    static const uint64_t maxLen = 36; // 0-9, a-z
    static uint8_t transform(char c){
        if(c>='0' && c<='9') return c-'0';
        if(c>='a' && c<='z') return c-'a'+'9'-'0'+1;
        assert(false);
    }
    bool operator==(const Hash& other) const {
        return hash1 == other.hash1 && hash2 == other.hash2;
    }
    bool operator<(const Hash& other) const {
        if(hash1 < other.hash1) return true;
        if(other.hash1 < hash1) return false;
        if(hash2 != other.hash2){
            DEBUG
            printf("Hash2 not equal!\n");
            cout  << str << "\n" << other.str << "\n------------------------------\n";
        }
        return hash2 < other.hash2;
    }
};

struct ItemType{
    string name, type, id_str;
    vector<char> industry;
    Hash id_hash;
    ItemType(const string& itemString){
        vector<string> splited_itemString = split(itemString, ',');
        if(splited_itemString.size() != 4){
            cerr << itemString << endl;
            DEBUG
            exit(1);
        }
        string typeName, pure_id_str;
        tie(typeName, pure_id_str) = splitLast(splited_itemString[0], '_');
        id_hash = Hash(pure_id_str);
        name = move(splited_itemString[1]);
        type = move(splited_itemString[2]);
        industry = move(industryParser(splited_itemString[3]));
        id_str = move(splited_itemString[0]);
    }
    ItemType(ItemType&& other){
        name = move(other.name);
        type = move(other.type);
        id_str = move(other.id_str);
        industry = move(other.industry);
        id_hash = other.id_hash;
    }
    bool operator==(const ItemType& other) const {
        return id_hash == other.id_hash;
    }
    bool operator<(const ItemType& other) const {
        return id_hash < other.id_hash;
    }
    friend ostream& operator<<(ostream& os, const ItemType& item){
        os << item.name;
        return os;
    }
};
ostream& outputItemType(ostream& os, const ItemType& item){
    os << item.id_str << ',' << item.name << ',' <<
        item.type << ',';
    outputIndustry(os, item.industry);
    return os;
}

ostream& operator<<(ostream& os, const set<string>& vct){
    for(auto iter=vct.cbegin();iter!=vct.cend();iter++){
        os << *iter << ", ";
    }
    return os;
}

map<Hash, ItemType>
nodeReader(const string& fileName){
    ifstream f;
    f.open(fileName, ios::in);
    if(!f.good()){
        cerr << "open file '" << fileName << "' error!" << endl;
        exit(1);
    }
    map<Hash, ItemType> result;
    string line;
    getline(f, line);
    vector<string> kinds = split(line, ',');
    size_t black_size = 0;
    set<string> typeSet;
    while(getline(f, line)){
        ItemType item(line);
        typeSet.insert(item.type);
        if(!item.industry.empty()){
            black_size ++;
        }
        Hash id_hash = item.id_hash;
        auto insertResult = result.emplace(id_hash, move(item));
        assert(insertResult.second);
    }
    DEBUG
    cout << typeSet << endl;
    cout << "black / total = " << black_size << '/' << result.size() << endl;
    f.close();
    return result;
}
using LinkItemType = tuple<string, Hash, Hash>;
vector<LinkItemType>
linkReader(const string& fileName){
    ifstream f;
    f.open(fileName, ios::in);
    if(!f.good()){
        cerr << "open file '" << fileName << "' error!" << endl;
        exit(1);
    }
    vector<LinkItemType> result;
    string line;
    getline(f, line);
    vector<string> kinds = split(line, ',');
    set<string> relations;
    while(getline(f, line)){
        vector<string> splitedStr = split(line, ',');
        if(splitedStr.size() != 3){
            cerr << "Error in parsing " << fileName << endl;
            cerr << "error read: " << endl;
            cerr << line << endl;
            cerr << "error reason: splitedStr.size() != 3" << endl;
            assert(0);
        }
        string hashStr1 = get<1>( splitLast(splitedStr[1], '_') );
        string hashStr2 = get<1>( splitLast(splitedStr[2], '_') );
        Hash source(hashStr1);
        Hash target(hashStr2);
        relations.insert(splitedStr[0]);
        result.emplace_back(make_tuple(move(splitedStr[0]), move(source), move(target)));
    }
    f.close();
    DEBUG
    cout << relations << endl;
    return result;
}
void outputSubGraph(const map<Hash, ItemType>& nodes, const vector<LinkItemType>& linkItems
                    , const vector<Hash>& subGraph, const tuple<string,string>& fileNames){
    DEBUG
    map<Hash, vector<LinkItemType>> links;
    for(const LinkItemType& linkItem: linkItems){
        Hash from_hash = get<1>(linkItem);
        auto pos = links.find(from_hash);
        if(pos == links.end()){
            links.emplace(from_hash, vector<LinkItemType>{linkItem});
        }
        else{
            pos->second.emplace_back(linkItem);
        }
    }


        {// output Node.csv
            ofstream f;
            const string& fileName = get<0>(fileNames);
            f.open(fileName, ios::out);
            if(!f.good()){
                cerr << "try to write '" << fileName << "', but failed!\n";
                exit(1);
            }
            f << "id,name,type,industry\n";
            for(const Hash& h: subGraph){
                const ItemType& item = nodes.at(h);
                outputItemType(f, item) << '\n';
            }
            f.close();
        }
        {
            ofstream f;
            const string & fileName = get<1>(fileNames);
            f.open(fileName, ios::out);
            if(!f.good()){
                cerr << "try to write '" << fileName << "', but failed!\n";
                exit(1);
            }
            f << "relation,source,target\n";
            set<Hash> subNodes;
            subNodes.insert(subGraph.cbegin(), subGraph.cend());
            for(const Hash& h: subGraph){
                auto pos = links.find(h);
                if(pos == links.cend()){
                    continue;
                }
                const vector<LinkItemType>& l = pos->second;
                for(const LinkItemType& li: l){
                    if(subNodes.find(get<2>(li)) == subNodes.cend()){
                        continue;
                    }
                    f << get<0>(li) << ',' << nodes.at(get<1>(li)).id_str << ','
                      << nodes.at(get<2>(li)).id_str << '\n';
                }
            }
            f.close();
        }
}
void outputSubGraphs(const map<Hash, ItemType>& nodes, const vector<LinkItemType>& linkItems, const vector<vector<Hash>>& subGraphs, string folderName){
    assert(!folderName.empty());
    if(folderName.back()!='/' || folderName.back() != '\\'){
        folderName += '/';
    }
    auto outputFolder = filesystem::path(folderName);
    if(!filesystem::is_directory(outputFolder)){
        if(!filesystem::create_directory(outputFolder)){
            cerr << "try to mkdir '" << outputFolder << "', but failed!" << endl;
            exit(1);
        }
    }
    map<Hash, vector<LinkItemType>> links;
    for(const LinkItemType& linkItem: linkItems){
        Hash from_hash = get<1>(linkItem);
        auto pos = links.find(from_hash);
        if(pos == links.end()){
            links.emplace(from_hash, vector<LinkItemType>{linkItem});
        }
        else{
            pos->second.emplace_back(linkItem);
        }
    }
    int count = 0;
    for(const vector<Hash>& subGraph: subGraphs){
        {// output Node.csv
            ofstream f;
            stringstream ss;
            ss << folderName << setw(8) << setfill('0') << count << "-Node-" << setw(8) << setfill('0') << subGraph.size() <<".csv";
            f.open(ss.str(), ios::out);
            if(!f.good()){
                cerr << "try to write '" << ss.str() << "', but failed!\n";
                exit(1);
            }
            f << "id,name,type,industry\n";
            for(const Hash& h: subGraph){
                const ItemType& item = nodes.at(h);
                outputItemType(f, item) << '\n';
            }
            f.close();
        }
        {
            ofstream f;
            stringstream ss;
            ss << folderName << setw(8) << setfill('0') << count << "-Link-"
               << setw(8) << setfill('0') << subGraph.size() <<".csv";
            f.open(ss.str(), ios::out);
            if(!f.good()){
                cerr << "try to write '" << ss.str() << "', but failed!\n";
                exit(1);
            }
            f << "relation,source,target\n";
            set<Hash> subNodes;
            subNodes.insert(subGraph.cbegin(), subGraph.cend());
            for(const Hash& h: subGraph){
                auto pos = links.find(h);
                if(pos == links.cend()){
                    continue;
                }
                const vector<LinkItemType>& l = pos->second;
                for(const LinkItemType& li: l){
                    if(subNodes.find(get<2>(li)) == subNodes.cend()){
                        continue;
                    }
                    f << get<0>(li) << ',' << nodes.at(get<1>(li)).id_str << ','
                      << nodes.at(get<2>(li)).id_str << '\n';
                }
            }
            f.close();
        }
        count++;
    }
}
#endif