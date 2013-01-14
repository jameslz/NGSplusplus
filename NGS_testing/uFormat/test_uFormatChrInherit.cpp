/**< Test Common inherited functions */
#include "NGS++.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <thread>
#include <random>
#include <string.h>
#include <time.h>
#include <gtest/gtest.h>

#define CHROMDIVIDESIZE 500
using namespace std;
using namespace NGS;


#define ONELENGHT 101
class StandardChroms
{
public:
	StandardChroms()
	{
		/**< m_uBasicNGSChroms[0]: Empty chrom without a name (""). */
       oneChr.setChr("chr1");

       oneChr.addData(uBasicNGS("chr1",100,200));

       manyChr.setChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));

       emptyChr.setChr("chr1");
	}
	uBasicNGSChrom oneChr;
    uBasicNGSChrom manyChr;
    uBasicNGSChrom emptyChr;
};



class ChromDivide : public testing::Test {
 protected:
/**< As always, this is inclusive so 100-199 is of size 100 */
  virtual void SetUp() {

    uChromTestOverlap.setChr("chr1");
    uChromTestOverlap.addData(uBasicNGS ("chr1", 300, CHROMDIVIDESIZE));
    uChromTestOverlap.addData(uBasicNGS("chr1", 100, 199));
    uChromTestOverlap.addData(uBasicNGS("chr1", 100, 299));
  }
uBasicNGSChrom uChromTestOverlap;
};



/*
 * Setters/Getters testing - start/end (positions)
 *	Valid cases:
 *		SETGETSTART
 *		SETGETEND
 *	Invalid cases:
 *		SETSTARTILLEGAL
 *		SETSENDILLEGAL
 */
TEST(uBasicNGSCHR_avgSiteSize, ONESITE){
       StandardChroms ourChroms;
       ourChroms.oneChr.addData(uBasicNGS("chr1",100,200));
       EXPECT_EQ(101,ourChroms.oneChr.avgSiteSize());
 }
TEST(uBasicNGSCHR_avgSiteSize, NOSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(0,ourChroms.emptyChr.avgSiteSize());
 }
 TEST(uBasicNGSCHR_avgSiteSize, MANYSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(((101+71+131)/3),ourChroms.manyChr.avgSiteSize());
 }
/**<  */
 TEST(uBasicNGSCHR_minSiteSize, ONESITE){
       StandardChroms ourChroms;
       EXPECT_EQ(101,ourChroms.oneChr.minSiteSize());
 }
TEST(uBasicNGSCHR_minSiteSize, NOSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(0,ourChroms.emptyChr.minSiteSize());
 }
 TEST(uBasicNGSCHR_minSiteSize, MANYSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(71,ourChroms.manyChr.minSiteSize());
 }
 /**<  */
 TEST(uBasicNGSCHR_maxSiteSize, ONESITE){
       StandardChroms ourChroms;
       EXPECT_EQ(101,ourChroms.oneChr.maxSiteSize() );
 }
TEST(uBasicNGSCHR_maxSiteSizee, NOSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(0,ourChroms.emptyChr.maxSiteSize());
 }
 TEST(uBasicNGSCHR_maxSiteSize, MANYSITE){
       StandardChroms ourChroms;
       EXPECT_EQ((101+71+131),ourChroms.manyChr.maxSiteSize());
 }
 /**<  */
 TEST(uBasicNGSCHR_sumSiteSize, ONESITE){
       StandardChroms ourChroms;
       EXPECT_EQ(101,ourChroms.oneChr.sumSiteSize());
 }
TEST(uBasicNGSCHR_sumSiteSize, NOSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(0,ourChroms.emptyChr.sumSiteSize());
 }
 TEST(uBasicNGSCHR_sumSiteSize, MANYSITE){
       StandardChroms ourChroms;
       EXPECT_EQ(131,ourChroms.manyChr.sumSiteSize());
 }
