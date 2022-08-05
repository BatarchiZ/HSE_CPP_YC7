#include <cassert>
#include <string_view>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

struct Lines
{
    double tf_idf = 0;
    int textNumber = 0;
    double size = 0;
    std::vector<std::pair<std::string_view*, int>> tfV;
    std::string text;
    double tf = 0;
    double idf = 0;
};

bool operator<(const Lines& lhs, const Lines& rhs)
{
    return rhs.tf_idf < lhs.tf_idf;
}

std::vector<std::pair<std::string_view, double>> splitByWords(const std::string_view& line, const char delim)
{
    std::vector<std::pair<std::string_view, double>> words;
    size_t startPos = 0;
    do
    {
        size_t pos = line.find(delim, startPos);

        if (pos == std::string_view::npos)
        {
            std::string_view word = line.substr(startPos);
            if(word.empty())
            {break;}
            std::pair<std::string_view, double> tempP = {word, 0};
            words.push_back(tempP);
            break;
        }
        std::string_view word = line.substr(startPos, pos - startPos);
        std::pair<std::string_view, double> tempP = {word, 0};
        words.push_back(tempP);
        startPos = pos + 1;
    } while (startPos != 0);
    return words;
}


std::vector<std::string> splitByWords2(std::string& line)
{
    std::vector<std::string> words;
    std::string dummy;
    for (char& i: line)
    {

        if (!isalpha(i))
        {
            remove(dummy.begin(), dummy.end(), ' ');
            if (dummy.size() > 1)
            {
                words.push_back(dummy);
            }
            dummy = "";
            continue;
        }
        dummy += i;
    }
    remove(dummy.begin(), dummy.end(), ' ');
    if (dummy.size() > 1)
    {
        words.push_back(dummy);
    }

    return words;
}

void lineWordFreq(std::vector<std::pair<std::string_view, int>>& basisV,
                  const std::vector<std::string>& compVs, Lines& structure)
{
    std::vector<std::pair<std::string_view*, int>> vForTF;
    for (std::pair<std::string_view, int>& bPair: basisV)
    {

        std::pair<std::string_view*, int> p = {nullptr, 0};
        p.first = &(bPair.first);
        for (const std::string& word: compVs)
        {
            structure.size = compVs.size();
            if (bPair.first == word)
            {
                p.second += 1;
            }
        }
        if (p.second != 0)
        { vForTF.push_back(p);}
        else
        { p.first = nullptr; }
    }
    if (vForTF.size() != 0)
    { structure.tfV = vForTF; }
}
void calculateIDF(std::vector<std::pair<std::string_view, double>>& basisV, const std::vector<std::pair<std::string_view, double>>& textV,double textsize)
{
    for(std::pair<std::string_view, double>&p : basisV)
    {
        for(std::pair<std::string_view, double> pline: textV)
        {
            size_t find = pline.first.find(p.first);
            if(find!= std::string_view::npos)
            {
                p.second += 1;
            }
        }
        double idf = log(textsize/p.second);
        p.second = idf;
    }
}
void calculateTFIDF(std::vector<std::pair<std::string_view, double>>& basisV, std::vector<std::string>& compVs, Lines& structure)
{
    for(std::pair<std::string_view, double>& bp : basisV)
    {   double counter = 0;
        for(const std::string& word : compVs)
        {
            if(bp.first == word)
            {
                counter += 1;
            }
        }
        if(counter != 0)
        {
//            double tf = counter/compVs.size();
//            structure.tf += tf;
            double tf_idf = ((counter/compVs.size())*bp.second);
            structure.tf_idf += tf_idf*1000;
        }
    }
}

void convToLower(std::string& line2)
{
    for (char& c: line2)
    {
        if (c >= 'A' && c <= 'Z')
        { c = c + 32; }
    }
}
void remove1223(std::vector<std::pair<std::string_view, double>> &v)
{
    auto end = v.end();
    for (auto it = v.begin(); it != end; ++it) {
        end = std::remove(it + 1, end, *it);
    }

    v.erase(end, v.end());
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count)
{
    std::vector<std::pair<std::string_view, double>> basisV = splitByWords(query, ' ');
    remove1223(basisV);



    std::vector<Lines> ansV;
    ////Maybe here
    std::vector<std::pair<std::string_view, double>> textV = splitByWords(text, '\n');
    size_t textsize = 0; ///Not preprocessed text lol
    for(std::pair<std::string_view, double> p: textV)
    {
        if(p.first.size()!=0)
            ++textsize;
    }
    calculateIDF(basisV, textV, textsize);

    for(int i = 0; i < textV.size(); i++)
    {
        Lines structure;
        structure.textNumber = i + 1;
        std::string toLowerStr(textV[i].first);
        convToLower(toLowerStr);

        std::vector<std::string> compVs = splitByWords2(toLowerStr);

        calculateTFIDF(basisV, compVs, structure);
        if (structure.tf_idf != 0)
        { ansV.push_back(structure);}
    }
    std::sort(ansV.begin(), ansV.end());
    std::vector<std::string_view> ansV3;

    if (ansV.empty())
        return ansV3;
    for (int i = 0; i < results_count; i++)
    {
        if (ansV[i].tf_idf != 0 && i< ansV.size())
        {
            ansV3.push_back(textV[ansV[i].textNumber-1].first);
        }
    }
    return ansV3;
}
