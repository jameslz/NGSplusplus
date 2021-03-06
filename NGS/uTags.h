// ***************************************************************************
// uTags.h (c) 2013
// Alexei Nordell-Markovits : Sherbrooke University
// Charles Joly Beauparlant : Laval University
//
//       This file is part of the NGS++ library.
//
//    The NGS++ library is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with this program (lgpl-3.0.txt).  If not, see <http://www.gnu.org/licenses/>.
// ***************************************************************************


#ifndef UTAGS_H_INCLUDED
#define UTAGS_H_INCLUDED

#include "uFormats.h"
//#include "uFormatExperiment.h"
#include <memory>
#include "utility/utility.h"


namespace BamTools{
    class BamReader;
}
namespace NGS {
//Our Tag format
//We used this to store mapped tags from NGS experiments
//This is used for single End tags
class uToken;
class uParser;
class uBasicNGS;
class uBasicNGSChrom;
class uBasicNGSExperiment;
class uTags;
class uTagsChrom;
class uTagsExperiment;
class uGene;

class uTags: public uGenericNGS<uTags>
{

#define FORWARCHARD '+'
#define REVERSECHAR '-'

private:
    //0 = FORWARD, 1 = REVERSE
    //Optional
    //Map score, quality of the alignement.
    short int mapScore=255; /*!<Total mapping quality of the read */

    std::string sequence=""; /*!<Sequence associated with the read. Optional */
    char* name=nullptr;  /*!<Name or ID associated with read. not guaranteed to be unique*/
    char* phredScore=nullptr; /*!<PhredScore associated with each position of the read*/
    char* m_cigar=nullptr; /*!<Cigar flag as defined by the SAM format*/
    bool Unmapped=true;
    //Using Samtools definition here. Replace above variables by this when possible.
    int flag=0;  /*!<Sam flag as defined by the SAM format*/
    //Pointer to next element of the template, not sure we really want to use this?.
    int PELength=0; /**< Length of the total segment if paired. Can also be used for estimated length */

public:

    uTags();
    uTags(const uBasicNGS & otherItem);
    uTags(const uRegion & otherItem);
    uTags(const uGene & otherItem);
    uTags(uToken pToken);


    uTags(std::string pChr, long long int pStart, long long int pEnd, float pScore);
    uTags(std::string pChr, long long int pStart, long long int pEnd, StrandDir pStrand=StrandDir::FORWARD);
    uTags(std::string pChr, long long int pStart, long long int pEnd, StrandDir pStrand, float pScore);

    uTags(const uTags& copy_from);
    uTags& operator=  (uTags const& assign_from);
    ~uTags();

    uTags getCopy()const;
    uTags getCompletedCopy()const;

    bool isEqual(const uTags & pCompared)const;

    uToken createToken()const;

    void print(std::ostream &pOut) const; /**< Prints a human readable version of the data */

    bool isMapped() const;
    void setMapped(bool pmapped);

    void setCigar(std::string pCigar);
    std::string getCigar() const;

    void setFlag(int pflag);
    int getFlag() const;

    void setSequence(std::string pSeq);
    std::string getSequence() const;

    void setPhred(std::string Phred);
    std::string getPhred() const;

    void setName(std::string pName);

    std::string getName() const;
    bool isPE() const;

    void setPELength(int length);
    int getPeLength() const;

    void setMapQual(short int score);
    short int getMapQual() const;
};


class uRegionChrom;
class uBasicNGSChrom;
class uGeneChrom;
class uTagsChrom: public uGenericNGSChrom<uTagsChrom,uTags>
{

private:

public:

    uTagsChrom():uGenericNGSChrom(){};
    uTagsChrom(const std::string & ourChr):uGenericNGSChrom(ourChr)
    { }
     ~uTagsChrom() {};
    uTagsChrom getCopy() const;

    uTagsChrom(const uGenericNGSChrom<uTagsChrom,uTags>&);
    uTagsChrom& operator=(const uTagsChrom& copFrom);
    uTagsChrom(const uTagsChrom&);
    uTagsChrom(const uRegionChrom &);
    uTagsChrom(const uBasicNGSChrom &);
    uTagsChrom(const uGeneChrom &);
    uTagsChrom(const std::vector<uTags> & copyVec):uGenericNGSChrom(copyVec){};

    std::vector<float> getRegionSignal(long int start, long int end, bool overlap);
//
//    template <class _OTHER_>
//    uTags generateRandomSiteWithID(const int size, std::mt19937& engine, _OTHER_ exclList, const int sigma=0, const std::string ID="") const;
};

/**< Our complete tag experiment */
class uTagsExperiment: public uGenericNGSExperiment<uTagsExperiment,uTagsChrom, uTags>
{

private:
    void loadWithBamTools_Core(BamTools::BamReader& pReader, int pBlockSize=1);
    void loadWithBamTools_All(BamTools::BamReader& pReader, int pBlockSize=1);

public:
     ~uTagsExperiment() {};
    std::vector<float> getRegionSignal(std::string chrom, long int start, long int end, bool overlap);
    uTagsExperiment getCopy() const;

    void loadWithBamTools(BamTools::BamReader& pReader, int blockSize=0, bool pLoadCore=false);

};
} // End of namespace NGS

namespace factory
{
    NGS::uTags makeTagfromSamString(std::string samString, bool minimal=false);
}

#endif // UTAGS_H_INCLUDED