/**<  */
 TEST(uBasicNGSCHR_inferChrSize, ONESITE){
       StandardChroms ourChroms;
       ourChroms.oneChr.inferChrSize();
       EXPECT_EQ(200,ourChroms.oneChr.getChromSize() );
 }
TEST(uBasicNGSCHR_inferChrSize, NOSITE){
       StandardChroms ourChroms;
       ourChroms.emptyChr.inferChrSize();
       EXPECT_EQ(0,ourChroms.emptyChr.getChromSize() );
 }
 TEST(uBasicNGSCHR_inferChrSize, MANYSITE){
       StandardChroms ourChroms;
       ourChroms.manyChr.inferChrSize();
       EXPECT_EQ(300,ourChroms.manyChr.getChromSize() );
 }
/**<  Count Unique*/
 TEST(uBasicNGSCHR_countUnique, ONESITE){
       StandardChroms ourChroms;
       EXPECT_EQ(1,ourChroms.oneChr.countUnique() );
 }
TEST(uBasicNGSCHR_countUnique, NOSITE){
         StandardChroms ourChroms;
         EXPECT_EQ(0 , ourChroms.emptyChr.countUnique());
 }
 TEST(uBasicNGSCHR_countUnique, MANYSITENOUNIQUE){
       StandardChroms ourChroms;
       EXPECT_EQ( 3, ourChroms.manyChr.countUnique());
 }
TEST(uBasicNGSCHR_countUnique, MANYSITEWITHUNIQUE){
       uBasicNGSChrom newManyChr("chr1");
       newManyChr.addData(uBasicNGS("chr1",100,200));
       newManyChr.addData(uBasicNGS("chr1",100,200));
       newManyChr.addData(uBasicNGS("chr1",150,200));
       newManyChr.addData(uBasicNGS("chr1",100,2500));
       newManyChr.addData(uBasicNGS("chr1",230,300));
       newManyChr.addData(uBasicNGS("chr1",120,250));
       EXPECT_EQ(5 , newManyChr.countUnique());
 }

/**< Testing DivideItemsIntoNBin */
TEST_F(ChromDivide, DIVIDEINTONBINFAILCHROM){
    EXPECT_ANY_THROW(uChromTestOverlap.divideItemsIntoNBins(7));
    EXPECT_ANY_THROW(uChromTestOverlap.divideItemsIntoNBins(32));
}
TEST_F(ChromDivide, DIVIDEINTONBINCHROMIGNORE){
    //std::cout << uChromTestOverlap.count();
    uChromTestOverlap.divideItemsIntoNBins(4, SplitType::IGNORE);
    EXPECT_EQ(uChromTestOverlap.count(),12);
}
TEST_F(ChromDivide, DIVIDEINTOBINEXTEND){

    uChromTestOverlap.divideItemsIntoNBins(4, SplitType::EXTEND);
    EXPECT_EQ(uChromTestOverlap.count(),12);
}
TEST_F(ChromDivide, DIVIDEINTOBINADD){

    uChromTestOverlap.divideItemsIntoNBins(4, SplitType::ADD);
    EXPECT_EQ(uChromTestOverlap.count(),13);
}
TEST_F(ChromDivide, DIVIDECHROMINTONBINOFSIZEFAIL){
    EXPECT_ANY_THROW(uChromTestOverlap.divideItemsIntoBinofSize(30));

    EXPECT_ANY_THROW(uChromTestOverlap.divideItemsIntoBinofSize(300));
}
TEST_F(ChromDivide, DIVIDECHROMINTONBINOFSIZEIGNORE){

    uChromTestOverlap.divideItemsIntoBinofSize(90, SplitType::IGNORE);
    EXPECT_EQ(uChromTestOverlap.count(),5);
}
TEST_F(ChromDivide, DIVIDECHROMINTOBINOFSIZEEXTEND){
    uChromTestOverlap.divideItemsIntoBinofSize(90, SplitType::EXTEND);
    EXPECT_EQ(uChromTestOverlap.count(),5);
}
TEST_F(ChromDivide, DIVIDECHROMINTOBINOFSIZEADD){
    uChromTestOverlap.divideItemsIntoBinofSize(90, SplitType::ADD);

    EXPECT_EQ(uChromTestOverlap.count(),8);
}

