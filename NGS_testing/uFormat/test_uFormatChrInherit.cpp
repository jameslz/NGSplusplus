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

class StandardChroms : public testing::Test {
 protected:
/**< As always, this is inclusive so 100-199 is of size 100 */
  virtual void SetUp() {

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
       uBasicNGSChrom oneChrom("chr1");
       oneChrom.addData(uBasicNGS("chr1",100,200));
       EXPECT_EQ(oneChrom.avgSiteSize(), 101);
 }
TEST(uBasicNGSCHR_avgSiteSize, NOSITE){
        uBasicNGSChrom emptyChr("chr1");
       EXPECT_EQ(emptyChr.avgSiteSize(),0);
 }
 TEST(uBasicNGSCHR_avgSiteSize, MANYSITE){
       uBasicNGSChrom manyChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));
       EXPECT_EQ(manyChr.avgSiteSize(),((101+71+131)/3));
 }
/**<  */
 TEST(uBasicNGSCHR_minSiteSize, ONESITE){
       uBasicNGSChrom oneChrom("chr1");
       oneChrom.addData(uBasicNGS("chr1",100,200));
       EXPECT_EQ(oneChrom.minSiteSize() , 101);
 }
TEST(uBasicNGSCHR_minSiteSize, NOSITE){
       uBasicNGSChrom emptyChr("chr1");
       EXPECT_EQ(emptyChr.minSiteSize(),0);
 }
 TEST(uBasicNGSCHR_minSiteSize, MANYSITE){
       uBasicNGSChrom manyChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));
       EXPECT_EQ(manyChr.minSiteSize(),(71));
 }
 /**<  */
 TEST(uBasicNGSCHR_maxSiteSize, ONESITE){
       uBasicNGSChrom oneChrom("chr1");
       oneChrom.addData(uBasicNGS("chr1",100,200));
       EXPECT_EQ(oneChrom.maxSiteSize() , 101);
 }
TEST(uBasicNGSCHR_maxSiteSizee, NOSITE){
       uBasicNGSChrom emptyChr("chr1");
       EXPECT_EQ(emptyChr.maxSiteSize(),0);
 }
 TEST(uBasicNGSCHR_maxSiteSize, MANYSITE){
       uBasicNGSChrom manyChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));
       EXPECT_EQ(manyChr.maxSiteSize(),(101+71+131));
 }
 /**<  */
 TEST(uBasicNGSCHR_sumSiteSize, ONESITE){
       uBasicNGSChrom oneChrom("chr1");
       oneChrom.addData(uBasicNGS("chr1",100,200));
       EXPECT_EQ(oneChrom.sumSiteSize() , 101);
 }
TEST(uBasicNGSCHR_sumSiteSize, NOSITE){
       uBasicNGSChrom emptyChr("chr1");
       EXPECT_EQ(emptyChr.sumSiteSize(),0);
 }
 TEST(uBasicNGSCHR_sumSiteSize, MANYSITE){
       uBasicNGSChrom manyChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));
       EXPECT_EQ(manyChr.sumSiteSize(),131);
 }
/**<  */
 TEST(uBasicNGSCHR_inferChrSize, ONESITE){
       uBasicNGSChrom oneChrom("chr1");
       oneChrom.addData(uBasicNGS("chr1",100,200));
       oneChrom.inferChrSize();
       EXPECT_EQ(oneChrom.getChromSize() , 200);
 }
TEST(uBasicNGSCHR_inferChrSize, NOSITE){
       uBasicNGSChrom emptyChr("chr1");
       emptyChr.inferChrSize();
       EXPECT_EQ(emptyChr.getChromSize() , 0);
 }
 TEST(uBasicNGSCHR_inferChrSize, MANYSITE){
       uBasicNGSChrom manyChr("chr1");
       manyChr.addData(uBasicNGS("chr1",100,200));
       manyChr.addData(uBasicNGS("chr1",230,300));
       manyChr.addData(uBasicNGS("chr1",120,250));
       manyChr.inferChrSize();
       EXPECT_EQ(manyChr.getChromSize() , 300);
 }
/**<  */
 TEST_F(StandardChroms, uBasicNGSCHR_countUnique_ONESITE){
       EXPECT_EQ(oneChr.countUnique() , 1);
 }
TEST_F(StandardChroms, uBasicNGSCHR_countUnique_NOSITE){
         EXPECT_EQ(emptyChr.countUnique() , 0);
 }
 TEST_F(StandardChroms, uBasicNGSCHR_countUnique_MANYSITENOUNIQUE){
       EXPECT_EQ(manyChr.countUnique() , 3);
 }
TEST(StandardChroms, uBasicNGSCHR_countUnique_MANYSITEWITHUNIQUE){
       ASSERT_TRUE(false);
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
TEST(uBasicNGSCHR_generateRandomSite, MAKEITEM){
       ASSERT_TRUE(false);
 }
TEST(uBasicNGSCHR_generateRandomSite, MAKEMANYDIFFERENTITEMS){
       ASSERT_TRUE(false);
 }
TEST(uBasicNGSCHR_generateRandomSite, MAKEMANYEXCLUSIONLIST){
       ASSERT_TRUE(false);
 }

TEST(uBasicNGSCHR_addNRandomSite, MAKECORRECTNUMBER){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_addNRandomSite, MAKECORRECTNUMBERMANYDIFFERENT){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_addNRandomSite, MAKEMANYEXCLUSIONLIST){
       ASSERT_TRUE(false);
 }

/**< Get overlapping */
 TEST(uBasicNGSCHR_getOverlapping, NORMAL){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getOverlapping, FIRSTEMPTY){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getOverlapping, SECONDEMPTY){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getOverlapping, SOMEOVERLAP){
       ASSERT_TRUE(false);
 }
 TEST(uBasicNGSCHR_getOverlapping, NOOVERLAP){
       ASSERT_TRUE(false);
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
