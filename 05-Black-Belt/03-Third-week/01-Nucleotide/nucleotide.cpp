#include <cstddef>
#include <iostream>
#include <map>

#include <cstdint>
#include <array>
#include <map>
#include <cstddef>

struct Nucleotide {
    char Symbol;
    size_t Position;
    int ChromosomeNum;
    int GeneNum;
    bool IsMarked;
    char ServiceInfo;
};

struct CompactNucleotide {
    uint64_t ServiceInfo:8;
    uint64_t Symbol:2;
    uint64_t IsMarked:1;
    uint64_t ChromosomeNum:6;
    uint64_t GeneNum:15;
    uint64_t Position:32;
};

const std::map<char, int> direct_code = {{'A', 0}, {'T', 1}, {'G', 2}, {'C', 3}};
const std::map<int, char> inverse_code = {{0, 'A'}, {1, 'T'}, {2, 'G'}, {3, 'C'}};

bool operator == (const Nucleotide& lhs, const Nucleotide& rhs) {
  return (lhs.Symbol == rhs.Symbol)
      && (lhs.Position == rhs.Position)
      && (lhs.ChromosomeNum == rhs.ChromosomeNum)
      && (lhs.GeneNum == rhs.GeneNum)
      && (lhs.IsMarked == rhs.IsMarked)
      && (lhs.ServiceInfo == rhs.ServiceInfo);
}


CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide result;
    result.ServiceInfo = static_cast<uint64_t>(n.ServiceInfo);
    result.Symbol = direct_code.at(n.Symbol);
    result.IsMarked = static_cast<uint64_t>(n.IsMarked);
    result.ChromosomeNum = static_cast<uint64_t>(n.ChromosomeNum);
    result.GeneNum = static_cast<uint64_t>(n.GeneNum);
    result.Position = n.Position;
    
    return result;
};


Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide result;
    result.ServiceInfo = static_cast<char>(cn.ServiceInfo);
    result.Symbol =  inverse_code.at(cn.Symbol);
    result.IsMarked = (cn.IsMarked == 1);
    result.ChromosomeNum = cn.ChromosomeNum;
    result.GeneNum = cn.GeneNum;
    result.Position = cn.Position;
    
    return result;
}