/**<  Random generation test*/
TEST(uBasicNGSCHR_generateRandomSite, NOCHRSIZE){
       StandardChroms ourChroms;
       std::random_device rd;
       std::mt19937 gen(rd());
       EXPECT_THROW(ourChroms.manyChr.generateRandomSite(100,gen,0,"test"),param_throw );
 }
TEST(uBasicNGSCHR_generateRandomSite, LARGETTHENCHRSIZE){
       StandardChroms ourChroms;
       ourChroms.manyChr.setChromSize(500);
       std::random_device rd;
       std::mt19937 gen(rd());
       EXPECT_THROW(ourChroms.manyChr.generateRandomSite(1000,gen,0,"test"),param_throw );
 }

TEST(uBasicNGSCHR_generateRandomSite, MAKEITEM){
       StandardChroms ourChroms;
       ourChroms.manyChr.setChromSize(500);
       std::random_device rd;
       std::mt19937 gen(rd());
       uBasicNGS aItem= ourChroms.manyChr.generateRandomSite(100,gen,0);
       EXPECT_EQ(100,aItem.getLenght());
       uBasicNGS aItem2= ourChroms.manyChr.generateRandomSite(150,gen,0);
       EXPECT_EQ(150,aItem2.getLenght());
 }
TEST(uBasicNGSCHR_generateRandomSite, MAKEMANYDIFFERENTITEMS){
       StandardChroms ourChroms;
       ourChroms.manyChr.setChromSize(500);

       std::mt19937 gen(432);
       uBasicNGS aItem= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
       uBasicNGS aItem2= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
       uBasicNGS aItem3= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
       uBasicNGS aItem4= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
       EXPECT_EQ(aItem2.getLenght(),aItem.getLenght());
       EXPECT_EQ(aItem3.getLenght(),aItem2.getLenght());
       EXPECT_EQ(aItem4.getLenght(),aItem3.getLenght());

       EXPECT_NE(aItem.getStart(),aItem2.getStart());
       EXPECT_NE(aItem2.getStart(),aItem3.getStart());
       EXPECT_NE(aItem3.getStart(),aItem4.getStart());
 }
TEST(uBasicNGSCHR_generateRandomSite, MAKEMANYEXCLUSIONLIST){
    //  StandardChroms ourChroms;
    //  uBasicNGSChrom exclusionChrom;
    //  newManyChr.addData(uBasicNGS("chr1",100,200));
    //  newManyChr.addData(uBasicNGS("chr1",150,500));
    //  newManyChr.addData(uBasicNGS("chr1",800,1000));
    //  ourChroms.manyChr.setChromSize(1500);
    //  std::mt19937 gen(432);

     // uBasicNGS aItem= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
     // uBasicNGS aItem2= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
     // uBasicNGS aItem3= ourChroms.manyChr.generateRandomSite(100,gen,0,"test");
       ASSERT_TRUE(false);

 }

TEST(uBasicNGSCHR_addNRandomSite, MAKECORRECTNUMBER){
       StandardChroms ourChroms;
       ourChroms.emptyChr.setChromSize(1000);
       std::random_device rd;
       std::mt19937 gen(rd());
       ourChroms.emptyChr.addNRandomSite(50,10,gen,0);
       EXPECT_EQ(10,ourChroms.emptyChr.count());
 }
 TEST(uBasicNGSCHR_addNRandomSite, MAKECORRECTNUMBERMANYDIFFERENT){
       StandardChroms ourChroms;
       ourChroms.emptyChr.setChromSize(1000);
       std::random_device rd;
       std::mt19937 gen(4523);
       ourChroms.emptyChr.addNRandomSite(50,10,gen,0);
       EXPECT_EQ(10,ourChroms.emptyChr.count());
 }
 TEST(uBasicNGSCHR_addNRandomSite, MAKEMANYEXCLUSIONLIST){
       ASSERT_TRUE(false);
 }

/**< Get overlapping */
 TEST(uBasicNGSCHR_getOverlapping, NORMAL){
       StandardChroms ourChroms;
       uBasicNGSChrom oneItem;
       oneItem.addData(uBasicNGS("chr1",50,100));
       uBasicNGSChrom retChr=ourChroms.manyChr.getOverlapping(oneItem);
       EXPECT_EQ(1,retChr.count());
 }
 TEST(uBasicNGSCHR_getOverlapping, FIRSTEMPTY){
       StandardChroms ourChroms;
       uBasicNGSChrom oneItem;
       oneItem.addData(uBasicNGS("chr1",50,100));
       uBasicNGSChrom retChr=ourChroms.emptyChr.getOverlapping(oneItem);
       EXPECT_EQ(0,retChr.count());
 }
 TEST(uBasicNGSCHR_getOverlapping, SECONDEMPTY){
       StandardChroms ourChroms;
       uBasicNGSChrom oneItem;
       uBasicNGSChrom retChr=ourChroms.manyChr.getOverlapping(oneItem);
       EXPECT_EQ(0,retChr.count());
 }
 TEST(uBasicNGSCHR_getOverlapping, SOMEOVERLAP){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getOverlapping, NOOVERLAP){
       StandardChroms ourChroms;
       uBasicNGSChrom oneItem;
       oneItem.addData(uBasicNGS("chr1",500,1000));
       uBasicNGSChrom retChr=ourChroms.emptyChr.getOverlapping(oneItem);
       EXPECT_EQ(0,retChr.count());
 }

 TEST(uBasicNGSCHR_getNotOverlapping, NORMAL){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getNotOverlapping, FIRSTEMPTY){
       ASSERT_TRUE(false);
 }
  TEST(uBasicNGSCHR_getNotOverlapping, SECONDEMPTY){
       ASSERT_TRUE(false);
 }
  TEST(uBasicNGSCHR_getNotOverlapping, SOMEOVERLAP){
       ASSERT_TRUE(false);
 }
  TEST(uBasicNGSCHR_getNotOverlapping, NOOVERLAP){
       ASSERT_TRUE(false);
 }


/**<  getDistinct*/
TEST_F(ChromDivide, distinctChrTestBorder)
{
     ASSERT_TRUE(false);
  //   auto testChrom=uChromTestOverlap.getDistinct(5, 200);
  //   EXPECT_EQ(1,testChrom.count());
}
TEST(uBasicNGSCHR_getDistinct, NORM)
{
     uBasicNGSChrom emptyChr("chr2");
     auto testChrom =emptyChr.getDistinct(500, 2000);
     EXPECT_EQ(0,testChrom.count());
}

TEST(uBasicNGSCHR_getDistinct, distinctChrTestEmpty)
{
     uBasicNGSChrom emptyChr("chr2");
     auto testChrom =emptyChr.getDistinct(500, 2000);
     EXPECT_EQ(0,testChrom.count());
}
TEST(uBasicNGSCHR_getDistinct, MULTIPLE)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getDistinct, MANY)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getDistinct, CUSTOMSORT)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getDistinct, FAILNOSORT)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getDistinct, MORECUSTOMSORT)
{
     ASSERT_TRUE(false);
}
/**< Test getSubset, RemoveSubset */

TEST_F(ChromDivide, subsetCountChr)
{
     ASSERT_TRUE(false);
     //EXPECT_EQ(3,uChromTestOverlap.getSubsetCount(1, 2000));
}

TEST(uBasicNGSCHR_getSubset, NORMAL)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getSubset, EMPTY)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getSubset, MANY)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_removeSubset, NORMAL)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_removeSubset, EMPTY)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_removeSubset, MANY)
{
     ASSERT_TRUE(false);
}


/**<  getSubsetCount*/

TEST(uBasicNGSCHR_getSubsetCount, NORMAL)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getSubsetCount, EMPTY)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getSubsetCount, MANY)
{
     ASSERT_TRUE(false);
}
/**< AddData */

TEST(uBasicNGSCHR_addData, VALID)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_addData, INVALIDCHR)
{
     ASSERT_TRUE(false);
}


/**<  GetStartFunct, getEndFunct */


TEST(uBasicNGSCHR_getStartFunct, HASBEENSET)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getStartFunct, NOTBEENSET)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getEndFunct, HASBEENSET)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getEndFunct, NOTBEENSET)
{
     ASSERT_TRUE(false);
}


TEST(uBasicNGSCHR_getCompFunct, BEENSET)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getCompFunct, NOTBEENSET)
{
     ASSERT_TRUE(false);
}


/**<  setChromSize*/

TEST(uBasicNGSCHR_setChromSize, VALID)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_setChromSize, INVALID_UNDER0)
{
     ASSERT_TRUE(false);
}

/**< getSite */
TEST(uBasicNGSCHR_getSite, VALIDREQUEST)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_getSite, INVALID)
{
     ASSERT_TRUE(false);
}

/**< SortSites */
TEST(uBasicNGSCHR_sortSites, DEFAULT)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_sortSites, CUSTOM)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_sortSites, CUSTOMSTARTFUNCT)
{
     ASSERT_TRUE(false);
}

/**< Custom next and Prec */
TEST_F(ChromDivide, FINDNEXT_TEST){
    uChromTestOverlap.addData(uBasicNGS("chr1", 200, 800));
    uChromTestOverlap.addData(uBasicNGS("chr1", 250, 800));
    uChromTestOverlap.sortSites();
    auto second=uChromTestOverlap.findNextSite(235);
    EXPECT_EQ(250,second->getStart());

    EXPECT_EQ(uChromTestOverlap.end(), uChromTestOverlap.findNextSite(1500));
}
TEST_F(ChromDivide, FINDPREC_TEST){
 uChromTestOverlap.addData(uBasicNGS("chr1", 200, 800));
    uChromTestOverlap.addData(uBasicNGS("chr1", 250, 800));
    uChromTestOverlap.sortSites();
    auto first=uChromTestOverlap.findPrecedingSite(195);
    EXPECT_EQ(100,first->getStart());
}
TEST(uBasicNGSCHR_findNext, CUSTOM)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_findPrec, CUSTOM)
{
     ASSERT_TRUE(false);
}

TEST(uBasicNGSCHR_getSubset, CUSTOM)
{
     ASSERT_TRUE(false);
}
/**< isSorted() */
TEST(uBasicNGSCHR_isSorted, NOT)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_isSorted, EMPTY)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_isSorted, DEFAULT)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_isSorted, CUSTOM)
{
     ASSERT_TRUE(false);
}
/**< minSite(comp ) */
TEST(uBasicNGSCHR_minSite, NORMAL)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_minSite, EMPTY)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_minSite, EXCEPTIOn)
{
     ASSERT_TRUE(false);
}

/**< maxSite(comp ) */
TEST(uBasicNGSCHR_maxSite, NORMAL)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_maxSite, EMPTY)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_maxSite, EXCEPTIOn)
{
     ASSERT_TRUE(false);
}
/**< DivideItemsIntoBin */

TEST(uBasicNGSCHR_divideItemsIntoBinofSize, NORMAL)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_divideItemsIntoBinofSize, EMPTYCHR)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_divideItemsIntoBinofSize, NOCHR)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_divideItemsIntoNBins, NORMAL)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_divideItemsIntoNBins, EMPTYCHR)
{
     ASSERT_TRUE(false);
}
TEST(uBasicNGSCHR_divideItemsIntoNBins, NOCHR)
{
     ASSERT_TRUE(false);
}
/**<  */